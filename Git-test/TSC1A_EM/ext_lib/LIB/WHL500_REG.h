/*
 * WHL500_REG.h
 *
 *  Created on: May 13, 2024
 *      Author: Chinn
 */

#ifndef INC_WHL500_REG_H_
#define INC_WHL500_REG_H_


#include <REG_MAP/KMUTNB_ADCS_REG.h>



KMUTNB_ADCS_REG whl500_reg[4][413];

enum  WHLNUM
{
	WHEEL1 = 0,
	WHEEL2 = 1,
	WHEEL3 = 2,
	WHEEL4 = 3,
};

enum WHL500_REG
{
	gndwdt = 1,
	csp_can_addr = 10,
	csp_can_mask = 11,
	csp_can_promisc = 14,
	csp_version     = 16,
	csp_dedup  = 17,
	boot_img1  = 20,
	boot_img0  = 21,
	boot_img2  = 22,
	boot_img3  = 23,
	boot_cur   = 24,
	boot_cnt   = 25,
	boot_err   = 26,
	stdbuf_in  = 28,
	stdbuf_out = 29,
	temp = 30,
	serial0 = 31,
	serial1 = 32,
	serial2 = 33,
	serial3 = 34,
	rtos_print = 47,
	csp_buf_out = 51,
	csp_conn_out =52,
	csp_conn_ovf = 53,
	csp_conn_noroute = 54,
	csp_inval_reply = 55,
	csp_errno = 56,
	csp_can_errno = 57,
	csp_print_rdp = 58,
	csp_print_packet = 59,
	hall_nibble = 100,
	hall_cnt = 101,
	hall_dir = 104,
	angle = 105,
	sine = 106,
	amplitude = 107,
	sine_isr_ticks = 108,
	angle_offset = 109,
	sine_period = 110,
	cooldown = 111,
	waveform = 112,
	angle_advance = 113,
	angle_hyst = 114,
	angle_gain = 115,
	protect_cnt = 117,
	hall0_isr_ticks = 118,
	hall0_isr_count = 119,
	diff_angle = 123,
	sine_isr_inter = 124,
	switch_angle = 129,
	rpm_err = 132,
	rpm_err_sum = 133,
	pid_en = 134,
	kp = 135,
	ki = 136,
	kd = 137,
	max_windup = 138,
	mrpm = 143,
	momentum = 144,
	sine_bc_flip = 145,
	hall_flip_order = 146,
	hall_flip_dir = 147,
	hall_err = 148,
	angle_open = 149,
	angle_open_rate = 150,
	torque_filtered = 151,
	torque_latest = 152,
	tmp_fault_io = 156,
	kpi_low_rpm = 159,
	kpi_high_rpm = 160,
	kp_low =   161,
	kp_high =  162,
	ki_high =  163,
	ki_low = 164,
	period0 = 205,
	period0_mean = 207,
	period_total = 222,
	temp_brd = 230,
	current = 231,
	curlim_soft = 232,
	curlim_hard = 233,
	templim_hard = 234,
	auto_resume = 236,
	temp_st1 = 237,
	temp_st2 = 238,
	voltage = 239,
	pwm_isr_ticks = 240,
	pwm_period = 242,
	temp_ina = 245,
	pwm_polarity = 246,
	pwm_dither_sum = 247,
	amplitude_max_step = 249,
	drv_fault = 250,
	drv_enable = 252,
	drv_enable_init = 253,
	moment_of_inertia_mg2pi = 259,
	moment_of_inertia = 260,
	momentum_set = 261,
	torque_set = 262,
	odometer = 266,
	torque_duration = 281,
	amplitude_max = 284,
	test_ampl = 285,
	test_freq = 286,
	ctrl_ticks = 287,
	mrpm_target = 289,
	mrpm_set = 290,
	mrpm_per_sec = 291,
	mrpm_max = 292,
	mrpm_per_sec_max= 293,
	ctrl_late = 294,
	stuck_protect_en = 295,
	ina_fault_io = 301,
	ina_config = 302,
	ina_adcconfig = 303,
	ina_currlsbcalc = 304,
	ina_tempcoconfig = 305,
	ina_vshunt = 306,
	ina_vbus = 307,
	ina_dietemp = 308,
	ina_current = 309,
	ina_power = 310,
	ina_energy = 311,
	ina_charge = 312,
	ina_diag_alert = 313,
	ina_shunt_ov_lvl = 314,
	ina_shunt_uv_lvl = 315,
	ina_bus_ov_lvl = 316,
	ina_bus_uv_lvl = 317,
	ina_temp_lim = 318,
	ina_pwr_lim = 319,
	ina_manuf_id = 320,
	ina_device_id = 321,
	tmp_result = 400,
	tmp_slew = 401,
	tmp_alert_status = 402,
	tmp_conf = 403,
	tmp_alert_enable = 404,
	tmp_limit_low = 405,
	tmp_limit_high = 406,
	tmp_slew_limit = 408,
	tmp_uid1 = 409,
	tmp_uid2 = 410,
	tmp_uid3 = 411,
	tmp_dieid = 412,


};

