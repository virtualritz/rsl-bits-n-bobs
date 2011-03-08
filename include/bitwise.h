/**
 * Bitwise operations on floats in RenderMan shading language.
 * Based on http://lua-users.org/lists/lua-l/2002-09/msg00134.html
 * and http://lua-users.org/wiki/BitUtils
 *
 * If you don't want to add the overhead of the lookup table to
 * your shaders, define SLIM_XOR. Note that this will make the
 * operations a lot slower.
 *
 *
 * Copyright (C) 2011 by Moritz Moeller
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE
 *
 */


#ifdef SLIM_XOR
#define xor xorCalc
#else
#define xor xorLookup
#endif


#define int float
#define FF 4294967295.0 // 2^32 - 1


/**
 * Biwise 'exclusive or' aka 'xor'.
 * All other bitwise operations can be described using this (except
 * 'not' which works w/o it.
 */
float xorLookup( int a, b )
{
    // tab[i][j] = xor(i-1, j-1)
    int tab[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14,
        2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13,
        3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12,
        4, 5, 6, 7, 0, 1, 2, 3, 12, 13, 14, 15, 8, 9, 10, 11,
        5, 4, 7, 6, 1, 0, 3, 2, 13, 12, 15, 14, 9, 8, 11, 10,
        6, 7, 4, 5, 2, 3, 0, 1, 14, 15, 12, 13, 10, 11, 8, 9,
        7, 6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9, 8,
        8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7,
        9, 8, 11, 10, 13, 12, 15, 14, 1, 0, 3, 2, 5, 4, 7, 6,
        10, 11, 8, 9, 14, 15, 12, 13, 2, 3, 0, 1, 6, 7, 4, 5,
        11, 10, 9, 8, 15, 14, 13, 12, 3, 2, 1, 0, 7, 6, 5, 4,
        12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3,
        13, 12, 15, 14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2,
        14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1,
        15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
    };

    int res = 0;
    int c = 1;

    while ( 0 < a && 0 < b ) {
        int a2 = mod( a, 16 );
        int b2 = mod( b, 16 );
        res += tab[ 16 * a2 + b2 ] * c;
        a = ( a - a2 ) / 16;
        b = ( b - b2 ) / 16;
        c = c * 16;
    }

    res += a * c + b * c;

    return res;
}


/**
 * this version doesn't use a table but is slower
 * as it requires more calls to mod() and worse:
 * to pow()!
 */
float xorCalc( int x, y )
{
    int z = 0;

    int i;

    for ( i = 0; i < 32; i += 1 ) {
        if ( 0 == mod( x, 2 ) ) {              // x had a '0' in bit i
            if ( 1 == mod( y, 2 ) ) {          // y had a '1' in bit i
                y -= 1;                        // set bit i of z to '1'
                z += pow( 2, i );
            }

        } else {                               // x had a '1' in bit i
            x -= 1;

            if ( 0 == mod( y, 2 ) ) {          // y had a '0' in bit i
                z += pow( 2, i );              // set bit i of z to '1'

            } else {
                y -= 1;
            }
        }

        y /= 2;
        x /= 2;
    }

    return z;
}


float not( int a )
{
    return FF - a;
}


float and( int a, b )
{
    return 0.5 * (( a + b ) - xor( a, b ) );
}


float or( int a, b )
{
    return FF - and( FF - a, FF - b );
}
