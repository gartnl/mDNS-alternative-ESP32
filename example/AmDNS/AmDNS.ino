#include "WiFi.h"
#include "AmDNS.h"

// replace with your own credentials and hostname
char * ssid = "******";
char * pass = "******";
char * hostname = "test_host";

AmDNS myhost(hostname);

void setup() {

  Serial.begin(115200);
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address is : ");Serial.println(WiFi.localIP());
  WiFi.setSleep(false);

  myhost.begin(); //starts the AmDNS
}

void loop() {
//whatever you fancy
}
