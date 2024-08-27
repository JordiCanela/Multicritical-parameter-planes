# Multicritical Parameter Planes 

This repository contains an implementation in C of the algorithm descrived in the paper [1] to draw parameter planes of rational maps with 2 or more free critical points. 

## Dependencies

`Main_libattopng.c` uses the library `libattopng`, a minimal C library to create uncompressed PNG images. It can be downloaded at https://github.com/misc0110/libattopng

## Usage

Since `Main_libattopng.c` uses `libattopng`, the documents `libattopng.h` and `libattopng.c` need to be added to the project. `Main_libattopng.c` reads the colours used from the document `coulourmap.map`. The parameters of the program are chosen to draw the parameter plane of the rational map of Section 4.1 at [1]. In order to change the rational map you want to draw, you need to change the function within `evaluateFunction`, to change the corresponding critical points wihin `defcriticalpoints`, and to indicate the total number of critical points within the parameter libattopng. If necessary, the stop condition needs to be adapted to the permanent (super)attracting points or cycles of the rational map (for the implemented map there are two supperattracting fixed points, 0 and infinity).

## How to cite

When citing the use of the repository, also refer to the paper [1] (where the algorithm used was presented).

## Authors

Beatriz Campos, Jordi Canela, Alberto Rodríguez-Arenas, Purificación Vindel.

## References 

[1] B. Campos, J. Canela, A. Rodríguez-Arenas, P. Vindel, _Computing parameter planes of iterative root-finding methods with several free critical points_, to appear in Mathematics and Computers in Simulation.
