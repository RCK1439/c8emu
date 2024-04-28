# C8EMU: An interpreter for Chip-8

c8emu is an emulator (or interpreter) built for the [Chip-8](https://en.wikipedia.org/wiki/CHIP-8) programming language. It is built entirely in C with the C99 standard for Windows with use of the [raylib](https://www.raylib.com/) to handle input and graphics.

## Building

c8emu makes use of Makefiles to compile the project.

1. Ensure you have [mingw](https://www.mingw-w64.org/) installed.
2. In the project root, run ```make release``` in the command line. This will build the executable with optimizations turned on to the ```bin``` directory.

## Running

The emulator takes in only one argument, which is the filepath to a Chip-8 ROM file.

Something like this: 
```.\c8emu.exe <source>```


Start by running a little test program in the ```tests/``` directory:

```.\bin\c8emu.exe .\tests\1-chip8-logo.ch8```

## Sources I used for development

http://emulator101.com/

http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

https://austinmorlan.com/posts/chip8_emulator/

## Test ROMs

https://github.com/corax89/chip8-test-rom

https://github.com/Timendus/chip8-test-suite/tree/main

(Others to be listed, but they were also found on github)