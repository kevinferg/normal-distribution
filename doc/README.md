
## Compilation
### Windows
The included `Makefile` works on Windows with MinGW -- Install MinGW, in particular `gcc` and `mingw32-make`. 

To compile the project, run the following in the main directory:
```
mingw32-make
```

In some rare cases, the make tool will not track changes correctly and compilation will fail; For this you can run `mingw32-make clean` first.

### Not Windows
Copy the contents of `doc/Makefiles/nix_makefile` into `Makefile`:
```
cp doc/Makefiles/nix_makefile makefile
```

To compile the project, run the following in the main directory:
```
make
```

In some rare cases, the make tool will not track changes correctly and compilation will fail; For this you can run `make clean` first.


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
|   |-- Makefiles
|   |   |-- nix_makefile
|   |   |-- win_makefile
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