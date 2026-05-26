#ifndef _newPort_h_
#define _newPort_h_

typedef enum {
	e_none = 0,
	e_uart,
	e_i2c,
	e_spi,
	e_can,
}Commu_type;

class newPort
{
private:
    
    uint8_t _mode_in; //new_Port_type
    //new_addr_union_t _addr;
    //uint8_t _addr_size;
    uint32_t _Timeout=1;
public:
    
    newPort(uint8_t mode){
    	_mode_in = mode;
        // _addr_size = addr_size;
        // if (_addr_size == 1)
        // {
        //     _addr._8ul = (uint8_t)addr;
        // }
        // else{
        //     _addr._16ul = addr;
        // }
        

    }

    uint8_t _mode() {return _mode_in;}
    void mode(uint8_t mode_in) { _mode_in = mode_in;}



    void set_timeout(uint32_t timeout){
        _Timeout = timeout;
    }

    uint32_t get_timeout(){
        return _Timeout;
    }

    virtual void init(void*){
        ;
    }

    virtual void suspend(void*){
        ;
    }

    virtual void resume(void*){
        ;
    }

    virtual uint8_t readBytes(void*){
    	return 1;
    }

    virtual uint8_t writeBytes(void*){
    	return 1;
    }

    virtual uint8_t readMEMs(uint8_t _addr,uint8_t reg , uint8_t *buffer, const uint8_t len){
    	return 1;
    }

    virtual uint8_t writeMEMs(uint8_t _addr,uint8_t reg, uint8_t *buffer, const uint8_t len){
    	return 1;
    }
};







#endif
