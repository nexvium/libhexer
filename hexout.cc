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
#include "libhexer/hexout.h"

#include <assert.h>

static const char XUPPER[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                               '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'  };
static const char XLOWER[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                               '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'  };

namespace libhexer {
    const HexOut::Config HexOut::CONFIG_DEFAULT = {
            .letter_case = LOWER,
            .partial_group = LEADING,
            .group_size = 0,
            .group_separator = ' '
     };

    HexOut XOUT;
}

using namespace libhexer;

const char * HexOut::_GetHexChars(LetterCase lcase) const
{
    const char * xchar = nullptr;

    switch (lcase) {
        case HexOut::LOWER:
            xchar = XLOWER;
            break;
        case HexOut::UPPER:
            xchar = XUPPER;
            break;
        default:
            throw std::runtime_error("invalid letter case");
    }

    return xchar;
}

string HexOut::_IntN(size_t len, uint64_t val) const
{
    assert(len <= 8);

    uint8_t buffer[8];
    for (size_t i = 1; i <= len; i++) {
        buffer[i-1] = val >> ((len - i) * 8) & 0xFF;
    }

    return ToString(buffer, len);
}

inline size_t _CalcStrLen(size_t nbytes, size_t grpsz)
{
    size_t ngroups = 1;
    if (grpsz > 0) {
        ngroups = nbytes / grpsz + (nbytes % grpsz ? 1 : 0);
    }
    return nbytes * 2 + (ngroups - 1);
}

string HexOut::ToString(void * dat, size_t len) const
{
    string str;

    str.resize(_CalcStrLen(len, _group_size));
    ToCString(dat, len, str.data(), str.size() + 1);

    return str;
}

char* HexOut::ToCString(void * dat, size_t len, char * buf, size_t buflen) const
{
    if (buflen < _CalcStrLen(len, _group_size) + 1) {
        throw std::runtime_error("buffer too small");
    }

    size_t i = 0, j = 0;
    auto bytes = (uint8_t *)dat;
    if (_group_size > 0 && _partial_group == LEADING) {
        i = _group_size - (len % _group_size);
    }

    char * str = buf;
    while (j < len) {
        if (_group_size > 0 && j > 0 && i % _group_size == 0) {
            *str++ =_group_separator;
        }
        *str++ = _xchars[bytes[j] >> 4 & 0xF];
        *str++ = _xchars[bytes[j] >> 0 & 0xF];
        i++, j++;
    }
    *str = 0;

    return buf;
}
