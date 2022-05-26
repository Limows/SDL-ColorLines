# SDL-ColorLines

Original nzimin version with updates

## Toolchain & SDK

TODO: Add link to SDK

## Install Tools & Build

Debian Recipe:

```sh
sudo mkdir /opt/toolchains/
sudo tar -C /opt/toolchains/ -xzvf ~/Downloads/a320_sdk.zip

cd ~/Projects/
git clone https://github.com/Limows/SDL-ColorLines
cd SDL-ColorLines/A320/
. /opt/toolchains/a320/setenv-a320.sh
make clean
make
```

## Copy executable file to console

Use usb mini cable, or just copy ColorLines folder to SD card

## Controls

 - Joystick - directions
 - Start center - select
 - - new game
 - Select - back to menu
 - X - about

## Features

 - Game save your score automatically

## Screenshots

![A320_title](screenshots/title.png)
![A320_game](screenshots/game.png)
![A320_about](screenshots/about.png)
