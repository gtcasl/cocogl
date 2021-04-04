.PHONY: EGL GLES_CM GLESv1

all: EGL GLES_CM GLESv1

EGL:
	$(MAKE) -C EGL

GLES_CM:
	$(MAKE) -C GLES_CM

GLESv1:
	$(MAKE) -C GLESv1
	
clean:
	$(MAKE) -C EGL clean
	$(MAKE) -C GLES_CM clean
	$(MAKE) -C GLESv1 clean