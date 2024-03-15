install:
	git clone https://github.com/libsdl-org/SDL.git -b SDL2
	cd SDL && mkdir build
	cd SDL/build && ../configure
	cd SDL/build && make
	cd SDL/build && sudo make install

uninstall: clean
	cd SDL/build && sudo make uninstall
	rm -rf SDL

build:
	gcc -o exec JeuDeLaVie.c -lSDL

run: build
	./exec

clean:
	rm -f exec