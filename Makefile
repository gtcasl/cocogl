.PHONY: third_party src demo md2viewer

all: third_party src demo md2viewer

third_party:
	$(MAKE) -C third_party

src:
	$(MAKE) -C src

demo:
	$(MAKE) -C demo

md2viewer:
	$(MAKE) -C md2viewer

run-demo:
	$(MAKE) -C demo run

run-md2viewer:
	$(MAKE) -C md2viewer run
	
clean:
	$(MAKE) -C third_party clean
	$(MAKE) -C src clean
	$(MAKE) -C demo clean
	$(MAKE) -C md2viewer clean