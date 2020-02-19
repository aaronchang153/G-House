CC=g++
CFLAGS=-Wall -c -I src -g
LFLAGS=-lbluetooth -lpthread -lwiringPi

OBJ_DIR=obj
BIN_DIR=bin

VPATH=src/:test/

ghouse: btserver.o log.o main.o periph.o sensor.o | $(BIN_DIR)
	$(CC) $(LFLAGS) -o $(BIN_DIR)/$@ $(addprefix $(OBJ_DIR)/, $^)

btserverUnitTest: btserver.o btserverUnitTest.o | $(BIN_DIR)
	$(CC) $(LFLAGS) -o $(BIN_DIR)/$@ $(addprefix $(OBJ_DIR)/, $^)

sensorUnitTest: sensor.o sensorUnitTest.o | $(BIN_DIR)
	$(CC) $(LFLAGS) -o $(BIN_DIR)/$@ $(addprefix $(OBJ_DIR)/, $^)

sensorContinuous: sensor.o sensorContinuous.o | $(BIN_DIR)
	$(CC) $(LFLAGS) -o $(BIN_DIR)/$@ $(addprefix $(OBJ_DIR)/, $^)

periphUnitTest: periph.o periphUnitTest.o | $(BIN_DIR)
	$(CC) $(LFLAGS) -o $(BIN_DIR)/$@ $(addprefix $(OBJ_DIR)/, $^)

%.o: %.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/$@ $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(BIN_DIR):
	mkdir $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(BIN_DIR)/*
