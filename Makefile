build-x11:
	gcc main.c -o kmenu -lraylib -lm -lX11
build:
	gcc main.c -o kmenu -lraylib -lm -lwayland-client
run: build-x11
	# ./kmenu -f ~/.config/kmenu/config
	./kmenu -h
