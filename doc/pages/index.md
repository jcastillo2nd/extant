# extant # {#index}

[mpools]: @ref pool.h "Memory pool operations"
[mregions]: @ref region.h "Memory region operations"
[node]: @ref xtnt_node "Node object"
[set]: @ref xtnt_node_set "Set of Node objects"

# About extant #

The extant library ( libextant ) is a collection of functions that facilitate
operations including [logging](#loggersystem), dataset handling, dynamic shared
libraries and symbol resolution and memory pool operations with a [consistent
interface](#returnstatus) for error handling.

# Target systems #

The library targets Linux/Unix environments, though this may be ported in the
future. Ideally, the code should conform to C99, though exceptions may exist
for optional functionality.

@todo Explore C11 for use of `_Generic` keyword over compile time typedefs for
      `xtnt_int_t`, `xtnt_uint_t`, `xtnt_real_t` and the like.

The runtime dependencies vary, with a strong preference towards glibc standard
library / Posix.1-2008, though some functionality may optionally be
implemented through external libraries:

* libltdl - Optional DSO implementation backend

The build environment favors gcc, autotools and the c99 standard. Actual build
requirements to be determined.

@todo List out the build software requirements

# API Interfaces #

The API interfaces scheduled for first major ( 1.0 ) release include:

* [dso](@ref dso) - Implementation of shared object loading
  * dso - object loading and symbol resolution
  * plugin - predefined symbol set and interface version resolution
* [error](@ref returnstatus) - Implementation of consistent error handling
* [log](@ref loggersystem) - Implementation of logging system
* [memory](@ref memorymanagement) - Implementation of memory management
  * pool - homogenous preallocation
  * region - heterogenous preallocation
* [set](@ref nodesets) - Implementation of node collections
  * [array](@ref arraysets) - Finite sized array
  * [list](@ref listsets) - Doubly linked nodes
  * [queue](@ref queuesets) - FIFO node set
  * [stack](@ref stacksets) - FILO node set
  * [tree](@ref treesets) - Self balancing trees
    * AVL
    * B tree
    * Red/Black
    * Octree

# Conventions #

The library itself uses a few conventions. While there are some documented
exceptions to this, it can be assumed that the following conditions are true
with the library.

* Functions will return an `xtnt_status_t` ( unsigned int ) and if data is to
  be returned, the functions will accept an indirect reference parameter to
  store the result.

* Operands are passed in by reference ( e.g. pointers ) on non memory allocation
  / deallocation functions.

* The `*_create()` and `*_destroy()` functions (de)allocate memory and will
  consistently accept indirect references ( e.g. pointer to a pointer) as they
  directly call `malloc()` or `free()` and update the pointer.

* Structures are not `typedef`'d, so must be declared with `struct struct-type`
  when being initialized or user code must establish their own `typedef`'s.

# Things to note #

There are some things to keep in mind while using the library:

* The `_initialize()` and `_uninitialize()` functions do not allocate any memory
  so leverage these functions instead of the corresponding `_create()` and
  `_destroy()` calls when additional memory control is desired such as when
  using memory [pools][mpools] or [regions][mregions].

* There is **no operand error checking** done within the library to reduce
  operational overhead. You can _always_ use non-interchangeable functions on
  the same data set such as a
  [memory pool allocation](@ref xtnt_mpool_allocate) where a 
  [memory region allocation](@ref xtnt_mregion_allocate) was meant to be
  used, as they both operate on [memory objects](@ref xtnt_memory_object).
  This is intentional in some cases, such as working with a single [set] as a
  queue and stack at the same time.

  @note Be careful with function calls that operate on the same data structure
  and ensure it is the intended operation.

  @warning **There is no error checking done for operands within the library.**

* While [memory objects](@ref xtnt_memory_object) support locking, and do lock
  when performing allocation and deallocation operations, best practice is to
  _limit allocation/deallocation to a single thread for
  [memory object](@ref xtnt_memory_object) operations, e.g. each thread should
  have it's own [pool][mpools] or [region][mregions]._


# Where to go from here #

The best place to start will be with the [error](@ref returnstatus)
documentation to better understand how the functions return data.

From there, the usage of the library would be the next place to go. For
example, if usage of the library is primarily for the logging, then the next
chapter would become the [logger system](@ref loggersystem).

The individual pages for the code are available for reference, though code
examples exist exclusively in the pages for the respective topics.
