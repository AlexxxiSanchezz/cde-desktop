/*
 * CDE - Common Desktop Environment
 *
 * Copyright (c) 1993-2012, The Open Group. All rights reserved.
 *
 * These libraries and programs are free software; you can
 * redistribute them and/or modify them under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * These libraries and programs are distributed in the hope that
 * they will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with these libraries and programs; if not, write
 * to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301 USA
 */
/* $XConsortium: TokenMessageArg.h /main/1 1996/07/29 17:06:23 cde-hp $ */
// Copyright (c) 1994 James Clark
// See the file COPYING for copying permission.

#ifndef TokenMessageArg_INCLUDED
#define TokenMessageArg_INCLUDED 1
#ifdef __GNUG__
#pragma interface
#endif

#include "MessageArg.h"
#include "types.h"
#include "Mode.h"
#include "Syntax.h"
#include "Sd.h"
#include "Ptr.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

class TokenMessageArg : public MessageArg {
public:
  TokenMessageArg(Token token, Mode mode,
		  const ConstPtr<Syntax> &syntax,
		  const ConstPtr<Sd> &sd);
  MessageArg *copy() const;
  void append(MessageBuilder &) const;
private:
  Token token_;
  Mode mode_;
  ConstPtr<Syntax> syntax_;
  ConstPtr<Sd> sd_;
};

#ifdef SP_NAMESPACE
}
#endif

#endif /* not TokenMessageArg_INCLUDED */
