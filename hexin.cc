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
#include "libhexer/hexin.h"

#include <string.h>

namespace libhexer {
    HexIn XIN;
}

using namespace libhexer;

const size_t NUM_CHARS = 256;

const int8_t CHAR_INVALID = -1;
const int8_t CHAR_IGNORE  = 16;

// Set all characters in map to invalid.
void SetInvalidChars(char map[NUM_CHARS])
{
    memset(map, CHAR_INVALID, NUM_CHARS);
}

// Set hex characters to their value.
void SetHexChars(char map[NUM_CHARS])
{
    int i;

    for (i = '0'; i <= '9'; i++)
        map[i] = char(i - '0');

    for (i = 'A'; i <= 'F'; i++) {
        map[i] = char(i - 'A' + 10);
        map[i | (1 << 5)] = map[i];
    }
}

HexIn::HexIn(const char *str, uint8_t *buf, size_t n)
{
    Ignore(NULL);
    const char *end = Parse(str, buf, n);
    if (*end != 0) {
        if (_chr[(uint8_t)*end] == CHAR_INVALID)
            throw std::runtime_error("invalid hex character");
        else
            throw std::runtime_error("buffer too small");
    }
}

void HexIn::Ignore(const char *chars)
{
    SetInvalidChars(_chr);
    if (chars != NULL) {
        while (*chars != 0)
            _chr[(int)*chars++] = CHAR_IGNORE;
    }
    SetHexChars(_chr);
}

char *HexIn::Parse(const char *str, uint8_t *buf, size_t n) const
{
    size_t j = 0;

    if (str == NULL)
        return NULL;

    size_t i;
    for (i = 0; str[i] != 0 && j / 2 < n; i++) {
        uint8_t c = str[i];

        if (_chr[c] == CHAR_INVALID)
            break;
        else if (_chr[c] == CHAR_IGNORE)
            continue;
        else if (j % 2 == 0)
            buf[j / 2]  = uint8_t(_chr[c] << 4);
        else
            buf[j / 2] |= uint8_t(_chr[c]);

        j++;
    }

    return (char *)str + i;
}

char *HexIn::Parse(const char *str, uint8_t **buf, size_t *n) const
{
    if (str == NULL)
        return NULL;

    // find the number of hex chars as well as their end
    const char *pos = str;
    size_t xdigits = 0;
    while (*pos != 0) {
        uint8_t c = *pos++;

        if (_chr[c] == CHAR_INVALID)
            break;
        else if (_chr[c] == CHAR_IGNORE)
            continue;
        else
            xdigits++;
    }

    if (buf == NULL && n == NULL)
        return (char *)pos;

    size_t size = xdigits / 2 + xdigits % 2;
    if (n != NULL)
        *n = size;

    if (buf == NULL)
        return (char *)pos;

    // use "operator new(size)" instead of "new uint8_t[size]" so that the
    // memory can be released with "delete" rather than requiring "delete[]".
    *buf = static_cast<uint8_t *>(operator new (size));

    return Parse(str, *buf, size);
}

uint8_t HexIn::UInt8(const char *str) const
{
    uint8_t byte;
    char *end = Parse(str, &byte, 1);
    if (*end != 0)
        throw std::runtime_error("string is not 8-bit value");
    return byte;
}

uint16_t HexIn::UInt16(const char *str) const
{
    uint8_t bytes[2];
    char *end = Parse(str, (uint8_t *)&bytes, 2);
    if (*end != 0)
        throw std::runtime_error("string is not 16-bit value");
    return uint16_t(bytes[1] << 8 | bytes[0]);
}

uint32_t HexIn::UInt32(const char *str) const
{
    uint8_t bytes[4];
    char *end = Parse(str, (uint8_t *)&bytes, 4);
    if (*end != 0)
        throw std::runtime_error("string is not 32-bit value");
    return bytes[3] << 24 | bytes[2] << 16 | bytes[1] << 8 | bytes[0];
}

uint64_t HexIn::UInt64(const char *str) const
{
    uint8_t bytes[8];
    char *end = Parse(str, (uint8_t *)&bytes, 8);
    if (*end != 0)
        throw std::runtime_error("string is not 64-bit value");
    return (uint64_t)bytes[0] << 56 | (uint64_t)bytes[1] << 48 |
           (uint64_t)bytes[2] << 40 | (uint64_t)bytes[3] << 32 |
           (uint64_t)bytes[4] << 24 | (uint64_t)bytes[5] << 16 |
           (uint64_t)bytes[6] <<  8 | (uint64_t)bytes[7];
}
