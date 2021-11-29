# Modernized UNIX v6 Allocator

This is a translation of the classic UNIX v6 `alloc`/`free` implementation,
[retrieved from The UNIX Heritage
Society](https://minnie.tuhs.org/cgi-bin/utree.pl?file=V6/usr/source/iolib/alloc.c)
on 2021-11-28, to modern C.

The original code uses a very early dialect of C, which is closer to
[B](https://en.wikipedia.org/wiki/B_(programming_language)). For example, global
variables are not initialized with `=`; `+=` and `&=` were spelled `=+` and
`=&`; modern types like `size_t`, `void*` did not exist; everything except
`char` was a machine word and so type-punning was the norm; and so on.

The main significant change in this version is to enable it to handle machines
of any word size, rather than assuming a 16-bit machine as the original code
does. Additionally, I use modern types and names, which (I think) clarify the
code significantly and, by removing the puns and terseness, expose the algorithm
as beautifully simple.

This version builds cleanly as C17 and runs seemingly-correctly on Ubuntu 21.10
with clang 13.0.0. (On macOS 12, there are some warnings; notably, macOS
declares `sbrk` to take `int`, not `intptr_t`.)

See [the commentary by ljrk](https://ljrk.codeberg.page/unixv6-alloc.html) for
a discussion of the allocator’s mechanism.
