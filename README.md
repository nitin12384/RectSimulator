# RectSimulator

	2D physics simulation of two rectangles in closed box

# Current Progress 

1.Proper Drawing and Initialisation of rectangle ✅

2.Animation of rectangle✅

3.Physics Update of Coords according to velocity and acceleration✅

4.Collision Handling With the walls

5.Adding of more than one bodies in the simulation

# Language Used 

1. Plain C++ with console graphics using graphics.h 

# Setting Up the environment and execution
(Instructions for Windows operating system only, Tested on Windows 10 ) <br>
Refrences : https://www.youtube.com/watch?v=J0_vt-7Ok6U

1. Download the 32-bit compiler . Link : https://jmeubank.github.io/tdm-gcc/ 
Download this file ` MinGW.org based tdm-gcc-10.3.0.exe, 60.2 MB `

2. Download graphics.h library files . Link : https://drive.google.com/file/d/16xZBvFXf7yFjxwTpuyevK1KPuLgUeZFh/view (Also in graphics-header-files direcory in the repo)
These files are required : 
- graphics.h
- winbgim.h
- libbgi.a

3. Install the compiler, and place ` graphics.h ` & ` winbgim.h ` files in ` /include ` folder of compiler and ` libbgi.a ` file in ` /lib ` folder of compiler
Notes : 
- Uncheck "Check for updated files of TDM-GCC server"

4. Find the path of compiler's g++.exe file . Ex : "C:\TDM-GCC-32\bin\g++.exe"

5. Run the program as : `<compiler_path> -o <output_file_path> <code_file_path> -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32`
Example : ` C:\TDM-GCC-32\bin\g++.exe -o main.exe main.cpp -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 `

# Execution Screenshots 

<img src="./images/SS1.png" />
<img src="./images/SS2.png" />

# Other Info

Start Date : 1 December 2019
