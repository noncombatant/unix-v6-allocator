# Modernized UNIX v6 Allocator

This is a translation of the classic UNIX v6 `alloc`/`free` implementation,
[retrieved from The UNIX Heritage
Society](https://minnie.tuhs.org/cgi-bin/utree.pl?file=V6/usr/source/iolib/alloc.c)
on 2021-11-28, to modern C.

This version builds cleanly as C17 and runs seemingly-correctly on Ubuntu 21.10
with clang 13.0.0. (On macOS 12, there are some warnings; notably, macOS
declares `sbrk` to take `int`, not `intptr_t`.)

See [the commentary by ljrk](https://ljrk.codeberg.page/unixv6-alloc.html) for
a discussion of the allocator’s mechanism.
