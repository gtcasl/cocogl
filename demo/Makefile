all:
	$(CXX) main.cpp renderer.cpp mesh.cpp utils.cpp -std=c++17 -O0 -g -Wall -Wextra -pedantic -Wfatal-errors -L../src/GLES_CM -I../include -lm `sdl2-config --cflags --libs` -lGLES_CM -o demo
clean:
	-rm -f demo *.o