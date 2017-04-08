# kws-eval

[![Build Status](https://travis-ci.org/jpuigcerver/kws-eval.svg?branch=master)](https://travis-ci.org/jpuigcerver/kws-eval)

Tools for the evaluation of Keyword Spotting.

## Tools
### Icdar17KwsEval
This is the official evaluation tool for the ICDAR2017 Handwritten Keyword
Spotting Competition (ICDAR2017 KWS).

This computes the (Global) Average Precision and Mean Average Precision given
a list of reference objects and a list of detected objects.

The format of the reference and hypothesis/detected objects is almost identical:
```
# The files can include some header lines, prefixed with the character #.
# After the header lines with comments, each line contains the description of
# an object.
queryID documentID BBx BBy BBw BBh [score]
queryID documentID BBx BBy BBw BBh [score]
```

`BBx`, `BBy`, `BBw` and `BBh` refer to the x,y-coordinates, width and height of
the bounding box of the reference/detected object.

The score field is only present in the hypotheses file, and is used to sort
the detected objects IN DECREASING ORDER. Thus, higher scores mean higher
confidence.


## Install

Basically, you only need a modern C++ compiler and CMake, which we use for
the building process.

There are no dependencies with any external library, unless you want to build
the unit tests, in which case you will need to install Google Test and
Google Mock.

### Requirements
- C++ compiler with support for C++11
- CMake >= 2.8
- Optional: https://github.com/google/googletest

### Steps
1. `git clone https://github.com/jpuigcerver/kws-eval/`
2. `cd kws-eval && mdkir build && cd build`
3. `cmake -DCMAKE_BUILD_TYPE=RELEASE ..`
4. `make`
5. `sudo make install`

This will install the tools to the default CMake install path
(typically /usr/local). If you want to change the installation directory,
pass  `-DCMAKE_INSTALL_PREFIX=/path/to/your/destination` to the `cmake` call.
