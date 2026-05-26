/*
 * WHL500_REG.h
 *
 *  Created on: May 13, 2024
 *      Author: Chinn
 */

#ifndef INC_WHL500_REG_H_
#define INC_WHL500_REG_H_


#include <KMUTNB_ADCS_REG.h>



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

class WHLREG{

public:
	KMUTNB_ADCS_REG whl500_reg[413];
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
	int16_t  WHL_SINE_D =0;
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
	uint32_t WHL_PERIOD0_TOTAL_D = 0;
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



	void whl500_reg_setup()
	{
		whl500_reg[gndwdt].init((uint8_t*)&WHL_GNDWDT_D,sizeof(WHL_GNDWDT_D),(uint8_t)data_type::_32UL);
		whl500_reg[csp_can_addr].init((uint8_t*)&WHL_CSP_CAN_ADDR_D,sizeof(WHL_CSP_CAN_ADDR_D),(uint8_t)data_type::_16UL);
		whl500_reg[csp_can_mask].init((uint8_t*)&WHL_CSP_CAN_MASK_D,sizeof(WHL_CSP_CAN_MASK_D),(uint8_t)data_type::_8UL);
		whl500_reg[csp_can_promisc].init((uint8_t*)&WHL_CSP_CAN_PROMISC_D,sizeof(WHL_CSP_CAN_PROMISC_D),(uint8_t)data_type::_8UL);
		whl500_reg[csp_version].init((uint8_t*)&WHL_CSP_VERSION_D,sizeof(WHL_CSP_VERSION_D),(uint8_t)data_type::_8UL);
		whl500_reg[csp_dedup].init((uint8_t*)&WHL_CSP_DEDUP_D,sizeof(WHL_CSP_DEDUP_D),(uint8_t)data_type::_8UL);
		whl500_reg[boot_img1].init((uint8_t*)&WHL_BOOT_IMG1_D,sizeof(WHL_BOOT_IMG1_D),(uint8_t)data_type::_8UL);
		whl500_reg[boot_img0].init((uint8_t*)&WHL_BOOT_IMG0_D,sizeof(WHL_BOOT_IMG0_D),(uint8_t)data_type::_8UL);
		whl500_reg[boot_img2].init((uint8_t*)&WHL_BOOT_IMG2_D,sizeof(WHL_BOOT_IMG2_D),(uint8_t)data_type::_8UL);
		whl500_reg[boot_img3].init((uint8_t*)&WHL_BOOT_IMG3_D,sizeof(WHL_BOOT_IMG3_D),(uint8_t)data_type::_8UL);
		whl500_reg[boot_cur].init((uint8_t*)&WHL_BOOT_CUR_D,sizeof(WHL_BOOT_CUR_D),(uint8_t)data_type::_8UL);
		whl500_reg[boot_cnt].init((uint8_t*)&WHL_BOOT_CNT_D,sizeof(WHL_BOOT_CNT_D),(uint8_t)data_type::_8UL);
		whl500_reg[boot_err].init((uint8_t*)&WHL_BOOT_ERR_D,sizeof(WHL_BOOT_ERR_D),(uint8_t)data_type::_8UL);
		whl500_reg[stdbuf_in].init((uint8_t*)&WHL_STDBUF_IN_D,sizeof(WHL_STDBUF_IN_D),(uint8_t)data_type::_16UL);
		whl500_reg[stdbuf_out].init((uint8_t*)&WHL_STDBUF_OUT_D,sizeof(WHL_STDBUF_OUT_D),(uint8_t)data_type::_16UL);
		whl500_reg[temp].init((uint8_t*)&WHL_TEMP_D,sizeof(WHL_TEMP_D),(uint8_t)data_type::_16L);
		whl500_reg[serial0].init((uint8_t*)&WHL_SERIAL0_D,sizeof(WHL_SERIAL0_D),(uint8_t)data_type::_32UL);
		whl500_reg[serial1].init((uint8_t*)&WHL_SERIAL1_D,sizeof(WHL_SERIAL1_D),(uint8_t)data_type::_32UL);
		whl500_reg[serial2].init((uint8_t*)&WHL_SERIAL2_D,sizeof(WHL_SERIAL2_D),(uint8_t)data_type::_32UL);
		whl500_reg[serial3].init((uint8_t*)&WHL_SERIAL3_D,sizeof(WHL_SERIAL3_D),(uint8_t)data_type::_32UL);
		whl500_reg[rtos_print].init((uint8_t*)&WHL_RTOS_PRINT_D,sizeof(WHL_RTOS_PRINT_D),(uint8_t)data_type::_8UL);
		whl500_reg[csp_buf_out].init((uint8_t*)&WHL_CSP_BUF_OUT_D,sizeof(WHL_CSP_BUF_OUT_D),(uint8_t)data_type::_8UL);
		whl500_reg[csp_conn_out].init((uint8_t*)&WHL_CSP_CONN_OUT_D,sizeof(WHL_CSP_CONN_OUT_D),(uint8_t)data_type::_8UL);
		whl500_reg[csp_conn_ovf].init((uint8_t*)&WHL_CSP_CONN_OVF_D,sizeof(WHL_CSP_CONN_OVF_D),(uint8_t)data_type::_8UL);
		whl500_reg[csp_conn_noroute].init((uint8_t*)&WHL_CSP_CONN_NOROUTE_D,sizeof(WHL_CSP_CONN_NOROUTE_D),(uint8_t)data_type::_8UL);
		whl500_reg[csp_inval_reply].init((uint8_t*)&WHL_CSP_INVAL_REPLY_D,sizeof(WHL_CSP_INVAL_REPLY_D),(uint8_t)data_type::_8UL);
		whl500_reg[csp_errno].init((uint8_t*)&WHL_CSP_ERRNO_D,sizeof(WHL_CSP_ERRNO_D),(uint8_t)data_type::_8UL);
		whl500_reg[csp_can_errno].init((uint8_t*)&WHL_CSP_CAN_ERRNO_D,sizeof(WHL_CSP_CAN_ERRNO_D),(uint8_t)data_type::_8UL);
		whl500_reg[csp_print_rdp].init((uint8_t*)&WHL_CSP_PRINT_RDP_D,sizeof(WHL_CSP_PRINT_RDP_D),(uint8_t)data_type::_8UL);
		whl500_reg[csp_print_packet].init((uint8_t*)&WHL_CSP_PRINT_PACKET_D,sizeof(WHL_CSP_PRINT_PACKET_D),(uint8_t)data_type::_8UL);
		whl500_reg[hall_nibble].init((uint8_t*)&WHL_HALL_NIBBLE_D,sizeof(WHL_HALL_NIBBLE_D),(uint8_t)data_type::_32UL);
		whl500_reg[hall_cnt].init((uint8_t*)&WHL_HALL_CNT_D,sizeof(WHL_HALL_CNT_D),(uint8_t)data_type::_32UL);
		whl500_reg[hall_dir].init((uint8_t*)&WHL_HALL_DIR_D,sizeof(WHL_HALL_DIR_D),(uint8_t)data_type::_32L);
		whl500_reg[angle].init((uint8_t*)&WHL_ANGLE_D,sizeof(WHL_ANGLE_D),(uint8_t)data_type::_32L);
		whl500_reg[sine].init((uint8_t*)&WHL_SINE_D,sizeof(WHL_SINE_D),(uint8_t)data_type::_16L);
		whl500_reg[amplitude].init((uint8_t*)&WHL_AMPLITUDE_D,sizeof(WHL_AMPLITUDE_D),(uint8_t)data_type::_16L);
		whl500_reg[sine_isr_ticks].init((uint8_t*)&WHL_SINE_ISR_TICKS_D,sizeof(WHL_SINE_ISR_TICKS_D),(uint8_t)data_type::_16UL);
		whl500_reg[angle_offset].init((uint8_t*)&WHL_ANGLE_OFFSET_D,sizeof(WHL_ANGLE_OFFSET_D),(uint8_t)data_type::_16L);
		whl500_reg[sine_period].init((uint8_t*)&WHL_SINE_PERIOD_D,sizeof(WHL_SINE_PERIOD_D),(uint8_t)data_type::_16UL);
		whl500_reg[cooldown].init((uint8_t*)&WHL_COOLDOWN_D,sizeof(WHL_COOLDOWN_D),(uint8_t)data_type::_16UL);


		whl500_reg[current].init((uint8_t*)&WHL_CURRENT_D,sizeof(WHL_CURRENT_D),(uint8_t)data_type::_16L);
		whl500_reg[torque_filtered].init((uint8_t*)&WHL_TORQUE_FILTERED_D,sizeof(WHL_TORQUE_FILTERED_D),(uint8_t)data_type::_32F);
		whl500_reg[torque_latest].init((uint8_t*)&WHL_TORQUE_LATEST_D,sizeof(WHL_TORQUE_LATEST_D),(uint8_t)data_type::_32F);


		whl500_reg[mrpm].init((uint8_t*)&WHL_MRPM_D,sizeof(WHL_MRPM_D),(uint8_t)data_type::_32L);

		whl500_reg[moment_of_inertia].init((uint8_t*)&WHL_MOMENT_OF_INERTIA_D,sizeof(WHL_MOMENT_OF_INERTIA_D),(uint8_t)data_type::_32F);
		whl500_reg[torque_duration].init((uint8_t*)&WHL_TORQUE_DURATION_D,sizeof(WHL_TORQUE_DURATION_D),(uint8_t)data_type::_8UL);
		whl500_reg[mrpm_target].init((uint8_t*)&WHL_MRPM_TARGET_D,sizeof(WHL_MRPM_TARGET_D),(uint8_t)data_type::_32L);

	}

};

#endif /* INC_WHL500_REG_H_ */