typedef struct{

	uint32_t WHL_GNDWDT_D = 0;
	uint16_t WHL_CSP_CAN_ADDR_D = 0;
	uint8_t  WHL_CSP_CAN_MASK_D = 0;
	uint8_t  WHL_CSP_CAN_PROMISC_D = 0;
	uint8_t  WHL_CSP_VERSION_D = 0;
	uint8_t  WHL_CSP_DEDUP_D = 0;
	uint8_t  WHL_BOOT_IMG1_D = 0;
	uint8_t  WHL_BOOT_IMG0_D = 0;
	uint8_t  WHL_BOOT_IMG2_D = 0;
	uint8_t  WHL_BOOT_IMG3_D = 0;
	uint8_t  WHL_BOOT_CUR_D = 0;
	uint8_t  WHL_BOOT_CNT_D = 0;
	uint8_t  WHL_BOOT_ERR_D = 0;
	uint16_t WHL_STDBUF_IN_D = 0;
	uint16_t WHL_STDBUF_OUT_D = 0;
	int16_t  WHL_TEMP_D = 0;
	uint32_t WHL_SERIAL0_D = 0;
	uint32_t WHL_SERIAL1_D = 0;
	uint32_t WHL_SERIAL2_D = 0;
	uint32_t WHL_SERIAL3_D = 0;
	uint8_t  WHL_RTOS_PRINT_D = 0;
	uint8_t  WHL_CSP_BUF_OUT_D = 0;
	uint8_t  WHL_CSP_CONN_OUT_D = 0;
	uint8_t  WHL_CSP_CONN_OVF_D = 0;
	uint8_t  WHL_CSP_CONN_NOROUTE_D = 0;
	uint8_t  WHL_CSP_INVAL_REPLY_D = 0;
	uint8_t  WHL_CSP_ERRNO_D = 0;
	uint8_t  WHL_CSP_CAN_ERRNO_D = 0;
	uint8_t  WHL_CSP_PRINT_RDP_D = 0;
	uint8_t  WHL_CSP_PRINT_PACKET_D = 0;
	uint32_t  WHL_HALL_NIBBLE_D = 0;
	uint32_t  WHL_HALL_CNT_D = 0;
	int32_t  WHL_HALL_DIR_D = 0;
	int32_t  WHL_ANGLE_D = 0;
	int16_t  WHL_SINE_D = 0;
	int16_t  WHL_AMPLITUDE_D = 0;
	uint16_t WHL_SINE_ISR_TICKS_D = 0;
	int16_t  WHL_ANGLE_OFFSET_D = 0;
	uint16_t WHL_SINE_PERIOD_D = 0;
	uint16_t WHL_COOLDOWN_D =0;
	uint8_t  WHL_WAVEFORM_D = 0;
	int32_t  WHL_ANGLE_ADVANCE_D =0;
	uint8_t  WHL_ANGLE_HYST_D =0;
	uint16_t  WHL_ANGLE_GAIN_D =0;
	uint16_t WHL_PROTECT_CNT_D = 0;
	uint8_t  WHL_HALL0_ISR_TICKS_D = 0;
	uint8_t  WHL_HALL0_ISR_COUNT_D = 0;
	uint32_t WHL_DIFF_ANGLE_D = 0;
	uint16_t WHL_SINE_ISR_INTER_D = 0;
	uint32_t WHL_SWITCH_ANGLE_D = 0;
	float    WHL_RPM_ERR_D = 0;
	float    WHL_RPM_ERR_SUM_D =0;
	uint8_t  WHL_PID_EN_D = 0;
	float 	 WHL_KP_D;
	float 	 WHL_KI_D;
	float 	 WHL_KD_D;
	float 	 WHL_MAX_WINDUP_D;
	int32_t  WHL_MRPM_D = 0;
	int32_t  WHL_MOMENTUM_D = 0;
	uint8_t  WHL_SINE_BC_FLIP_D = 0;
	uint8_t  WHL_HALL_FLIP_ORDER_D = 0;
	uint8_t  WHL_HALL_FLIP_DIR_D = 0;
	uint32_t WHL_HALL_ERR_D =0;
	uint32_t WHL_ANGLE_OPEN_D =0;
	int16_t  WHL_ANGLE_OPEN_RATE_D = 0;
	float    WHL_TORQUE_FILTERED_D = 0;
	float    WHL_TORQUE_LATEST_D = 0;
	uint32_t WHL_TMP_FAULT_IO_D = 0;
	uint16_t WHL_KPI_LOW_RPM_D = 0;
	uint16_t WHL_KPI_HIGH_RPM_D = 0;
	float    WHL_KP_LOW_D = 0;
	float    WHL_KP_HIGH_D = 0;
	float    WHL_KI_HIGH_D = 0;
	float    WHL_KI_LOW_D = 0;
	uint32_t WHL_PERIOD0_D = 0;
	float    WHL_PERIOD0_MEAN_D = 0;
	uint32_t WHL_PERIOD_TOTAL_D = 0;
	int16_t  WHL_TEMP_BRD_D = 0;
	int16_t  WHL_CURRENT_D = 0;
	uint16_t WHL_CURLIM_SOFT_D = 0;
	uint16_t WHL_CURLIM_HARD_D = 0;
	uint16_t WHL_TEMPLIM_HARD_D = 0;
	uint8_t  WHL_AUTO_RESUME_D = 0;
	int16_t  WHL_TEMP_ST1_D = 0;
	int16_t  WHL_TEMP_ST2_D = 0;
	uint16_t WHL_VOLTAGE_D = 0;
	uint32_t WHL_PWM_ISR_TICKS_D = 0;
	uint16_t WHL_PWM_PERIOD_D = 0;
	int16_t  WHL_TEMP_INA_D = 0;
	uint8_t  WHL_PWM_POLARITY_D = 0;
	uint32_t WHL_PWM_DITHER_SUM_D = 0;
	uint16_t WHL_AMPLITUDE_MAX_STEP_D = 0;
	uint8_t  WHL_DRV_FAULT_D = 0;
	uint8_t  WHL_DRV_ENABLE_D = 0;
	uint8_t  WHL_DRV_ENABLE_INIT_D = 0;
	uint32_t WHL_MOMENT_OF_INERTIA_MG2PI_D = 0;
	float    WHL_MOMENT_OF_INERTIA_D = 0;
	float    WHL_MOMENTUM_SET_D = 0;
	float    WHL_TORQUE_SET_D = 0;
	uint16_t WHL_ODOMETER_D = 0;
	uint8_t  WHL_TORQUE_DURATION_D = 0;
	uint16_t WHL_AMPLITUDE_MAX_D = 0;
	float    WHL_TEST_AMPL_D = 0;
	float    WHL_TEST_FREQ_D = 0;
	uint32_t WHL_CTRL_TICKS_D = 0;
	int32_t  WHL_MRPM_TARGET_D = 0;
	int32_t  WHL_MRPM_SET_D = 0;
	int32_t  WHL_MRPM_PER_SEC_D = 0;
	uint32_t WHL_MRPM_MAX_D = 0;
	uint32_t WHL_MRPM_PER_MAX_D = 0;
	uint32_t WHL_CTRL_LATE_D = 0;
	uint8_t  WHL_STUCK_PROTECT_EN_D = 0;
	uint32_t WHL_INA_FAULT_IO_D = 0;
	int16_t  WHL_INA_CONFIG_D = 0;
	int16_t  WHL_INA_ADCCONFIG_D = 0;
	int16_t  WHL_INA_CURRLSBCALC_D = 0;
	int16_t  WHL_INA_TEMPCOCONFIG_D = 0;
	uint8_t  WHL_INA_VSHUNT_D = 0;
	uint8_t  WHL_INA_VBUS_D = 0;
	int16_t  WHL_INA_DIETEMP_D = 0;
	uint8_t  WHL_INA_CURRENT_D = 0;
	uint8_t  WHL_INA_POWER_D = 0;
	uint8_t  WHL_INA_ENERGY_D = 0;
	uint8_t  WHL_INA_CHARGE_D = 0;
	int16_t  WHL_INA_DIAG_ALERT_D = 0;
	int16_t  WHL_INA_SHUNT_OV_lVl_D = 0;
	int16_t  WHL_INA_SHUNT_UV_lVl_D = 0;
	int16_t  WHL_INA_BUS_OV_lVl_D = 0;
	int16_t  WHL_INA_BUS_UV_lVl_D = 0;
	int16_t  WHL_INA_TEMP_LIM_D = 0;
	int16_t  WHL_INA_PWR_LIM_D = 0;
	int16_t  WHL_INA_MANUF_ID_D = 0;
	int16_t  WHL_INA_DEVICE_ID_D = 0;
	int16_t  WHL_TMP_RESULT_D = 0;
	int16_t  WHL_TMP_SLEW_D = 0;
	int16_t  WHL_TMP_ALERT_STATUS_D = 0;
	int16_t  WHL_TMP_CONF_D = 0;
	int16_t  WHL_TMP_ALERT_ENABLE_D = 0;
	int16_t  WHL_TMP_LIMIT_LOW_D = 0;
	int16_t  WHL_TMP_LIMIT_HIGH_D = 0;
	int16_t  WHL_TMP_HYSTERESIS_D = 0;
	int16_t  WHL_TMP_SLEW_LIMIT_D = 0;
	int16_t  WHL_TMP_UID1_D = 0;
	int16_t  WHL_TMP_UID2_D = 0;
	int16_t  WHL_TMP_UID3_D = 0;
	int16_t  WHL_TMP_DIEID_D = 0;
}Register;

