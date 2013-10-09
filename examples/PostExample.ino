#include <SPI.h>
#include <WiFi.h>

#include "M2XStreamClient.h"

char ssid[] = "NETGEAR93"; //  your network SSID (name)
char pass[] = "basiccream298";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
char server[] = "192.168.1.6";    // name address server
int port = 9393;

char feedId[] = "31502478bad5bb5344455810ec67d9dd"; // Feed you want to post to
char streamName[] = "temperature"; // Stream you want to post to
char m2xKey[] = "f040df815ae1bf1de0c41c4f59b6c52c"; // Your M2X access key

const int temperaturePin = 0;
WiFiClient client;
M2XStreamClient m2xClient(&client, m2xKey, server, port);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while(true);
  }

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
}

void loop() {
  float voltage, degreesC, degreesF;

  voltage = getVoltage(temperaturePin);
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;

  Serial.print("voltage: ");
  Serial.print(voltage);
  Serial.print("  deg C: ");
  Serial.print(degreesC);
  Serial.print("  deg F: ");
  Serial.println(degreesF);

  int response = m2xClient.send(feedId, streamName, degreesC);
  Serial.print("M2x client response code: ");
  Serial.println(response);

  if (response == -1) while(1) ;

  delay(5000);
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

float getVoltage(int pin)
{
  return (analogRead(pin) * 0.004882814);
}