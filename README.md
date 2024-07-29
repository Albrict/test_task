# test_task
## How to compile
### Linux
You will need FLTK 1.3.9 to compile this project. You can install this library using package manager or compile it yourself: https://www.fltk.org/software.php

`mkdir build`

Then 

`cd build && cmake .. -DCMAKE_BUILD_TYPE=RELEASE`

And type
`make`
### Windows
You will need MSYS2 enviroment to compile this project on Windows and FLTK 1.3.9. I recommend to just download FLTK sources and compile them, then install into your system, because MSYS2 pacman distributes often DLL's, not static libraries.
After that, do all steps above for Linux(except maybe you will need to use `ninja`, instead of `make`)

### Binaries
There's already built binaries for both systems: https://github.com/Albrict/test_task/releases/tag/1.0

## How to launch
Just type

`./test_task ../input_data.txt` or `./test_task`
