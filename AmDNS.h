#ifndef AmDNS_h
#define AmDNS_h
#include "WiFi.h"
#include "AsyncUDP.h"


class AmDNS : private AsyncUDP {
  
private:
    char* _hostname;
    int _size;
    void _doPacket(AsyncUDPPacket * packet);
public:
    AmDNS(char*); // constructor
    virtual ~AmDNS(); // destructor
    bool begin();
  
};
#endif
