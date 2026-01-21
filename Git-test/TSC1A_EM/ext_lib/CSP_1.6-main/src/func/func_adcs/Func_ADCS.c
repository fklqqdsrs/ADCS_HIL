#include <src/func/func_adcs/Func_ADCS.h>

/**
 * The function `set_ms_rpm_react_view` establishes a connection, sends a packet containing a view and
 * RPM value, and waits for a response confirming successful transmission of the RPM view.
 * 
 * @param view The `view` parameter in the `set_ms_rpm_react_view` function represents the view number
 * that you want to set for the MS RPM view. It is of type `uint8_t`, which means it is an unsigned
 * 8-bit integer. This parameter is used to specify the view
 * @param rpm_view The `rpm_view` parameter in the `set_ms_rpm_react_view` function represents the RPM
 * (Revolutions Per Minute) value that you want to set for a specific view in the system. This value
 * will be sent over a CSP (Communicating Sequential Processes) connection to the designated
 * @param timeout The `timeout` parameter in the `set_ms_rpm_react_view` function is used to specify
 * the maximum time in milliseconds to wait for a response before timing out. If a response is not
 * received within this time frame, the function will return `CSP_ERR_TIMEDOUT`.
 * 
 * @return The function `set_ms_rpm_react_view` returns an `int8_t` value, which indicates the status
 * of the operation. The possible return values are:
 * - `CSP_ERR_NONE` if the RPM view response was successful.
 * - `CSP_ERR_NOMEM` if the connection failed.
 * - `CSP_ERR_BUSY` if failed to get a CSP buffer.
 * - `C
 */
int8_t set_ms_rpm_react_view(uint8_t view, uint32_t rpm_view, uint32_t timeout)
{
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, ADDRESS_ADCS, PORT_ADCS, timeout, CSP_O_NONE);
	if (conn == NULL)
	{
		csp_log_error("Connection failed\r\n");
		return CSP_ERR_NOMEM;
	}
	size_t packet_size = sizeof(register_pack) + sizeof(uint32_t);
	csp_packet_t *packet = csp_buffer_get(packet_size);

	if (packet == NULL)
	{
		csp_log_error("Failed to get CSP buffer\r\n");
		csp_close(conn);
		return CSP_ERR_BUSY;
	}

	register_pack reg_pack_s = {MODULE_REACT_VIEW, WRITE_REGISTER_ADCS, MS_RPM_VIEW_REGISTER, view};
	memcpy(packet->data, &reg_pack_s, sizeof(register_pack));
	memcpy(packet->data + sizeof(register_pack), &rpm_view, sizeof(rpm_view));

	packet->length = packet_size;

	if (!csp_send(conn, packet, timeout))
	{
		csp_log_error("Send failed\r\n");
		csp_buffer_free(packet);
		csp_close(conn);
		return CSP_ERR_TX;
	}

   while ((packet = csp_read(conn, timeout)) != NULL)
    {
        register_pack received_pack;
        memcpy(&received_pack, packet->data, sizeof(register_pack));

        if (received_pack.module_t == MODULE_REACT_VIEW && received_pack.action_t == WRITE_REGISTER_ADCS &&
            received_pack.register_t == MS_RPM_VIEW_REGISTER && received_pack.register_sub_id_t == view)
        {
            csp_buffer_free(packet);
            csp_close(conn);
            csp_log_info("RPM view response success\r\n");
            return CSP_ERR_NONE;
        }
        csp_buffer_free(packet);
    }
	csp_close(conn);
	return CSP_ERR_TIMEDOUT;
}




/**
 * The function `get_ms_rpm_react_view` establishes a connection, sends a packet to request RPM data
 * from a specific view, and waits for and processes the response.
 * 
 * @param view The `view` parameter in the `get_ms_rpm_react_view` function represents the specific
 * view for which you want to retrieve the RPM value. It is of type `uint8_t`, which means it can hold
 * values from 0 to 255. This parameter is used to specify the
 * @param timeout The `timeout` parameter in the `get_ms_rpm_react_view` function is the duration in
 * milliseconds for which the function will wait to establish a connection, send a packet, and receive
 * a response before timing out. If the timeout period elapses without a successful response, the
 * function will return
 * 
 * @return The function `get_ms_rpm_react_view` returns an unsigned 32-bit integer (`uint32_t`). The
 * return value can be either the RPM view value received successfully from the connected device, or
 * `0xFFFFFFFF` if there was a failure during the communication process or if a timeout occurred
 * without receiving a response.
 */
