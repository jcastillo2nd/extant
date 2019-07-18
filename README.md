# Extant library (libextant) #

The Extant library is a multi-function library of routines that facilitate
operations in logging, dynamic shared object symbols, plugin architectures, set
manipulation, memory regions and pooling among others.

The specific list of features are outlined in the documentation.

The target platform is linux, any porting to other systems is dependent on
community support.

The library is written with C99 as the target language to support compiling on
a significant number of architectures where linux is supported.

The library is currently tested against the following:

* Linux 4.4.159
* GCC 8.1.1
* GNU C Library 2.28
* libcheck 0.12.0
* Doxygen 1.8.14

## Building ##

### Dependencies ###

The library build requires:

* gcc
* make
* pkg-config
* libtool

**Optionally**:

* libcheck ( For running unit tests )
* Doxygen ( For generating documentation )
* libltdl ( For DSO implementation )

### configure, make, make install ###

When building from a release package ( e.g. `make dist` output ), the package
is unpacked and follows the typical `configure`, `make`, `make install`. The
optional `make check` and `make html` ( and related `make install-html` can be
used to both run the library tests ( Requires libcheck ) and documentation (
Requires Doxygen ).

Note that the `make install-html` builds the documentation prior to installing
it. Neither `make check` or `make html`/`make install-html` are available
without their respective dependencies ( libcheck, Doxygen ).

With both libcheck and Doxygen present on the system, the following can be run
to unpack, configure, build and conditionally install ( default under
`/usr/local` ) the software and documentation if the unit tests pass:

    > tar xzf extant-0.1.tar.gz
    > cd extant-0.1
    > ./configure
    > make
    > make check && make install && make install-html

### Options ###

For _full_ and **up to date** options listing, refer to the documentation.

#### ltdl vs dlopen ####

Enforce libltdl as the DSO library

  > --with-libltdl          Enforce libltdl DSO library

**Note:** If libtool dl library is available ( libltdl ) on configure, then libtool is
used unless configured with the `--without-libltdl` option.

#### logger options ####

Set the default logging level.

  > --with-log-level        Set default log level [(default: 15)]

Set the default log batch size which controls how often the logger thread will
yield to the scheduler.

  > --with-log-batch-size   Set default log batch size [(default: 16)]

#### set options ####

Control the maximum height of self-balancing trees.

  > --with-tree-max-height  Set AVL/RB tree max height [(default: 16)]

## Developing ##

Development is very similar to building, however does require a few additional
dependencies. This includes the **autotools** (autoheader, aclocal, autoconf,
automake, libtoolize, etc).

In most cases where a CI/CD pipeline is not available, the build process
supports using a build directory to avoid cluttering the src path with built
objects:

    > git clone https://github.com/jcastillo2nd/extant.git
    > cd extant
    > autoreconf --install
    > mkdir -p build && cd build
    > ../configure
    > make
    > make check

The `autoreconf` command will generate macro files and other artifacts in
addition to the build and config paths that need to be excluded from git when
adding changes to commit.
