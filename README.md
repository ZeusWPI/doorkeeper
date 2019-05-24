# doorkeeper

We needed something that kan easily be matched to a lock, so we build this. It uses an Arduino
based microcontroller, ethernet shield (can be replaced by other networking gear) and a
servomotor.

The Arduino hosts a simple webserver that responds to JSON requests. Prototyping makes uses of
the `"command"` key.

At the time of writing the following commands are implementen:

* `{"command":"open"}`: opens the door
* `{"command":"close"}`: closes the door
