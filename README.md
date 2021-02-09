# aolc

## Description
In essence, this project is implementation of `<string.h>` in assembly.
In particular, this repository contains reimplementations of all functions
defined in the C Standard for `<string.h>` via (mostly) x86 assembly, as well as
a small amount of C code to export names, manage keyword definitions/reexports,
and perform option-handling logic.
### /include
Contains all aolc-implemented libc-spec header files - notably `string.h`.
#### /include/internal
Contains internal-facing header files (utilities, glue code, &c).
### /src
Contains production source code files.
#### /src/asm 
Includes all .S (x86-64 assembly) files. This is where the bulk of the implementation can be
found.
#### /src/c
This is where the aforementioned small amount of C code is located.
### /lib
Contains produced static libraries.
#### /lib/`aolc.a`
The core library produced by the project; contains all reimplementations of the
given libc headers.
#### /lib/`sys_libc.a`
Contains re-exported versions of the actual system libc implementations of the
given libc headers, prefixed with an underscore -- for use in testing.
### /test
Includes tests for each function within `<string.h>` as defined by the C
Standard. May end up pulling from GNU and Newlib for help on this front.
### /benchmarks
Includes microbenchmark code to compare the speed of our functions to that of
glibc's equivalent implementations; currently utilizes Google's benchmark
library.
### /docs
Contains documentation and additional notes.
### /build
Contains build erratta and produced binaries.
### /external
Contains external library dependencies, each included within the overall
project as a submodule - currently, google/googletest and google/benchmark.

## Implementation Status
| Specification |  C Function  |  Impl. Status  |  Test Coverage |                  Owner                 |
|:-------------:|:-------------|:--------------:|:--------------:|:---------------------------------------|
| `<string.h>`  | memcpy       |       ✔        |      Core      | Marcus Plutowski <achierius@gmail.com> |
| `<string.h>`  | memmove      |      TODO      |      Core      | Jiahong Long <j4long@ucsd.edu>         |
| `<string.h>`  | memchr       |      TODO      |      Core      | Jiahong Long <j4long@ucsd.edu>         |
| `<string.h>`  | memcmp       |      TODO      |      Core      ||
| `<string.h>`  | memset       |       ✔        |      Core      | Marcus Plutowski <achierius@gmail.com> |
| `<string.h>`  | strcat       |      WIP      |      Core      | Chris Carbone <chris.m.carbone@gmail.com> |
| `<string.h>`  | strncat      |      WIP      |      Core      | Chris Carbone <chris.m.carbone@gmail.com> |
| `<string.h>`  | strchr       |      WIP      |      Core      | Chris Carbone <chris.m.carbone@gmail.com> |
| `<string.h>`  | strrchr      |      WIP      |      Core      | Chris Carbone <chris.m.carbone@gmail.com> |
| `<string.h>`  | strcmp       |      WIP       |      Core      | Scott Durand <sdurand543@berkeley.edu> |
| `<string.h>`  | strncmp      |      WIP       |      Core      | Scott Durand <sdurand543@berkeley.edu> |
| `<string.h>`  | strcoll      |      TODO      |      Core      ||
| `<string.h>`  | strcpy       |       ✔        |      Core      | Marcus Plutowski <achierius@gmail.com> |
| `<string.h>`  | strncpy      |       ✔        |      Core      | Marcus Plutowski <achierius@gmail.com> |
| `<string.h>`  | strerror     |      TODO      |      Core      ||
| `<string.h>`  | strlen       |       ✔        |      Core      | Marcus Plutowski <achierius@gmail.com> |
| `<string.h>`  | strspn       |      TODO      |      Core      ||
| `<string.h>`  | strcspn      |      TODO      |      Core      ||
| `<string.h>`  | strpbrk      |      TODO      |      Core      | Marcus Plutowski <achierius@gmail.com> |
| `<string.h>`  | strstr       |       ✔        |      Core      | Marcus Plutowski <achierius@gmail.com> |
| `<string.h>`  | strtok       |      TODO      |      Core      ||
| `<string.h>`  | strxfrm      |      TODO      |      Core      ||
| GNU           | mempcpy      |      TBD       |      Core      ||
| GNU, POSIX    | strerror\_r  |      TBD       |      Core      ||
| WDTR 24731    | strcat\_s    |      TBD       |      Core      ||
| WDTR 24731    | strcpy\_s    |      TBD       |      Core      ||
| Open/Free BSD | strlcat      |      TODO      |      Core      ||
| Open/Free BSD | strlcpy      |      TODO      |      Core      ||
| BSD, POSIX    | strdup       |      TODO      |      Core      ||
| POSIX 2008    | strsignal    |      TBD       |      Core      ||
| POSIX         | strtok\_r    |      TBD       |      Core      ||
| POSIX         | memccpy      |      TBD       |      Core      ||

