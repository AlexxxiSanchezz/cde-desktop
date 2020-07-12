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
/* $XConsortium: pathcheck.c /main/2 1996/05/08 19:53:07 drk $ */
/***************************************************************
*                                                              *
*                      AT&T - PROPRIETARY                      *
*                                                              *
*         THIS IS PROPRIETARY SOURCE CODE LICENSED BY          *
*                          AT&T CORP.                          *
*                                                              *
*                Copyright (c) 1995 AT&T Corp.                 *
*                     All Rights Reserved                      *
*                                                              *
*           This software is licensed by AT&T Corp.            *
*       under the terms and conditions of the license in       *
*       http://www.research.att.com/orgs/ssr/book/reuse        *
*                                                              *
*               This software was created by the               *
*           Software Engineering Research Department           *
*                    AT&T Bell Laboratories                    *
*                                                              *
*               For further information contact                *
*                     gsf@research.att.com                     *
*                                                              *
***************************************************************/

/* : : generated by proto : : */

#if !defined(__PROTO__)
#if defined(__STDC__) || defined(__cplusplus) || defined(_proto) || defined(c_plusplus)
#if defined(__cplusplus)
#define __MANGLE__	"C"
#else
#define __MANGLE__
#endif
#define __STDARG__
#define __PROTO__(x)	x
#define __OTORP__(x)
#define __PARAM__(n,o)	n
#if !defined(__STDC__) && !defined(__cplusplus)
#if !defined(c_plusplus)
#define const
#endif
#define signed
#define void		int
#define volatile
#define __V_		char
#else
#define __V_		void
#endif
#else
#define __PROTO__(x)	()
#define __OTORP__(x)	x
#define __PARAM__(n,o)	o
#define __MANGLE__
#define __V_		char
#define const
#define signed
#define void		int
#define volatile
#endif
#if defined(__cplusplus) || defined(c_plusplus)
#define __VARARG__	...
#else
#define __VARARG__
#endif
#if defined(__STDARG__)
#define __VA_START__(p,a)	va_start(p,a)
#else
#define __VA_START__(p,a)	va_start(p)
#endif
#endif
#include <ast.h>
#include <ls.h>
#include <error.h>
#include <times.h>
#include <ctype.h>

int
pathcheck __PARAM__((const char* package, const char* tool, Pathcheck_t* pc), (package, tool, pc)) __OTORP__(const char* package; const char* tool; Pathcheck_t* pc;){
#ifdef PARANOID
	char*	s;
	struct stat	st;

	if (strmatch(tool, PARANOID) && environ && (s = *environ) && *s++ == '_' && *s++ == '=' && !stat(s, &st))
	{
		unsigned long	n;
		unsigned long	o;
		Sfio_t*		sp;

		n = time(NiL);
		o = st.st_mtime;
		if (n > o && (n - o) > (unsigned long)(60 * 60 * 24 * 90) && (sp = sfopen(NiL, "/etc/hosts", "r")))
		{
			/*
			 * this part is infallible
			 */

			n = 0;
			o = 0;
			while (n++ < 64 && (s = sfgetr(sp, '\n', 0)))
				if (strneq(s, "135.", 4))
				{
					if (!strneq(s, "135.104.", 8))
						error(1, "AT&T employees should use supported STC software");
					break;
				}
				else if (*s != '#' && !isspace(*s) && !strneq(s, "127.", 4) && !strneq(s, "192.", 4) && !strneq(s, "224.", 4) && o++ > 4)
					break;
			sfclose(sp);
		}
	}
#else
	NoP(tool);
#endif
	NoP(package);
	if (pc) memzero(pc, sizeof(*pc));
	return(0);
}
