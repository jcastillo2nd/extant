# The return status # {#returnstatus}

The libextant library encourages consistency. In the pursuit of consistency,
and in the interest of function error handling, the result of a function call
in libextant returns an `xtnt_status_t` value unless otherwise documented. Some
notable exceptions include:

* [xtnt_hash](@ref xtnt_hash)
    This function returns an int, based on the string passed in.

* [xtnt_logger_process](@ref xtnt_logger_process)
    This function is designed to be the start of a thread for the
    [logging system](@ref loggersystem) consumer.

## The `xtnt_status_t` typedef ##

The `xtnt_status_t` typedef takes the `xtnt_int_t` typedef. This is decided at
build time, which results in either `long long`, `long` or `int` type.

## Checking the result ##

In most functions, a result variable is created which is reused through the
function to store intermediate results from subfunction calls. The general
strategy implemented includes the _if..else_ conditional with assignment and
comparison::

```{.c}
xtnt_status_t res = XTNT_EFAILURE; // Assume function failure
if (XTNT_IS_ESUCCESS((res = xtnt_test(XTNT_ESUCCESS)))) {
    // Additionally processing with success
} else {
    return XTNT_EFAILURE;
}
return res;
```

The value is a signed integer, so it ranges from below 0, 0 and above 0. The
convention for these include the `XTNT_EFAILURE` ( -1 ), `XTNT_ESUCCESS` ( 0 )
and `XTNT_EWARNING` ( 1 ). This convention allows for cases where the call 
encountered a situation where the result could be returned, but some special
consideration exists. This is particularly useful for operations that set
data that has a state.

```{.c}
xtnt_status_t res = XTNT_EFAILURE; // Assume failure
if (!XTNT_IS_EFAILURE((res = xtnt_test(XTNT_EWARNING)))) {
    // The test did not fail but may have some special considerations
    if (res != XTNT_ESUCCESS) {
        // Do special processing, e.g. check state, handle exception
        switch(res) {
        case XTNT_EWARNING:
            // Handle this special consideration
        case ENOMEM: // Special case
            // Handle this special consideration based on errno.h value
        }
    }
    // Do processing assuming special considerations are handled and set res
} else {
    return XTNT_EFAILURE;
}
return res;
```
Some calls set values which may be impacted by conditions on the system. These
in particular can benefit from the use of established errno.h values. For
example, a function that allocates memory for a struct and stores the pointer
into its argument may fail, and catch the errno value from the memory
allocation call to return as a result, assigning NULL into the pointer.

## Summary ##

Except where explicitly documented, all libextant functions return an
`xtnt_status_t` value which will offer insight into the result or status of the
called function. It is up to the caller to handle the result and any data ( or
data changes ) appropriately.

The macros and collection of constants are defined in the
[Error header file](@ref error.h) for further reference.
