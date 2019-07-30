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

#define CHECKN(a,b,c) do { \
    string s = b(a); \
    printf("INPUT: %s  OUTPUT: %s  EXPECTED: %s\n", #a, s.c_str(), c); \
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
    XOUT.SetGroupSize(1).SetGroupSeparator(":");
    CHECKN(0XCAFEBABEFACE, XOUT.Int48, "ca:fe:ba:be:fa:ce");

    XOUT.SetGroupSize(2);
    CHECKN(0xdecade, XOUT.Int24, "de:cade");

    return 0;
}