uint32_t get_ms_rpm_react_view(uint8_t view, uint32_t timeout)
{
	printf("Connecting to ADDRESS_ADCS on PORT_REACT_VIEW...\n");
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, ADDRESS_ADCS, PORT_ADCS, timeout, CSP_O_NONE);
	if (conn == NULL)
	{
		printf("Connection failed.\n");
		return 0xFFFFFFFF;
	}
	printf("Connected successfully.\n");

	size_t packet_size = sizeof(register_pack);
	csp_packet_t *packet = csp_buffer_get(packet_size);
	if (packet == NULL)
	{
		printf("Failed to get CSP buffer.\n");
		csp_close(conn);
		return 0xFFFFFFFF;
	}

	register_pack reg_pack_s = {MODULE_REACT_VIEW, READ_REGISTER_ADCS, MS_RPM_VIEW_REGISTER, view};
	memcpy(packet->data, &reg_pack_s, sizeof(register_pack));
	packet->length = packet_size;

	printf("Sending packet with module_t = %d, action_t = %d, register_t = %d, view = %d\n", reg_pack_s.module_t, reg_pack_s.action_t, reg_pack_s.register_t, view);

	if (!csp_send(conn, packet, timeout))
	{
		printf("Send failed.\n");
		csp_buffer_free(packet);
		csp_close(conn);
		return 0xFFFFFFFF;
	}

	printf("Packet sent successfully. Waiting for response...\n");

	while ((packet = csp_read(conn, timeout)) != NULL)
	{
		if (packet->length == (sizeof(register_pack) + sizeof(int32_t)))
		{
			register_pack received_pack;
			int32_t rpm_view;

			memcpy(&received_pack, packet->data, sizeof(register_pack));
			memcpy(&rpm_view, packet->data + sizeof(register_pack), sizeof(int32_t));

			if (received_pack.module_t == MODULE_REACT_VIEW && received_pack.register_t == MS_RPM_VIEW_REGISTER)
			{
				printf("RPM view received: %ld \n", rpm_view);
				csp_buffer_free(packet);
				csp_close(conn);
				return rpm_view;
			}
			else
			{
				printf("Received incorrect module or register.\n");
			}
		}
		else
		{
			printf("Invalid packet length received.\n");
		}
		csp_buffer_free(packet);
	}

	printf("No response received. Timeout occurred.\n");
	csp_close(conn);
	return 0xFFFFFFFF;
}







/**
 * The function `get_imu` establishes a connection to an IMU module, sends a data request packet,
 * receives and validates the response packet, and returns the IMU data if successful.
 * 
 * @param imu_data The `imu_data` parameter in the `get_imu` function is a pointer to a struct of type
 * `axial_imu`. This function is responsible for connecting to a specific address and port, sending a
 * request packet for IMU data, receiving and processing the response packet containing IMU data
 * @param timeout The `timeout` parameter in the `get_imu` function is the duration in milliseconds for
 * which the function will wait to establish a connection, send a request packet, and receive a
 * response packet before timing out. If the specified timeout is reached at any stage during the
 * communication process, the function will
 * 
 * @return The function `get_imu` returns an `int8_t` value, which can be one of the following:
 * - `CSP_ERR_NONE` if the IMU data was successfully received and stored in `imu_data`.
 * - `CSP_ERR_NOMEM` if the connection to the IMU failed due to memory allocation issues.
 * - `CSP_ERR_BUSY` if there was a failure
 */
