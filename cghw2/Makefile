TARGET = HW
OBJS = glew.o \
	   tiny_obj_loader.o \
	   main.o

CCFLAG = -DGLEW_STATIC -I.
CPFLAG = -DGLEW_STATIC -I. -lglfw3 -lopengl32 -lgdi32 -D__NO_INLINE__ -static -O2 -std=c++0x

build: glew.o tiny_obj_loader.o main.o
	g++ -o ${TARGET} ${OBJS} ${CPFLAG}
glew.o: glew.c
	gcc -c glew.c ${CCFLAG}
tiny_obj_loader.o: tiny_obj_loader.cc
	gcc -c tiny_obj_loader.cc ${CCFLAG}
main.o: main.cpp
	gcc -c main.cpp ${CPFLAG}
clean:
	rm ${OBJS} ${TARGET}
