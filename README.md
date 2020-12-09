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
### /build
Contains build erratta and produced binaries.

## Implementation Status
|  C Header  |  C Function  |  Status  |  Handler |
|:-----------|:-------------|:--------:|:--------:|
|`<string.h>`|memcpy| Done | Marcus Plutowski <achierius@gmail.edu> |
|`<string.h>`|memmove| TODO ||
|`<string.h>`|memchr| TODO ||
|`<string.h>`|memcmp| TODO ||
|`<string.h>`|memset| Done | Marcus Plutowski <achierius@gmail.edu> |
|`<string.h>`|strcat| TODO ||
|`<string.h>`|strncat| TODO ||
|`<string.h>`|strchr| TODO ||
|`<string.h>`|strrchr| TODO ||
|`<string.h>`|strcmp| TODO ||
|`<string.h>`|strncmp| TODO ||
|`<string.h>`|strcoll| TODO ||
|`<string.h>`|strcpy| TODO ||
|`<string.h>`|strncpy| Done | Marcus Plutowski <achierius@gmail.edu> |
|`<string.h>`|strerror| TODO ||
|`<string.h>`|strlen| Done | Marcus Plutowski <achierius@gmail.edu> |
|`<string.h>`|strspn| TODO ||
|`<string.h>`|strcspn| TODO ||
|`<string.h>`|strpbrk| TODO ||
|`<string.h>`|strstr| TODO ||
|`<string.h>`|strtok| TODO ||
|`<string.h>`|strxfrm| TODO ||

## Usage
In order to use the functions implemented thus far, simply run `make lib` and
link the static library (aolc.a) that it generates; within your code, make sure
to import `"string.h"`, _not_ `<string.h>`, at least until we finish the libc
package with `<string.h>` swapped out.


## Contributing
### Branch Convention
For now, simply make sure to develop implementations for function `<X>` on the
branch `feature/string/<X>` — i.e. `strlen` is managed on `feature/string/strlen`. Rebase before merging, and make sure your
merges/PRs target develop.
### Tests
Simply run `make test` in the project root directory to build and run the test
suite. Please make sure to implement at least rudimentary tests for any new
implementations; hoping to get an actual test framework set up _soon_.
### Style
With regards to C code, this project uses the Linux Kernel style guide, following
in the style of musl. Determining an appropriate style guide for the underlying
assembly is a work-in-progress; broadly, make sure to thoroughly (albeit not
excessively) comment all code. Of particular importance is annotating the
contents/purposes of the registers you're using at least once per function —
this helps greatly with readability.

## Technical Notes
### Compatability 
As-is, this project is set up to build solely on ELF64-compatible
architectures; however, changing the Makefile to build for other architectures
wouldn't be too hard, within reason — as long as they're still x86-64
compatible, of course.
### Static Library?
As of right now, _no_ extant assembly implementations have been written so as
to be position-independent — as such, creating a PIE binary (as would be
required for a shared/dynamic library) is not possible. This may be a goal in
the future, but it is not as of right now.
### Calling Convention
Presently, all assembly is written with the assumption that the corresponding
C declaration is annotated with `__asmlinkage`, and thus that all arguments are
passed in on the stack. Support for `__fastcall` may be implemented in the
future.
### Useful References
 - [Wikibooks page on <string.h>](https://en.wikibooks.org/wiki/C_Programming/string.h)
 - [glibc reference manual](https://www.gnu.org/software/libc/manual/pdf/libc.pdf)
 - [musl reference manual](https://musl.libc.org/manual.html)
 - [x86-64 cheat sheet](https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf)
 - [x86-64 calling convention](https://aaronbloomfield.github.io/pdr/book/x86-64bit-ccc-chapter.pdf)


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
