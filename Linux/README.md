# SDL-ColorLines

SDL Linux version

## Install Tools & Build

Debian Recipe:

```sh
sudo apt install libsdl*1.2-dev

cd ~/Projects/
git clone https://github.com/Limows/SDL-ColorLines
cd SDL-ColorLines/X86-Linux/
make clean
make
```

## Create packages for Linux distributions

Tar:

```sh
make dist
```

## Controls

 - Cursor keys - directions
 - Enter - select
 - F6 - new game
 - F2 - pause music
 - ESC - back to menu
 - F4 - about

## Features

 - Game save your score automatically
 - Music
 - Animations
 - Mouse controls

## Tested devices

 - Thinkpad T470 with Debian sid

## Screenshots

- On Debian sid:

![LINUX_title](screenshots/title.png)
![LINUX_game](screenshots/game.png)
![LINUX_about](screenshots/about.png)
