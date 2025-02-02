# Projet-C
pour compiler
gcc Cmain.c Caffichage.c struct_etud.c tourelle.c -o monjeu -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
pour installer raylib
sudo apt update && sudo apt install git build-essential cmake libgl1-mesa-dev libx11-dev libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev

et ensuite

git clone https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
