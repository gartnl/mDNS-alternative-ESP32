## Basic mDNS responder for ESP32 ##

The standard mDNS library for ESP32 is not working as well as expected.
To get at least basic A-record response this library builds on top of
the default AsyncUDP library.

usage: 

-Define an object of type AmDNS object_name(char * your_host_name)

-After connecting to your network start the responder by object_name.begin()


TODO: 

-extend the library to respond to _http discovery

-extend to respond to _arduino discovery without breaking OTA library
