TARGET = HW
OBJS = glew.o \
	   tiny_obj_loader.o \
	   main.o

CCFLAG = -DGLEW_STATIC -I.
CPFLAG = -DGLEW_STATIC -I. -lglfw3 -lopengl32 -lgdi32 -D__NO_INLINE__ -static -O2 -std=c++0x

build: ${OBJS}
	g++ -o ${TARGET} ${OBJS} ${CPFLAG}
%.o: %.c
	gcc -c $< ${CCFLAG}
%.o: %.cc
	gcc -c $< ${CCFLAG}
%.o: %.cpp
	gcc -c $< ${CPFLAG}
clean:
	rm ${OBJS} ${TARGET}
