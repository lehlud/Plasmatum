all:
	mkdir -p build
	cmake -Bbuild .
	cmake --build build
	strip build/plsm

clean:
	rm -rf build
