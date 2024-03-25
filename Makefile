OFiles=display.o game.o main.o
SDLFlags=`sdl2-config --cflags --libs` -lSDL2 -lSDL2_ttf

install:
	sudo apt install libsdl2-dev libsdl2-2.0-0 -y
	sudo apt install libfreetype6-dev libsdl2-ttf-dev libsdl2-ttf-2.0-0 -y

uninstall: clean
	sudo apt remove libsdl2-dev libsdl2-2.0-0 -y
	sudo apt remove libfreetype6-dev libsdl2-ttf-dev libsdl2-ttf-2.0-0 -y

build: $(OFiles)
	gcc -o exec $^ $(SDLFlags)

%.o: %.c
	gcc -o $@ -c $< $(SDLFlags)

run: build
	./exec

clean:
	rm -f $(OFiles) exec