# c8emu: An interpreter for Chip-8

c8emu is a mini Chip-8 emulator/interpreter for the [Chip-8](https://en.wikipedia.org/wiki/CHIP-8) programming language. This project is an educational project for me with regard to getting into emulator development as well as programming portable and cross-platform C code.

![[image]](resources/test_opcode.png)

So far, I've got this working on Linux and Windows. The project is set up that it can be easy to make it work on Mac, but, unfortunately, I do not own an Apple device, so I am unable to test it.

To further extend my knowledge on this, I'd like to learn [CMake](https://cmake.org/) and perhaps [Premake](https://premake.github.io/)

## Minimum requirements

- **GPU**: OpenGL 3.3 capable graphics device is required

## Controls

The original Chip-8 had an interesting 16-key keypad layout that looks like the following:

|1|2|3|C|
|-|-|-|-|
|4|5|6|D|
|7|8|9|E|
|A|0|B|F|

My implementation uses the QWER-layout, which means the keys of the Chip-8 respectively map to the following:

|1|2|3|4|
|-|-|-|-|
|Q|W|E|R|
|A|S|D|F|
|Z|X|C|V|

## Building

### On Linux

1. Clone the repository
```sh
git clone https://github.com/RCK1439/c8emu.git
```

2. `cd` to the root directory
```sh
cd c8emu/
```

3. Finally, build the project
```sh
cd src/ && make release
```
This will compile the project in "release" mode with full optimizations

### On Windows

Eveything is mostly the as with the Linux compilation steps, but you will need to have [w64devkit](https://github.com/skeeto/w64devkit) installed on your system.

## Running

After successful compilation, the executable should be placed in a `bin/` directory in the root folder of the project.

### On Linux
```bash
./bin/c8emu <rom_file>
```

### On Windows
```powershell
.\bin\c8emu.exe <rom_file>
```

## Libraries

- [raylib](https://www.raylib.com/) v5.5 for graphics, input, sound and window management
- [rklog](https://github.com/RCK1439/rklog) for debugging and logging


## Sources used for development

- http://emulator101.com/
- http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
- https://austinmorlan.com/posts/chip8_emulator/

## Test ROMs used for debugging

- https://github.com/corax89/chip8-test-rom
- https://github.com/Timendus/chip8-test-suite/tree/main
- (Others to be listed, but they were also found on github)