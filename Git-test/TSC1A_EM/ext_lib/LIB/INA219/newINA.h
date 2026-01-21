#ifndef _NEWINA_H_
#define _NEWINA_H_

#include "NEWIO/newDevice.h"
#include "LIB/I2C/newPortI2C.h"
#include "LIB/utility/multconvert.h"

/** calculated I2C address: 0 = GND, 1 = V+ **/
/* The address is controlled by the A0 and A1 inputs on the INA219:
 *
 * Calculated address: b100ABCD
 * A0 controls C and D: GND = 00, V+ = 01, SDA = 10, SCL = 11
 * A1 controls A and B: GND = 00, V+ = 01, SDA = 10, SCL = 11
 *
 * E.g. if A0 is tied to ground and A1 is tied to V+,
 * the resulting address is b1000100 = 0x44
 *
 * SDA and SCL options aren't implemented.
 */

#define INA219_CALC_ADDRESS(INA_ADDR0, INA_ADDR1) \
    (0x40 | (INA_ADDR0 != 0 ? 0x01 : 0x00) | (INA_ADDR1 != 0 ? 0x04 : 0x00))

/** default I2C address **/
#define INA219_ADDRESS (0x40) // 1000000 (A0+A1=GND)

/** read **/
#define INA219_READ (0x01)

/*=========================================================================
    CONFIG REGISTER (R/W)
**************************************************************************/

/** config register address **/
#define INA219_REG_CONFIG (0x00)

/** reset bit **/
#define INA219_CONFIG_RESET (0x8000) // Reset Bit

/** mask for bus voltage range **/
#define INA219_CONFIG_BVOLTAGERANGE_MASK (0x2000) // Bus Voltage Range Mask

/** bus voltage range values **/
enum
{
    INA219_CONFIG_BVOLTAGERANGE_16V = (0x0000), // 0-16V Range
    INA219_CONFIG_BVOLTAGERANGE_32V = (0x2000), // 0-32V Range
};

/** mask for gain bits **/
#define INA219_CONFIG_GAIN_MASK (0x1800) // Gain Mask

/** values for gain bits **/
enum
{
    INA219_CONFIG_GAIN_1_40MV = (0x0000),  // Gain 1, 40mV Range
    INA219_CONFIG_GAIN_2_80MV = (0x0800),  // Gain 2, 80mV Range
    INA219_CONFIG_GAIN_4_160MV = (0x1000), // Gain 4, 160mV Range
    INA219_CONFIG_GAIN_8_320MV = (0x1800), // Gain 8, 320mV Range
};

/** mask for bus ADC resolution bits **/
#define INA219_CONFIG_BADCRES_MASK (0x0780)

/** values for bus ADC resolution **/
enum
{
    INA219_CONFIG_BADCRES_9BIT = (0x0000),  // 9-bit bus res = 0..511
    INA219_CONFIG_BADCRES_10BIT = (0x0080), // 10-bit bus res = 0..1023
    INA219_CONFIG_BADCRES_11BIT = (0x0100), // 11-bit bus res = 0..2047
    INA219_CONFIG_BADCRES_12BIT = (0x0180), // 12-bit bus res = 0..4097
    INA219_CONFIG_BADCRES_12BIT_2S_1060US =
        (0x0480), // 2 x 12-bit bus samples averaged together
    INA219_CONFIG_BADCRES_12BIT_4S_2130US =
        (0x0500), // 4 x 12-bit bus samples averaged together
    INA219_CONFIG_BADCRES_12BIT_8S_4260US =
        (0x0580), // 8 x 12-bit bus samples averaged together
    INA219_CONFIG_BADCRES_12BIT_16S_8510US =
        (0x0600), // 16 x 12-bit bus samples averaged together
    INA219_CONFIG_BADCRES_12BIT_32S_17MS =
        (0x0680), // 32 x 12-bit bus samples averaged together
    INA219_CONFIG_BADCRES_12BIT_64S_34MS =
        (0x0700), // 64 x 12-bit bus samples averaged together
    INA219_CONFIG_BADCRES_12BIT_128S_69MS =
        (0x0780), // 128 x 12-bit bus samples averaged together

};

