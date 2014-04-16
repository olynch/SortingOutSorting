SortingOutSorting
=================

This purpose of this project is to compare different sorting algorithms and see how fast they would run on different lengths of input arrays.

###Compiling

#####Dependencies

- A working C++11 compiler (clang and gcc both work).
- The Boost libraries (www.boost.org).
- CMake (www.cmake.org).
- Make (or ninja).

#####Instructions

1. Run `cmake .` in the root directory of the project. (If you want to use ninja, run `cmake -G Ninja .`).
2. Run `make` (or `ninja`).
3. Run the executable with `./bin/SortingOutSorting`. It requires two arguments, the beginning and end of the path to each data file. For instance if the data files are in data/unsortedarrays/arrays(n).txt, you should run `./bin/SortingOutSorting data/unsortedarrays .txt`.

###Viewing results

The results are stored in .json files under server/. To view them, run a basic http webserver on the server directory. An easily available one is already included in a basic install of python, to use this change to the server directory and run `python -m SimpleHTTPServer`, or for python3 `python -m http.server`, then navigate to localhost:8000 in your web browser.
If you decide to rerun all of the sorts, the results will be placed in the .json files, and the graphs will be automatically regenerated.
