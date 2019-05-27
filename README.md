# doorkeeper

We needed something that kan easily be matched to a lock, so we build this. It uses an Arduino
based microcontroller, ethernet shield (can be replaced by other networking gear) and a
servomotor.

The Arduino hosts a simple webserver that responds to JSON requests. Prototyping makes uses of
the `"text"` key.

At the time of writing the following commands are implementen:

* `{"text":"open"}`: opens the door
* `{"text":"close"}`: closes the door

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


## Maintainer

Francis Begyn  
francis.begyn@gmail.com