Register whlreg[4];


void whl500_reg_setup(uint8_t num)
{
	whl500_reg[num][gndwdt].init((uint8_t*)&whlreg[num].WHL_GNDWDT_D,sizeof(whlreg[num].WHL_GNDWDT_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][csp_can_addr].init((uint8_t*)&whlreg[num].WHL_CSP_CAN_ADDR_D,sizeof(whlreg[num].WHL_CSP_CAN_ADDR_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][csp_can_mask].init((uint8_t*)&whlreg[num].WHL_CSP_CAN_MASK_D,sizeof(whlreg[num].WHL_CSP_CAN_MASK_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][csp_can_promisc].init((uint8_t*)&whlreg[num].WHL_CSP_CAN_PROMISC_D,sizeof(whlreg[num].WHL_CSP_CAN_PROMISC_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][csp_version].init((uint8_t*)&whlreg[num].WHL_CSP_VERSION_D,sizeof(whlreg[num].WHL_CSP_VERSION_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][csp_dedup].init((uint8_t*)&whlreg[num].WHL_CSP_DEDUP_D,sizeof(whlreg[num].WHL_CSP_DEDUP_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][boot_img1].init((uint8_t*)&whlreg[num].WHL_BOOT_IMG1_D,sizeof(whlreg[num].WHL_BOOT_IMG1_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][boot_img0].init((uint8_t*)&whlreg[num].WHL_BOOT_IMG0_D,sizeof(whlreg[num].WHL_BOOT_IMG0_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][boot_img2].init((uint8_t*)&whlreg[num].WHL_BOOT_IMG2_D,sizeof(whlreg[num].WHL_BOOT_IMG2_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][boot_img3].init((uint8_t*)&whlreg[num].WHL_BOOT_IMG3_D,sizeof(whlreg[num].WHL_BOOT_IMG3_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][boot_cur].init((uint8_t*)&whlreg[num].WHL_BOOT_CUR_D,sizeof(whlreg[num].WHL_BOOT_CUR_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][boot_cnt].init((uint8_t*)&whlreg[num].WHL_BOOT_CNT_D,sizeof(whlreg[num].WHL_BOOT_CNT_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][boot_err].init((uint8_t*)&whlreg[num].WHL_BOOT_ERR_D,sizeof(whlreg[num].WHL_BOOT_ERR_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][stdbuf_in].init((uint8_t*)&whlreg[num].WHL_STDBUF_IN_D,sizeof(whlreg[num].WHL_STDBUF_IN_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][stdbuf_out].init((uint8_t*)&whlreg[num].WHL_STDBUF_OUT_D,sizeof(whlreg[num].WHL_STDBUF_OUT_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][temp].init((uint8_t*)&whlreg[num].WHL_TEMP_D,sizeof(whlreg[num].WHL_TEMP_D),(uint8_t)data_type::_16L);
	whl500_reg[num][serial0].init((uint8_t*)&whlreg[num].WHL_SERIAL0_D,sizeof(whlreg[num].WHL_SERIAL0_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][serial1].init((uint8_t*)&whlreg[num].WHL_SERIAL1_D,sizeof(whlreg[num].WHL_SERIAL1_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][serial2].init((uint8_t*)&whlreg[num].WHL_SERIAL2_D,sizeof(whlreg[num].WHL_SERIAL2_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][serial3].init((uint8_t*)&whlreg[num].WHL_SERIAL3_D,sizeof(whlreg[num].WHL_SERIAL3_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][rtos_print].init((uint8_t*)&whlreg[num].WHL_RTOS_PRINT_D,sizeof(whlreg[num].WHL_RTOS_PRINT_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][csp_buf_out].init((uint8_t*)&whlreg[num].WHL_CSP_BUF_OUT_D,sizeof(whlreg[num].WHL_CSP_BUF_OUT_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][csp_conn_out].init((uint8_t*)&whlreg[num].WHL_CSP_CONN_OUT_D,sizeof(whlreg[num].WHL_CSP_CONN_OUT_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][csp_conn_ovf].init((uint8_t*)&whlreg[num].WHL_CSP_CONN_OVF_D,sizeof(whlreg[num].WHL_CSP_CONN_OVF_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][csp_conn_noroute].init((uint8_t*)&whlreg[num].WHL_CSP_CONN_NOROUTE_D,sizeof(whlreg[num].WHL_CSP_CONN_NOROUTE_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][csp_inval_reply].init((uint8_t*)&whlreg[num].WHL_CSP_INVAL_REPLY_D,sizeof(whlreg[num].WHL_CSP_INVAL_REPLY_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][csp_errno].init((uint8_t*)&whlreg[num].WHL_CSP_ERRNO_D,sizeof(whlreg[num].WHL_CSP_ERRNO_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][csp_can_errno].init((uint8_t*)&whlreg[num].WHL_CSP_CAN_ERRNO_D,sizeof(whlreg[num].WHL_CSP_CAN_ERRNO_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][csp_print_rdp].init((uint8_t*)&whlreg[num].WHL_CSP_PRINT_RDP_D,sizeof(whlreg[num].WHL_CSP_PRINT_RDP_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][csp_print_packet].init((uint8_t*)&whlreg[num].WHL_CSP_PRINT_PACKET_D,sizeof(whlreg[num].WHL_CSP_PRINT_PACKET_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][hall_nibble].init((uint8_t*)&whlreg[num].WHL_HALL_NIBBLE_D,sizeof(whlreg[num].WHL_HALL_NIBBLE_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][hall_cnt].init((uint8_t*)&whlreg[num].WHL_HALL_CNT_D,sizeof(whlreg[num].WHL_HALL_CNT_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][hall_dir].init((uint8_t*)&whlreg[num].WHL_HALL_DIR_D,sizeof(whlreg[num].WHL_HALL_DIR_D),(uint8_t)data_type::_32L);
	whl500_reg[num][angle].init((uint8_t*)&whlreg[num].WHL_ANGLE_D,sizeof(whlreg[num].WHL_ANGLE_D),(uint8_t)data_type::_32L);
	whl500_reg[num][sine].init((uint8_t*)&whlreg[num].WHL_SINE_D,sizeof(whlreg[num].WHL_SINE_D),(uint8_t)data_type::_16L);
	whl500_reg[num][amplitude].init((uint8_t*)&whlreg[num].WHL_AMPLITUDE_D,sizeof(whlreg[num].WHL_AMPLITUDE_D),(uint8_t)data_type::_16L);
	whl500_reg[num][sine_isr_ticks].init((uint8_t*)&whlreg[num].WHL_SINE_ISR_TICKS_D,sizeof(whlreg[num].WHL_SINE_ISR_TICKS_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][angle_offset].init((uint8_t*)&whlreg[num].WHL_ANGLE_OFFSET_D,sizeof(whlreg[num].WHL_ANGLE_OFFSET_D),(uint8_t)data_type::_16L);
	whl500_reg[num][sine_period].init((uint8_t*)&whlreg[num].WHL_SINE_PERIOD_D,sizeof(whlreg[num].WHL_SINE_PERIOD_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][cooldown].init((uint8_t*)&whlreg[num].WHL_COOLDOWN_D,sizeof(whlreg[num].WHL_COOLDOWN_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][waveform].init((uint8_t*)&whlreg[num].WHL_WAVEFORM_D,sizeof(whlreg[num].WHL_WAVEFORM_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][angle_advance].init((uint8_t*)&whlreg[num].WHL_ANGLE_ADVANCE_D,sizeof(whlreg[num].WHL_ANGLE_ADVANCE_D),(uint8_t)data_type::_32L);
	whl500_reg[num][angle_hyst].init((uint8_t*)&whlreg[num].WHL_ANGLE_HYST_D,sizeof(whlreg[num].WHL_ANGLE_HYST_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][angle_gain].init((uint8_t*)&whlreg[num].WHL_ANGLE_GAIN_D,sizeof(whlreg[num].WHL_ANGLE_GAIN_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][protect_cnt].init((uint8_t*)&whlreg[num].WHL_PROTECT_CNT_D,sizeof(whlreg[num].WHL_PROTECT_CNT_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][hall0_isr_ticks].init((uint8_t*)&whlreg[num].WHL_HALL0_ISR_TICKS_D,sizeof(whlreg[num].WHL_HALL0_ISR_TICKS_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][hall0_isr_count].init((uint8_t*)&whlreg[num].WHL_HALL0_ISR_COUNT_D,sizeof(whlreg[num].WHL_HALL0_ISR_COUNT_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][diff_angle].init((uint8_t*)&whlreg[num].WHL_DIFF_ANGLE_D,sizeof(whlreg[num].WHL_DIFF_ANGLE_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][sine_isr_inter].init((uint8_t*)&whlreg[num].WHL_SINE_ISR_INTER_D,sizeof(whlreg[num].WHL_SINE_ISR_INTER_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][switch_angle].init((uint8_t*)&whlreg[num].WHL_SWITCH_ANGLE_D,sizeof(whlreg[num].WHL_SWITCH_ANGLE_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][rpm_err].init((uint8_t*)&whlreg[num].WHL_RPM_ERR_D,sizeof(whlreg[num].WHL_RPM_ERR_D),(uint8_t)data_type::_32F);
	whl500_reg[num][rpm_err_sum].init((uint8_t*)&whlreg[num].WHL_RPM_ERR_SUM_D,sizeof(whlreg[num].WHL_RPM_ERR_SUM_D),(uint8_t)data_type::_32F);
	whl500_reg[num][pid_en].init((uint8_t*)&whlreg[num].WHL_PID_EN_D,sizeof(whlreg[num].WHL_PID_EN_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][kp].init((uint8_t*)&whlreg[num].WHL_KP_D,sizeof(whlreg[num].WHL_KP_D),(uint8_t)data_type::_32F);
	whl500_reg[num][ki].init((uint8_t*)&whlreg[num].WHL_KI_D,sizeof(whlreg[num].WHL_KI_D),(uint8_t)data_type::_32F);
	whl500_reg[num][kd].init((uint8_t*)&whlreg[num].WHL_KD_D,sizeof(whlreg[num].WHL_KD_D),(uint8_t)data_type::_32F);
	whl500_reg[num][max_windup].init((uint8_t*)&whlreg[num].WHL_MAX_WINDUP_D,sizeof(whlreg[num].WHL_MAX_WINDUP_D),(uint8_t)data_type::_32F);
	whl500_reg[num][mrpm].init((uint8_t*)&whlreg[num].WHL_MRPM_D,sizeof(whlreg[num].WHL_MRPM_D),(uint8_t)data_type::_32L);
	whl500_reg[num][momentum].init((uint8_t*)&whlreg[num].WHL_MOMENTUM_D,sizeof(whlreg[num].WHL_MOMENTUM_D),(uint8_t)data_type::_32L);
	whl500_reg[num][sine_bc_flip].init((uint8_t*)&whlreg[num].WHL_SINE_BC_FLIP_D,sizeof(whlreg[num].WHL_SINE_BC_FLIP_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][hall_flip_order].init((uint8_t*)&whlreg[num].WHL_HALL_FLIP_ORDER_D,sizeof(whlreg[num].WHL_HALL_FLIP_ORDER_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][hall_flip_dir].init((uint8_t*)&whlreg[num].WHL_HALL_FLIP_DIR_D,sizeof(whlreg[num].WHL_HALL_FLIP_DIR_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][hall_err].init((uint8_t*)&whlreg[num].WHL_HALL_ERR_D,sizeof(whlreg[num].WHL_HALL_ERR_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][angle_open].init((uint8_t*)&whlreg[num].WHL_ANGLE_OPEN_D,sizeof(whlreg[num].WHL_ANGLE_OPEN_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][angle_open_rate].init((uint8_t*)&whlreg[num].WHL_ANGLE_OPEN_RATE_D,sizeof(whlreg[num].WHL_ANGLE_OPEN_RATE_D),(uint8_t)data_type::_16L);
	whl500_reg[num][torque_filtered].init((uint8_t*)&whlreg[num].WHL_TORQUE_FILTERED_D,sizeof(whlreg[num].WHL_TORQUE_FILTERED_D),(uint8_t)data_type::_32F);
	whl500_reg[num][torque_latest].init((uint8_t*)&whlreg[num].WHL_TORQUE_LATEST_D,sizeof(whlreg[num].WHL_TORQUE_LATEST_D),(uint8_t)data_type::_32F);
	whl500_reg[num][tmp_fault_io].init((uint8_t*)&whlreg[num].WHL_TMP_FAULT_IO_D,sizeof(whlreg[num].WHL_TMP_FAULT_IO_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][kpi_low_rpm].init((uint8_t*)&whlreg[num].WHL_KPI_LOW_RPM_D,sizeof(whlreg[num].WHL_KPI_LOW_RPM_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][kpi_high_rpm].init((uint8_t*)&whlreg[num].WHL_KPI_HIGH_RPM_D,sizeof(whlreg[num].WHL_KPI_HIGH_RPM_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][kp_low].init((uint8_t*)&whlreg[num].WHL_KP_LOW_D,sizeof(whlreg[num].WHL_KP_LOW_D),(uint8_t)data_type::_32F);
	whl500_reg[num][kp_high].init((uint8_t*)&whlreg[num].WHL_KP_HIGH_D,sizeof(whlreg[num].WHL_KP_HIGH_D),(uint8_t)data_type::_32F);
	whl500_reg[num][ki_high].init((uint8_t*)&whlreg[num].WHL_KI_HIGH_D,sizeof(whlreg[num].WHL_KI_HIGH_D),(uint8_t)data_type::_32F);
	whl500_reg[num][ki_low].init((uint8_t*)&whlreg[num].WHL_KI_LOW_D,sizeof(whlreg[num].WHL_KI_LOW_D),(uint8_t)data_type::_32F);
	whl500_reg[num][period0].init((uint8_t*)&whlreg[num].WHL_PERIOD0_D,sizeof(whlreg[num].WHL_PERIOD0_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][period0_mean].init((uint8_t*)&whlreg[num].WHL_PERIOD0_MEAN_D,sizeof(whlreg[num].WHL_PERIOD0_MEAN_D),(uint8_t)data_type::_32F);
	whl500_reg[num][period_total].init((uint8_t*)&whlreg[num].WHL_PERIOD_TOTAL_D,sizeof(whlreg[num].WHL_PERIOD_TOTAL_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][temp_brd].init((uint8_t*)&whlreg[num].WHL_TEMP_BRD_D,sizeof(whlreg[num].WHL_TEMP_BRD_D),(uint8_t)data_type::_16L);
	whl500_reg[num][current].init((uint8_t*)&whlreg[num].WHL_CURRENT_D,sizeof(whlreg[num].WHL_CURRENT_D),(uint8_t)data_type::_16L);
	whl500_reg[num][curlim_soft].init((uint8_t*)&whlreg[num].WHL_CURLIM_SOFT_D,sizeof(whlreg[num].WHL_CURLIM_SOFT_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][curlim_hard].init((uint8_t*)&whlreg[num].WHL_CURLIM_HARD_D,sizeof(whlreg[num].WHL_CURLIM_HARD_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][templim_hard].init((uint8_t*)&whlreg[num].WHL_TEMPLIM_HARD_D,sizeof(whlreg[num].WHL_TEMPLIM_HARD_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][auto_resume].init((uint8_t*)&whlreg[num].WHL_AUTO_RESUME_D,sizeof(whlreg[num].WHL_AUTO_RESUME_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][temp_st1].init((uint8_t*)&whlreg[num].WHL_TEMP_ST1_D,sizeof(whlreg[num].WHL_TEMP_ST1_D),(uint8_t)data_type::_16L);
	whl500_reg[num][temp_st2].init((uint8_t*)&whlreg[num].WHL_TEMP_ST2_D,sizeof(whlreg[num].WHL_TEMP_ST2_D),(uint8_t)data_type::_16L);
	whl500_reg[num][voltage].init((uint8_t*)&whlreg[num].WHL_VOLTAGE_D,sizeof(whlreg[num].WHL_VOLTAGE_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][pwm_isr_ticks].init((uint8_t*)&whlreg[num].WHL_PWM_ISR_TICKS_D,sizeof(whlreg[num].WHL_PWM_ISR_TICKS_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][pwm_period].init((uint8_t*)&whlreg[num].WHL_PWM_PERIOD_D,sizeof(whlreg[num].WHL_PWM_PERIOD_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][temp_ina].init((uint8_t*)&whlreg[num].WHL_TEMP_INA_D,sizeof(whlreg[num].WHL_TEMP_INA_D),(uint8_t)data_type::_16L);
	whl500_reg[num][pwm_polarity].init((uint8_t*)&whlreg[num].WHL_PWM_POLARITY_D,sizeof(whlreg[num].WHL_PWM_POLARITY_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][pwm_dither_sum].init((uint8_t*)&whlreg[num].WHL_PWM_DITHER_SUM_D,sizeof(whlreg[num].WHL_PWM_DITHER_SUM_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][amplitude_max_step].init((uint8_t*)&whlreg[num].WHL_AMPLITUDE_MAX_STEP_D,sizeof(whlreg[num].WHL_AMPLITUDE_MAX_STEP_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][drv_fault].init((uint8_t*)&whlreg[num].WHL_DRV_FAULT_D,sizeof(whlreg[num].WHL_DRV_FAULT_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][drv_enable].init((uint8_t*)&whlreg[num].WHL_DRV_ENABLE_D,sizeof(whlreg[num].WHL_DRV_ENABLE_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][drv_enable_init].init((uint8_t*)&whlreg[num].WHL_DRV_ENABLE_INIT_D,sizeof(whlreg[num].WHL_DRV_ENABLE_INIT_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][moment_of_inertia_mg2pi].init((uint8_t*)&whlreg[num].WHL_MOMENT_OF_INERTIA_MG2PI_D,sizeof(whlreg[num].WHL_MOMENT_OF_INERTIA_MG2PI_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][moment_of_inertia].init((uint8_t*)&whlreg[num].WHL_MOMENT_OF_INERTIA_D,sizeof(whlreg[num].WHL_MOMENT_OF_INERTIA_D),(uint8_t)data_type::_32F);
	whl500_reg[num][momentum_set].init((uint8_t*)&whlreg[num].WHL_MOMENTUM_SET_D,sizeof(whlreg[num].WHL_MOMENTUM_SET_D),(uint8_t)data_type::_32F);
	whl500_reg[num][torque_set].init((uint8_t*)&whlreg[num].WHL_TORQUE_SET_D,sizeof(whlreg[num].WHL_TORQUE_SET_D),(uint8_t)data_type::_32F);
	whl500_reg[num][odometer].init((uint8_t*)&whlreg[num].WHL_ODOMETER_D,sizeof(whlreg[num].WHL_ODOMETER_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][torque_duration].init((uint8_t*)&whlreg[num].WHL_TORQUE_DURATION_D,sizeof(whlreg[num].WHL_TORQUE_DURATION_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][amplitude_max].init((uint8_t*)&whlreg[num].WHL_AMPLITUDE_MAX_D,sizeof(whlreg[num].WHL_AMPLITUDE_MAX_D),(uint8_t)data_type::_16UL);
	whl500_reg[num][test_ampl].init((uint8_t*)&whlreg[num].WHL_TEST_AMPL_D,sizeof(whlreg[num].WHL_TEST_AMPL_D),(uint8_t)data_type::_32F);
	whl500_reg[num][test_freq].init((uint8_t*)&whlreg[num].WHL_TEST_FREQ_D,sizeof(whlreg[num].WHL_TEST_FREQ_D),(uint8_t)data_type::_32F);
	whl500_reg[num][ctrl_ticks].init((uint8_t*)&whlreg[num].WHL_CTRL_TICKS_D,sizeof(whlreg[num].WHL_CTRL_TICKS_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][mrpm_target].init((uint8_t*)&whlreg[num].WHL_MRPM_TARGET_D,sizeof(whlreg[num].WHL_MRPM_TARGET_D),(uint8_t)data_type::_32L);
	whl500_reg[num][mrpm_set].init((uint8_t*)&whlreg[num].WHL_MRPM_SET_D,sizeof(whlreg[num].WHL_MRPM_SET_D),(uint8_t)data_type::_32L);
	whl500_reg[num][mrpm_per_sec].init((uint8_t*)&whlreg[num].WHL_MRPM_PER_SEC_D,sizeof(whlreg[num].WHL_MRPM_PER_SEC_D),(uint8_t)data_type::_32L);
	whl500_reg[num][mrpm_max].init((uint8_t*)&whlreg[num].WHL_MRPM_MAX_D,sizeof(whlreg[num].WHL_MRPM_MAX_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][mrpm_per_sec_max].init((uint8_t*)&whlreg[num].WHL_MRPM_PER_MAX_D,sizeof(whlreg[num].WHL_MRPM_PER_MAX_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][ctrl_late].init((uint8_t*)&whlreg[num].WHL_CTRL_LATE_D,sizeof(whlreg[num].WHL_CTRL_LATE_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][stuck_protect_en].init((uint8_t*)&whlreg[num].WHL_STUCK_PROTECT_EN_D,sizeof(whlreg[num].WHL_STUCK_PROTECT_EN_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][ina_fault_io].init((uint8_t*)&whlreg[num].WHL_INA_FAULT_IO_D,sizeof(whlreg[num].WHL_INA_FAULT_IO_D),(uint8_t)data_type::_32UL);
	whl500_reg[num][ina_config].init((uint8_t*)&whlreg[num].WHL_INA_CONFIG_D,sizeof(whlreg[num].WHL_INA_CONFIG_D),(uint8_t)data_type::_16L);
	whl500_reg[num][ina_adcconfig].init((uint8_t*)&whlreg[num].WHL_INA_ADCCONFIG_D,sizeof(whlreg[num].WHL_INA_ADCCONFIG_D),(uint8_t)data_type::_16L);
	whl500_reg[num][ina_currlsbcalc].init((uint8_t*)&whlreg[num].WHL_INA_CURRLSBCALC_D,sizeof(whlreg[num].WHL_INA_CURRLSBCALC_D),(uint8_t)data_type::_16L);
	whl500_reg[num][ina_tempcoconfig].init((uint8_t*)&whlreg[num].WHL_INA_TEMPCOCONFIG_D,sizeof(whlreg[num].WHL_INA_TEMPCOCONFIG_D),(uint8_t)data_type::_16L);
	whl500_reg[num][ina_vshunt].init((uint8_t*)&whlreg[num].WHL_INA_VSHUNT_D,sizeof(whlreg[num].WHL_INA_VSHUNT_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][ina_vbus].init((uint8_t*)&whlreg[num].WHL_INA_VBUS_D,sizeof(whlreg[num].WHL_INA_VBUS_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][ina_dietemp].init((uint8_t*)&whlreg[num].WHL_INA_DIETEMP_D,sizeof(whlreg[num].WHL_INA_DIETEMP_D),(uint8_t)data_type::_16L);
	whl500_reg[num][ina_current].init((uint8_t*)&whlreg[num].WHL_INA_CURRENT_D,sizeof(whlreg[num].WHL_INA_CURRENT_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][ina_power].init((uint8_t*)&whlreg[num].WHL_INA_POWER_D,sizeof(whlreg[num].WHL_INA_POWER_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][ina_energy].init((uint8_t*)&whlreg[num].WHL_INA_ENERGY_D,sizeof(whlreg[num].WHL_INA_ENERGY_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][ina_charge].init((uint8_t*)&whlreg[num].WHL_INA_CHARGE_D,sizeof(whlreg[num].WHL_INA_CHARGE_D),(uint8_t)data_type::_8UL);
	whl500_reg[num][ina_diag_alert].init((uint8_t*)&whlreg[num].WHL_INA_DIAG_ALERT_D,sizeof(whlreg[num].WHL_INA_DIAG_ALERT_D),(uint8_t)data_type::_16L);
	whl500_reg[num][ina_shunt_ov_lvl].init((uint8_t*)&whlreg[num].WHL_INA_SHUNT_OV_lVl_D,sizeof(whlreg[num].WHL_INA_SHUNT_OV_lVl_D),(uint8_t)data_type::_16L);
	whl500_reg[num][ina_shunt_uv_lvl].init((uint8_t*)&whlreg[num].WHL_INA_SHUNT_UV_lVl_D,sizeof(whlreg[num].WHL_INA_SHUNT_UV_lVl_D),(uint8_t)data_type::_16L);
	whl500_reg[num][ina_bus_ov_lvl].init((uint8_t*)&whlreg[num].WHL_INA_BUS_OV_lVl_D,sizeof(whlreg[num].WHL_INA_BUS_OV_lVl_D),(uint8_t)data_type::_16L);
	whl500_reg[num][ina_bus_uv_lvl].init((uint8_t*)&whlreg[num].WHL_INA_BUS_UV_lVl_D,sizeof(whlreg[num].WHL_INA_BUS_UV_lVl_D),(uint8_t)data_type::_16L);
	whl500_reg[num][ina_temp_lim].init((uint8_t*)&whlreg[num].WHL_INA_TEMP_LIM_D,sizeof(whlreg[num].WHL_INA_TEMP_LIM_D),(uint8_t)data_type::_16L);
	whl500_reg[num][ina_pwr_lim].init((uint8_t*)&whlreg[num].WHL_INA_PWR_LIM_D,sizeof(whlreg[num].WHL_INA_PWR_LIM_D),(uint8_t)data_type::_16L);
	whl500_reg[num][ina_manuf_id].init((uint8_t*)&whlreg[num].WHL_INA_MANUF_ID_D,sizeof(whlreg[num].WHL_INA_MANUF_ID_D),(uint8_t)data_type::_16L);
	whl500_reg[num][ina_device_id].init((uint8_t*)&whlreg[num].WHL_INA_DEVICE_ID_D,sizeof(whlreg[num].WHL_INA_DEVICE_ID_D),(uint8_t)data_type::_16L);
	whl500_reg[num][tmp_result].init((uint8_t*)&whlreg[num].WHL_TMP_RESULT_D,sizeof(whlreg[num].WHL_TMP_RESULT_D),(uint8_t)data_type::_16L);
	whl500_reg[num][tmp_slew].init((uint8_t*)&whlreg[num].WHL_TMP_SLEW_D,sizeof(whlreg[num].WHL_TMP_SLEW_D),(uint8_t)data_type::_16L);
	whl500_reg[num][tmp_alert_status].init((uint8_t*)&whlreg[num].WHL_TMP_ALERT_STATUS_D,sizeof(whlreg[num].WHL_TMP_ALERT_STATUS_D),(uint8_t)data_type::_16L);
	whl500_reg[num][tmp_conf].init((uint8_t*)&whlreg[num].WHL_TMP_CONF_D,sizeof(whlreg[num].WHL_TMP_CONF_D),(uint8_t)data_type::_16L);
	whl500_reg[num][tmp_alert_enable].init((uint8_t*)&whlreg[num].WHL_TMP_ALERT_ENABLE_D,sizeof(whlreg[num].WHL_TMP_ALERT_ENABLE_D),(uint8_t)data_type::_16L);
	whl500_reg[num][tmp_limit_low].init((uint8_t*)&whlreg[num].WHL_TMP_LIMIT_LOW_D,sizeof(whlreg[num].WHL_TMP_LIMIT_LOW_D),(uint8_t)data_type::_16L);
	whl500_reg[num][tmp_limit_high].init((uint8_t*)&whlreg[num].WHL_TMP_LIMIT_HIGH_D,sizeof(whlreg[num].WHL_TMP_LIMIT_HIGH_D),(uint8_t)data_type::_16L);
//	whl500_reg[num][tmp_hysteresis].init((uint8_t*)&whlreg[num].WHL_TMP_HYSTERESIS_D,sizeof(whlreg[num].WHL_TMP_HYSTERESIS_D),(uint8_t)data_type::_16L);
	whl500_reg[num][tmp_slew_limit].init((uint8_t*)&whlreg[num].WHL_TMP_SLEW_LIMIT_D,sizeof(whlreg[num].WHL_TMP_SLEW_LIMIT_D),(uint8_t)data_type::_16L);
	whl500_reg[num][tmp_uid1].init((uint8_t*)&whlreg[num].WHL_TMP_UID1_D,sizeof(whlreg[num].WHL_TMP_UID1_D),(uint8_t)data_type::_16L);
	whl500_reg[num][tmp_uid2].init((uint8_t*)&whlreg[num].WHL_TMP_UID2_D,sizeof(whlreg[num].WHL_TMP_UID2_D),(uint8_t)data_type::_16L);
	whl500_reg[num][tmp_uid3].init((uint8_t*)&whlreg[num].WHL_TMP_UID3_D,sizeof(whlreg[num].WHL_TMP_UID3_D),(uint8_t)data_type::_16L);
	whl500_reg[num][tmp_dieid].init((uint8_t*)&whlreg[num].WHL_TMP_DIEID_D,sizeof(whlreg[num].WHL_TMP_DIEID_D),(uint8_t)data_type::_16L);



}


#endif /* INC_WHL500_REG_H_ */
