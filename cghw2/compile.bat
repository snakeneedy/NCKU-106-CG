@REM gcc -DGLEW_STATIC -I. -c glew.c
g++ -o HW -static -O2 -std=c++0x -DGLEW_STATIC main.cpp tiny_obj_loader.cc glew.o -I.  -lglfw3 -lopengl32 -lgdi32 -D__NO_INLINE__
strip HW.exe
