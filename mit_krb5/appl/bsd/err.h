/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: err.h,v $
 * Revision 1.1.2.2  1996/10/03  14:38:33  arvind
 * 	Submit code review fixes
 * 	[1996/08/28  20:36 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/1]
 *
 * 	Remove unnecessary ifdefs - makes code harder to read.
 * 	[1996/05/30  21:17 UTC  mullan_s  /main/DCE_1.2.2/1]
 *
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:39 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/03  20:05 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/2]
 *
 * 	Port to HP-UX
 * 	[1996/05/03  20:05 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/2]
 *
 * Revision 1.1.2.1  1996/06/04  21:46:27  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:39 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 * 
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/03  20:05 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/2]
 * 
 * 	Port to HP-UX
 * 	[1996/05/03  20:05 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/2]
 * 
 * $EndLog$
 */
/*-
 * Copyright (c) 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)err.h	8.1 (Berkeley) 6/2/93
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#ifndef _ERR_H_
#define	_ERR_H_

#ifdef HAVE_STDARG_H
#include <stdarg.h>
void		err(int, const char *, ...);
void		errx(int, const char *, ...);
void		warn(const char *, ...);
void		warnx(const char *, ...);
#else
#include <varargs.h>
void		err(int, const char *, va_alist);
void		errx(int, const char *, va_alist);
void		warn(const char *, va_alist);
void		warnx(const char *, va_alist);
#endif
void		verr(int, const char *, va_list);
void		verrx(int, const char *, va_list);
void		vwarn(const char *, va_list);
void		vwarnx(const char *, va_list);

#endif /* !_ERR_H_ */
