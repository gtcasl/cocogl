all:
	$(CXX) main.cpp renderer.cpp md2.cpp utils.cpp -std=c++17 -O0 -g -Wall -Wextra -pedantic -Wfatal-errors -L../src/GLES_CM -I../include -lm `sdl2-config --cflags --libs` -lGLES_CM -o md2viewer
	
run:
	LD_LIBRARY_PATH=../src/GLES_CM:$(LD_LIBRARY_PATH) ./md2viewer models/tekkaman.md2

clean:
	-rm -f md2viewer *.o