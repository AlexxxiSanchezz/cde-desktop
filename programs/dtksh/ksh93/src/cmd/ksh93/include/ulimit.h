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
/* $XConsortium: ulimit.h /main/3 1995/11/01 16:44:27 rswiston $ */
/***************************************************************
*                                                              *
*                      AT&T - PROPRIETARY                      *
*                                                              *
*        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF        *
*                    AT&T BELL LABORATORIES                    *
*         AND IS NOT TO BE DISCLOSED OR USED EXCEPT IN         *
*            ACCORDANCE WITH APPLICABLE AGREEMENTS             *
*                                                              *
*                Copyright (c) 1995 AT&T Corp.                 *
*              Unpublished & Not for Publication               *
*                     All Rights Reserved                      *
*                                                              *
*       The copyright notice above does not evidence any       *
*      actual or intended publication of such source code      *
*                                                              *
*               This software was created by the               *
*           Advanced Software Technology Department            *
*                    AT&T Bell Laboratories                    *
*                                                              *
*               For further information contact                *
*                    {research,attmail}!dgk                    *
*                                                              *
***************************************************************/

/* : : generated by proto : : */
                  
#ifndef _ULIMIT_H
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

#define _ULIMIT_H 1
/*
 * This is for the ulimit built-in command
 */

#include	"shtable.h"
#include	"FEATURE/rlimits"
#if defined(_sys_resource) && defined(_lib_getrlimit)
#   include	"FEATURE/time"
#   include	<sys/resource.h>
#   if !defined(RLIMIT_FSIZE) && defined(_sys_vlimit)
	/* This handles hp/ux problem */ 
#	include	<sys/vlimit.h>
#	define RLIMIT_FSIZE	(LIM_FSIZE-1)
#	define RLIMIT_DATA	(LIM_DATA-1)
#	define RLIMIT_STACK	(LIM_STACK-1)
#	define RLIMIT_CORE	(LIM_CORE-1)
#	define RLIMIT_CPU	(LIM_CPU-1)
#	ifdef LIM_MAXRSS
#		define RLIMIT_RSS       (LIM_MAXRSS-1)
#	endif /* LIM_MAXRSS */
#   endif
#   undef _lib_ulimit
#else
#   ifdef _sys_vlimit
#	include	<sys/vlimit.h>
#	undef _lib_ulimit
#	define RLIMIT_FSIZE	LIM_FSIZE
#	define RLIMIT_DATA	LIM_DATA
#	define RLIMIT_STACK	LIM_STACK
#	define RLIMIT_CORE	LIM_CORE
#	define RLIMIT_CPU	LIM_CPU
#	ifdef LIM_MAXRSS
#		define RLIMIT_RSS       LIM_MAXRSS
#	endif /* LIM_MAXRSS */
#   else
#	ifdef _lib_ulimit
#	    define vlimit ulimit
#	endif /* _lib_ulimit */
#   endif /* _lib_vlimit */
#endif

#ifdef RLIM_INFINITY
#   define INFINITY	RLIM_INFINITY
#else
#   ifndef INFINITY
#	define INFINITY	-1L
#   endif /* INFINITY */
#endif /* RLIM_INFINITY */

#if defined(_lib_getrlimit) || defined(_lib_vlimit) || defined(_lib_ulimit)
#   ifndef RLIMIT_CPU
#	define RLIMIT_CPU	0
#   endif /* !RLIMIT_CPU */
#   ifndef RLIMIT_DATA
#	define RLIMIT_DATA	0
#   endif /* !RLIMIT_DATA */
#   ifndef RLIMIT_RSS
#	define RLIMIT_RSS	0
#   endif /* !RLIMIT_RSS */
#   ifndef RLIMIT_STACK
#	define RLIMIT_STACK	0
#   endif /* !RLIMIT_STACK */
#   ifndef RLIMIT_CORE
#	define RLIMIT_CORE	0
#   endif /* !RLIMIT_CORE */
#   ifndef RLIMIT_VMEM
#	define RLIMIT_VMEM	0
#   endif /* !RLIMIT_VMEM */
#   ifndef RLIMIT_NOFILE
#	define RLIMIT_NOFILE	0
#   endif /* !RLIMIT_NOFILE */
#else
#   define _no_ulimit
#endif
extern __MANGLE__ const char		e_unlimited[];

#endif /* _ULIMIT_H */