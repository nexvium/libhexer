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
#ifndef LIBHEXER_HEXER_H
#define LIBHEXER_HEXER_H

#define LIBHEXER_VERSION "0.x.y"

#include "libhexer/hexout.h"
#include "libhexer/hexin.h"

/*
 * These macros are for convenience with printf("%s")-style calls.  They use
 * a global HexOut object.
 */
#define XUINT8(a)  libhexer::XOUT.UInt8(a).c_str()
#define XUINT16(a) libhexer::XOUT.UInt16(a).c_str()
#define XUINT24(a) libhexer::XOUT.UInt24(a).c_str()
#define XUINT32(a) libhexer::XOUT.UInt32(a).c_str()
#define XUINT40(a) libhexer::XOUT.UInt40(a).c_str()
#define XUINT48(a) libhexer::XOUT.UInt48(a).c_str()
#define XUINT56(a) libhexer::XOUT.UInt56(a).c_str()
#define XUINT64(a) libhexer::XOUT.UInt64(a).c_str()
#define XTOSTR(d,l) libhexer::XOUT.ToString(d,l).c_str()
#define XTOCSTR(d,dl,b,bl) libhexer::XOUT.ToCString(d,dl,b,bl)

namespace libhexer
{
    extern HexOut   XOUT;
    extern HexIn    XIN;
}

#endif //LIBHEXER_HEXER_OUT_H