/** mask for shunt ADC resolution bits **/
#define INA219_CONFIG_SADCRES_MASK \
    (0x0078) // Shunt ADC Resolution and Averaging Mask

/** values for shunt ADC resolution **/
enum
{
    INA219_CONFIG_SADCRES_9BIT_1S_84US = (0x0000),   // 1 x 9-bit shunt sample
    INA219_CONFIG_SADCRES_10BIT_1S_148US = (0x0008), // 1 x 10-bit shunt sample
    INA219_CONFIG_SADCRES_11BIT_1S_276US = (0x0010), // 1 x 11-bit shunt sample
    INA219_CONFIG_SADCRES_12BIT_1S_532US = (0x0018), // 1 x 12-bit shunt sample
    INA219_CONFIG_SADCRES_12BIT_2S_1060US =
        (0x0048), // 2 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_4S_2130US =
        (0x0050), // 4 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_8S_4260US =
        (0x0058), // 8 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_16S_8510US =
        (0x0060), // 16 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_32S_17MS =
        (0x0068), // 32 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_64S_34MS =
        (0x0070), // 64 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_128S_69MS =
        (0x0078), // 128 x 12-bit shunt samples averaged together
};

/** mask for operating mode bits **/
#define INA219_CONFIG_MODE_MASK (0x0007) // Operating Mode Mask

/** values for operating mode **/
enum
{
    INA219_CONFIG_MODE_POWERDOWN = 0x00,       /**< power down */
    INA219_CONFIG_MODE_SVOLT_TRIGGERED = 0x01, /**< shunt voltage triggered */
    INA219_CONFIG_MODE_BVOLT_TRIGGERED = 0x02, /**< bus voltage triggered */
    INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED =
        0x03,                                   /**< shunt and bus voltage triggered */
    INA219_CONFIG_MODE_ADCOFF = 0x04,           /**< ADC off */
    INA219_CONFIG_MODE_SVOLT_CONTINUOUS = 0x05, /**< shunt voltage continuous */
    INA219_CONFIG_MODE_BVOLT_CONTINUOUS = 0x06, /**< bus voltage continuous */
    INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS =
        0x07, /**< shunt and bus voltage continuous */
};

/** shunt voltage register **/
#define INA219_REG_SHUNTVOLTAGE (0x01)

/** bus voltage register **/
#define INA219_REG_BUSVOLTAGE (0x02)

/** power register **/
#define INA219_REG_POWER (0x03)

/** current register **/
#define INA219_REG_CURRENT (0x04)

/** calibration register **/
#define INA219_REG_CALIBRATION (0x05)
// #include "UART\RingBufferUART_IT.h"
// extern RingBufferUART_IT cube_pc104;

typedef struct
{
    uint16_t cal_number;
    float current_div;
    float power_mult;
    uint16_t Vrange;
    uint16_t gain;
    uint16_t bus_bit;
    uint16_t shunt_bit;
    int16_t offset_bit = 0;
} current_info_t;

current_info_t ina_cur_default = {
    .cal_number = 8192,
    .current_div = 100,
    .power_mult = 0.2,
    .Vrange = INA219_CONFIG_BVOLTAGERANGE_32V,
    .gain = INA219_CONFIG_GAIN_8_320MV,
    .bus_bit = INA219_CONFIG_BADCRES_12BIT_128S_69MS,
    .shunt_bit = INA219_CONFIG_SADCRES_12BIT_128S_69MS,
    .offset_bit = 0,
};

/*!
 *   @brief  Class that stores state and functions for interacting with INA219
 *  current/power monitor IC
 */
class newINA : public newDevice
{

public:
    newINA(newPortI2C *i2c_dev, uint8_t addr = INA219_ADDRESS) : newDevice(i2c_dev, addr, 1)
    {
        _pport = (newPortI2C *)this->getPort();
        _pport->set_timeout(1000);
        have_info=0;
        _success=0;
        _info=NULL;
    }
    newINA(newPortI2C *i2c_dev, current_info_t *info, uint8_t addr = INA219_ADDRESS) : newDevice(i2c_dev, addr, 1)
    {
        _info = info;
        have_info =1;
        _pport = (newPortI2C *)this->getPort();
        _pport->set_timeout(1000);
        _success=0;
    }
    uint8_t get_success() { return _success; }

