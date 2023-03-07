WARNING: Project is not maintained!

![Clix](res/Clix.png)

# Clix

Clix - the simpest portable interpreter!

It uses OS' standard library and ValeraC.

# Build

Install dependcies
```bash
apt install cmake clang-14 make git
```
Make clang-14 visible as clang:
```bash
sudo ln -s /usr/bin/clang-14 /usr/bin/clang
sudo ln -s /usr/bin/clang++-14 /usr/bin/clang++
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

You will get `clix` executable.

# Examples

See `examples` folder.

# Run

In build folder (not installed):

```bash
./clix <program>
```
