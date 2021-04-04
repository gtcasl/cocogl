.PHONY: shared src demo md2viewer

all: shared src demo md2viewer

shared:
	$(MAKE) -C shared

src:
	$(MAKE) -C src

demo:
	$(MAKE) -C demo

md2viewer:
	$(MAKE) -C md2viewer
	
clean:
	$(MAKE) -C shared clean
	$(MAKE) -C src clean
	$(MAKE) -C demo clean
	$(MAKE) -C md2viewer clean