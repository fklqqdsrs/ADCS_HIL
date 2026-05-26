#ifndef _newDevice_h_
#define _newDevice_h_

#include "newHeader.h"

class newDevice
{
private:
    uint16_t _addr;
    uint8_t _addr_size;
    
    uint8_t _reg_size=1;
public:
    
newPort *_pport;

    newDevice(newPort *port,uint16_t addr,uint8_t reg_size =1){
        _pport = port;
        _addr = addr;
        _reg_size = reg_size;
    }

    newPort *getPort() { return _pport ;}
    
    uint16_t addr(){ return _addr; }
    
    uint8_t reg_size() { return _reg_size; }

    virtual uint8_t begin() { return 1;}
    virtual void read() {}
    virtual void write() {}
    
    
    virtual uint8_t readMEMs(uint16_t reg, uint8_t *buffer, const uint8_t len){
    	return _pport->readMEMs(_addr,reg,buffer,len);
        
    }
    
    virtual uint8_t writeMEMs(uint16_t reg, uint8_t *buffer, const uint8_t len){
    	return _pport->readMEMs(_addr,reg,buffer,len);
        
    }


};


#endif
