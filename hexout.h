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
    enum LetterCase { LOWER, UPPER };
    enum PartialGroup { LEADING, TRAILING };

    /*
     * Configure hex output options.  By default, lower case is used and no
     * grouping is done.
     */
    struct Config {
        LetterCase      letter_case;
        PartialGroup    partial_group;
        size_t          group_size;
        char            group_separator;
    };
    static const Config CONFIG_DEFAULT;

    /*
     * Return a new HexOut object.  This allows creating an object and
     * configuring it at the same time using chaining (for example,
     *     xout = HexOut::New().SetGroupSize(4);
     * ).  It *does* invoke the constructor twice, but it is relatively cheap,
     * so the small cost may be worth the convenience.
     */
    static HexOut New(void)
    {
        return HexOut();
    }

    /*
     * Create a HexOut object.  If the cfg parameter is omitted, the default
     * options are used.
     */
    HexOut(const Config & cfg = CONFIG_DEFAULT)
    {
        SetConfig(cfg);
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
     * Set all configuration options at once.
     */
    void SetConfig(const Config & cfg)
    {
        _xchars             = _GetHexChars(cfg.letter_case);
        _partial_group      = _CheckPartialGroup(cfg.partial_group);
        _group_size         = _CheckGroupSize(cfg.group_size);
        _group_separator    = cfg.group_separator;
    }

    /*
     * Set letter case to use for hex digits A-F.  Throws exception on invalid
     * setting.  Returns self to allow chaining.
     */
    HexOut & SetLetterCase(LetterCase lcase)
    {
        _xchars = _GetHexChars(lcase);
        return *this;
    }

    /*
     * Set partial group mode (whether an incomplete group should be at the
     * start of the hex string or the end).  Throws exception on invalid
     * setting.  Returns self to allow chaining.
     */
    HexOut & SetPartialGroup(PartialGroup pg)
    {
        _partial_group = _CheckPartialGroup(pg);
        return *this;
    }

    /*
     * Set group size. Size of 0 means no grouping.  Throws exception on invalid
     * size.  Returns self to allow chaining.
     */
    HexOut & SetGroupSize(size_t size)
    {
        _group_size = _CheckGroupSize(size);
        return *this;
    }

    /*
     * Set group separator, which may be any non-zero character. Throws
     * exception on invalid character.
     */
    HexOut & SetGroupSeparator(char sep)
    {
        _group_separator = _CheckGroupSeparator(sep);
        return *this;
    }

    /*
     * Return a hex string of a single N-bit wide integer.
     */
    string UInt8(uint8_t val)   const { return _UIntN(1, val); }
    string UInt16(uint16_t val) const { return _UIntN(2, val); }
    string UInt24(uint32_t val) const { return _UIntN(3, val); }
    string UInt32(uint32_t val) const { return _UIntN(4, val); }
    string UInt40(uint64_t val) const { return _UIntN(5, val); }
    string UInt48(uint64_t val) const { return _UIntN(6, val); }
    string UInt56(uint64_t val) const { return _UIntN(7, val); }
    string UInt64(uint64_t val) const { return _UIntN(8, val); }

    /*
     * Return a hex string of arbitrarily long data.
     */
    string ToString(void * dat, size_t len) const;

    /*
     * Write the hex string into the user-provided buffer.  An exception is
     * thrown if the buffer is not big enough.  Returns buf.
     */
    char *ToCString(void * dat, size_t len, char *buf, size_t buflen) const;

private:
    const char *    _xchars;
    PartialGroup    _partial_group;
    size_t          _group_size;
    char            _group_separator;

    inline PartialGroup _CheckPartialGroup(PartialGroup pg) const
    {
        if (pg != LEADING && pg != TRAILING) {
            throw std::runtime_error("invalid partial group setting");
        }
        return pg;
    }

    inline size_t _CheckGroupSize(size_t sz) const
    {
        if (sz > MAX_GROUP_SIZE) {
            throw std::runtime_error("group size too big");
        }
        return sz;
    }

    inline char _CheckGroupSeparator(char sep) const
    {
        if (sep < 1) {
            throw std::runtime_error("invalid group separator");
        }
        return sep;
    }

    const char * _GetHexChars(LetterCase lcase) const;
    string _UIntN(size_t len, uint64_t val) const;
};

} //namespace

#endif //LIBHEXER_HEXER_OUT_H
