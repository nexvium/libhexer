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

using namespace libhexer;

static const char XUPPER[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                         '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'  };
static const char XLOWER[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                         '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'  };

static const char XTABLE[256] = {
  /* 00-0F */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 10-1F */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 20-2F */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 30-3F */  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
  /* 40-4F */ -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 50-5F */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 60-6F */ -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 70-7F */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 80-8F */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 90-9F */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* A0-AF */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* B0-BF */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* C0-CF */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* D0-DF */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* E0-EF */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* F0-FF */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

const HexOut::Config HexOut::CONFIG_DEFAULT;

HexOut XOUT;

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

/* This implementation but doesn't do groups.
string HexOut::_XIntN(int nibbles, uint64_t val) const
{
    string hex(nibbles, '0');

    for (int i = nibbles - 1; val != 0; i--) {
        hex[i] = _xchars[val & 0xF];
        val >>= 4;
    }

    return hex;
}
*/

inline size_t _CalcStrLen(size_t nbytes, size_t grpsz, size_t seplen)
{
    size_t ngroups = nbytes / grpsz + (nbytes % grpsz ? 1 : 0);
    return nbytes * 2 + seplen * (ngroups - 1);
}

string HexOut::_XIntN(size_t nbytes, uint64_t val) const
{
    string hex;
    hex.reserve(_CalcStrLen(nbytes, _group_size, _group_separator.length()));

    size_t i = 0;   // group index
    size_t j = 0;   // byte index

    if (_partial_group == FRONT) {
        i = _group_size - (nbytes % _group_size);
    }
    while (j < nbytes) {
        if (j > 0 && i % _group_size == 0) {
            hex += _group_separator;
        }
        hex += _xchars[(val >> ((nbytes - j) * 8 - 4)) & 0xF];
        hex += _xchars[(val >> ((nbytes - j) * 8 - 8)) & 0xF];
        i++, j++;
    }

    return hex;
}