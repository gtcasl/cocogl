.PHONY: shared EGL GLES_CM GLESv1

all: shared EGL GLES_CM GLESv1

shared:
	$(MAKE) -C shared

EGL:
	$(MAKE) -C EGL

GLES_CM:
	$(MAKE) -C GLES_CM

GLESv1:
	$(MAKE) -C GLESv1
	
clean:
	$(MAKE) -C shared clean
	$(MAKE) -C EGL clean
	$(MAKE) -C GLES_CM clean
	$(MAKE) -C GLESv1 clean