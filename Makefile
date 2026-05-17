# Install
BIN = chirons_disk_blaster
# Flags
# CFLAGS += -g -std=c89 -Wall -Wextra -pedantic -DSDL_DISABLE_IMMINTRIN_H
CFLAGS += -g -std=gnu11 -Wall -Wextra -pedantic -DSDL_DISABLE_IMMINTRIN_H
SRC = main.c
OBJ = $(SRC:.c=.o)
ifeq ($(OS),Windows_NT)
BIN := $(BIN).exe
# LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lm -lGLU32 -lGLEW32 -mwindows
# Note: -mwindows is what prevents a new command line windows from opening when you double click the app. 
# Note: -lcomdlg32 is for the Windows dialog box. 
# Note: -ldwmapi for Dark Mode Windows stuff.  
# Note: -lsetupapi for USB Floppy Drive detection as well as the presence of a floppy disk in the USB Floppy Drive. 
# Note: -lcomctl32 is for the advanced system dialog box. 
# Note: -lole32 -luuid is for dialog box stuff to pick files or folders or some shit I dunno. 
# Note: -lGLEW32 is needed for OpenGL 3 I believe. 
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lm -lGLU32 -lGLEW32 -lcomdlg32 -ldwmapi -lsetupapi -lcomctl32 -lole32 -luuid -mwindows
RESOURCE_OBJ = resource.o
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		LIBS = -lSDL2 -framework OpenGL -lm -lGLEW
	else
		LIBS = -lSDL2 -lGL -lm -lGLU -lGLEW
	endif
RESOURCE_OBJ =
endif

$(BIN): $(RESOURCE_OBJ)
	@mkdir -p bin
	rm -f bin/$(BIN) $(OBJS)
	$(CC) $(SRC) $(CFLAGS) $(RESOURCE_OBJ) -o bin/$(BIN) $(LIBS)

resource.o: resource.rc icon.ico
	windres resource.rc -O coff -o resource.o

clean:
	rm -f resource.o bin/$(BIN)