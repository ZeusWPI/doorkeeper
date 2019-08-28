#include <PWMServo.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include "./tokens.h"

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
EthernetClient client;

// Servo settings
PWMServo door;

// Ethernet functions
// Skip all the HTTP headers, straight to the body
bool skipRespHeaders(EthernetClient* client)
{
    char endOfHeaders[] = "\r\n\r\n";
    bool skipped = client->find(endOfHeaders);
    if(!skipped){
    }
    return skipped;
}

// Authenticate against TOKEN
bool authenticate(const String token)
{
    if (token.equals(MMTOKEN) || token.equals(SLACKTOKEN))  {
        return true;
    } else {
        return false;
    }
}

// verify if all json fields are present
bool verifyData(DynamicJsonDocument* doc)
{
    bool userPresent = !(*doc)["username"].isNull();
    bool tokenPresent = !(*doc)["token"].isNull();
    bool textPresent = !(*doc)["text"].isNull();
    if (userPresent && tokenPresent && textPresent) {
        return true;
    }
    return false;
}

// Process an incoming command request
bool handleIncoming(String *command, String *user)
{
    client = server.available();
    if(client && skipRespHeaders(&client)){
        DynamicJsonDocument doc(256);
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
        const String recvToken = doc["token"];
        bool auth = authenticate(recvToken);
        if (auth) {
            *command = String((char*)doc["text"]);
            *user = String((char*)doc["username"]);
            String rsp_msg = String(*user+":"+*command);
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

// Sends the processed commands back to mattermore to send to the channel
bool mattermoreResponse(String *command, String *user)
{
    if (client.connect(MATTERMORE_SERVER_HOST, MATTERMORE_SERVER_PORT))
    {
        String msg = String("command="+*command+"&user="+*user+"&token="+DOORKEEPER_TOKEN);
        client.println("POST /doorkeeper HTTP/1.1");
        client.println("User-Agent: Arduino/1.0");
        client.println("Host: mattermost.zeus.gent");
        client.println("Accept: */*");
        client.println("Accept-Encoding: gzip, deflate");
        client.println("Connection: close");
        client.println("Content-Type: application/x-www-form-urlencoded; charset=utf-8");
        client.print("Content-Length: ");
        client.println(msg.length());
        client.println();
        client.println(msg);
        delay(250);
        client.stop();
        return true;
    }
    client.stop();
    return false;
}

bool initEthernet()
{
    // Network configuration
    byte mac[] = {0x90, 0xA2, 0xDA, 0x10, 0xFA, 0x91};    // MAC address
    IPAddress ip(10, 0, 1, 5);                           // IP address
    IPAddress subnetMask(255,255,0,0);
    IPAddress gateway(10,0,0,1);
    // Ethernet connection setup
    Ethernet.begin(mac, ip);
    Ethernet.setSubnetMask(subnetMask);
    Ethernet.setGatewayIP(gateway);
    return true;
}

// Maintain ethernet connection
void maintainEthernet()
{
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
void openDoor()   {door.write(10);}
void closeDoor()  {door.write(170);}
void halt()       {door.write(94);}
void lockDoor()
{
    closeDoor();
    delay(2250);
    halt();
}
void delayedClose(int dur)
{
    delay(dur);
    lockDoor();
}

void triggerDelayedButton(void)
{
    Serial.println("Closing door");
    delayedClose(1);
}

// Process a command
void handleCommand(String *command, int operTime)
{
    if(command->equals("open")){
        openDoor();
        delay(operTime);
        halt();
    } else if(command->equals("close")) {
        closeDoor();
        delay(operTime);
        halt();
    } else if(command->equals("lock")) {
        lockDoor();
    } else if(command->equals("delay")) {
        delayedClose(10000);
    } else {
        halt();
    }
}

// button variables
const int buttonPin = 3;
int buttonState;
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;

// Global variables for the loop
String rxCommand;
String username;
bool processed = false;

void setup()
{
    Serial.begin(9600);
    Serial.println("Booting up ...");
    delay(1000);

    //Serial.println("Initialising servo controller ...");
    // Config servo motor control pin, 500-2500 is for the 10kg/cm servo
    door.attach(SERVO_PIN_A);
    halt();

    //Serial.println("Initialising internet ...");
    // Ethernet setup
    while(!initEthernet()){
      // If init fails, keep retrying
    }

    //Serial.println("Starting webserver ...");
    // Start the webserver
    server.begin();

    // button for delayed close
    pinMode(buttonPin, INPUT);

    //Serial.println("Doorkeeper: online");
}

void loop()
{
    // Maintain the ethernet connection
    maintainEthernet();

    // Open/close doors
    processed = handleIncoming(&rxCommand, &username);
    if (processed) {
        mattermoreResponse(&rxCommand, &username);
        handleCommand(&rxCommand, 1750);
        processed = false;
        delay(100);
    }

    buttonState = digitalRead(buttonPin);
    if (buttonState != lastButtonState) {
        if (buttonState == LOW) {
            mattermoreResponse(&String("delay"),&String("Doorkeeper"));
            delayedClose(10);
        }
    }
    lastButtonState = buttonState;
    delay(50);
}