## Usage
In order to use the functions implemented thus far, simply run `make lib` and
link the static library (aolc.a) that it generates; within your code, make sure
to import `<string.h>`. If your project does not use any extensions to the
spec (e.g. POSIX's `strtok_r`), it is not necessary to compile with our `string.h`
in your include path - your system's `<string.h>` will work just fine.

## Contributing
### Branch Convention
For now, simply make sure to develop implementations for function `<X>` on the
branch `feature/string/<X>` — i.e. `strlen` is managed on `feature/string/strlen`.
Rebase before merging, and make sure your merges/PRs target develop.
### Style
Warning: repository style is currently mid-transition, so you will likely run
into code which does not follow style guidelines; we're working on fixing this.
#### C
With regards to C code, this project uses the Linux Kernel style guide, following
in the style of musl. There should, of course, be relatively little of this
code, _if any_ beyond the core header files - most all of the implementation
should be written in assembly.
#### ASM
Determining an appropriate style guide for the underlying assembly is a work-
in-progress; broadly, make sure to thoroughly (albeit not excessively)
comment all code. Of particular importance is annotating the contents/purpose
of whatever registers are in use by a function, at least once per function —
this greatly aids third-party code comprehension.
#### C++
This repository follows the Google C++ Style Guide for all C++ source material;
most 'supporting' code (including testing and benchmarking) is to be
implemented in C++ and thus in line with the aforementioned style.
### Testing
#### Running Tests
Simply run `make check` in the project root directory to build and run the test
suite.
#### Testing Your Own Code
Please make sure to implement at least rudimentary tests for any new
implementations; all tests are to be written in C++, and should make sure to
use the `googletest` framework rather than defining their own test semantics.
See the Googletest Primer in the references section below for more info. Note
that we do not define our own `main`, and as such neither should any of your
tests: simply implement the TEST()s that you need to achieve coverage and let
Googletest handle gluing them together.

All TEST()s implemented for a given `<string.h>` function should be placed
within a test suite with the name of that function - e.g. all tests for
`strlen` should be declared in the form `TEST(strlen, [test-name])`.
When an implementation is ready to PR, make sure to add the name of your file
to `$(TEST\_NAMES)` and the name of the target function to `$(STRING_FUNCS_DONE)`
so that `make check` will consider them properly for regression testing; we're
currently working on a more ergonomic solution for this part of the workflow.

Unlike with tests, we do not expect contributors to necessarily provide
benchmarks for their own implementations; nevertheless, it is appreciated if
you have the time.
#### Running Benchmarks
In order to run benchmarks for a given function `<X>`, run `make bench-<X>` in
the project root directory; this will run our benchmark suite on both of our
and glibc's implementations, then compare the result and print out the
difference. Note firstly that the 'old' implementation is in this case glibc,
and secondly that the printed difference is multiplicative; therefore, a
reported difference of "+1.513000" would indicate that our function took 1.513
times longer to run than glibc's equivalent implementation.

We are currently exploring multithreading the execution of benchmarks.

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
calling convention reference link below for more detail.
### Useful References
 - [Wikibooks page on <string.h>](https://en.wikibooks.org/wiki/C_Programming/string.h)
 - [glibc reference manual](https://www.gnu.org/software/libc/manual/pdf/libc.pdf)
 - [musl reference manual](https://musl.libc.org/manual.html)
 - [x86-64 cheat sheet](https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf)
 - [x86-64 calling convention](https://aaronbloomfield.github.io/pdr/book/x86-64bit-ccc-chapter.pdf)
 - [Googletest Primer](https://github.com/google/googletest/blob/master/googletest/docs/primer.md)


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
### Inspiration
Implementation strategies broadly inspired by musl first and foremost, as well
as by glibc.

## License
This project is licensed under the BSD License 2.0.
