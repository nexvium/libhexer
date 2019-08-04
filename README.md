# libhexer

A small, simple C/C++ library for reading and writing arbitrary binary data in 
hexadecimal format.

## Motivation

This library was motivated by working on an existing code base that made
extensive use of hexadecimal to exchange binary data between programs.  I was
dismayed to see the source littered with code fragments like

    for (i = 0; i < 8; i++) {
        if (i == 4)
            printf(" ");
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

    XOUT.GroupSize(4).GroupSeparator(' ');
    printf("%s: %s\n", XTOSTR(id, 8), XTOSTR(data, 64));

to output hex in the format

    xxxxxxxx xxxxxxxx: xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx ...

## Usage

    #include <libhexer/hexer.h>
    
    // Output fixed-sized integers using default settings.
    printf("%s", XINT32(a));
    printf("%s", XINT40(b));

    // Output SHA-256 digest using defaults.
    printf("%s", XTOSTR(digest, 32));

    using namespace libhexer;

    // Create custom output object that uses upper-case letters,
    // groups of four bytes, and a single space between groups.
    auto xout = HexOut::New().SetLetterCase(HexOut::UPPER)
                             .SetGroupSize(4)
                             .SetGroupSeparator(' ');
    
    // Output fixed-sized integers using custom settings.
    std::cout << xout.Int64(a) << std::endl;
    fprintf(f, "%s", xout.Int64(a).c_str());
