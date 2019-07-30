/*
** Copyright 2019 Javier Alvarado
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include "libhexer/hexer.h"

#include <assert.h>

#include <string>

//using namespace libhexer;

#define CHECKN(a,b,c) do { \
    std::string s = b(a); \
    printf("INPUT: %s  OUTPUT: %s  EXPECTED: %s\n", #a, s.c_str(), c); \
    assert(s == c); \
    } while (0)

int main(int, char **)
{
    /* Test convenience macros. */
    printf("%s\n", XINT8(0xbe));
    printf("%s\n", XINT16(0xaced));
    printf("%s\n", XINT24(0xdecade));
    printf("%s\n", XINT32(0xbabeface));
    uint8_t buffer[32] = { 0x3f,0x4f,0xb3,0xd9,0x15,0x2a,0xff,0x94,
                           0x97,0xbb,0xd7,0x3c,0x3c,0x35,0x14,0xac,
                           0x7c,0x7a,0xc3,0x21,0x9d,0x71,0x05,0x56,
                           0x47,0xe0,0x42,0x19,0xeb,0x48,0x79,0x22 };
    printf("%s\n", XDATN(buffer, sizeof(buffer)));

    using namespace libhexer;

    /* Change default object's settings to format MAC address. */
    XOUT.SetGroupSize(1).SetGroupSeparator(":");
    CHECKN(0XCAFEBABEFACE, XOUT.Int48, "ca:fe:ba:be:fa:ce");

    XOUT.SetGroupSize(2);
    CHECKN(0xdecade, XOUT.Int24, "de:cade");

    return 0;
}