    uint8_t readMEMs(const uint16_t reg, uint8_t *buffer, const uint8_t len)
    {
        return _pport->readMEMs(this->addr(), reg, buffer, len);
    }
    uint8_t writeMEMs(const uint16_t reg, uint8_t *buffer, const uint8_t len)
    {
        return _pport->writeMEMs(this->addr(), reg, buffer, len);
    }

    void begin_calibration(current_info_t *info, uint8_t find_offset = 0)
    {
        newCalibration(info);
        if (find_offset == 0 || get_success() == 1)
        {
            return;
        }
        //info->offset_bit = 0;
        //info->offset_bit += getCurrent_raw();
        // info->offset_bit += getCurrent_raw();
        // info->offset_bit = (info->offset_bit/2);
    }

    uint8_t begin(uint8_t calib = 0)
    {
        
        newCalibration((have_info == 1)? _info: &ina_cur_default);
        if (calib == 0 || get_success() == 1)
        {
            return _success;
        }
        //_info->offset_bit = 0;
        //_info->offset_bit += getCurrent_raw();
            
        return _success;
    }

    void setCalibration_32V_2A()
    {
        ina219_calValue.number = (uint16_t)4096;
        ina219_currentDivider_mA = 10; // Current LSB = 100uA per bit (1000/100 = 10)
        ina219_powerMultiplier_mW = 2; // Power LSB = 1mW per bit (2/1)

        // Set Calibration register to 'Cal' calculated above
        // Adafruit_BusIO_Register calibration_reg =
        //    Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
        // Setcalibration_reg.write(ina219_calValue.number, 2);
        _success = writeMEMs(INA219_REG_CALIBRATION, &ina219_calValue.bytes[0], 2);
        // Set Config register to take into account the settings above
        UINT16UNION_t config;
        config.number = INA219_CONFIG_BVOLTAGERANGE_32V |
                        INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
                        INA219_CONFIG_SADCRES_12BIT_1S_532US |
                        INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
        swapDataByte(&config.bytes[0]);
        _success = writeMEMs(INA219_REG_CONFIG, &config.bytes[0], 2);
    }

