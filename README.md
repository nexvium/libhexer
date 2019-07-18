# libhex

A small, simple C/C++ library for reading and writing arbitrary binary data in 
hexadecimal format.

## Motivation

This library is the result of me working on an existing code base that made
extensive use of hexadecimal to exchange binary data between programs.  I was
dismayed to see the source code littered with fragments like

    for (i = 0; i < 4; i++) {
        printf("%02X", id[i]);
    }
    printf(":");
    for (i = 0; i < 64; i++) {
        if (i % 4 == 0)
            printf(" ");
        printf("%02X", data[i];
    }
    print("\n");

all over the place.  And sometimes with stupid bugs that were obviously due to
copying-and-pasting and failing to make some minor but required modification.

Or even worse, something like

    printf("%08X:", *(uint32_t *)&id[0]);
    for (i = 0; i < 16; i++) {
        printf(" %08X", *(uint32_t)&data[i*4]);
    }
    print("\n");

resulting in ambiguous or erroneous endianess when trying to interpret the
output.

When it came time to work on another project without the legacy code, I decided
to implement a more readable, maintainable, and less error-prone approach.

## Usage

    #include <libhexer/hexer.h>
    
    using libhexer;
    
    // Output SHA-256 using defaults.
    XPRINTN(digest, 32));
    
    // Create output object with defaults
    // (no grouping, lower case letters).
    HexOut xout;
    
    // Output SHA-256 digest.
    std::cout << xout.String(digest, 32) << std::endl;
    fprintf("%s\n", xout.String(digest, 32).c_str());

