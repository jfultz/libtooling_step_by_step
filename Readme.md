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

### Web resources
Information seems to be spread around, and Stack Overflow has some useful stuff, but often falls surprisingly short in concrete examples.  Here are some websites that helped me put this repo together.
* [clang doxygen docs](https://clang.llvm.org/doxygen/index.html).  Any nontrivial usage requires using this as a reference.  Fortunately, it's well-linked to the source code because, sometimes, it's required to understand the documentation (sadly). However, doxygen's insistence on hiding all of the useful classes behind a closed namespace subtree, and then continuing to re-close it every time you hit the back button is absolutely evil.
* [Official clang docs to use clang as a library](http://clang.llvm.org/docs/index.html#using-clang-as-a-library).  This is great when you're starting from nothing, but it leaves oh-so-many questions unanswered.
* [Official clang docs matcher reference](https://clang.llvm.org/docs/LibASTMatchersReference.html).  Valuable breakout of just the various matcher APIs.
* [Bites, Bytes, Boos blog post](https://kevinaboos.wordpress.com/2013/07/23/clang-tutorial-part-ii-libtooling-example/), which has a nice example of an AST visitor and rewriting.  [Code here](https://github.com/kevinaboos/LibToolingExample).  He also talks about clang plugins.
* [It's Compiling blog post](https://heejune.me/2016/08/17/build-your-own-clang-example-outside-of-the-llvm-source-tree/
).  It is amazingly tough to build libTooling code outside of the clang build system.  The documentation for this is basically nonexistent.  Unfortunately, I didn't notice this post until I'd largely reverse-engineered it myself, but the post did help me to clean up some of my messy, ignorant cmake.

### Additional info:
* To get an AST dump, do:

````
    clang++ -Xclang -ast-dump -fsyntax-only filename.cpp
````
