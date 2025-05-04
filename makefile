# NOTE: this makefile only works on windows, not on linux ;p

CC = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -lws2_32 -pthread

SRV_DIR = srv
SRC_DIR = srv/src

SRV_PROD = srv_prod

BIN = $(SRV_PROD)/chat_server

SOURCES = $(SRV_DIR)/chat_server.cpp $(SRC_DIR)/server.cpp


OBJECTS = $(patsubst %.cpp, $(SRV_PROD)/%.o, $(notdir $(SOURCES)))

all: $(SRV_PROD) $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN) -lws2_32

$(SRV_PROD)/%.o: $(SRV_DIR)/%.cpp | $(SRV_PROD)
	$(CC) $(CXXFLAGS) -c $< -o $@

$(SRV_PROD)/%.o: $(SRC_DIR)/%.cpp | $(SRV_PROD)
	$(CC) $(CXXFLAGS) -c $< -o $@

$(SRV_PROD):
	if not exist $(SRV_PROD) mkdir $(SRV_PROD)

run_server: $(BIN)
	./$(BIN)

clean:
	del /Q $(subst /,\,$(SRV_PROD)\*.o) $(subst /,\,$(BIN))
	if exist $(SRV_PROD) rmdir /S /Q $(SRV_PROD)

.PHONY: all run_server clean
