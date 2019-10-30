all:
	$(CXX) main.cpp test.cpp mesh.cpp -std=c++17 -O0 -g -Wall -Wextra -pedantic -Wfatal-errors -L../src/EGL -L../src/GLES -I../include -I../shared -lm `sdl2-config --cflags --libs` -lEGL -lGLESv1_CM -o demo
clean:
	-rm -f demo *.o