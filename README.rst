#######################################
RenderMan Shading Language Bits ’n Bobs
#######################################

This repository contains various useful RenderMan SL headers and code
snippets for shader writers, TDs and certainly for people who are
learning RSL.


=======
Headers
=======


---------
bitwise.h
---------

Bitwise operations (and, not, or, xor) on floats. RSL lacks native
support for these. This header solves this w/o introducing the need
to use any external DSO.
