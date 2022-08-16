# Clix

Clix - the simpest portable interpreter!

It uses OS' standard library and ValeraC.

# Build

Install dependcies
```bash
apt install cmake clang-14 make git
```
Clone repository and build program
```bash
cd ~
git clone https://github.com/NDRAEY/Clix
cd Clix
git submodule init
git submodule update --remote

mkdir build
cd build
cmake ..
make -j4
```
