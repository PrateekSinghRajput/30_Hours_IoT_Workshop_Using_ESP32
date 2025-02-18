#include <WiFi.h>

void setup() 
{  
  Serial.begin(115200);
  Serial.println();
  Serial.println("MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() 
{
  // Nothing here
}
