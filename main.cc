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

#include <iostream>
#include <string>

#define CHECKN(a,b,c) do { \
    std::string s = b(a); \
    printf("INPUT: %s  OUTPUT: %s  EXPECTED: %s\n", #a, s.c_str(), c); \
    assert(s == c); \
    } while (0)

// TODO: Make all tests actually test results instead of outputting them.

void TestHexOut(void)
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
    printf("SHA-256: %s\n", XTOSTR(buffer, sizeof(buffer)));

    /* Modify the global HexOut object used by macros. */
    libhexer::XOUT.SetGroupSize(4);
    printf("SHA-256: %s\n", XTOSTR(buffer, sizeof(buffer)));

    using namespace libhexer;

    /* Use custom HexOut object. */
    auto xout = HexOut::New().SetGroupSize(1).SetGroupSeparator(':');

    CHECKN(0XCAFEBABEFACE, xout.Int48, "ca:fe:ba:be:fa:ce");

    xout.SetGroupSize(2);
    CHECKN(0xdecade, xout.Int24, "de:cade");

    xout.SetPartialGroup(HexOut::TRAILING);
    CHECKN(0xdecade, xout.Int24, "deca:de");
}

void TestHexIn(void)
{
    using namespace std;
    using namespace libhexer;

    HexIn xin;
    HexOut xout;

    uint64_t value = 0;
    const char *rest = xin.Parse("DEADBEEFCAFEBABE", (uint8_t *)&value, 8);
    cout << hex << value << " + \"" << rest << "\"" << endl;

    value = 0;
    rest = xin.Parse("DEADBEEF CAFEBABE", (uint8_t *)&value, 8);
    cout << hex << value << " + \"" << rest << "\"" << endl;

    value = 0;
    xin.Ignore(" ");
    rest = xin.Parse("DEADBEEF CAFEBABE", (uint8_t *)&value, 8);
    cout << hex << value << " + \"" << rest << "\"" << endl;

    value = 0;
    rest = xin.Parse("Dead Beef\tCafe Babe", (uint8_t *)&value, 8);
    cout << hex << value << " + \"" << rest << "\"" << endl;

    value = 0;
    xin.Ignore(" \t");
    rest = xin.Parse("Dead Beef\tCafe Babe", (uint8_t *)&value, 8);
    cout << hex << value << " + \"" << rest << "\"" << endl;

    value = 0;
    xin.Ignore(":");
    rest = xin.Parse("00:1e:8c:0e:7d:e6 <-- HWaddr", (uint8_t *)&value, 8);
    cout << hex << value << " + \"" << rest << "\"" << endl;

    uint8_t *data = NULL;
    size_t   count = 0;
    xin.Ignore(": ");
    rest = xin.Parse("020C0600 00000005 : 000029FF F5C618D1 C1B7ED48 395925C2"
                     "E76D65ED 4AC39588 AA1AD11F F99F1178 A0ADFA44 950154F5"
                     "DD504AC1 DD36933C DC95E371 14DC3251 EEEF1ECC 81BB6495"
                     "8F91DA0D 27F888AB 8F2D2614 6FCB9378 05003DB0",
                     &data, &count);

    cout << xout.ToString(data, count) << endl;

    xout.SetGroupSize(8).SetLetterCase(HexOut::UPPER);
    cout << xout.ToString(data, count) << endl;

    delete data;
    data = NULL;

    xin.Ignore(NULL);
    xin.Ignore("");
    cout << hex << xin.UInt16("1fe") << endl;

    cout << hex << xin.UInt64("4AC39588AA1AD11F") << endl;
}

int main(int, char **)
{
    std::cout << "HEX OUT:" << std::endl;
    TestHexOut();
    std::cout << std::endl;

    std::cout << "HEX IN:" << std::endl;
    TestHexIn();

    return 0;
}
