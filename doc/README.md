
## Compilation
### Windows
The included `Makefile` works on Windows with MinGW -- Install MinGW, in particular `gcc` and `mingw32-make`. 

To compile the project, run the following in the main directory:
- Windows (MinGW with gcc and mingw32-make installed): 
```
mingw32-make
```
- Linux (gcc installed):
```
make
```

In some rare cases, the make tool will not track changes correctly and compilation will fail; For this you can run `mingw32-make clean` (Windows) or `make` (Linux) first.

## Project structure
```
|-- Makefile
|
|-- README.md
|
|-- LICENSE
|
|-- .gitignore
|
|-- bin/
|   |-- (Executable files)
|
|-- doc/
|   |-- README.md
|
|-- inc/
|   |-- (All .h header files)
|
|-- obj/
|   |-- (All object and dependency files)
|
|-- src/
|   |-- main.c
|   |-- (Other .c source files)
|
```

---