# libc Headers

## Overview
This file contains a list of estimates as to the amount of work that would be
required for implement each of the 29 headers specified by the C11 standard;
this is mostly for the purpose of planning future additions to the 

## Metric

- 0: Trivial (purely C-level -- macro definitions or similar)
- 1: Simple (Requires minimal and technically unchallenging code)
- 2: Involved (Worth a project in and of itself, but not too awful)
- 3: Difficult (Would require a project in and of itself; perhaps very nitty-gritty)
- ?: More investigation necessary

## Implementation Status
|  C Standard  |     C Header        | Difficulty |  Impl. Status  |
|:-------------|:-------------------:|:----------:|:--------------:|
| ANSI C       |   `<assert.h>`      |      1     |        x       |
| C 99         |   `<complex.h>`     |      2     |        x       |
| ANSI C       |   `<ctype.h>`       |      1+    |        x       |
| ANSI C       |   `<errno.h>`       |      0     |        x       |
| C 99         |   `<fenv.h>`        |      2?    |        x       |
| ANSI C       |   `<float.h>`       |      0     |        x       |
| ANSI C       |   `<inttypes.h>`    |      0     |        x       |
| C 99         |   `<iso646.h>`      |      0?    |        x       |
| C 95 - NA1   |   `<limits.h>`      |      0     |        x       |
| ANSI C       |   `<locale.h>`      |      2+    |        x       |
| ANSI C       |   `<math.h>`        |      1     |        x       |
| ANSI C       |   `<setjmp.h>`      |      3+    |        x       |
| ANSI C       |   `<signal.h>`      |      3-    |        x       |
| C 11         |   `<stdalign.h>`    |      ?     |        x       |
| ANSI C       |   `<stdarg.h>`      |      2     |        x       |
| C 11         |   `<stdatomic.h>`   |      3+    |        x       |
| C 99         |   `<stdbool.h>`     |      0     |        x       |
| ANSI C       |   `<stddef.h>`      |      0     |        x       |
| C 99         |   `<stdint.h>`      |      0     |        x       |
| ANSI C       |   `<stdio.h>`       |      3-    |        x       |
| ANSI C       |   `<stdlib.h>`      |      3     |        x       |
| C 11         |   `<stdnoreturn.h>` |      ?     |        x       |
| ANSI C       |   `<string.h>`      |      2+    |       WIP      |
| C 99         |   `<tgmath.h>`      |      2+    |        x       |
| C 11         |   `<threads.h>`     |      3     |        x       |
| ANSI C       |   `<time.h>`        |      3     |        x       |
| C 11         |   `<uchar.h>`       |      1     |        x       |
| C 95 - NA1   |   `<wchar.h>`       |      2+    |        x       |
| C 95 - NA1   |   `<wctype.h.h>`    |      2+    |        x       |
