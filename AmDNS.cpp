/*==================================
 * target: ESP32 in Arduino IDE
 * A simple mDNS responder for A-records only
 * based on the standard AsyncUDP library
 * Very basic responder:
 * if received multicast udp on 5353 is a question
 * and the requested name is my hostname
 * respond with an A-record for my IP
 ===================================*/
#include <arduino.h>
#include "AmDNS.h" 

AmDNS::AmDNS(char* hostname)
{
  _size = strlen(hostname);
  _hostname = new char[_size + 1];
  strcpy(_hostname, hostname);
}
AmDNS::~AmDNS() {
  delete[] _hostname;
}

bool AmDNS::begin() {
  if (listenMulticast(IPAddress(224, 0, 0, 251), 5353)) {
    Serial.println("UDP connected");
    onPacket([this](AsyncUDPPacket packet) {
      if (packet.data()[2]) return; // not a question
      _doPacket(&packet);
    });
    return true;
  }
  return false;
}

void AmDNS::_doPacket(AsyncUDPPacket * packet) {

  //quick namecheck first, compare length of _hostname vs requested name
  char * p_instance = (char*)packet->data() + 0xC;
  if (_size != *p_instance) return; // unequal namelength

  //full namecheck char by char, case insensitive
  char * p_name = (char*)_hostname;
  p_instance++;
  while (*p_name != 0) {
    if (((*p_instance) & 0x5F) != ((*p_name) & 0x5F)) return; //namecheck failed
    p_instance++; p_name++;
  }

  //yes, it is a request to me, start sending A_record
  //calculate an reserve space for simple A record response
  //size = original message - 4 + 14
  char answer[packet->length() + 10];
  memcpy(answer, packet->data(), packet->length() - 4); //copy original
  answer[2] = 0x84; //athoritative answer
  answer[5] = 0; // set questions to zer0
  answer[7] = 1; // and answers to 1
  //setup the A-record
  char trailer[14] = {0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, 0x04, 0xFF, 0xFF, 0xFF, 0xFF};
  //add-in wifi-ip
  trailer[10] = WiFi.localIP()[0];
  trailer[11] = WiFi.localIP()[1];
  trailer[12] = WiFi.localIP()[2];
  trailer[13] = WiFi.localIP()[3];
  //copy record to end of answer
  char * p = answer + packet->length() - 4;
  memcpy ( p, trailer, 14 );
  //reply is ready , if remote port is 5353 send as multicast
  //if remoteport differs, send as unicast
  if (packet->remotePort() == 5353) {
    writeTo((const unsigned char*)answer, sizeof(answer), IPAddress(224, 0, 0, 251), 5353);
  }
  else {
    writeTo((const unsigned char*)answer, sizeof(answer), packet->remoteIP(), packet->remotePort());
  }
}
