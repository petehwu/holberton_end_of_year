#include <WiFi.h> //wifi library
#include "esp_wpa2.h"
#include <WiFiClientSecure.h>
#include <U8x8lib.h>

#define EAP_ANONYMOUS_IDENTITY ""
#define EAP_IDENTITY ""
#define EAP_PASSWORD ""
#define sensor 36

int delta = 300;
int interval = 6;
int sampleInterval = 10000; // 1000 = 1 second, 60000 = 1 minute
int counter = 0;
int totval = 0;
int prevVal = -1;

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

const char* ssid = "Holberton 2.4Ghz"; // Eduroam SSID
const char* server = "petehwu.online";  // Server URL
const String pre = "{\"sensor_id\": 2, \"sensor_value\":";
const String post = "}";
const String sensor_input = "POST https://petehwu.online/sensor_in HTTP/1.1";
const String water_detected = "POST https://petehwu.online/water_detected HTTP/1.1";

const char* test_root_ca= \
     "-----BEGIN CERTIFICATE-----\n" \
     "MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n" \
     "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
     "DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n" \
     "SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n" \
     "GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n" \
     "AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n" \
     "q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n" \
     "SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n" \
     "Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n" \
     "a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n" \
     "/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n" \
     "AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n" \
     "CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n" \
     "bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n" \
     "c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n" \
     "VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n" \
     "ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n" \
     "MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n" \
     "Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n" \
     "AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n" \
     "uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n" \
     "wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n" \
     "X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n" \
     "PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n" \
     "KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n" \
     "-----END CERTIFICATE-----\n";


// You can use x.509 client certificates if you want
//const char* test_client_key = "";   //to verify the client
//const char* test_client_cert = "";  //to verify the client
WiFiClientSecure client;

void setup() {

  
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(100);
  u8x8.begin();
  u8x8.setFont(u8x8_font_5x7_r);

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  u8x8.drawString(0, 1, "connecting... ");
  do {
    WiFi.disconnect(true); //disconnect from wifi to set new wifi
    WiFi.mode(WIFI_STA); //init wifi mode
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_ANONYMOUS_IDENTITY, strlen(EAP_ANONYMOUS_IDENTITY)); 
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
    esp_wifi_sta_wpa2_ent_enable(&config); //set config settings to enable function
    WiFi.begin(ssid);
    // attempt to connect to Wifi network:
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
  }  while (WiFi.status() != WL_CONNECTED);
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI: " );
  Serial.println(WiFi.RSSI());
 
}

void loop() {
  char outStr[16];
  String tot;
  int retry = 0;
  int retry2 = 0;
  int sensorVal;


  sensorVal = analogRead(sensor);
  if (prevVal == -1)
  {
    prevVal = sensorVal;
  }
  // draw to OLED screen
  u8x8.begin();
  u8x8.setFont(u8x8_font_5x7_r);
  u8x8.drawString(0, 1, "Connected To:");
  (String(ssid)).toCharArray(outStr, 16);
  u8x8.drawString(0, 2, outStr);
  u8x8.drawString(0, 3, "Device IP:");
  (WiFi.localIP().toString()).toCharArray(outStr, 16);
  u8x8.drawString(0, 4, outStr);
  ("RSSI: " + String(WiFi.RSSI())).toCharArray(outStr, 16);
  u8x8.drawString(0, 5, outStr);
  u8x8.drawString(0, 6, "Sensor Value:");
  String(sensorVal).toCharArray(outStr, 16);
  u8x8.drawString(0, 7, outStr);

 
  Serial.println();
  Serial.print("Sensor value = ");
  Serial.println(String(sensorVal));
  Serial.print("RSSI: " );
  Serial.println(WiFi.RSSI());
 


  if (counter < interval) 
  {
    Serial.println("in if");
    Serial.print("counter value: ");
    Serial.println(counter);
    totval = totval + sensorVal;
    Serial.println("Totalvalue " + String(totval));
    counter++;
  }
  else
  {
    Serial.println("in else");
    Serial.println("total: " + String(totval));
    Serial.println("counter: " + String(counter));
    //client.setCertificate(test_client_key); // for client verification
    //client.setPrivateKey(test_client_cert);  // for client verification
    client.setCACert(test_root_ca);
    client.connect(server, 443);
    Serial.println("\nStarting connection to HTTPS server...");
    
    while (client.connected() == false && retry < 5)
    {
      //delay 1 second and try connecting to https again.  5 tries before giving up.
      delay(1000);
      client.setCACert(test_root_ca);
      client.connect(server, 443);
      Serial.println("Re-trying https connection");
      retry++;
    }
    if (client.connected() == false)
      Serial.println("Connection failed!");
    else {
      Serial.println("Connected to server!");
      // Make a HTTP request:
      client.println(sensor_input);
      client.println("Host: " + String(server));
      client.println("Connection: close");
      client.println("Content-Type: application/json;");
      client.print("Content-Length: ");
      tot = pre + (totval /  counter) + post;
      client.println(tot.length());
      client.println();
      client.println(tot);
      Serial.println(tot);
      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          Serial.println("headers received");
          break;
        }
      }
      // if there are incoming bytes available
      // from the server, read them and print them:
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
      client.stop();
    }
    totval = sensorVal;
    counter = 1; 
  }
  // handle case when sensor value changes more than preset amount
  if ( (prevVal - sensorVal) > delta)
  {
        //client.setCertificate(test_client_key); // for client verification
    //client.setPrivateKey(test_client_cert);  // for client verification
    client.setCACert(test_root_ca);
    Serial.println("\nStarting connection to HTTPS server...");
    client.connect(server, 443);
    while (client.connected() == false && retry2 < 5)
    {
      //delay 1 second and try connecting to https again.  5 tries before giving up.
      delay(1000);
      client.connect(server, 443);
      Serial.println("Retrying https2 connection");
      retry2++;
    }
    if (client.connected() == false)
      Serial.println("Connection failed!");
    else {
      Serial.println("Connected to server!");
      // Make a HTTP request:
      client.println(water_detected);
      client.println("Host: " + String(server));
      client.println("Connection: close");
      client.println("Content-Type: application/json;");
      client.print("Content-Length: ");
      tot = pre + String(sensorVal) + post;
      client.println(tot.length());
      client.println();
      client.println(tot);
      Serial.println(tot);
      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          Serial.println("headers received");
          break;
        }
      }
      // if there are incoming bytes available
      // from the server, read them and print them:
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
      client.stop();
    }
  }
  prevVal = sensorVal;
  delay(sampleInterval - (1000 * retry) - (1000 * retry2));
}
