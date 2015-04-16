CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	LDFLAGS = -lglut -lGLU -lGL
endif
	
RM = /bin/rm -f
OUTPUT = as3
SOURCES = main.cpp vertex.cpp vector.cpp
OBJECTS = $(SOURCES:%.cpp=%.o)
all: main
main: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(OUTPUT) $(LDFLAGS)
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean: 
	$(RM) *.o $(OUTPUT)