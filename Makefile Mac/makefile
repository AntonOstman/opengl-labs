# set this variable to the director in which you saved the common files
commondir = ../common/

all : lab1-1

lab1-1 : lab1-1.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o lab1-1 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab1-1.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -lm -lstdc++ -framework OpenGL -framework Cocoa -framework CoreGraphics

clean :
	rm lab1-1
