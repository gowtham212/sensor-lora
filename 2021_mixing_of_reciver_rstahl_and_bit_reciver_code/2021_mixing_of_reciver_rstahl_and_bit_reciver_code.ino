#include <SPI.h>
#include <LoRa.h>
#include <Ethernet.h>
#define ss 3
#define rst 9
#define dio0 2
String b1, b2, b3, b4, b5, b6, b7, b8;
int b9 = 0.0;
int b10 = 0.0;
int b11, b21, b31, b41, b51, b61, b71, b81, b91;
byte mac[] = { 0xAE, 0xAD, 0xDE, 0xEF, 0xEE, 0xED };
IPAddress dnServer(172, 30, 29, 1);
// the router's gateway address:
// the subnet:
IPAddress subnet(255, 255, 255, 0);

//IPAddress ip(172, 30, 29, 21); //rstahl ip
//IPAddress gateway(172, 30, 29, 1);//rstahl gatway
//char serv[] = "s-iot01.stahl.de";//rstahl server name

IPAddress ip(10, 10, 43, 15);
IPAddress gateway(10, 10, 40, 1);
byte serv[] = {172, 16, 5, 233} ; //Enter the IPv4 address // inner ip bit

EthernetClient cliente;

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10 * 1000; // delay between updates, in milliseconds

int counter = 0;

void ethernet()
{
  //new****
  Ethernet.init(10);  // Most Arduino shields
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, dnServer, gateway, subnet);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.print("connecting to server");
  //  Serial.print(String(serv));
  Serial.println("...");
  // end ****
}

void setup() {


  Serial.begin(9600);
  pinMode(5, OUTPUT); //red
  pinMode(6, OUTPUT); //blue



  while (!Serial);
  Serial.println("LoRa Reciver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  LoRa.setPreambleLength(8);
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  ethernet();


}
void(* resetFunc) (void) = 0;//declare reset function at address 0

void lora() {

  int packetSize = LoRa.parsePacket();
  digitalWrite(6, LOW); //blue
  if (packetSize) {
    // received a packet
    Serial.println("Received packet '");

    // read packet
    while (LoRa.available()) {
      String a = LoRa.readString();
      Serial.println(a);
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
      Serial.print("bme680  = ");
      Serial.println(b8);
      Serial.print("ktype  = ");
      Serial.println(b9);
      Serial.print("ktype1  = ");
      Serial.println(b10);
      digitalWrite(6, HIGH); //blue
      digitalWrite(5, LOW); //red
      Ethernet.maintain();
    }
  }
}

void sender() {
  cliente.stop();
   //if (cliente.connect(serv, 81)) { //rstahl port
   if (cliente.connect(serv, 80)) { //Connecting at the IP address and port we saved before
   Serial.println("connected");
    cliente.print("GET /projects/iot/insert-folder/insert.php?");
    //cliente.print("GET /iot/insert-folder/insert.php?");//rstahl url
    cliente.print("temperature=");
    cliente.print(b2);
    cliente.print("&current=");
    cliente.print(b7);
    cliente.print("&humidity=");
    cliente.print(b4);
    cliente.print("&pressure=");
    cliente.print(b6);
    cliente.print("&dust=");
    cliente.print(b5);
    cliente.print("&lux=");
    cliente.print(b3);
    cliente.print("&pt100=");
    cliente.print(b1);
    cliente.print("&ktype=");
    cliente.print(b9);
    cliente.print("&ktype1=");
    cliente.print(b10);
    cliente.print("&pressure1=");
    cliente.println(b8);
    Serial.println("inserted");
    counter=0;


     while (cliente.connected()) {
      if (cliente.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = cliente.read();
        Serial.print(c);
      }
    }
    lastConnectionTime = millis();
    Serial.println("disconnected");


  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
    digitalWrite(5, HIGH);
    counter++;
    Serial.print("counter");
    Serial.println(counter);
    if (counter == 40) {
      delay(5000);
      resetFunc(); //call reset
      counter = 0;
    }
  }
}
void loop() {
  lora();
  if (millis() - lastConnectionTime > postingInterval) {
    sender();
  }
  delay(100);
}
