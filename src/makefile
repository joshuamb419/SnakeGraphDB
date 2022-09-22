CC = g++
CFLAGS = -std=c++11 -fdiagnostics-color=always -g
TARGET = ./target/
EXE_FILE = main
TAR_NAME = JoshuaBrown_Assign3

SRCEXT = cpp
SOURCES = $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS = $(addprefix $(TARGET),$(notdir $(SOURCES:.cpp=.o)))

build: $(TARGET) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXE_FILE)

test: $(TARGET) main.cpp
	$(CC) $(CFLAGS) main.cpp -o test

$(TARGET):
	mkdir $(TARGET)

$(TARGET)%.o: %.$(SRCEXT)
	$(CC) $(CFLAGS) -c -o ./$@ $<

tar:
	tar -cvf $(TAR_NAME).tar Makefile *.cpp *.h *.hpp

clean:
	rm -r $(TARGET)
	rm -f $(EXE_FILE)