# SDL-ColorLines

Motorola EZX version, originally ported by BruceLee

## Toolchain & SDK

TODO: Add link to SDK

## Install Tools & Build

Debian Recipe:

```sh
sudo yum -y install epel-release

sudo yum -y install @development
sudo yum -y install glibc.i686 libstdc++.i686
sudo yum -y install p7zip

sudo mkdir /opt/toolchains/
sudo tar -C /opt/toolchains/ -xzvf ~/Downloads/rzx50_sdk.zip

cd ~/Projects/
git clone https://github.com/Limows/SDL-ColorLines
cd SDL-ColorLines/RZX50/
. /opt/toolchains/mipsel-linux-uclibc/setenv-rzx.sh
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

![RZX_title](screenshots/title.png)
![RZX_game](screenshots/game.png)
![RZX_about](screenshots/about.png)
