# avrcpp
Some development utilities for avr development in C/C++ using avr-g++. 
This project is not supposed to be a complete STL implementation, 
but it should get you started. Most of this is a header only, but stuff like enabling purely
and deleted virtual functions have been done via an implementation file. 

## GPL3 Licensed
Please take note of the GPL3 license. 
This means that if you distribute your project, 
it should be licensed with a GPL3 compatible license. 
If you use this for personal use, then no worries.

## Blog
[my blog](https://blog.gtz.dk) has extensive explanations of how the various utilities work. 
If you have any questions, please read that first. 

Here is a list of related posts:
- [new & delete](https://blog.gtz.dk/avr/c++/2019/05/17/cpp-with-avr-gpp.html)
- [avr memory model](https://blog.gtz.dk/avr/c++/2019/03/24/avr-memory-model-the-practical-explanation.html) (just some background, for understanding everything)
- [smart pointers](https://blog.gtz.dk/avr/c++/2019/03/24/avr-memory-model-the-practical-explanation.html)
- [type traits](https://blog.gtz.dk/avr/c++/2021/03/30/avr-type-traits.html)
- ??

## How To Use / Install
Make sure that you have `avr-g++` installed.

You simply include whatever you need from the `include` directory.

This is (mostly) a header only library, 
but you can choose to compile the `new`, `delete` and purely virtual implementations via CMake.

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE
make
```

#### Authors
- [Asger Gitz-Johansen](https://github.com/sillydan1)