    void setCalibration_32V_1A()
    {
        ina219_calValue.number = 10240;
        // Set multipliers to convert raw current/power values
        ina219_currentDivider_mA = 25;    // Current LSB = 40uA per bit (1000/40 = 25)
        ina219_powerMultiplier_mW = 0.8f; // Power LSB = 800uW per bit
        _success = writeMEMs(INA219_REG_CALIBRATION, &ina219_calValue.bytes[0], 2);
        UINT16UNION_t config;
        config.number = INA219_CONFIG_BVOLTAGERANGE_32V |
                        INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
                        INA219_CONFIG_SADCRES_12BIT_1S_532US |
                        INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
        swapDataByte(&config.bytes[0]);
        _success = writeMEMs(INA219_REG_CONFIG, &config.bytes[0], 2);
    }
    void setCalibration_16V_400mA()
    {
        ina219_calValue.number = 8192;
        // Set multipliers to convert raw current/power values
        ina219_currentDivider_mA = 20;    // Current LSB = 50uA per bit (1000/50 = 20)
        ina219_powerMultiplier_mW = 1.0f; // Power LSB = 1mW per bit
        _success = writeMEMs(INA219_REG_CALIBRATION, &ina219_calValue.bytes[0], 2);
        UINT16UNION_t config;
        config.number = INA219_CONFIG_BVOLTAGERANGE_16V |
                        INA219_CONFIG_GAIN_1_40MV | INA219_CONFIG_BADCRES_12BIT |
                        INA219_CONFIG_SADCRES_12BIT_1S_532US |
                        INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
        swapDataByte(&config.bytes[0]);
        _success = writeMEMs(INA219_REG_CONFIG, &config.bytes[0], 2);
    }
    void newCalibration(current_info_t *info = &ina_cur_default)
    {
        _info = info;
        UINT16UNION_t config;
        // char tem[30] = "";

        ina219_calValue.number = _info->cal_number;
        // Set multipliers to convert raw current/power values
        ina219_currentDivider_mA = _info->current_div; // Current LSB = 50uA per bit (1000/50 = 20)
        ina219_powerMultiplier_mW = _info->power_mult; // Power LSB = 1mW per bit
        swapDataByte(&ina219_calValue.bytes[0]);
        // sprintf(tem,"cal number : %d\r\n",ina219_calValue.number);
        // adcs_pc104.sendstring(tem);
        // _success =HAL_I2C_Mem_Write(getHandleTypeDef(),INA219_REG_CALIBRATION,1,&ina219_calValue.bytes[0], 2, 1000);

        _success = writeMEMs(INA219_REG_CALIBRATION, &ina219_calValue.bytes[0], 2);
        // HAL_Delay(100);
        //_success = readMEMs(INA219_REG_CALIBRATION, &config.bytes[0], 2);
        // sprintf(tem,"result : %d\r\n",config.number)
        // cube_pc104.sendstring(tem);

        config.number = _info->Vrange |
                        _info->gain |
                        _info->bus_bit |
                        _info->shunt_bit |
                        INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
        // sprintf(tem,"config : %d\r\n",config.number);
        // adcs_pc104.sendstring(tem);
        swapDataByte(&config.bytes[0]);

        _success = writeMEMs(INA219_REG_CONFIG, &config.bytes[0], 2);
        // HAL_Delay(100);
        //_success = readMEMs(INA219_REG_CONFIG, &config.bytes[0], 2);
        // swapDataByte(&config.bytes[0]);
        // sprintf(tem,"result : %d\r\n",config.number);
        // adcs_pc104.sendstring(tem);
    }
    void recalibrate(uint16_t cal_number, float current_div, uint16_t Vrange, uint16_t gain, uint16_t bus_bit, uint16_t shunt_bit)
    {
        UINT16UNION_t config;
        _info->Vrange = Vrange;
        _info->gain = gain;
        _info->bus_bit = bus_bit;
        _info->shunt_bit = shunt_bit;
        
        // char tem[30] = "";
        // little_to_big()
        ina219_calValue.number = cal_number;
        // Set multipliers to convert raw current/power values
        ina219_currentDivider_mA = current_div; // Current LSB = 50uA per bit (1000/50 = 20)
        ina219_powerMultiplier_mW = 0.2f;       // Power LSB = 1mW per bit
        swapDataByte(&ina219_calValue.bytes[0]);
        // sprintf(tem,"cal number : %d\r\n",ina219_calValue.number);
        // adcs_pc104.sendstring(tem);
        // _success =HAL_I2C_Mem_Write(getHandleTypeDef(),INA219_REG_CALIBRATION,1,&ina219_calValue.bytes[0], 2, 1000);
        _success = writeMEMs(INA219_REG_CALIBRATION, &ina219_calValue.bytes[0], 2);
        // HAL_Delay(100);
        //_success = readMEMs(INA219_REG_CALIBRATION, &config.bytes[0], 2);
        // sprintf(tem,"result : %d\r\n",config.number)
        // cube_pc104.sendstring(tem);

        config.number = Vrange |
                        gain | bus_bit |
                        shunt_bit |
                        INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

        swapDataByte(&config.bytes[0]);

        _success = writeMEMs(INA219_REG_CONFIG, &config.bytes[0], 2);
    }

