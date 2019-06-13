#include <PWMServo.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <ArduinoJson.h>

// token settings
const char *TOKEN="foobar";

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

// Servo settings
PWMServo door;

// Ethernet functions
// Skip all the HTTP headers, straight to the body
bool skipRespHeaders(EthernetClient* client){
  char endOfHeaders[] = "\r\n\r\n";
  bool skipped = client->find(endOfHeaders);
  if(!skipped){
  }
  return skipped;
}

// Authenticate against TOKEN
bool authenticate(char* token) {
  if ((strcmp(token, MMTOKEN)==0) || (strcmp(token, SLACKTOKEN)==0))  {
    return true;
  } else {
    return false;
  }
}

// verify if all json fields are present
bool verifyData(DynamicJsonDocument* doc) {
  bool userPresent = !(*doc)["username"].isNull();
  bool tokenPresent = !(*doc)["token"].isNull();
  bool textPresent = !(*doc)["text"].isNull();
  if (userPresent && tokenPresent && textPresent) {
    return true;
  }
  return false;
}

// Process an incoming command request
bool handleIncoming(char* command){
  EthernetClient client = server.available();
  if(client && skipRespHeaders(&client)){
    DynamicJsonDocument doc(512);
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
    bool dataPresent = verifyData(&doc);
    if (!dataPresent) {
      client.println("HTTP/1.1 500 ERROR");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.println("Not all data fields were present");
      client.stop();
      return false;
    }
    const char *recvToken = doc["token"];
    bool auth = authenticate(recvToken);
    if (auth) {
      //*command = doc["text"];
      strcpy(command, doc["text"]);
      char* user = doc["username"];
      char* msg = strcat(user,":");
      char* rsp_msg = strcat(msg,command);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.println(rsp_msg);
      delay(10);
      client.stop();
      return true;
    } else {
      client.println("HTTP/1.1 403 UNAUTHORIZED");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.println("Invalid token");
      delay(10);
      client.stop();
      return false;
    }
  } else {
    return false;
  }
}

bool initEthernet(){
  // Network configuration
  byte mac[] = {0x90, 0xA2, 0xDA, 0x10, 0xFA, 0x91};    // MAC address
  IPAddress ip(10, 0, 1, 5);                           // IP address
  // Ethernet connection setup
  Ethernet.begin(mac, ip);
  return true;
}

// Maintain ethernet connection
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

/*
  Servo functions
  94 ms is hold pos
  > 94 -> open, CCW from the back
  < 94 -> close, CW from the back
*/
// Functional functions
void openDoor() {
  door.write(10);
}
void closeDoor() {
  door.write(170);
}
void halt() {
  door.write(94);
}
void lockDoor() {
  closeDoor();
  delay(2250);
  halt();
}

// Process a command
void handleCommand(char* command, int operTime) {
  if(strcmp(command,"open")==0){
    openDoor();
    delay(operTime);
    halt();
  } else if(strcmp(command,"close")==0) {
    closeDoor();
    delay(operTime);
    halt();
  } else if(strcmp(command,"lock")==0) {
    lockDoor();
  } else if(strcmp(command,"delay")==0) {
    delay(15000);
    lockDoor();
  } else {
    halt();
  }
}

void setup() {
  // Config servo motor control pin, 500-2500 is for the 10kg/cm servo
  door.attach(SERVO_PIN_A);
  halt();

  // Ethernet setup
  while(!initEthernet()){
    // If init fails, keep retrying
  }

  // Start the webserver
  server.begin();
  Serial.println("Booting ...");
}

// Global variables for the loop
char rxCommand[256];
bool processed = false;

void loop() {
  // Maintain the ethernet connection
  maintainEthernet();

  // Open/close doors
  processed = handleIncoming(rxCommand);
  if (processed) {
      handleCommand(rxCommand, 1750);
    processed = false;
    delay(100);
  }
  delay(50);
}

