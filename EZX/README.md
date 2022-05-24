# SDL-ColorLines

Motorola EZX version, originally ported by BruceLee

## Toolchain & SDK

Motorola A1200, E6: // TODO: Add normal link mirrored to forum.motofan.ru

Motorola A780, E680: // TODO: Add normal link mirrored to forum.motofan.ru

Download MotoEZX Toolchains & SDK from [this link](http://www.mediafire.com/?meqnmgujgjq).

Download E680 Toolchains & SDK from [this link](https://code.google.com/archive/p/moto-e680-develop/downloads).

## Install Tools & Build

CentOS 7 Recipe:

```sh
sudo yum -y install epel-release

sudo yum -y install @development
sudo yum -y install glibc.i686 libstdc++.i686
sudo yum -y install p7zip

sudo mkdir /opt/toolchains/
sudo tar -C /opt/toolchains/ -xzvf ~/Downloads/motoezx-toolchains*.tar.gz* # A1200, E6
sudo tar -C /opt/toolchains/ -xzvf ~/Downloads/motoe680-toolchains*.tar.gz* # A780, E680
sudo ln -s /opt/toolchains/motoe680 /usr/local/arm # A780, E680

cd ~/Projects/
git clone https://github.com/Limows/SDL-ColorLines
cd SDL-ColorLines/EZX/
. /opt/toolchains/motoezx/setenv-a1200-devezx.sh # A1200, E6
. /opt/toolchains/motoe680/setenv-e680.sh # A780, E680
make clean
make # A1200, E6
make -f Makefile.e680 # A780, E680
```

## Copy executable file to Motorola E6 and run it

FTP:

```sh
ftp 192.168.16.2
User: ezx
ftp> binary
ftp> cd /mmc/mmca1/
ftp> put ColorLines.ezx
ftp> quit
```

## Create PKG-package for Motorola E6 or A1200

Use this command: tar -cvzf ColorLines.pkg ColorLines