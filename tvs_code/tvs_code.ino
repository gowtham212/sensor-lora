// Select your modem:
#define TINY_GSM_MODEM_SIM800
// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon
// Range to attempt to autobaud
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200
// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(2,3); // tx pin to d2 rx pin d3
float p25 = 10.0;
float y = 10.0;
float temp = 10.0;

#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "bsnlnet";
const char gprsUser[] = "";
const char gprsPass[] = "";


const char server[] = "apps.bittechpro.in";

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#ifdef USE_SSL
TinyGsmClientSecure client(modem);
const int  port = 443;
#else
TinyGsmClient client(modem);
const int  port = 80;
#endif

void setup() {
  // Set console baud rate
  SerialMon.begin(115200);
  delay(10);
  SerialMon.println("Wait...");

  // Set GSM module baud rate
  // TinyGsmAutoBaud(SerialAT,GSM_AUTOBAUD_MIN,GSM_AUTOBAUD_MAX);
  SerialAT.begin(9600);
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  modem.restart();
  // modem.init();

  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem: ");
  SerialMon.println(modemInfo);

#if TINY_GSM_USE_GPRS
  // Unlock your SIM card with a PIN if needed
  if ( GSM_PIN && modem.getSimStatus() != 3 ) {
    modem.simUnlock(GSM_PIN);
  }
#endif
}

void loop() {


#if TINY_GSM_USE_GPRS && defined TINY_GSM_MODEM_XBEE
  // The XBee must run the gprsConnect function BEFORE waiting for network!
  modem.gprsConnect(apn);
#endif

  SerialMon.print("Waiting for network...");
  if (!modem.waitForNetwork(240000L)) {
    SerialMon.println(" fail");
    delay(3000);
    return;
  }
  SerialMon.println(" OK");

  if (modem.isNetworkConnected()) {
    SerialMon.println("Network connected");
  }

#if TINY_GSM_USE_GPRS && defined TINY_GSM_MODEM_HAS_GPRS
  SerialMon.print(F("Connecting to "));
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn)) {
    SerialMon.println(" fail");
    delay(3000);
    return;
  }
  SerialMon.println(" OK");
#endif

  SerialMon.print("Connecting to ");
  SerialMon.println(server);
  if (!client.connect(server, port)) {
    SerialMon.println(" fail");
    delay(3000);
    return;
  }
  SerialMon.println(" OK");
  String postData;
  //Post Data
  postData = "/projects/tvs1/data.php?" ;

  postData += "temperature=" + String(temp) + "&dust=" + (p25) + "&current=" + String(y) ;
  // Make a HTTP GET request:
  Serial.println(postData);
  SerialMon.println("Performing HTTP GET request...");
  client.print(String("GET ") + postData + " HTTP/1.1\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.print("Connection: close\r\n\r\n");

  client.println();

  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 10000L) {
    // Print available data
    while (client.available()) {
      char c = client.read();
      SerialMon.print(c);
      timeout = millis();
    }
  }
  SerialMon.println();

  client.stop();
  SerialMon.println(F("Server disconnected"));
 delay(3000);

}
