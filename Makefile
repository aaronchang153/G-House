CC=g++
CFLAGS=-Wall -c -I src
LFLAGS=-lbluetooth -lpthread

OBJ_DIR=obj
BIN_DIR=bin

VPATH=src/:test/

%.o: %.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/$@ $<

btserverUnitTest: btserver.o btserverUnitTest.o | $(BIN_DIR)
	$(CC) $(LFLAGS) -o $(BIN_DIR)/$@ $(addprefix $(OBJ_DIR)/, $^)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(BIN_DIR):
	mkdir $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(BIN_DIR)/btserverUnitTest
