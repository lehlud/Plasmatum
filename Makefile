all:
	mkdir -p build
	cmake -Bbuild .
	cmake --build build
	strip build/plasmatum

clean:
	rm -rf build

install: all
	cp build/plasmatum /usr/local/bin/
