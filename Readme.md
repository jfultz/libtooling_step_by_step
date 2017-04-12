### To build
    mkdir inputs_build && cd inputs_build && cmake ../inputs && cd ..
    mkdir build && cd build
    cmake .. -DCMAKE_PREFIX_PATH=/path/to/llvm

### To run...
The above commands build all of the examples.  In the build directory, run something like:

    ./simple_print_ast -p=../inputs_build ../inputs/print_int.cpp

Note that to support anything with includes, you're going to have to set up include paths.  libTooling only knows how to find include paths when by default when you build inside of a clang layout.  The easiest way to do this is to set the `CPATH` environment variable to your list of includes.  Generally pretty easy on Linux or Windows, but ridiculously annoying on Mac (with my Homebrew install of clang that doesn't even include the C headers).  Here's the `CPATH` I used on Mac:

````
export CPATH=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1:/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/8.1.0/include:/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/usr/include
````
