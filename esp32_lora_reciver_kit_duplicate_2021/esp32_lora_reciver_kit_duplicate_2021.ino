#include <SPI.h>
#include <LoRa.h>
#define ss 5
#define rst 14
#define dio0 2
String b1, b2, b3, b4, b5, b6, b7, b8;
int b11, b21, b31, b41, b51, b61, b71, b81, b91;
#include <Esp32WifiManager.h>
#include "WiFi.h"
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
//uint8_t mac[6] {0xA7, 0xD9, 0xB3, 0x0D, 0xAA, 0xCD}; //MAC Address you want to set 84:F3:EB:AF:4E:44
const char *ssid = "BIT";  //ENTER YOUR WIFI SETTINGS
const char *password = "";
int ssid1_len = 0;
char ssid1[] = {};

const int GPIO_PIN_NUMBER_27 = 27;
const int GPIO_PIN_NUMBER_26 = 26;
const int GPIO_PIN_NUMBER_25 = 25;
//Web/Server address to read/write from
const char *host = "";
int counter = 0;
void(* resetFunc) (void) = 0;
int counter1 = 0;

void setup() {
  Serial.begin(9600);
   
 while (!Serial);
 
  Serial.println("LoRa Reciver");


  //  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  LoRa.setSPIFrequency(10E6);

  //
  //  //replace the LoRa.begin(---E-) argument with your location's frequency
  //  //433E6 for Asia
  //  //866E6 for Europe
  //  //915E6 for North America
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  LoRa.setPreambleLength(8);
  Serial.println("LoRa Initializing OK!");
  delay(1000);
 
  Serial.print("ESP32 MAC: ");
  Serial.println(WiFi.macAddress()); //This will read MAC Address of ESP
  pinMode(GPIO_PIN_NUMBER_27, OUTPUT);
  pinMode(GPIO_PIN_NUMBER_26, OUTPUT);
  pinMode(GPIO_PIN_NUMBER_25, OUTPUT);


  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting to");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  int u = 1;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(GPIO_PIN_NUMBER_27, HIGH);
    digitalWrite(GPIO_PIN_NUMBER_26, LOW);
    digitalWrite(GPIO_PIN_NUMBER_25, LOW);
    u++;
    if (u > 32) {
      Serial.println("" );
      Serial.print("No SSID in ");
      digitalWrite(GPIO_PIN_NUMBER_27, HIGH);
      digitalWrite(GPIO_PIN_NUMBER_26, LOW);
      digitalWrite(GPIO_PIN_NUMBER_25, LOW);

      Serial.println(ssid);
      open_wifi();
      break;
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(2000);
 

 
}


void loop() {

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    counter1=0;
    Serial.println("Received packet '");

    // read packet
    while (LoRa.available()) {
      String a = LoRa.readString();
      int a1 = a.indexOf("a");
      int a2 = a.indexOf("b");
      int a3 = a.indexOf("c");
      int a4 = a.indexOf("d");
      int a5 = a.indexOf("e");
      int a6 = a.indexOf("f");
      int a7 = a.indexOf("g");
      int a8 = a.indexOf("h");
      int a9 = a.indexOf("i");
      int a10 = a.indexOf("j");
      int a11 = a.indexOf("k");
      int a12 = a.indexOf("l");
      int a13 = a.indexOf("m");
      int a14 = a.indexOf("n");
      int a15 = a.indexOf("o");
      int a16 = a.indexOf("p");
     



      b1 = a.substring(a1 + 1, a2);
      b2 = a.substring(a3 + 1, a4);
      b3 = a.substring(a5 + 1, a6);
      b4 = a.substring(a7 + 1, a8);
      b5 = a.substring(a9 + 1, a10);
      b6 = a.substring(a11 + 1, a12);
      b7 = a.substring(a13 + 1, a14);
      b8 = a.substring(a15 + 1, a16);
     int b9 =0;
     int b10 =0;



      Serial.print("pt100 = ");
      Serial.println(b1);
      Serial.print("dalls = ");
      Serial.println(b2);
      Serial.print("tsl   = ");
      Serial.println(b3);
      Serial.print("humi  = ");
      Serial.println(b4);
      Serial.print("dust  = ");
      Serial.println(b5);
      Serial.print("bme280   = ");
      Serial.println(b6);
      Serial.print("ct    = ");
      Serial.println(b7);
      Serial.print("ktype = ");
      Serial.println(b9);
      Serial.print("ktyp1 = ");
      Serial.println(b10);
      Serial.print("bme680  = ");
      Serial.println(b8);
      digitalWrite(GPIO_PIN_NUMBER_27, LOW);
      digitalWrite(GPIO_PIN_NUMBER_26, HIGH);
      digitalWrite(GPIO_PIN_NUMBER_25, LOW);
      HTTPClient http;    //Declare object of class HTTPClient

      String postData;
      //Post Data
      postData = "temperature=" + String(b2) + "&current=" + (b7) + "&humidity=" + String(b4) + "&pressure=" + String(b6) + "&dust=" + String(b5) + "&lux=" + String(b3) + "&pt100=" + String(b1) + "&ktype=" + String(b10) + "&ktype1=" + String(b9) + "&pressure1=" + String(b8);


      http.begin("http://10.10.110.32/iot/insert-folder/insert.php?");              //Specify request destination
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

      int httpCode = http.POST(postData);   //Send the request
      String payload = http.getString();    //Get the response payload
      Serial.print("httpCode:");
      Serial.println(httpCode);
      if (httpCode == 200)
      {
        digitalWrite(GPIO_PIN_NUMBER_27, LOW);
        digitalWrite(GPIO_PIN_NUMBER_26, HIGH);
        digitalWrite(GPIO_PIN_NUMBER_25, HIGH);
      }
      else if (httpCode == -1)
      {
        digitalWrite(GPIO_PIN_NUMBER_27, HIGH);
        digitalWrite(GPIO_PIN_NUMBER_26, HIGH);
        digitalWrite(GPIO_PIN_NUMBER_25, LOW);
        Serial.println("connection failed");
        Serial.print("counter:");
        counter++;
        Serial.println(counter);
        if (counter == 10) {
          resetFunc();
        }
      }
      Serial.print("payload:");//Print HTTP return code
      Serial.println(payload);    //Print request response payload

      http.end();
      //Close connection}

      delay(5000);  //Post Data at every 5 seconds



    }

  }
delay(100);
}
void open_wifi() {
  WiFi.disconnect();
  Serial.println("Setup done");
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  WiFi.mode(WIFI_STA);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
    digitalWrite(GPIO_PIN_NUMBER_27, HIGH);
    digitalWrite(GPIO_PIN_NUMBER_26, LOW);
    digitalWrite(GPIO_PIN_NUMBER_25, LOW);
  }
  else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      byte encryption = WiFi.encryptionType(i);
      int temp = int(encryption);
      Serial.print("Encryption Type:");
      Serial.println(temp);
      if (!temp) {
        Serial.print("Open : ");
        Serial.println(WiFi.SSID(i));
        ssid1_len = WiFi.SSID(i).length() + 1;
        ssid1[ssid1_len];
        WiFi.SSID(i).toCharArray(ssid1, ssid1_len);
        break;
      }
      delay(10);
    }
  }
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid1);
  WiFi.begin(ssid1);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("1.");
  }
  // Wait a bit before scanning again
  delay(5000);
}
