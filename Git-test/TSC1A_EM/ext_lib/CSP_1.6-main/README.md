![CI Status](https://github.com/libcsp/libcsp/actions/workflows/build-test.yml/badge.svg)

# The Cubesat Space Protocol

Cubesat Space Protocol (CSP) is a small protocol stack written in C. CSP
is designed to ease communication between distributed embedded systems
in smaller networks, such as Cubesats. The design follows the TCP/IP
model and includes a transport protocol, a routing protocol and several
MAC-layer interfaces. The core of `libcsp`
includes a router, a connection oriented socket API and
message/connection pools.

The protocol is based on an very lightweight header containing both transport and
network-layer information. Its implementation is designed for, but not
limited to, embedded systems with very limited CPU and memory resources.
The implementation is written in GNU C and is currently ported to run on FreeRTOS, Zephyr 
and Linux (POSIX).

The idea is to give sub-system developers of cubesats the same features
of a TCP/IP stack, but without adding the huge overhead of the IP
header. The small footprint and simple implementation allows a small
8-bit system to be fully connected on the network. This allows all
subsystems to provide their services on the same network level, without
any master node required. Using a service oriented architecture has
several advantages compared to the traditional mater/slave topology used
on many cubesats.

  - Standardised network protocol: All subsystems can communicate with
    eachother (multi-master)
  - Service loose coupling: Services maintain a relationship that
    minimizes dependencies between subsystems
  - Service abstraction: Beyond descriptions in the service contract,
    services hide logic from the outside world
  - Service reusability: Logic is divided into services with the
    intention of promoting reuse.
  - Service autonomy: Services have control over the logic they
    encapsulate.
  - Service Redundancy: Easily add redundant services to the bus
  - Reduces single point of failure: The complexity is moved from a
    single master node to several well defined services on the network

The implementation of `libcsp` is written
with simplicity in mind, but it's compile time configuration allows it
to have some rather advanced features as well.

The latest version of the /doc folder is compiled to HTML and hosted on:
  [libcsp.github.io/libcsp/](https://libcsp.github.io/libcsp/)


# The Cubesat Space Protocol (CSP) on STM32 Nucleo-F722ZE and Nucleo-L496ZG-P

This project showcases the seamless integration of The Cubesat Space Protocol (CSP) into STM32 microcontrollers, specifically the Nucleo-F722ZE and Nucleo-L496ZG-P families. The implementation includes the incorporation of USART and CAN drivers, enabling efficient interfacing with CSP. To enhance the development environment, unnecessary components related to Windows, Linux, and examples have been removed. This adjustment is aimed at preventing build failures during debugging processes in STM32CubeIDE.


```

████████╗███████╗ ██████╗ ██╗     ██████╗███████╗██████╗     ███████╗████████╗███╗   ███╗██████╗ ██████╗ 
╚══██╔══╝██╔════╝██╔════╝███║    ██╔════╝██╔════╝██╔══██╗    ██╔════╝╚══██╔══╝████╗ ████║╚════██╗╚════██╗
   ██║   ███████╗██║     ╚██║    ██║     ███████╗██████╔╝    ███████╗   ██║   ██╔████╔██║ █████╔╝ █████╔╝
   ██║   ╚════██║██║      ██║    ██║     ╚════██║██╔═══╝     ╚════██║   ██║   ██║╚██╔╝██║ ╚═══██╗██╔═══╝ 
   ██║   ███████║╚██████╗ ██║    ╚██████╗███████║██║         ███████║   ██║   ██║ ╚═╝ ██║██████╔╝███████╗
   ╚═╝   ╚══════╝ ╚═════╝ ╚═╝     ╚═════╝╚══════╝╚═╝         ╚══════╝   ╚═╝   ╚═╝     ╚═╝╚═════╝ ╚══════╝
                                                                                                   
```


# The basics of CSP
The following diagram shows a conceptual overview of the different
blocks in CSP. The shown interface is CAN
(src/interfaces/csp_if_can.c, driver:src/drivers/can_stm32/csp_driver_can.c).

### Project Structure Diagram
```
    buffer                  connection   send   read/accept
       ^                         |         |         ^
       |                         |         v         |
       |   +-----------------+   |     +----------------+    +-------------------+
       |   | connection pool |   |     |    CSP Core    |    | csp_task_router() |
       |   |  csp_conn.c     |<--+---->| * routing      |<---|    csp_route.c    |
       |   +-----------------+         | * crypt        |    +-------------------+
       |                               | * UDP/RDP      |              ^
       |                  csp_route_t  | * CRC32        |              |
       v                 +------------>|                |     +----------------+
+--------------+         |             |                |     | incoming queue |
| buffer pool  |         |             |                |     |     qfifo.c    |
| csp_buffer.c |  +-------------+      +----------------+     +----------------+
+--------------+  |routing table|            |                         ^
       ^          |  rtable.c   |            v                         |
       |          +-------------+      +------------------------------------+
       |                               |  (next_hop)                        |
       |                               |    CAN interface (csp_if_can.c)    |
       +------------------------------>|                      csp_can_rx()  |
                                       +------------------------------------+
                                             |   CAN frame (8 bytes)   ^
                                             v                         |
                                     csp_can_tx_frame()       socketcan_rx_thread()
                                             (drivers/can_stm32/csp_driver_can.c)
```

## Overview
The integration process is explained comprehensively in this guide, emphasizing the incorporation of CSP into STM32 Nucleo microcontrollers. The guide focuses on the creation of a dedicated task specifically designed to handle the `csp_route_work()` function. Furthermore, it provides detailed steps to configure the project for both Nucleo-F722ZE and Nucleo-L496ZG-P. Additionally, instructions are provided for setting up CAN and USART drivers to facilitate communication with CSP.


### Task Configuration
To reserve a task for the csp_route_work() function, create a task with the following specifications:

**Priority:** osPriorityRealtime  
**Stack Size:** 512  
**Allocation:** Dynamic  

Here's an example task creation in FreeRTOS:
```
osThreadDef(cspRouteWorkTask, csp_route_work, osPriorityRealtime, 0, 512);
osThreadId cspRouteWorkTaskHandle = osThreadCreate(osThread(cspRouteWorkTask), NULL);
```

### Board Selection in CSP_config.h
Choose the appropriate Nucleo model in the CSP_config.h file. Modify the following lines based on your Nucleo model:
```
#define BOARD_stm32g4xx_hal 1
#define BOARD_stm32f7xx_hal 2
#define BOARD_stm32h7xx_hal 3
#define BOARD_stm32flxx_hal 4

#define CSP_SELEC_BOARD BOARD_stm32g4xx_hal
```
For example, if you are using Nucleo-G431VKB, set the value as follows:
```
#define CSP_SELEC_BOARD BOARD_stm32g4xx_hal
```

### Driver Configuration
Include the necessary files and configure the settings for the desired drivers. Below is an example for CAN on Nucleo-G431VKB:
```
/* USER CODE BEGIN Includes */
#include "include/csp/csp.h"
#include "src/drivers/can_stm32/csp_driver_can.h"
#include <include/csp/drivers/usart.h>
/* USER CODE END Includes */

static uint8_t address = 8;

int main(void) {
   
    csp_conf_t csp_conf;
	csp_conf_get_defaults(&csp_conf);
	csp_conf.address = address;
	int error = csp_init(&csp_conf);                           //Initialize CSP
	if (error != CSP_ERR_NONE) {
		csp_log_error("csp_init() failed, error: %d", error);
		exit(1);
	}

    csp_route_start_task(600, osPriorityRealtime);           //Start router task

	csp_iface_t *default_iface = NULL;
    csp_driver_can_init(0, CSP_CAN_PROMISC, &default_iface); //Initialize CAN driver
}
```
Make sure to adapt the configurations based on your specific hardware and requirements. After completing these steps, you should have a basic setup for using CSP on STM32 Nucleo, with a dedicated task for handling the csp_route_work() function.

### Routing Configuration
Routes in the Cubesat Space Protocol (CSP) can be configured using text strings in the following format:
```
<address>[/mask] <interface name> [via]
```
**address:** Destination address; the routing table matches it against the CSP header destination.
**mask (optional):** Determines how many MSB bits of the address are to be matched. Mask values different from 0 and 5 are only supported by the CIDR routing table.
**interface name:** Name of the interface to route the packet on.
**via (optional) address:** If different from 255, route the packet to the via address instead of the address in the CSP header.

#### Example Routing Configuration
In this example, routing configurations are set up on the Nucleo-F, which serves as an intermediary between the USART and CAN interfaces. The goal is to route traffic between address 9 on Nucleo-G and address 7 on Ubuntu, utilizing both USART and CAN interfaces.

#### Route 9 (KISS)
```
csp_rtable_set(9, 14, KISS, CSP_NO_VIA_ADDRESS);
```
This configuration indicates that when Nucleo-G pings address 9 using the CAN interface, the interface will be changed to KISS to route to address 9 (CSP on Ubuntu).

This routing configuration helps facilitate communication between Nucleo-G and Ubuntu, ensuring that the traffic is correctly routed through the USART and CAN interfaces on Nucleo-F.
```
+--------------------+              +--------------------+
|   CSP on Ubuntu    |              |   CSP on Nucleo-F  |
|  Server Address 9  |      UART    |  Server Address 8  |
|  Ping Address 7    |<------------>|  Route 9 (KISS)    |
|                    |              |  Route 7 (CAN)     |
+--------------------+              +--------------------+
                                            ^
                                            |
                                            |
                                +-------------------------+            
                                |           CAN           |    
                                |                         |
                                v                         v
                      +--------------------+    +---------------------+
                      |   CSP on Nucleo-L  |    |   CSP on Nucleo-L   |
                      |  Server Address 7  |    |  Server Address 6   |
                      |                    |    |  Ping Address 7     |
                      |                    |    |  Ping Address 9     |
                      +--------------------+    +---------------------+
```

## Examples Task
The project is structured around three main components:
1. **router task (router_task)**
```
//EXAMPLE
void router_task (void const *argument) {
	for (;;) {
		csp_route_work();
		osDelay(1);
	}
}
```
2. **Server Task (server_task)**
The server_task handles incoming connections, reads packets, and processes data. It is based on the following example:
```
//EXAMPLE
void server_task(void const *argument) {
	/* Create socket with no specific socket options, e.g. accepts CRC32, HMAC, etc. if enabled during compilation */
	csp_socket_t sock = { 0 };

	/* Bind socket to all ports, e.g. all incoming connections will be handled here */
	csp_bind(&sock, CSP_ANY);

	/* Create a backlog of 10 connections, i.e. up to 10 new connections can be queued */
	csp_listen(&sock, 10);

	for (;;) {
		osDelay(1);
		/* Wait for a new connection, 10000 mS timeout */
		csp_conn_t *conn;
		if ((conn = csp_accept(&sock, 10000)) == NULL) {
			/* timeout */
			continue;
		}

		/* Read packets on connection, timout is 100 mS */
		csp_packet_t *packet;
		while ((packet = csp_read(conn, 100)) != NULL) {
			switch (csp_conn_dport(conn)) {
			case MY_SERVER_PORT:
				/* Process packet here */
				csp_print("Packet received on MY_SERVER_PORT: %s\r\n",(char* ) packet->data);
				csp_buffer_free(packet);
				break;

			default:
				/* Call the default CSP service handler, handle pings, buffer use, etc. */
				csp_service_handler(packet);
				break;
			}
		}

		/* Close current connection */
		csp_close(conn);
	}
}
```
3. **Client Task (client_task)**
The client_task sends various requests, such as pings and data packets, to the server. It is based on the following example:
```
//EXAMPLE
void client_task(void const *argument) {
	unsigned int count = 'A';
	for (;;) {
		osDelay(100);
		i =!i;
		if(count == 'Z') count ='A';
		/* Send ping to server, timeout 1000 mS, ping size 100 bytes */
				int result = csp_ping(server_address, 1000, 100, CSP_O_NONE);
				csp_print("Ping address: %u, result %d [mS]\r\n", server_address, result);
		        (void) result;

				/* Send reboot request to server, the server has no actual implementation of csp_sys_reboot() and fails to reboot */
				csp_reboot(server_address);
				csp_print("reboot system request sent to address: %u\r\n", server_address);

				/* Send data packet (string) to server */

				/* 1. Connect to host on 'server_address', port MY_SERVER_PORT with regular UDP-like protocol and 1000 ms timeout */
				csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, server_address, MY_SERVER_PORT, 1000, CSP_O_NONE);
				if (conn == NULL) {
					/* Connect failed */
					csp_print("Connection failed\r\n");
					return;
				}

				/* 2. Get packet buffer for message/data */
				csp_packet_t * packet = csp_buffer_get(100);
				if (packet == NULL) {
					/* Could not get buffer element */
					csp_print("Failed to get CSP buffer\r\n");
					return;
				}

				/* 3. Copy data to packet */
		        memcpy(packet->data, "Hello world ", 12);
		        memcpy(packet->data + 12, &count, 1);
		        memset(packet->data + 13, 0, 1);
		        count++;

				/* 4. Set packet length */
				packet->length = (strlen((char *) packet->data) + 1); /* include the 0 termination */

				/* 5. Send packet */
				csp_send(conn, packet);

				/* 6. Close connection */
				csp_close(conn);

	}
}
```

## Directory Hierarchy
```
|—— CHANGELOG
|—— CONTRIBUTING.md
|—— COPYING
|—— doc
|    |—— basic.rst
|    |—— example.rst
|    |—— history.rst
|    |—— libcsp.rst
|    |—— memory.rst
|    |—— mtu.rst
|    |—— protocolstack.rst
|    |—— structure.rst
|    |—— topology.rst
|—— include
|    |—— csp
|        |—— arch
|            |—— csp_clock.h
|            |—— csp_malloc.h
|            |—— csp_queue.h
|            |—— csp_semaphore.h
|            |—— csp_system.h
|            |—— csp_thread.h
|            |—— csp_time.h
|            |—— posix
|                |—— csp_system.h
|                |—— pthread_queue.h
|        |—— crypto
|            |—— csp_hmac.h
|            |—— csp_sha1.h
|            |—— csp_xtea.h
|        |—— csp.h
|        |—— csp_autoconfig.h
|        |—— csp_buffer.h
|        |—— csp_cmp.h
|        |—— csp_crc32.h
|        |—— csp_debug.h
|        |—— csp_endian.h
|        |—— csp_error.h
|        |—— csp_iflist.h
|        |—— csp_interface.h
|        |—— csp_platform.h
|        |—— csp_promisc.h
|        |—— csp_rtable.h
|        |—— csp_sfp.h
|        |—— csp_types.h
|        |—— drivers
|            |—— can_socketcan.h
|            |—— usart.h
|        |—— interfaces
|            |—— csp_if_can.h
|            |—— csp_if_i2c.h
|            |—— csp_if_kiss.h
|            |—— csp_if_lo.h
|            |—— csp_if_zmqhub.h
|—— Quick use of csp_10052024.pdf
|—— README00.md
|—— src
|    |—— arch
|        |—— csp_system.c
|        |—— csp_time.c
|        |—— freertos
|            |—— csp_clock.c
|            |—— csp_malloc.c
|            |—— csp_queue.c
|            |—— csp_semaphore.c
|            |—— csp_system.c
|            |—— csp_thread.c
|            |—— csp_time.c
|    |—— cli_Jab
|        |—— cli_csp.c
|        |—— cli_csp.h
|        |—— cli_func_adcs.c
|        |—— cli_func_csp.c
|        |—— cli_func_register.c
|        |—— cli_func_ssdv.c
|        |—— cli_func_time_sync.c
|        |—— cli_rtable.c
|        |—— cli_rtable.h
|        |—— print_list.c
|        |—— print_list.h
|        |—— script_management.c
|        |—— script_management.h
|    |—— crypto
|        |—— csp_hmac.c
|        |—— csp_sha1.c
|        |—— csp_xtea.c
|    |—— csp_bridge.c
|    |—— csp_buffer.c
|    |—— csp_conn.c
|    |—— csp_conn.h
|    |—— csp_crc32.c
|    |—— csp_debug.c
|    |—— csp_dedup.c
|    |—— csp_dedup.h
|    |—— csp_endian.c
|    |—— csp_hex_dump.c
|    |—— csp_iflist.c
|    |—— csp_init.c
|    |—— csp_init.h
|    |—— csp_io.c
|    |—— csp_io.h
|    |—— csp_port.c
|    |—— csp_port.h
|    |—— csp_promisc.c
|    |—— csp_promisc.h
|    |—— csp_qfifo.c
|    |—— csp_qfifo.h
|    |—— csp_route.c
|    |—— csp_services.c
|    |—— csp_service_handler.c
|    |—— csp_sfp.c
|    |—— drivers
|        |—— can_stm
|            |—— f103c8t6_can.c
|            |—— f103c8t6_can.h
|            |—— f722ze_can.c
|            |—— f722ze_can.h
|            |—— f767zgtx_can.c
|            |—— f767zgtx_can.h
|            |—— g431kb_can.c
|            |—— g431kb_can.h
|            |—— l496zg_can.c
|            |—— l496zg_can.h
|        |—— i2c
|            |—— f722ze_i2c.c
|            |—— f722ze_i2c.h
|            |—— g431kb_i2c.c
|            |—— g431kb_i2c.h
|            |—— l496zg-p_i2c.c
|            |—— l496zg-p_i2c.h
|        |—— radio
|            |—— l496zg-p_radio.c
|            |—— l496zg-p_radio.h
|            |—— radio_kiss.c
|        |—— usart
|            |—— f722ze_uart.c
|            |—— f722ze_uart.h
|            |—— l496zg-p_uart.c
|            |—— l496zg-p_uart.h
|            |—— usart_kiss.c
|    |—— func
|        |—— func_adcs
|            |—— Func_ADCS.c
|            |—— Func_ADCS.h
|        |—— func_register
|            |—— bit_management.c
|            |—— bit_management.h
|            |—— csp_register.c
|            |—— csp_register.h
|            |—— csp_register_service.c
|            |—— csp_register_service.h
|            |—— csp_register_service_handler.c
|            |—— format_pack.md
|            |—— test_register.py
|        |—— func_ssdv
|            |—— Func_SSDV.c
|            |—— func_SSDV.h
|        |—— func_timesync
|            |—— Func_Time_sync.c
|            |—— Func_Time_sync.h
|    |—— interfaces
|        |—— csp_if_can.c
|        |—— csp_if_can_pbuf.c
|        |—— csp_if_can_pbuf.h
|        |—— csp_if_i2c.c
|        |—— csp_if_kiss.c
|        |—— csp_if_lo.c
|    |—— rtable
|        |—— csp_rtable.c
|        |—— csp_rtable_cidr.c
|        |—— csp_rtable_internal.h
|        |—— csp_rtable_static.c
|    |—— transport
|        |—— csp_rdp.c
|        |—— csp_transport.h
|        |—— csp_udp.c
```
### Tested Platform
- software for write code
  ```
  STM32 CUBE IDE 1.16.1
  ```
- hardware
  ```
  NUCLEO F722ZE
  NUCLEO L496ZG-P
  ```