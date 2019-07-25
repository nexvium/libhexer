# libhexer

A small, simple C/C++ library for reading and writing arbitrary binary data in 
hexadecimal format.

## Motivation

This library was motivated by working on an existing code base that made
extensive use of hexadecimal to exchange binary data between programs.  I was
dismayed to see the source littered with code fragments like

    for (i = 0; i < 4; i++) {
        printf("%02X", id[i]);
    }
    printf(":");
    for (i = 0; i < 64; i++) {
        if (i % 4 == 0)
            printf(" ");
        printf("%02X", data[i]);
    }
    print("\n");

throughout, sometimes with stupid bugs that were obviously due to
copying-and-pasting and failing to make some minor but required modification.

Even worse, there was also code like

    printf("%08X:", *(uint32_t *)&id[0]);
    for (i = 0; i < 16; i++) {
        printf(" %08X", *(uint32_t)&data[i*4]);
    }
    print("\n");

resulting in ambiguous or erroneous endianess when trying to interpret the
output.

I decided to implement a more readable, maintainable, and less error-prone
approach. For example, the first block above can be replaced with

    XOUT.GroupSize(4).GroupSeparator(" ");
    printf("%s: %s\n", XDATN(id, 4), XDATN(data, 64));

## Usage

    #include <libhexer/hexer.h>
    
    using libhexer;
    
    // Output fixed-sized integers using defaults.
    printf("%s", XINT32(a));
    printf("%s", XINT40(b));

    // Output SHA-256 using defaults.
    printf("%s", XDATN(digest, 32));

    // Create output custom output object that uses upper-case letters,
    // groups of four bytes, and a single space between groups.
    HexOut xout({ .case = UPPER_CASE, .group = 4, .separator = " " });
    
    // Output fixed-sized integers using custom settings.
    std::cout << xout.Int32(a) << std::endl;
    printf("%s", xout.Int32(a).c_str())
