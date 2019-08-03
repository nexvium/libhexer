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
#include <string.h>

#include <string>

#define CHECKN(a,b,c) do { \
    std::string s = b(a); \
    printf("INPUT: %s  OUTPUT: %s  EXPECTED: %s\n", #a, s.c_str(), c); \
    assert(s == c); \
    } while (0)

int main(int, char **)
{
    /* Test macros that use global HexOut object. */
    assert(strcmp(XINT8(0XBE), "be") == 0);
    assert(strcmp(XINT16(0xaced), "aced") == 0);
    assert(strcmp(XINT24(0xdecade), "decade") == 0);
    assert(strcmp(XINT32(0xBabeFace), "babeface") == 0);
    uint8_t buffer[32] = { 0X3F,0X4F,0XB3,0XD9,0X15,0X2A,0XFF,0X94,
                           0X97,0XBB,0XD7,0X3C,0X3C,0X35,0X14,0XAC,
                           0X7C,0X7A,0XC3,0X21,0X9D,0X71,0X05,0X56,
                           0X47,0XE0,0X42,0X19,0XEB,0X48,0X79,0X22 };
    printf("SHA-256: %s\n", XBUFN(buffer, sizeof(buffer)));

    /* Modify the global HexOut object used by macros. */
    libhexer::XOUT.SetGroupSize(4);
    printf("SHA-256: %s\n", XBUFN(buffer, sizeof(buffer)));

    using namespace libhexer;

    /* Use custom HexOut object. */
    auto xout = HexOut::New().SetGroupSize(1).SetGroupSeparator(":");

    CHECKN(0XCAFEBABEFACE, xout.Int48, "ca:fe:ba:be:fa:ce");

    xout.SetGroupSize(2);
    CHECKN(0xdecade, xout.Int24, "de:cade");

    xout.SetPartialGroup(HexOut::TRAILING);
    CHECKN(0xdecade, xout.Int24, "deca:de");

    return 0;
}