int8_t get_imu(axial_imu *imu_data, uint32_t timeout)
{
	printf("Connecting to ADDRESS_ADCS on PORT_IMU...\n");
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, ADDRESS_ADCS, PORT_ADCS, timeout, CSP_O_NONE);
	if (conn == NULL)
	{
		printf("Connection failed.\n");
		return CSP_ERR_NOMEM;
	}
	printf("Connected successfully.\n");

	size_t packet_size = sizeof(register_pack);
	csp_packet_t *packet = csp_buffer_get(packet_size);

	if (packet == NULL)
	{
		printf("Failed to get CSP buffer.\n");
		csp_close(conn);
		return CSP_ERR_BUSY;
	}

	register_pack reg_pack_s = {MODULE_IMU, READ_REGISTER_ADCS, ALL_IMU_REGISTER, SUB_REG_IMU};
	memcpy(packet->data, &reg_pack_s, sizeof(register_pack));
	packet->length = sizeof(register_pack);

	printf("Sending IMU data request packet...\n");

	if (!csp_send(conn, packet, timeout))
	{
		printf("Send failed.\n");
		csp_buffer_free(packet);
		csp_close(conn);
		return CSP_ERR_TX;
	}

	printf("Packet sent successfully. Waiting for response...\n");

	while ((packet = csp_read(conn, timeout)) != NULL)
	{
		printf("Packet received.\n");

		if (packet->length == sizeof(register_pack) + sizeof(axial_imu))
		{
			register_pack received_pack;
			memcpy(&received_pack, packet->data, sizeof(register_pack));

			if (received_pack.module_t == MODULE_IMU && received_pack.register_t == ALL_IMU_REGISTER)
			{

				memcpy(imu_data, packet->data + sizeof(register_pack), sizeof(axial_imu));
				printf("IMU data received: x=%f, y=%f, z=%f\n", imu_data->x_imu, imu_data->y_imu, imu_data->z_imu);
				csp_buffer_free(packet);
				csp_close(conn);
				return CSP_ERR_NONE;
			}
			else
			{
				printf("Received incorrect module or register.\n");
			}
		}
		else
		{
			printf("Invalid packet length received.\n");
		}
		csp_buffer_free(packet);
	}

	printf("No response received. Timeout occurred.\n");
	csp_close(conn);
	return CSP_ERR_TIMEDOUT;
}

/**
 * The function `get_gps` establishes a connection, sends a request for GPS data, and waits for a
 * response before returning the received GPS data.
 * 
 * @param gps_data_s The `gps_data_s` parameter is a pointer to a `gps_data_t` structure where the GPS
 * data will be stored after receiving it from the GPS module.
 * @param timeout The `timeout` parameter in the `get_gps` function is the duration in milliseconds for
 * which the function will wait to establish a connection, send a request, and receive a response
 * before timing out. It is used to control the maximum time the function will spend on each operation
 * to prevent indefinite waiting.
 * 
 * @return The function `get_gps` returns an `int8_t` value, which could be one of the following:
 * - `CSP_ERR_NONE` if GPS data was successfully received and stored in `gps_data_s`.
 * - `CSP_ERR_NOMEM` if the connection to the GPS module failed.
 * - `CSP_ERR_BUSY` if there was a failure to get a CSP buffer.
 * -
 */
int8_t get_gps(gps_data_t *gps_data_s, uint32_t timeout)
{
	printf("Connecting to ADDRESS_GPS on PORT_GPS...\n");
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, ADDRESS_ADCS, PORT_ADCS, timeout, CSP_O_NONE);
	if (conn == NULL)
	{
		printf("Connection failed.\n");
		return CSP_ERR_NOMEM;
	}
	printf("Connected successfully.\n");

	size_t packet_size = sizeof(register_pack);
	csp_packet_t *packet = csp_buffer_get(packet_size);

	if (packet == NULL)
	{
		printf("Failed to get CSP buffer.\n");
		csp_close(conn);
		return CSP_ERR_BUSY;
	}

	register_pack reg_pack_s = {MODULE_GPS, READ_REGISTER_ADCS, ALL_GPS_REGISTER, SUB_REG_GPS};
	memcpy(packet->data, &reg_pack_s, sizeof(register_pack));
	packet->length = sizeof(register_pack);

	printf("Sending request for GPS data...\n");

	if (!csp_send(conn, packet, timeout))
	{
		printf("Send failed.\n");
		csp_buffer_free(packet);
		csp_close(conn);
		return CSP_ERR_TX;
	}

	printf("Request sent. Waiting for response...\n");

	while ((packet = csp_read(conn, timeout)) != NULL)
	{
		if (packet->length == sizeof(register_pack) + sizeof(gps_data_t))
		{ // Assuming NGET_GPS is used in the response as well
			register_pack received_pack;
			memcpy(&received_pack, packet->data, sizeof(register_pack));
			if (received_pack.module_t == MODULE_GPS && received_pack.register_t == ALL_GPS_REGISTER)
			{
				memcpy(gps_data_s, packet->data + sizeof(register_pack), sizeof(gps_data_t));
				printf("GPS Data received - Lat: %f, Long: %f, MSL: %f\n", gps_data_s->lat_gps, gps_data_s->long_gps, gps_data_s->msl_gps);
				csp_buffer_free(packet);
				csp_close(conn);
				return CSP_ERR_NONE;
			}
			else
			{
				printf("Invalid header received: %x\n", packet->data[0]);
			}
			csp_buffer_free(packet);
		}

		printf("No response received. Timeout occurred.\n");
		csp_close(conn);
		return CSP_ERR_TIMEDOUT;
	}
	return CSP_ERR_TIMEDOUT;
}




