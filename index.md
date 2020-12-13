# aocl

## Overview
In essence, this project is implementation of `<string.h>` in assembly.
In particular, this repository contains reimplementations of all functions
defined in the C Standard for `<string.h>` via (mostly) x86 assembly, as well as
a small amount of C code to export names, manage keyword definitions/reexports,
and perform option-handling logic. Implementation strategies are overall inspired
by musl first and foremost, followed by glibc.

Broadly, this is a toy project meant to help develop a better understanding of
both x86-64 assembly as well as the internals of the C Standard Library. Obviously,
other implementations exist (not least as produced by mainstream C compilers), so
there is little expectation of use outside of this repository.
## Usage
In order to use the functions implemented thus far, simply run `make lib` and
link the generated static library `lib/aolc.a`.
## Implementation Status
As of right now, the following components are implemented:
|  C Header  |  C Function  |  Status  |  Maintainer |
|:-----------|:-------------|:--------:|:--------:|
|`<string.h>`|memcpy| Done | Marcus Plutowski <achierius@gmail.com> |
|`<string.h>`|memmove| TODO ||
|`<string.h>`|memchr| TODO ||
|`<string.h>`|memcmp| TODO ||
|`<string.h>`|memset| Done | Marcus Plutowski <achierius@gmail.com> |
|`<string.h>`|strcat| TODO ||
|`<string.h>`|strncat| TODO ||
|`<string.h>`|strchr| TODO ||
|`<string.h>`|strrchr| TODO ||
|`<string.h>`|strcmp| WIP | Scott Durand <sdurand543@berkeley.edu> |
|`<string.h>`|strncmp| WIP | Scott Durand <sdurand543@berkeley.edu> |
|`<string.h>`|strcoll| TODO ||
|`<string.h>`|strcpy| Done | Marcus Plutowski <achierius@gmail.com> |
|`<string.h>`|strncpy| Done | Marcus Plutowski <achierius@gmail.com> |
|`<string.h>`|strerror| TODO ||
|`<string.h>`|strlen| Done | Marcus Plutowski <achierius@gmail.com> |
|`<string.h>`|strspn| TODO ||
|`<string.h>`|strcspn| TODO ||
|`<string.h>`|strpbrk| TODO ||
|`<string.h>`|strstr| TODO ||
|`<string.h>`|strtok| TODO ||
|`<string.h>`|strxfrm| TODO ||

## Contributors
- Marcus Plutowski <achierius@gmail.com>
- Scott Durand <sdurand543@berkeley.edu>

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
Presently, all assembly is written in accordance to x86-64 calling conventions; as such,
is is expected that, among other things, the first six integral arguments will be passed in
via the registers RDI, RSI, etc., with further arguments passed in on the stack. See the
reference links below for more detail.
### Useful References
 - [Wikibooks page on <string.h>](https://en.wikibooks.org/wiki/C_Programming/string.h)
 - [glibc reference manual](https://www.gnu.org/software/libc/manual/pdf/libc.pdf)
 - [musl reference manual](https://musl.libc.org/manual.html)
 - [x86-64 cheat sheet](https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf)
 - [x86-64 calling convention](https://aaronbloomfield.github.io/pdr/book/x86-64bit-ccc-chapter.pdf)

## License
This project is licensed under the BSD License 2.0.
