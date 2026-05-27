# KMENU

## A simple menu widget in c using raylib for rendering.

This is meant to be a menu widget for tiling window managers.

The usage is

```c
kmenu -f /home/user/.config/kmenu/config
```

The file can be anywhere and its a normal text file.

Usage:
This will only accept 3 flags

-h Help menu <br>
-f file flag, eg. `kmenu -f ~/.config/kmenu/config` <br>
-s string flag eg. `kmenu -s Shutdown;shutdown,Reboot;reboot\" ";" ` (Not yet available)

<br> The ";" is the delimiter used to separate commands.
<br> The supported formant for options is `[label];[command]` eg. `"Power off;/sbin/shutdown"`

```
#File example
ShutDown;/sbin/poweroff
Reboot;/sbin/reboot
Lock; swaylock &
Logout;hyprctl dispatch exit
```

## Build

Make is used for builds:

```
make build
make build-x11
```