    float getBusVoltage_V() { return ((int16_t)getBusVoltage_raw()) * 0.001; }
    float getShuntVoltage_mV() { return ((int16_t)getShuntVoltage_raw()) * 0.01; }
    float getCurrent_mA() { return ((float)getCurrent_raw()) / (float)ina219_currentDivider_mA; }
    float getPower_mW() { return ((float)getPower_raw()) * ina219_powerMultiplier_mW; }
    void powerSave(bool on)
    {
        /*
        Adafruit_BusIO_Register config_reg =
            Adafruit_BusIO_Register(i2c_dev, INA219_REG_CONFIG, 2, MSBFIRST);

        Adafruit_BusIO_RegisterBits mode_bits =
            Adafruit_BusIO_RegisterBits(&config_reg, 3, 0);
        if (on)
        {
            _success = mode_bits.write(INA219_CONFIG_MODE_POWERDOWN);
        }
        else
        {
            _success = mode_bits.write(INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS);
        }
        */
    }
    bool success() { return _success; }
    int16_t getBusVoltage_raw()
    {
        /*
        uint16_t value;

        Adafruit_BusIO_Register bus_voltage_reg =
            Adafruit_BusIO_Register(i2c_dev, INA219_REG_BUSVOLTAGE, 2, MSBFIRST);
        _success = bus_voltage_reg.read(&value);
        */
        INT16UNION_t value;
        _success = readMEMs(INA219_REG_BUSVOLTAGE, &value.bytes[0], 2);
        swapDataByte(&value.bytes[0]);
        // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
        // return (int16_t)((value >> 3) * 4);
        return (int16_t)((value.number >> 3) * 4);
    }
    int16_t getShuntVoltage_raw()
    {
        /*
        uint16_t value;
        Adafruit_BusIO_Register shunt_voltage_reg =
            Adafruit_BusIO_Register(i2c_dev, INA219_REG_SHUNTVOLTAGE, 2, MSBFIRST);
        _success = shunt_voltage_reg.read(&value);
        return value;
        */
        INT16UNION_t value;
        _success = readMEMs(INA219_REG_SHUNTVOLTAGE, &value.bytes[0], 2);
        swapDataByte(&value.bytes[0]);
        return value.number;
    }
    int16_t getCurrent_raw()
    {
        /*
        uint16_t value;

        // Sometimes a sharp load will reset the INA219, which will
        // reset the cal register, meaning CURRENT and POWER will
        // not be available ... avoid this by always setting a cal
        // value even if it's an unfortunate extra step
        Adafruit_BusIO_Register calibration_reg =
            Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
        calibration_reg.write(ina219_calValue.number, 2);

        // Now we can safely read the CURRENT register!
        Adafruit_BusIO_Register current_reg =
            Adafruit_BusIO_Register(i2c_dev, INA219_REG_CURRENT, 2, MSBFIRST);
        _success = current_reg.read(&value);
        return value;
        */

        _success = writeMEMs(INA219_REG_CALIBRATION, &ina219_calValue.bytes[0], 2);

        INT16UNION_t value;
        _success = readMEMs(INA219_REG_CURRENT, &value.bytes[0], 2);
        swapDataByte(&value.bytes[0]);
        return value.number;
    }
    int16_t getPower_raw()
    {
        /*
        uint16_t value;

        // Sometimes a sharp load will reset the INA219, which will
        // reset the cal register, meaning CURRENT and POWER will
        // not be available ... avoid this by always setting a cal
        // value even if it's an unfortunate extra step
        Adafruit_BusIO_Register calibration_reg =
            Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
        calibration_reg.write(ina219_calValue.number, 2);

        // Now we can safely read the POWER register!
        Adafruit_BusIO_Register power_reg =
            Adafruit_BusIO_Register(i2c_dev, INA219_REG_POWER, 2, MSBFIRST);
        _success = power_reg.read(&value);
        return value;
        */

        _success = writeMEMs(INA219_REG_CALIBRATION, &ina219_calValue.bytes[0], 2);

        INT16UNION_t value;
        _success = readMEMs(INA219_REG_POWER, &value.bytes[0], 2);
        // swapDataByte(&value.bytes[0]);
        return value.number;
    }

    current_info_t* get_info() {
        return _info;
    }

private:
    newPortI2C *_pport;

    uint8_t _success;
    uint8_t ina219_i2caddr = -1;
    UINT16UNION_t ina219_calValue;
    // The following multipliers are used to convert raw current and power
    // values to mA and mW, taking into account the current config settings
    float ina219_currentDivider_mA = 0;
    float ina219_powerMultiplier_mW = 0;
    void swapDataByte(uint8_t *_data)
    {
        uint8_t _t = _data[0];
        _data[0] = _data[1];
        _data[1] = _t;
    }
    uint16_t little_to_big(uint16_t little)
    {
        return ((little << 8) & 0xff00) | ((little >> 8) & 0x00ff);
    }
    current_info_t *_info;
    uint8_t have_info =0;
};
#endif //
