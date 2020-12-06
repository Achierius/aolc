# aocl

## Description
In essence, this project is implementation of `<string.h>` in assembly.
In particular, this repository contains reimplementations of all functions
defined in the C Standard for `<string.h>` via (mostly) x86 assembly, as well as
a small amount of C code to export names, manage keyword definitions/reexports,
and perform option-handling logic.
### /include
Contains all header files, notably `string.h`.
### /src
Contains all source files.
#### /src/asm 
Includes all .S files. This is where the bulk of the implementation can be
found.
#### /src/c
This is where the aforementioned small amount of C code is located.
Currently, also includes stub-implementations of any standard functions which have
yet to be implemented internally in assembly.
### /test
Includes tests for each function within `<string.h>` as defined by the C
Standard. May end up pulling from GNU and Newlib for help on this front.

## Implementation Status
|  C Header  |  C Function  |  Status  |
|:-----------|:-------------|:--------:|
|`<string.h>`|memcpy| TODO |
|`<string.h>`|memmove| TODO |
|`<string.h>`|memchr| TODO |
|`<string.h>`|memcmp| TODO |
|`<string.h>`|memset| TODO |
|`<string.h>`|strcat| TODO |
|`<string.h>`|strncat| TODO |
|`<string.h>`|strchr| TODO |
|`<string.h>`|strrchr| TODO |
|`<string.h>`|strcmp| TODO |
|`<string.h>`|strncmp| TODO |
|`<string.h>`|strcoll| TODO |
|`<string.h>`|strcpy| TODO |
|`<string.h>`|strncpy| TODO |
|`<string.h>`|strerror| TODO |
|`<string.h>`|strlen| TODO |
|`<string.h>`|strspn| TODO |
|`<string.h>`|strcspn| TODO |
|`<string.h>`|strpbrk| TODO |
|`<string.h>`|strstr| TODO |
|`<string.h>`|strtok| TODO |
|`<string.h>`|strxfrm| TODO |

## Usage
TODO

## Technical Notes
### Style
With regards to C code, this project uses the Linux Kernel style guide, following
in the style of musl. Determining an appropriate style guide for the underlying
assembly is a work-in-progress; broadly, make sure to thoroughly (albeit not
excessively) comment all code.
### Calling Convention
Presently, all assembly is written with the assumption that the corresponding
C declaration is annotated with `__asmlinkage`, and thus that all arguments are
passed in on the stack. Support for `__fastcall` may be implemented in the
future.

## General Notes
### Motivation
This is a toy project meant to help develop a better understanding of both x86
assembly and the internals of the C Standard Library. Other such
implementations exist (not least as produced by mainstream C compilers), so
there is little expectation of use outside of this repository.
### Priorities
Our priorities are broadly, in order from highest to lowest:
1. Functionality
2. Coverage
3. Efficiency
4. Compatability

Thus, when deciding where to put additional energy, we will firstly prioritize
fixing existing implementations, followed by creating new implementations,
optimizing existing implementations, and finally expanding support.
### Inpsiration
Implementation strategies broadly inspired by musl first and foremost, as well
as by glibc.

## License
This project is licensed under the BSD License 2.0.
