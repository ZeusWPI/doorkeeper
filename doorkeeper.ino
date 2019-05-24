#include <PWMServo.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <ArduinoJson.h>

// Ethernet settings
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(10, 0, 1, 20);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

// Servo settings
PWMServo door;

// Ethernet functions
bool skipRespHeaders(EthernetClient *client){
  char endOfHeaders[] = "\r\n\r\n";
  bool skipped = client->find(endOfHeaders);
  if(!skipped){
  }
  return skipped;
}

bool handleIcoming(const char **command){
  EthernetClient client = server.available();
  if(client && skipRespHeaders(&client)){
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, client);
    if (error) {
      client.println("HTTP/1.1 500 ERROR");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.println(error.c_str());
      client.stop();
      return false;
    }
    *command = doc["command"];
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.print("JSON parsed: ");
    client.println(*command);
    delay(10);
    client.stop();
    return true;
  }else{
    return false;
  }
}

bool initEthernet(){
  // Network configuration
  byte mac[] = {0x90, 0xA2, 0xDA, 0x10, 0xFA, 0x91};
  IPAddress ip(10, 0, 1, 20);
  // Ethernet connection setup
  Ethernet.begin(mac, ip);
  return true;
  // IP 10.3.21.14
}

void maintainEthernet(){
  switch (Ethernet.maintain())
  {
    case 1:
      Serial.println("Error: renewed fail");
      break;
    case 2:
      Serial.println("Renewed success");
      break;
    case 3:
      Serial.println("Error: rebind fail");
      break;
    case 4:
      Serial.println("Rebind success");
      break;
    default:
      break;
  }
}

// Servo functions
void openDoor() {
  door.write(44);
}

void closeDoor() {
  door.write(134);
}

void halt() {
  door.write(94);
}

void handleCommand(const char **const command) {
  if(strcmp(*command,"open")==0){
    openDoor();
    delay(500);
    halt();
  } else if(strcmp(*command,"close")==0){
    closeDoor();
    delay(500);
    halt();
  } else {
    halt();
  }
}

void setup() {
  // Config servo motor control pin
  door.attach(SERVO_PIN_A);
  halt();

  // Ethernet setup
  while(!initEthernet()){
    // If init fails, keep retrying
  }
  server.begin();
}

void loop() {
  maintainEthernet();
  const char* command;
  bool processed = handleIcoming(&command);
  if (processed) {
    handleCommand(&command);
    processed = false;
    setup();
  } else {
    processed = false;
  }
  delay(100);
}

