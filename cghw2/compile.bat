gcc -c glew.c -DGLEW_STATIC -I.
gcc -c tiny_obj_loader.cc -DGLEW_STATIC -I.
gcc -c main.cpp -DGLEW_STATIC -I. -lglfw3 -lopengl32 -lgdi32 -D__NO_INLINE__ -static -O2 -std=c++0x
g++ -o HW glew.o tiny_obj_loader.o main.o -DGLEW_STATIC -I. -lglfw3 -lopengl32 -lgdi32 -D__NO_INLINE__ -static -O2 -std=c++0x
strip HW.exe
