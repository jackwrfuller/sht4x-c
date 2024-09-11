SRC_DIR = .

INCLUDES = -I./src -I./include -I./lib/sht4x -I./lib/cJSON

CFLAGS = -std=c99 -Os -Wall -fstrict-aliasing -Wstrict-aliasing=1 -Wsign-conversion -fPIC $(INCLUDES) 

#./lib/sht4x/sensirion_i2c_hal.h ./lib/sht4x/sensirion_i2c_hal.c 
LIBS = $(wildcard ${SRC_DIR}/lib/sht4x/*.c) $(wildcard ${SRC_DIR}/lib/sht4x/*.h)\
	   $(wildcard ${SRC_DIR}/lib/cJSON/*)
SRC = ./src/reading.c


ifdef CI
    CFLAGS += -Werror
endif

.PHONY: all clean

all: sensor

sensor: clean
	$(CC) $(CFLAGS) -o $@ $(LIBS) $(SRC) src/main.c 
 
clean:
	$(RM) sensor
