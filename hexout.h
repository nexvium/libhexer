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
    enum LetterCase { LOWER = 0, UPPER = 1 };
    enum PartialGroup { FRONT = 0, BACK = 1 };

    /*
     * Configure hex output options. By default, lower case is used and no
     * grouping is done.
     */
    struct Config {
        LetterCase      letter_case;
        PartialGroup    partial_group;
        size_t          group_size;
        string          group_separator;

        Config()
          : letter_case(LOWER),
            partial_group(FRONT),
            group_size(0),
            group_separator("")
        {
        }
    };
    static const Config CONFIG_DEFAULT;

    /*
     * Create a HexOut object. If the cfg parameter is null, default options
     * are used.
     */
    HexOut(const Config & cfg = CONFIG_DEFAULT)
      : _xchars(_GetHexChars(cfg.letter_case)),
        _partial_group(_CheckPartialGroup(cfg.partial_group)),
        _group_size(_CheckGroupSize(cfg.group_size)),
        _group_separator(cfg.group_separator)
    {
    }

    /*
     * Get the current configuration settings.
     */
    Config GetConfig() const
    {
        Config cfg;

        cfg.letter_case     = _xchars[0xa] == 'a' ? LOWER : UPPER;
        cfg.partial_group   = _partial_group;
        cfg.group_size      = _group_size;
        cfg.group_separator = _group_separator;

        return cfg;
    }

    /*
     * Set letter case to use for hex values A-F. Throws exception on invalid
     * setting. Returns self to allow chaining.
     */
    HexOut & SetLetterCase(LetterCase lcase)
    {
        _xchars = _GetHexChars(lcase);
        return *this;
    }

    /*
     * Set partial group mode. Throws exception on invalid setting. Returns
     * self to allow chaining.
     */
    HexOut & SetPartialGroup(PartialGroup pg)
    {
        _partial_group = _CheckPartialGroup(pg);
        return *this;
    }

    /*
     * Set group size. Size of 0 means no grouping. Throws exception on invalid
     * size. Returns self to allow chaining.
     */
    HexOut & SetGroupSize(size_t size)
    {
        _group_size = _CheckGroupSize(size);
        return *this;
    }

    /*
     * Set group separator, which may be any string. Returns self to allow
     * chaining.
     */
    HexOut & SetGroupSeparator(const string & sep)
    {
        _group_separator = sep;
        return *this;
    }

    /*
     * Output ints that are N-bits wide.
     */
    string XInt8(uint8_t val)   const { return _XIntN(1, val); }
    string XInt16(uint16_t val) const { return _XIntN(2, val); }
    string XInt24(uint32_t val) const { return _XIntN(3, val); }

private:
    const char *    _xchars;
    PartialGroup    _partial_group;
    size_t          _group_size;
    string          _group_separator;

    inline size_t _CheckGroupSize(size_t sz) const {
        if (sz > MAX_GROUP_SIZE) {
            throw std::runtime_error("group size too big");
        }
        return sz;
    }

    inline PartialGroup _CheckPartialGroup(PartialGroup pg) const
    {
        if (pg != FRONT && pg != BACK) {
            throw std::runtime_error("invalid partial group setting");
        }
        return pg;
    }

    const char * _GetHexChars(LetterCase lcase) const;
    string _XIntN(size_t nbytes, uint64_t val) const;
};

} //namespace

#endif //LIBHEXER_HEXER_OUT_H
