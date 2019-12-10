all:
	$(CXX) main.cpp renderer.cpp md2.cpp utils.cpp -std=c++17 -O0 -g -Wall -Wextra -pedantic -Wfatal-errors -L../src/GLES_CM -I../include -lm `sdl2-config --cflags --libs` -lGLES_CM -o md2viewer
	
clean:
	-rm -f md2viewer *.o