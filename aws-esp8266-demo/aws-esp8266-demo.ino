#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "AWS.h"
#include "NTP.h"

#include "secrets.h"

AwsIot awsClient;

void waitUntilWifiConnected(String message)
{
  Serial.print(message);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("ok!");
}

void messageReceivedCallback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  delay(100);

  WiFi.hostname("<HOSTNAME>");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  waitUntilWifiConnected(String("Attempting to connect to SSID: ") + String(ssid));

  // Pacific standard time = UTC -7
  configTimeWithNTP(-7, false);

  awsClient.setThingname("<THINGNAME>");
  awsClient.setPublishTopic("<TOPIC>");
  awsClient.setHost("XXXXXXXXX-ats.iot.us-west-2.amazonaws.com");

#if LOAD_KEYS_FROM_SPIFFS
  awsClient.loadCertificatesFromSPIFFS();
#else
// Load from hardcoded keys
  BearSSL::X509List cert(cacert);
  BearSSL::X509List client_crt(client_cert);
  BearSSL::PrivateKey key(privkey);
  awsClient.loadCertificates(&cert, &client_crt, &key);
#endif

  // Optional
  awsClient.setShadowTopic("$aws/things/<THINGNAME>/shadow/update");
  awsClient.setSubscribeTopic("$aws/things/<THINGNAME>/shadow/update");
  awsClient.setCallback(messageReceivedCallback);

  awsClient.connect();
}

void loop()
{
  static unsigned long lastMillis = 0;
  static StaticJsonDocument<200> publishMessage;
  static char shadowMessage[50];

  if (!awsClient.connected())
  {
    waitUntilWifiConnected("Checking WiFi");
    awsClient.connect();
  }
  else
  {
    awsClient.loop();
    if (millis() - lastMillis > 5000)
    {
      lastMillis = millis();

      publishMessage["time"] = getTimestampAscii();
      publishMessage["temperature"] = random(100);
      publishMessage["humidity"] = random(100);
      publishMessage["distance"] = random(100);

      awsClient.publishMessage(publishMessage);

      sprintf(shadowMessage, "{\"state\":{\"reported\": {\"value\": %ld}}}", random(100));
      awsClient.updateDeviceShadow(shadowMessage);
    }
  }
}
