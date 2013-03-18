CC = g++
OFLAG = -o
CFLAGS = -c -Wall -g -DDEBUG
LIB = -lglew32 -lfreeglut64 -lopengl32 -lglu32
WIN_GUI_FLAG = -Wl,--subsystem,windows
LIB_DIR = -L"d:\MinGW\include\freeglut\lib" -L"D:\MinGW\include\glew\lib"
INC_DIR = -I"d:\MinGW\include\freeglut\include" -I"D:\MinGW\include\glew\include"


all: main.o
	$(CC) $(OFLAG) main main.o  $(LIB_DIR) $(LIB) $(WIN_GUI_FLAG)
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(INC_DIR)
clean:
	rm main.o main
