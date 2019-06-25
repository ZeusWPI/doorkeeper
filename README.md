# doorkeeper

We needed something that kan easily be matched to a lock, so we build this. It uses an Arduino
based microcontroller, ethernet shield (can be replaced by other networking gear) and a
servomotor.

The servo motor is powered seperately from the Arduino. Powering the servo motor from the Arduino
causes the Arduino voltage level to drop and the flash memory loses power. This basically makes it so
the Arduino "clears" all the data from the flash memory, losing the ip address, mac address, ...

The Arduino hosts a simple webserver that responds to JSON requests. Prototyping makes uses of
the `"text"` key.

At the time of writing the following commands are implementen:

* `{"text":"open"}`: opens the door
* `{"text":"close"}`: closes the door

In the kicad folder, the KiCAD files can be found (schematic + board files - eventually).

## Incoming requests

The incoming requests have the following structure

```
{
  "username": "username",
  "token": "slotmachien_token",
  "text": "command"
}
```

NOTICE: It might be obvious, but for obvious reasons the token should be changed on deployement. The
token is stored in the TOKEN constant.

## Delayed close button

The delayed close button will close the door after 10 seconds. During this period ll functionality of
doorkeeper is disable. Currently the setup is wired to trigger on the falling edge of a signal. By
default the Arduino pin is HIGH. When the button is pressed the signal gets pulled LOW. This triggers
the delayed close.

## Maintainer

Francis Begyn  
francis.begyn@gmail.com
