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

#define LIBHEXER_VERSION "0.9.0"

#include "libhexer/hexout.h"
#include "libhexer/hexin.h"

extern libhexer::HexOut XOUT;

/*
 * This macros are for convenience when using printf("%s").
 */
#define XINT8(a)  XOUT.Int8(a).c_str()
#define XINT16(a) XOUT.Int16(a).c_str()
#define XINT24(a) XOUT.Int24(a).c_str()
#define XINT32(a) XOUT.Int32(a).c_str()
#define XINT40(a) XOUT.Int40(a).c_str()
#define XINT48(a) XOUT.Int48(a).c_str()
#define XINT56(a) XOUT.Int56(a).c_str()
#define XINT64(a) XOUT.Int64(a).c_str()
#define XDATN(p,l) XOUT.Data(p,l).c_str()

namespace libhexer
{
/*
 * HexOut object used by convenience macros.
 */
//extern HexOut   XOUT;

/*
 * HexOut object used by convenience macros.
 */
//extern HexIn    XIN;

} //namespace

#endif //LIBHEXER_HEXER_OUT_H
