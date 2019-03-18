#define ESP32
#include "esp_wpa2.h"
#include <SocketIOClient.h>
#include <U8x8lib.h>
#define EAP_ANONYMOUS_IDENTITY ""
#define EAP_IDENTITY ""
#define EAP_PASSWORD ""
#define sensor 36

SocketIOClient client;
//make sure sensor_id are unique for each sensor
const String sensor_id = "5";
const char *ssid = "";
const char *password = "";
const String pre = "{\"sensor_id\" : " + sensor_id +", \"sensor_value\" : ";
const String post = "}";

char host[] = "hippocamp.site";
int port = 443;
extern String RID;
extern String Rname;
extern String Rcontent;

unsigned long previousMillis = 0;
unsigned long sensorPreviousMillis = 0;
long interval = 60000;
long sensorInterval = 10000;
int counter = 0;
unsigned long lastsend = 0;
int totval = 0;
int prevVal = -1;
int delta = 150;
int sendVal = 0;

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

hw_timer_t *timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;
uint32_t isrCount = 0, isrTime = 0;
char outStr[16];

void IRAM_ATTR onTimer()
{
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  isrCounter++;
  lastIsrAt = millis();
  portEXIT_CRITICAL_ISR(&timerMux);
  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  // It is safe to use digitalRead/Write here if you want to toggle an output
}

extern String RID;
extern String Rname;
extern String Rcontent;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
  delay(10);
  u8x8.begin();
  u8x8.setFont(u8x8_font_5x7_r);
  u8x8.drawString(0, 1, "connecting... ");
  WiFi.disconnect(true); //disconnect from any wifi
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /*  only needed for wpa2 enterprixe
    WiFi.mode(WIFI_STA); //init wifi mode
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_ANONYMOUS_IDENTITY, strlen(EAP_ANONYMOUS_IDENTITY)); 
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
    esp_wifi_sta_wpa2_ent_enable(&config); //set config settings to enable function
    WiFi.begin(ssid);
    */

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  u8x8.drawString(0, 1, "Connected To:");
  (String(ssid)).toCharArray(outStr, 16);
  u8x8.drawString(0, 2, outStr);
  u8x8.drawString(0, 3, "Device IP:");
  (WiFi.localIP().toString()).toCharArray(outStr, 16);
  u8x8.drawString(0, 4, outStr);
  ("RSSI: " + String(WiFi.RSSI())).toCharArray(outStr, 16);
  u8x8.drawString(0, 5, outStr);

  if (!client.connect(host, port))
  {
    Serial.println("Connection failed");
    return;
  }
  if (client.connected())
  {
    Serial.println("Connected");
    Serial.println("");
    client.send("connection", "message", "first send");
    Serial.println("after sending first message");
  }
}
void loop() {
  String sendMessage;
  int sensorVal;
  unsigned long currentMillis = millis();
  
  if (!client.connected())
  {
    Serial.println("try to connect again");
    if (! client.connect(host, port))
      Serial.println("connection failed again.  giving up");
  }
  if (currentMillis - sensorPreviousMillis > sensorInterval )
  {
    sensorPreviousMillis = currentMillis;
    sensorVal = analogRead(sensor);
    //re-draw screen every 10 seconds
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
    
    if (prevVal == -1)
    {
      prevVal = sensorVal;
      sendVal = sensorVal;
    }
    if (counter < 4)
      {
        totval = totval + sensorVal;
        counter++;
      }
      else
      {
        sendVal = totval / 4;
        totval = sensorVal;
        counter = 1;
      }
      //Serial.println("this is the PrevVal: " + String(prevVal));
      //Serial.println("This is the sensorVal: " + String(sensorVal));
      if ( (prevVal - sensorVal) > delta)
      {
        Serial.println("sensed watering.  Delta is: " + String(prevVal - sensorVal));
        client.sendJSON("watered", pre + String(sensorVal) + post);
        //send data right away
      }
       prevVal = sensorVal;
  
  }
  if (currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;
    digitalWrite(25, LOW);
    //client.heartbeat(0k);
    Serial.println("counter value " + String(counter));
    Serial.print("sending moisture sensor data: ");
    sendMessage = pre + String(sendVal) + post;
    Serial.println(sendMessage);
    
    client.sendJSON("reported", sendMessage);
    
    //Serial.print("**************************************  ");
    //Serial.println(lastsend);
    //client.send("atime", "message", "Time please?");
    //lastsend = millis();
    //Serial.print("**************************************  ");
    //Serial.println(lastsend);
  }
  if (client.monitor())
  {
    if (RID == "locate" && Rcontent== sensor_id)
    {
      if (digitalRead(25) == HIGH)
        digitalWrite(25, LOW);
      else
        digitalWrite(25, HIGH);
    }
  }

}
