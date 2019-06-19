PROJECT=./
TOOL=arduino-cli
PLATFORM=arduino:avr:uno
DEV=/dev/ttyACM0

rebuild: compile upload

upload:
	$(TOOL) upload -b $(PLATFORM) -p $(DEV) $(PROJECT)

compile:
	$(TOOL) compile -b $(PLATFORM) $(PROJECT)
