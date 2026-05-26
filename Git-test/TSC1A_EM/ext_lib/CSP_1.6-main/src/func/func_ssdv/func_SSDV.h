/*
 * func_SSDV.h
 *
 *  Created on: Aug 9, 2024
 *      Author: thaib
 */

#ifndef SRC_FUNC_SSDV_H_
#define SRC_FUNC_SSDV_H_

#include <include/csp/csp.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ADDRESS_SSDV 14
#define PORT_SSDV 15

/**
 * Stuct for test SSDV
 * The structure `camera_dfl_t` contains fields for resolution, quality, camera enable status, and
 * interval, with specified default values.
 * @property {uint8_t} res - The `res` property in the `camera_dfl_t` struct is a uint8_t type and has
 * a default value of 0.
 * @property {uint8_t} quality - The `quality` property in the `camera_dfl_t` struct represents the
 * quality setting of the camera. It is a `uint8_t` type, which means it is an unsigned 8-bit integer.
 * This property likely stores the image quality setting for the camera, such as the resolution or
 * @property {uint8_t} camEnable - The `camEnable` property in the `camera_dfl_t` struct is a uint8_t
 * type with a default value of 1. It is used to indicate whether the camera is enabled or not. A value
 * of 1 typically means the camera is enabled, while a value of 0
 * @property {uint32_t} interval - The `interval` property in the `camera_dfl_t` struct represents the
 * time interval in seconds between capturing images using the camera. In this case, the default value
 * for `interval` is 60 seconds.
 */
typedef struct {
	uint8_t res; //0
	uint8_t quality; //1
	uint8_t camEnable; //1
	uint32_t interval; //60
} __attribute__((packed)) camera_dfl_t;

/**
 * The function `set_ssdv_Parameter` sets parameters for a camera and sends them over a CSP connection.
 * 
 * @param res The `res` parameter in the `set_ssdv_Parameter` function is used to specify the
 * resolution setting for the camera. It is of type `uint8_t`, which means it can hold values from 0 to
 * 255.
 * @param quality Quality is a parameter that represents the image quality setting for the SSDV (Slow
 * Scan Digital Video) transmission. It is typically a value between 0 and 255, where higher values
 * indicate better image quality but may require more bandwidth for transmission.
 * @param camEnable The `camEnable` parameter in the `set_ssdv_Parameter` function is used to enable or
 * disable the camera. It is a uint8_t type parameter that determines whether the camera is enabled or
 * disabled in the configuration. A value of 1 typically represents enabled, while a value of
 * @param interval The `interval` parameter in the `set_ssdv_Parameter` function is of type `uint32_t`
 * and represents the time interval in milliseconds. This parameter is used to set the interval for
 * some camera-related functionality in the code snippet provided.
 * 
 * @return The function `set_ssdv_Parameter` returns an integer value, which could be one of the
 * following:
 * - `CSP_ERR_TX` if the result of the ping operation is less than 0
 * - `CSP_ERR_NONE` if the data in the packet received from the connection is 0x55
 * - `CSP_ERR_TIMEDOUT` if the function times out before
 */
int set_ssdv_Parameter(uint8_t res , uint8_t quality , uint8_t camEnable ,uint32_t interval);

// get parameter for ssdv
int get_ssdv_Parameter(uint8_t *res, uint8_t *quality, uint8_t *camEnable, uint32_t *interval);


/**
 * The function reset_ssdv_Parameter resets the SSDV parameters to zero.
 * 
 * @return The function `reset_ssdv_Parameter()` is returning the result of calling the function
 * `set_ssdv_Parameter(0, 0, 0, 0)`.
 */
int reset_ssdv_Parameter();

/**
 * The function `check_ssdv_Connection` pings a specified address and returns an error code if the ping
 * fails or the result time in milliseconds if successful.
 * 
 * @return The function `check_ssdv_Connection` returns the result of the `csp_ping` function call. If
 * the result is less than 0, it returns `CSP_ERR_TX`. Otherwise, it returns the result of the
 * `csp_ping` function.
 */
int check_ssdv_Connection();

//ควบคุมกล้อง ถ้ามี
int control_ssdv_Camera(uint8_t command);

//บันทึกวิดีโอ ถ้ามี
int start_ssdv_Recording();

//หยุดวิดีโอ ถ้ามี
int stop_ssdv_Recording();

//get status ssdv
int get_ssdv_Status(uint8_t *status);
#ifdef __cplusplus
 }
#endif
#endif /* SRC_FUNC_SSDV_H_ */
