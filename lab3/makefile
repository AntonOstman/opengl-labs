# set this variable to the director in which you saved the common files
commondir = ../common/

all : lab1

lab1 : lab1-1.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Windows/MicroGlut.c $(commondir)Windows/glew.c
	g++ -Wall -o lab1-1 -I$(commondir) -I../common/Windows -DGL_GLEXT_PROTOTYPES lab1-1.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Windows/MicroGlut.c $(commondir)Windows/glew.c -lopengl32 -lglu32 -lgdi32

lab12 : la1-2.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Windows/MicroGlut.c $(commondir)Windows/glew.c
	g++ -Wall -o lab1-2 -I$(commondir) -I../common/Windows -DGL_GLEXT_PROTOTYPES la1-2.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Windows/MicroGlut.c $(commondir)Windows/glew.c -lopengl32 -lglu32 -lgdi32

lab2 : la1-2.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Windows/MicroGlut.c $(commondir)Windows/glew.c
	g++ -Wall -o lab1-2 -I$(commondir) -I../common/Windows -DGL_GLEXT_PROTOTYPES la1-2.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Windows/MicroGlut.c $(commondir)Windows/glew.c -lopengl32 -lglu32 -lgdi32

lab22 : lab2-2.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Windows/MicroGlut.c $(commondir)Windows/glew.c
	g++ -Wall -o lab -I$(commondir) -I../common/Windows -DGL_GLEXT_PROTOTYPES lab2-2.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Windows/MicroGlut.c $(commondir)Windows/glew.c -lopengl32 -lglu32 -lgdi32

lab23 : lab2-3.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Windows/MicroGlut.c $(commondir)Windows/glew.c
	g++ -Wall -o lab23 -I$(commondir) -I../common/Windows -DGL_GLEXT_PROTOTYPES lab2-3.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Windows/MicroGlut.c $(commondir)Windows/glew.c -lopengl32 -lglu32 -lgdi32

lab31 : lab31.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Windows/MicroGlut.c $(commondir)Windows/glew.c
	g++ -Wall -o lab31 -I$(commondir) -I../common/Windows -DGL_GLEXT_PROTOTYPES lab31.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Windows/MicroGlut.c $(commondir)Windows/glew.c -lopengl32 -lglu32 -lgdi32

linux : lab31.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	g++ -Wall -o lab31 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab31.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm
clean :
	rm lab1-1

