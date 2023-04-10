CC = g++
CFLAGS = -std=c++20 -fdiagnostics-color=always -g
TARGET = ./target/
EXE_FILE = snake_graph
DBFOLDER = ./db_folder/
TAR_NAME = SnakeGraphDB

SRCEXT = cpp
SOURCES = $(shell find ./src/ -type f -name "*.$(SRCEXT)")
OBJECTS = $(addprefix $(TARGET),$(notdir $(SOURCES:.cpp=.o)))

build: $(TARGET) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXE_FILE)

run: $(DBFOLDER)
	make build
	./$(EXE_FILE)

test: $(TARGET) main.cpp
	$(CC) $(CFLAGS) main.cpp -o test

$(TARGET):
	mkdir $(TARGET)

$(DBFOLDER):
	mkdir $(DBFOLDER)

$(TARGET)%.o: ./src/%.$(SRCEXT)
	$(CC) $(CFLAGS) -c -o ./$@ $<

tar:
	tar -cvf $(TAR_NAME).tar Makefile *.cpp *.h *.hpp

clean:
	rm -r $(TARGET)
	rm -f $(EXE_FILE)