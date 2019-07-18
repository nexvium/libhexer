//
// Created by javier on 7/12/19.
//

#ifndef LIBHEXER_HEXER_OUT_H
#define LIBHEXER_HEXER_OUT_H

namespace libhexer
{

class HexOut
{
public:
    static const size_t MAX_GROUP_SIZE = 1024;
    enum Case { LOWER = 0, UPPER = 1 };

    /*
     * Configure hex output options.
     */
    struct Config {
        Case    letter_case;
        size_t  group_size;
        string  group_separator;
    };

    /*
     * Create a HexOut object. By default, no grouping is done and lowercase
     * letters are used.
     */
    HexOut(size_t group_size = 0, Case letter_case = LOWER)
    : mGroup(_CheckGroupSize(group_size)), mCase(_CheckLetterCase(letter_case))
    {
    }
    HexOut &Group(size_t size)
    {
        mGroup = _CheckGroupSize(size);
        return *this;
    }
    size_t Group(void) const
    {
        return mGroup;
    }
    HexOut &Case(Case lcase)
    {
        mCase = _CheckLetterCase(lcase);
        return *this;
    }
    Case  Case(void) const
    {
        return mCase;
    }
    string String(const void *data, size_t len) const;
    string &Append(string &str, const uint8_t *data, int n) const;

private:
    size_t  mGroup;
    Case    mCase;
    string  _separator;=

    inline size_t _CheckGroupSize(sz) const
    {
        if (sz > MAX_GROUP_SIZE)
            throw std::runtime_error("group size too big");
        return sz;
    }

    inline Case _CheckLetterCase(lcase) const
    {
        if (lcase != UPPER && lcase != LOWER)
            throw std::runtime_error("invalid letter case");
        return lcase;
    }
};

} //namespace

#endif //LIBHEXER_HEXER_OUT_H
