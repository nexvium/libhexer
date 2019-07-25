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
#ifndef LIBHEXER_HEXER_OUT_H
#define LIBHEXER_HEXER_OUT_H

#include <stddef.h>

#include <string>
#include <stdexcept>

namespace libhexer
{

using std::string;

class HexOut
{
public:
    static const size_t MAX_GROUP_SIZE = 1024;
    enum Case { LOWER = 0, UPPER = 1 };

    /*
     * Configure hex output options. By default, lower case is used and no
     * grouping is done.
     */
    struct Config {
        Case    letter_case;
        size_t  group_size;
        string  group_separator;

        Config(): letter_case(LOWER), group_size(0), group_separator("") {}
    };
    static const Config CONFIG_DEFAULT;

    /*
     * Create a HexOut object. If the cfg parameter is null, default options
     * are used.
     */
    HexOut(const Config & cfg = CONFIG_DEFAULT)
      : _xchars(_GetHexChars(cfg.letter_case)),
        _group_size(_CheckGroupSize(cfg.group_size)),
        _group_separator(cfg.group_separator)
    {
    }

    /*
     * Set letter case to use for hex values A-F. Throws exception on invalid
     * setting. Returns object to allow chaining.
     */
    HexOut & LetterCase(Case lcase)
    {
        _xchars = _GetHexChars(lcase);
        return *this;
    }

    /*
     * Get the letter case being used for hex values A-F.
     */
    Case LetterCase(void) const
    {
        return _xchars[0xa] == 'a' ? LOWER : UPPER;
    }

    /*
     * Set group size. Size of 0 means no grouping. Throws exception on invalid
     * size. Returns object to allow chaining.
     */
    HexOut & GroupSize(size_t size)
    {
        _group_size = _CheckGroupSize(size);
        return *this;
    }

    /*
     * Get group size. Size of 0 means no grouping.
     */
    size_t GroupSize() const
    {
        return _group_size;
    }

    /*
     * Set group separator, which may be any string. Returns object to allow
     * chaining.
     */
    HexOut & GroupSeparator(const string & sep)
    {
        _group_separator = sep;
        return *this;
    }

    /*
     * Get group separator, which may be the empty string.
     */
    string GroupSeparator(void) const
    {
        return _group_separator;
    }

    /*
     * Output ints that are N-bits wide.
     */
    string XInt8(uint8_t val)   const { return _XIntN(2, val); }
    string XInt16(uint16_t val) const { return _XIntN(4, val); }

private:
    const char *    _xchars;
    size_t          _group_size;
    string          _group_separator;

    inline size_t _CheckGroupSize(size_t sz) const
    {
        if (sz > MAX_GROUP_SIZE)
            throw std::runtime_error("group size too big");
        return sz;
    }

    const char * _GetHexChars(Case lcase) const;
    string _XIntN(int nibbles, uint64_t val) const;
};

} //namespace

#endif //LIBHEXER_HEXER_OUT_H
