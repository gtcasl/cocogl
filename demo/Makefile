all:
	$(CXX) main.cpp test.cpp mesh.cpp -std=c++17 -O0 -g -Wall -Wextra -pedantic -Wfatal-errors -L../src/GLES_CM -I../include -I../shared -lm `sdl2-config --cflags --libs` -lGLES_CM -o demo
clean:
	-rm -f demo *.o