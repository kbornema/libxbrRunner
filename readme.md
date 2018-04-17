libxbrRunner
==========================
Licensed under the GNU Lesser General Public License, version 2.1. (See libxbr-standalone)

Description
==========================
This C++ program scales and interpolates all given 32bit .png files with xBR/hqx algorithms to make them smoother. 
See https://en.wikipedia.org/wiki/Hqx for an example.

This program does NOT require any third party libraries or headers (they should all be found on windows machines).

The program is supposed to be used as a command line program. 
I included some .bat files to convert all given *.png files of the given input folder to a given output folder (windows only).
NOTE: currently it will try to convert ALL files of the folder, regardless if they are actual .png files

Most of the source code is a combination of different sources (see below). I added main.cpp and a CMakeLists.txt.

Usage:
==========================
- Use CMake to create a project. 
- On windows it should compile without any problems. On other machines you should only need to change the function in main.cpp that reads the files in a folder.
- Create folders for the input and output files next to the program.
- Put any pixel .png into the input folder.
- Start the program with command line arguments: [inputFolder/] [outputFolder/] [xbr2x|xbr3x|xbr4x|hq2x|hq3x|hq4x] (Alternatively use the supplied .bat files on windows.)
- If everything worked you get the result images in output folder.

Used sources:
==========================
Algorithm for xBR and hqx: https://github.com/Treeki/libxbr-standalone
Licensed under the GNU Lesser General Public License, version 2.1. (See the header in xbr.h and xbr.c.)

Loading and saving png files: https://github.com/lvandeve/lodepng
Licensed under zlib License.

Loading all files in a folder (windows only): https://stackoverflow.com/a/20847429



