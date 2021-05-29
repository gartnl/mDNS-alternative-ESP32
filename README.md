## Basic mDNS responder for ESP32 Arduino IDE ##

The standard mDNS library for ESP32 is not working as well as expected.
To get at least basic A-record response this library builds on top of
the default AsyncUDP library.

usage: 

-Define an object of type AmDNS object_name(char * your_host_name)

-After connecting to your network start the responder by object_name.begin()

LIMITATIONS:

-Since mDNS is mostly used in .local domains there is no domaincheck, but .local is assumed

-only support for IP4 networks. Even a request for IP6 is answered with an IP4 address.

TODO: 

-check domains?

-filter out IP6 (AAAA record ) requests?

-extend the library to respond to _http discovery

-extend to respond to _arduino discovery without breaking OTA library
