# The Sets Interfaces # {#nodesets}

[node]: @ref xtnt_node "struct xtnt_node"
[set]: @ref xtnt_node_set "struct xtnt_node_set"
[array]: @ref array.h "Array operations"
[list]: @ref list.h "List operations"
[queue]: @ref queue.h "Queue operations"
[stack]: @ref stack.h "Stack operations"
[tree]: @ref tree.h "Tree operations"


[Sets][set] are collections of things. This is used by a set of functions that
operate on the set in particular ways. Some of the operations are compatible
with each other such as the [stack][stack] and [queue][queue] operations which
can both operate on the same [set][set] of [nodes][node] allowing for adding
a [node][node] to the beginning or end, or removing a [node][node] from the
beginning or end.

## Using a set ##

Using a [set][set] depends on type of operations you need to perform with it.
All [sets][set] rely on the same [node][node] object for holding data in the
[set][set], and some [set][set] operations are even interchangeable. Others are
not however, and require that a [set][set] utilizing these functions is not
operated on by any other class of [set] functions.

Some of the set types include:

* [array](@ref arraysets) - Finite sized array
    - Similar to a memory pool
* [list](@ref listsets) - Doubly linked nodes
    - Compatible with [stacks][stack] and [queues][queue]
* [queue](@ref queuesets) - FIFO node set
    - Compatible with [stacks][stack] and [lists][list]
* [stack](@ref stacksets) - FILO node set
    - Compatible with [queues][queue] and [lists][list]
* [tree](@ref treesets) - Self balancing trees

## Set lifecycle ##

@todo The lifecycle of a set and use case examples

# What is a node? #

## Node lifecycles ##

## Using a node ##
