build-x11:
	gcc main.c -o kmenu -lraylib -lm -lX11
build:
	gcc main.c -o kmenu -lraylib -lm -lwayland-client

build-out:
	gcc main.c -lraylib -lm -lX11

run: build-x11
	./kmenu -f ~/.config/kmenu/config
	# ./kmenu -h
	#

