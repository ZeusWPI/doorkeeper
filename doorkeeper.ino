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
bool skipRespHeaders(EthernetClient *client){
  char endOfHeaders[] = "\r\n\r\n";
  bool skipped = client->find(endOfHeaders);
  if(!skipped){
  }
  return skipped;
}

// Authenticate against TOKEN
bool authenticate(const char **token) {
  if (strcmp(*token, TOKEN)) {
    return true;
  } else {
    return false;
  }
}

// Process an incoming command request
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
    const char *recvToken = doc["token"];
    bool auth = authenticate(&recvToken);
    if (auth) {
      *command = doc["text"];
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.print("JSON parsed: ");
      client.println(*command);
      delay(10);
      client.stop();
      return true;
    } else {
      client.println("HTTP/1.1 403 UNAUTHORIZED");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.print("invalid_token_error");
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
  IPAddress ip(10, 0, 1, 20);                           // IP address
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
  1510 ms is hold pos
  > 1515 -> close, CW from the back
  < 1505 -> open, CCW from the back
*/
// Functional functions
void openDoor() {
  door.write(44);
}
void closeDoor() {
  door.write(134);
}
void halt() {
  door.write(94);
}

// Process a command
void handleCommand(const char **const command, int operTime) {
  if(strcmp(*command,"open")==0){
    openDoor();
    delay(operTime);
    halt();
  } else if(strcmp(*command,"close")==0){
    closeDoor();
    delay(operTime);
    halt();
  } else {
    halt();
  }
}

void setup() {
  // Config servo motor control pin, 500-2500 is for the 10kg/cm servo
  door.attach(SERVO_PIN_A, 500, 2500);
  halt();

  // Ethernet setup
  while(!initEthernet()){
    // If init fails, keep retrying
  }

  // Start the webserver
  server.begin();
}

void loop() {
  // Maintain the ethernet connection
  maintainEthernet();

  // Open/close doors
  const char* command;
  bool processed = handleIcoming(&command);
  if (processed) {
    handleCommand(&command, 500);
    processed = false;
    setup();
  } else {
    processed = false;
  }

  // Wait for next loop
  delay(100);
}

