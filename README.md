# AWS IoT with ESP8266 Demo

Example sending data from ESP8266 to AWS IoT Core.

## Getting Started

Add the ESP8266 board to Arduino IDE (Files > Preferences, then paste the URL in `Additional Boards Manager`):

```
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

Versions:
- Arduino: 1.8.12
- ESP8266 core: 2.6.3
- ESP8266FS plugin: 0.5.0

Install the following libraries (Sketch > Include Library > Manage Libraries):

- [PubSubClient 2.7.0](https://github.com/knolleary/pubsubclient)

## AWS IoT Certificates

Convert the certificates from AWS to `.der` files, then place them in `data/` (within the Arduino sketch folder). Use the [arduino-esp8266fs-plugin](https://github.com/esp8266/arduino-esp8266fs-plugin) to upload the files to the SPIFFS filesystem.

```bash
openssl x509 -in xxx-certificate.pem.crt -out cert.der -outform DER
openssl rsa -in xxx-private.pem.key -out private.der -outform DER
openssl x509 -in AmazonRootCA1.pem -out ca.der -outform DER
```