/**
 * The function `get_mtm` establishes a connection to an MTM server, sends a request for MTM data, and
 * retrieves the response containing axial MTM information.
 * 
 * @param axial_mtm_s The `axial_mtm_s` parameter is a pointer to a structure of type `axial_mtm`. This
 * function `get_mtm` is designed to connect to an MTM server, send a request for MTM data, and then
 * receive and process the response containing axial MTM data
 * @param timeout The `timeout` parameter in the `get_mtm` function represents the maximum time in
 * milliseconds to wait for a response from the MTM server before considering the operation as timed
 * out. This timeout value is used in various parts of the function to control the duration for
 * connecting, sending requests, and waiting
 * 
 * @return The function `get_mtm` returns an `int8_t` value, which indicates the status of the
 * operation. The possible return values are:
 * - `CSP_ERR_NONE` if the MTM data was successfully received and stored in `axial_mtm_s`.
 * - `CSP_ERR_NOMEM` if there was a failure in memory allocation or connection establishment.
 * - `CSP_ERR
 */
int8_t get_mtm(axial_mtm *axial_mtm_s, uint32_t timeout)
{
	printf("Connecting to MTM server...\n");
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, ADDRESS_ADCS, PORT_ADCS, timeout, CSP_O_NONE);
	if (conn == NULL)
	{
		printf("Connection failed.\n");
		return CSP_ERR_NOMEM;
	}
	printf("Connected successfully.\n");

	size_t packet_size = sizeof(register_pack);
	csp_packet_t *packet = csp_buffer_get(packet_size);

	if (packet == NULL)
	{
		printf("Failed to get CSP buffer.\n");
		csp_close(conn);
		return CSP_ERR_NOMEM;
	}

	register_pack reg_pack_s = {MODULE_MTM, READ_REGISTER_ADCS, ALL_MTM_REGISTER, SUB_REG_MTM};
	memcpy(packet->data, &reg_pack_s, sizeof(register_pack));
	packet->length = sizeof(register_pack);

	printf("Sending request for MTM data...\n");

	if (!csp_send(conn, packet, timeout))
	{
		printf("Send failed.\n");
		csp_buffer_free(packet);
		csp_close(conn);
		return CSP_ERR_TX;
	}

	printf("Request sent successfully. Waiting for response...\n");

	while ((packet = csp_read(conn, timeout)) != NULL)
	{

		if (packet->length == sizeof(register_pack) + sizeof(axial_mtm))
		{
			register_pack received_pack;
			memcpy(&received_pack, packet->data, sizeof(register_pack));
			if (received_pack.module_t == MODULE_MTM && received_pack.register_t == ALL_MTM_REGISTER)
			{
				memcpy(axial_mtm_s, packet->data + sizeof(register_pack), sizeof(axial_mtm));
				printf("MTM Data - X: %f, Y: %f, Z: %f\n", axial_mtm_s->x_mtm, axial_mtm_s->y_mtm, axial_mtm_s->z_mtm);
				csp_buffer_free(packet);
				csp_close(conn);
				return CSP_ERR_NONE;
			}
			else
			{
				printf("Invalid header received: %x\n", packet->data[0]);
			}
			csp_buffer_free(packet);
		}
	}
	printf("No response received. Timeout occurred.\n");
	csp_close(conn);
	return CSP_ERR_TIMEDOUT;
}




/**
 * The function `service_adcs_demo` processes different types of register requests and sends responses
 * accordingly.
 * 
 * @param conn The `conn` parameter in the `service_adcs_demo` function is of type `csp_conn_t*`, which
 * is a pointer to a CSP connection structure. This structure likely contains information about the
 * connection over which the packet is received and through which responses can be sent.
 * @param packet The `packet` parameter in the `service_adcs_demo` function is of type `csp_packet_t*`
 * and represents a CSP (Communications Stack Protocol) packet that contains data to be processed.
 */
