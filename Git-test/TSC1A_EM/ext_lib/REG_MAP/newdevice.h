#ifndef NEWDEVICE_HH
#define NEWDEVICE_HH

#include <stdint.h>



enum class sat_dir_enum_t : uint8_t {
    xPLUS,
    yPLUS,
    zPLUS,
    xMINUS,
    yMINUS,
    zMINUS
};

enum class device_axis_t : uint8_t {
    nothing,
    single,
    multi_axis
};

enum class device_state_t : uint8_t
{
    nothing,
    standby,
    manual,
    controlled,
    error,
    disable,
};

template <int device_len>
struct alignas(1) angle_array_t
{
    sat_dir_enum_t R_order[device_len][4];
    float angle[device_len][3];
};

template<class T> struct device_info_t{
    device_state_t status;             // Set/get; default: standby
    T type;                 // Used for init; default: nothing
    uint8_t* command_ptr;                  // Used for init; set/get; default: NULL
    uint8_t* sense_ptr;                    // Used for init; set/get; default: NULL
    float scalar;                       //  Set/get; default: 1
    uint8_t cmd_array_len;                // Set/get; default: sizeof(command)
    uint8_t sen_array_len;                  // Set/get; default: sizeof(sense)
    device_axis_t axis_type;
    angle_array_t<1> *rot_info;
} ;

template<class T>
class NEW_DEVICE_STATE
{
private:
    uint8_t init = 0;
    device_info_t<T> _data;

public:
    NEW_DEVICE_STATE() {
        init = 0;
    };

    void init_info(device_info_t<T> data,angle_array_t<1> rot_info = angle_array_t<1>{.R_order = {sat_dir_enum_t::xPLUS},.angle = {0},})
    {
        _data = data;
        init = 1;
        if (_data.rot_info == nullptr)
        {
            return;
        }

        *_data.rot_info = rot_info;
        
    }

    uint8_t get_init()
    {
        return init;
    }

    // Getters and Setters for device_info_t members
    device_state_t get_status() { return _data.status; }
    device_state_t* get_ptr_status() { return &_data.status; }
    void set_status(device_state_t status) { _data.status = status; }

    T get_type() { return _data.type; }
    void set_type(T type) { _data.type = type; }

    float scalar() { return _data.scalar; }
    void set_scalar(float ptr) { _data.scalar = ptr; }


    uint8_t* get_command_ptr() { return _data.command_ptr; }
    void set_command_ptr(uint8_t* ptr) { _data.command_ptr = ptr; }

    uint8_t* get_sense_ptr() { return _data.sense_ptr; }
    void set_sense_ptr(uint8_t* ptr) { _data.sense_ptr = ptr; }

    uint8_t get_cmd_array_len() { return _data.cmd_array_len; }
    void set_cmd_array_len(uint8_t len) { _data.cmd_array_len = len; }

    uint8_t get_sen_array_len() { return _data.sen_array_len; }
    void set_sen_array_len(uint8_t len) { _data.sen_array_len = len; }

    angle_array_t<1>* get_rot_info() { return _data.rot_info; }
    void set_rot_info(angle_array_t<1> *rot_info) { _data.rot_info = rot_info; }

    
};

#endif // NEWDEVICE_HH
