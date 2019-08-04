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
#ifndef LIBHEXER_HEXER_IN_H
#define LIBHEXER_HEXER_IN_H

#include <stddef.h>

#include <string>
#include <stdexcept>

namespace libhexer {

class HexIn {
public:
    /*
     * Create a HexIn object.  The ignore param may be a string containing
     * characters that should be silently ignored when parsing hex. If omitted
     * or an empty string, no characters are ignored.
     */
    HexIn(const char *ignore = NULL)
    {
        Ignore(ignore);
    }

    /*
     * Create a HexIn object and parse a hex string into the given buffer.
     * Throws exception on invalid char or if the buffer is too small.
     */
    HexIn(const char *str, uint8_t *buf, size_t n);

    /*
     * Set list of characters to ignore when parsing hex. Set list to empty
     * whe chars is NULL or "".
     */
    void Ignore(const char *chars);

    /*
     * Parse arbitrarily-long hex string. Return position after last parsed
     * digit. Return parsed data in buf, if not NULL. Return number of
     * bytes parsed in n, if not NULL.
     */
    char *Parse(const char *str, uint8_t *buf, size_t n) const;

    /*
     * Parse arbitrarily-long hex string. Return position after last parsed
     * digit. Return address and size of allocated buffer in buf and n,
     * respectively.
     */
    char *Parse(const char *str, uint8_t **buf, size_t *n) const;

    /*
     * Parse a hex integer. Returns position after last parsed digit.
     * Throws exception if str or num pointer is NULL.
     */
    char *UInt8(const char *str, uint8_t *num) const;
    char *UInt16(const char *str, uint16_t *num) const;
    char *UInt32(const char *str, uint32_t *num) const;
    char *UInt64(const char *str, uint64_t *num) const;

    /*
     * Parse a hex integer. Returns the integer. Throws exception if
     * string contains any non-hex, non-ignored chars, or if the hex value
     * is too big for the requested type.
     */
    uint8_t UInt8(const char *str) const;
    uint16_t UInt16(const char *str) const;
    uint32_t UInt32(const char *str) const;
    uint64_t UInt64(const char *str) const;

private:
    char _chr[256];
};

} //namespace

#endif  //LIBHEXER_HEXER_IN_H