void service_adcs_demo(csp_conn_t *conn, csp_packet_t *packet)
{
	register_pack received_pack;
	memcpy(&received_pack, packet->data, sizeof(register_pack));

	if (received_pack.module_t == MODULE_REACT_VIEW && received_pack.action_t == WRITE_REGISTER_ADCS && received_pack.register_t == MS_RPM_VIEW_REGISTER)
	{
		uint32_t received_view = (uint32_t)received_pack.register_sub_id_t;
		uint32_t rpm_view;

		memcpy(&rpm_view, packet->data + sizeof(register_pack), sizeof(rpm_view));
		csp_packet_t *response_packet = csp_buffer_get(sizeof(received_view));
		if (response_packet != NULL)
		{
			memcpy(response_packet->data, &received_pack, sizeof(register_pack));
			response_packet->length = sizeof(register_pack);

			if (!csp_send(conn, response_packet, 1000))
			{
				csp_log_error("Failed to send response\r\n");
			}
			csp_buffer_free(response_packet);
		}
	}
	else if (received_pack.module_t == MODULE_REACT_VIEW && received_pack.action_t == READ_REGISTER_ADCS && received_pack.register_t == MS_RPM_VIEW_REGISTER)
	{
		uint32_t value_react_view = 0;

		csp_packet_t *response_packet = csp_buffer_get(sizeof(register_pack) + sizeof(value_react_view));
		if (response_packet != NULL)
		{
			memcpy(response_packet->data, &received_pack, sizeof(register_pack));								// กำหนดข้อมูล register_pack เดิม
			memcpy(response_packet->data + sizeof(register_pack), &value_react_view, sizeof(value_react_view)); // กำหนดค่า RPM view

			response_packet->length = sizeof(register_pack) + sizeof(value_react_view);

			if (!csp_send(conn, response_packet, 1000))
			{
				csp_log_error("Failed to send response\r\n");
			}
			csp_buffer_free(response_packet);
		}
	}
	else if (received_pack.module_t == MODULE_IMU && received_pack.action_t == READ_REGISTER_ADCS && received_pack.register_t == ALL_IMU_REGISTER)
	{
		axial_imu imu_data = {0};

		imu_data.x_imu = 55; // พี่ทัน
		imu_data.y_imu = 55; // พี่ทัน y_imu
		imu_data.z_imu = 55; // พี่ทัน z_imu

		csp_packet_t *response_packet = csp_buffer_get(sizeof(register_pack) + sizeof(axial_imu));
		if (response_packet != NULL)
		{
			memcpy(response_packet->data, &received_pack, sizeof(register_pack));
			memcpy(response_packet->data + sizeof(register_pack), &imu_data, sizeof(axial_imu));
			response_packet->length = sizeof(register_pack) + sizeof(axial_imu);
			if (!csp_send(conn, response_packet, 1000))
			{
				csp_log_error("Failed to send response\r\n");
			}
			csp_buffer_free(response_packet);
		}
	}

	else if (received_pack.module_t == MODULE_GPS && received_pack.action_t == READ_REGISTER_ADCS && received_pack.register_t == ALL_GPS_REGISTER)
	{
		gps_data_t gps_data = {0};

		gps_data.lat_gps = 55;	// พี่ทัน
		gps_data.long_gps = 55; // พี่ทัน y_imu
		gps_data.msl_gps = 55;	// พี่ทัน z_imu

		csp_packet_t *response_packet = csp_buffer_get(sizeof(register_pack) + sizeof(gps_data_t));
		if (response_packet != NULL)
		{
			memcpy(response_packet->data, &received_pack, sizeof(register_pack));
			memcpy(response_packet->data + sizeof(register_pack), &gps_data, sizeof(gps_data_t));
			response_packet->length = sizeof(register_pack) + sizeof(gps_data_t);
			if (!csp_send(conn, response_packet, 1000))
			{
				csp_log_error("Failed to send response\r\n");
			}
			csp_buffer_free(response_packet);
		}
	}
	else if (received_pack.module_t == MODULE_MTM && received_pack.action_t == READ_REGISTER_ADCS && received_pack.register_t == ALL_MTM_REGISTER)
	{
		axial_mtm mtm_data = {0};

		mtm_data.x_mtm = 55; // พี่ทัน
		mtm_data.y_mtm = 55; // พี่ทัน y_imu
		mtm_data.z_mtm = 55; // พี่ทัน z_imu

		csp_packet_t *response_packet = csp_buffer_get(sizeof(register_pack) + sizeof(axial_mtm));
		if (response_packet != NULL)
		{
			memcpy(response_packet->data, &received_pack, sizeof(register_pack));
			memcpy(response_packet->data + sizeof(register_pack), &mtm_data, sizeof(axial_mtm));
			response_packet->length = sizeof(register_pack) + sizeof(axial_mtm);
			if (!csp_send(conn, response_packet, 1000))
			{
				csp_log_error("Failed to send response\r\n");
			}
			csp_buffer_free(response_packet);
		}
	}
	csp_buffer_free(packet);
}
