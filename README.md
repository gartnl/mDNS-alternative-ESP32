# mDNS-alternative-ESP32
A simple and quick alternative for mDNS on ESP32

mDNS on the ESP32 is a bit flaky and does not respond well.
Rather than writing a complete library I wrote a simple alternative using ASyncUDP.

It does not do anything but listen for a multicast DNS request to <hostname>.
The response is always a simple A-record with the local IP address.

Since it is basic there are no types or objects. UDP packet is changed by simple
memcpy's fo default values.

You can simply copy the routine to your project, and have a responsive mDNS responder.
