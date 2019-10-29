all:
	$(CC) hello.c -O0 -g -L../src/EGL -L../src/GLES -I../include -lm `sdl2-config --cflags --libs` -lEGL -lGLESv1_CM -o hello
clean:
	-rm -f hello *.o