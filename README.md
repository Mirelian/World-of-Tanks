
![Screenshot 2024-03-08 172143](https://github.com/Mirelian/World-of-Tanks/assets/64906131/0330b81d-29e5-4522-be93-d79bc134091e)

This project uses the gfx-framework.
```
git clone https://github.com/UPB-Graphics/gfx-framework
```

**Setup**:

1.  Open a command shell in the gfx-framework folder
2.  Run the following command:
```
cd src/lab_m1
git submodule add https://github.com/Mirelian/World-of-Tanks
git submodule update --init --recursive
```
3.  Open the lab_list.h file in src/lab_m1 and add:
```
#include "lab_m1/World-of-Tanks/World of Tanks.h"
```
4.  Open the main.cpp file in src and change the 'world' object to:
```
World* world = new m1::WOT;
```
5. Build and run the gfx-framewok folder with CMake
