all: stealth-converter

stealth-converter:
	cd build/ && make && mv $@ ..

clean:
	rm -f stealth-converter

distclean: clean
	rm -rf build/

.PHONY: clean distclean stealth-converter
