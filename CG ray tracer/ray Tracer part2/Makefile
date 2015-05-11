CC=g++
LDFLAGS:=-lGL -lGLU -lglut 

OBJECTS=raytracer.o
SOURCE=raytracer.cpp

all: raytracer

raytracer: $(OBJECTS)
		$(CC) $(OBJECTS) -o raytracer $(LDFLAGS)
