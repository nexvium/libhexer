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

using namespace libhexer;

#define CHECK(a,b,c) do { \
    string s = b(a); \
    printf("INPUT: %x  OUTPUT: %s  EXPECTED: %s\n", a, s.c_str(), c); \
    assert(s == c); \
    } while (0)

int main(int, char **)
{
//    /* Test convenience macros. */
//    printf("%s\n", XINT8(0xbe));
//    printf("%s\n", XINT16(0xaced));
//    printf("%s\n", XINT24(0xdecade));
//    printf("%s\n", XINT32(0xbabeface));

    /* Change default object's settings to format MAC address. */
//    XOUT.SetGroupSize(1).SetGroupSeparator(":");
//    printf("%s\n", XINT48(0xcafebabeface));

    //printf("%s"XOUT.XInt16(0xaced) == "aced");
    XOUT.SetGroupSize(1).SetGroupSeparator(":");
    CHECK(0xaced, XOUT.XInt16, "ac:ed");

    XOUT.SetGroupSize(2);
    CHECK(0xdecade, XOUT.XInt24, "de:cade");

    return 0;
}