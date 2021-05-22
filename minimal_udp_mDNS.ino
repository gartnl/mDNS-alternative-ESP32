/*
   an alternative to the not quite perfect functioning mDNS for ESP32

   Very basic, only respond to DNS request with A-record
   does not check anything but hostname
   does not advertise services, or respond to service requests

   But does the job for finding a device in a local network
   
   written by gartnl 22-05-2021, version 1.0
*/
#include "WiFi.h"
#include "AsyncUDP.h"
#include <Ticker.h>

#define showM // comment out if you don't want the LED to blink on multicast reception

const char * ssid = "*********"; // use your own credentials
const char * password = "**********";
const char hostname[] = "udptest"; // and hostname

AsyncUDP mmDNS;
#ifdef showM
Ticker got_packet;
#endif

void setup()
{
  // general setup serial and wifi
  Serial.begin(115200);
  Serial.print("Starting WiFi: ");
  pinMode(2, OUTPUT);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Local IP is : "); Serial.println(WiFi.localIP());
  WiFi.setSleep(false);

  // done with wifi, build mDNS responder

  if (mmDNS.listenMulticast(IPAddress(224, 0, 0, 251), 5353)) {
    mmDNS.onPacket([](AsyncUDPPacket packet) { //multicast packet found
#ifdef showM
      //show multicast activity by blinking led for 0.05 second
      digitalWrite(2, 1);
      got_packet.attach(0.05, []() {
        digitalWrite(2, 0);
      });
      //
#endif
       // the core of it all
      if (compareHost(packet.data(), hostname)) { // yep, it's me they're looking for
        char response[packet.length() + 10]; // buffer to response
        memcpy ( response, packet.data(), packet.length() - 4 ); //copy original request to response minus last 4 bytes
        response[2] = 0x84; response[5] = 0x00; response[7] = 0x01; //modify the dns header to single response
        char trailer[14] = {0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04}; // default trailer for simple A record response
        char * p = response + packet.length() - 4; // copy default trailer to response from 0x00 after domain
        memcpy ( p, trailer, 14 );
        char * i = response + sizeof(response) - 4; // copy local IP to last 4 bytes
        *i++ = WiFi.localIP()[0];
        *i++ = WiFi.localIP()[1];
        *i++ = WiFi.localIP()[2];
        *i = WiFi.localIP()[3];
        // now send the response as multicast
        mmDNS.writeTo((const unsigned char*)response, sizeof(response), IPAddress(224, 0, 0, 251), 5353);
      }
    });
  }
}


void loop()
{
  //vTaskDelay( 1000 );
}

/*
   compare hostname, does not check request type (A,MX etc ) of multiple questions and ignores domain

   but does check length of hostname, and content of hostname, case sensitive

*/

bool compareHost(const unsigned char * first, const char * second) {

  char * t = (char*)second;
  int length;
  first += 0xC;
  while (*t != 0) {
    t++;
    length++;
  };
  if (*first != length) return false;
  first++;
  while (*second != 0) {
    if (*first != *second) return false;
    first++; second++;
  }
  return true;
}
