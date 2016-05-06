TARGET = pr01

make: glew.o
	g++ -o ${TARGET} main.cpp glew.o -std=c++0x -DGLEW_STATIC -I. -lglfw3 -lopengl32 -lgdi32

glew.o: glew.c
	gcc -c glew.c -I. -DGLEW_STATIC

clean:
	rm ${TARGET} *.o

