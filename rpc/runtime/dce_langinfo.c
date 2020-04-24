/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc
.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/* 
**  Copyright (c) 1994 by
**      Hewlett-Packard Company, Palo Alto, Ca. 
 */
/*
 * HISTORY
 * $Log: dce_langinfo.c,v $
 * Revision 1.1.2.1  1996/05/10  14:14:01  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	 	HP revision /main/DCE_1.2/1  1996/03/18  20:24 UTC  psn
 * 	 	CHFts17319 - setlocale() needs to be a pure query call to catch
 * 	 	application specified locale instances (as opposed to that which
 * 	 	is specified by the LANG env variable).
 * 	 	[1996/01/22  20:47 UTC  kline_s  /main/HPDCE02/kline_s_mothra2/1]
 *
 * 	 	merge
 * 	 	[1995/01/04  16:25 UTC  mothra  /main/HPDCE02/1]
 *
 * 	 	9.0 UX specific routine to be used in place of the libc nl_langinfo()
 * 	 	routine which incorrectly returns the current codeset in use.
 * 	 	[1994/12/22  21:35 UTC  kline_s  /main/kline_s_mothra_latest/1]
 * 	[1996/05/10  14:13:12  arvind]
 *
 * $EndLog$
 */
/* This routine is a temporary workaround for 9.0.x versions of the
 * libc routine nl_langinfo().  Prior to 10.0, nl_langinfo()
 * was not correctly implemented in accordance to XPG/3 standards.  
 * Instead of returning the correct codeset of the current locale in 
 * use, nl_langinfo(CODESET) always returns "roman8".
 *
 * As of 10.0, HP-UX has fixed this problem.  Hence at that time, this
 * routine may be obsoleted out of the DCE source tree. (Note: at that
 * time, all instances in the DCE code which use this should also be
 * similarly modified to use nl_langinfo().
 */

#include <langinfo.h>
#include <locale.h>
#include <limits.h> 
#include <string.h>
#include <stdlib.h>

char *dce_langinfo(nl_item langinfo_val)
{
    char *ret_str, *lang_ptr, *codeset_ptr, *end_ptr;
    int count;

    /* Obtain codeset information via queried locale string, using  
     * LC_CTYPE as specified in XPG/3.  Locale string returned is
     * of the format /:locale;/ where locale may be of the form
     * lang or lang.codeset. If the codeset information can not be 
     * explicitly obtained, it will be implicitly derived from the 
     * locale name.
     */

    ret_str = setlocale(LC_CTYPE, NULL);
    if (ret_str == NULL)
        return("roman8");  /* empty/bogus locale - use default */

    if ((lang_ptr = strchr(ret_str, (int)':')) == NULL)
        return("roman8");  /* bogus locale - use default */

    lang_ptr++;

    if ((codeset_ptr = strchr(lang_ptr, (int)'.')) == NULL) {
        /* need to derive codeset name implicitly from locale name */

        end_ptr = lang_ptr;
        while (*end_ptr != ';') { 
             end_ptr++;
        }
        count = end_ptr - lang_ptr;

        /* Iterate through the set of known possible locales available */

        if (strncmp("C", lang_ptr, count) == 0)
                return("roman8");
        if (strncmp("POSIX", lang_ptr, count) == 0)
                return("roman8");

        if ((strncmp("american", lang_ptr, count) == 0) ||
            (strncmp("c-french", lang_ptr, count) == 0) ||
            (strncmp("danish", lang_ptr, count) == 0) ||
            (strncmp("dutch", lang_ptr, count) == 0) ||
            (strncmp("english", lang_ptr, count) == 0) ||
            (strncmp("finnish", lang_ptr, count) == 0) ||
            (strncmp("french", lang_ptr, count) == 0) ||
            (strncmp("german", lang_ptr, count) == 0) ||
            (strncmp("italian", lang_ptr, count) == 0) ||
            (strncmp("norwegian", lang_ptr, count) == 0) ||
            (strncmp("portuguese", lang_ptr, count) == 0) ||
            (strncmp("spanish", lang_ptr, count) == 0) ||
            (strncmp("swedish", lang_ptr, count) == 0) ||
            (strncmp("icelandic", lang_ptr, count) == 0))
                return("roman8");

        if (strncmp("katakana", lang_ptr, count) == 0)
                return("kana8");
        if (strncmp("japanese", lang_ptr, count) == 0)
                return("japanese15");
        if (strncmp("korean", lang_ptr, count) == 0)
                return("korean15");
        if (strncmp("chinese-s", lang_ptr, count) == 0)
                return("chinese-s");
        if (strncmp("chinese-t", lang_ptr, count) == 0)
                return("roc15");

        if ((strncmp("arabic", lang_ptr, count) == 0) ||
            (strncmp("arabic-w", lang_ptr, count) == 0))
                return("arabic8");
        if (strncmp("greek", lang_ptr, count) == 0)
                return("greek8");
        if (strncmp("hebrew", lang_ptr, count) == 0)
                return("hebrew8");
        if (strncmp("turkish", lang_ptr, count) == 0)
                return("turkish8");
        if (strncmp("thai", lang_ptr, count) == 0)
                return("thai8");
        if ((strncmp("czech", lang_ptr, count) == 0) ||
            (strncmp("hungarian", lang_ptr, count) == 0) ||
            (strncmp("polish", lang_ptr, count) == 0) ||
            (strncmp("rumanian", lang_ptr, count) == 0) ||
            (strncmp("serbocroatian", lang_ptr, count) == 0) ||
            (strncmp("slovene", lang_ptr, count) == 0) ||
            (strncmp("", lang_ptr, count) == 0))
                return("iso88592");
        if ((strncmp("russian", lang_ptr, count) == 0) ||
            (strncmp("bulgarian", lang_ptr, count) == 0))
                return("iso88595");

        /* should never get this far... return default */
        return("roman8");
    }

    /* implied else - explicit codeset */
    codeset_ptr++;
    end_ptr = codeset_ptr;
    while (*end_ptr != ';') {
        end_ptr++;
    }
    count = end_ptr - codeset_ptr;

    /* Iterate through the set of known possible codesets available */

    if (strncmp("iso88591", codeset_ptr, count) == 0)
        return("iso88591");
    if (strncmp("iso88592", codeset_ptr, count) == 0)
        return("iso88592");
    if (strncmp("iso88593", codeset_ptr, count) == 0)
        return("iso88593");
    if (strncmp("iso88594", codeset_ptr, count) == 0)
        return("iso88594");
    if (strncmp("iso88595", codeset_ptr, count) == 0)
        return("iso88595");
    if (strncmp("iso88596", codeset_ptr, count) == 0)
        return("iso88596");
    if (strncmp("iso88597", codeset_ptr, count) == 0)
        return("iso88597");
    if (strncmp("iso88598", codeset_ptr, count) == 0)
        return("iso88598");
    if (strncmp("iso88599", codeset_ptr, count) == 0)
        return("iso88599");

    if ((strncmp("SJIS", codeset_ptr, count) == 0)  ||
            (strncmp("sjis", lang_ptr, count) == 0))
        return("japan15");
    if ((strncmp("eucJP", codeset_ptr, count) == 0) ||
            (strncmp("euc", lang_ptr, count) == 0))
        return("ujis");
    if (strncmp("eucKR", codeset_ptr, count) == 0)
        return("korean15");
    if (strncmp("eucCN", codeset_ptr, count) == 0)
        return("eucCN");
    if (strncmp("eucTW", codeset_ptr, count) == 0)
        return("eucTW");
    if (strncmp("big5", codeset_ptr, count) == 0)
        return("big5");
    if (strncmp("ccdc", codeset_ptr, count) == 0)
        return("roc15");


    if (strncmp("roman8", codeset_ptr, count) == 0)
        return("roman8");
    if (strncmp("kana8", codeset_ptr, count) == 0)
        return("kana8");
    if (strncmp("arabic8", codeset_ptr, count) == 0)
        return("arabic8");
    if (strncmp("greek8", codeset_ptr, count) == 0)
        return("greek8");
    if (strncmp("hebrew8", codeset_ptr, count) == 0)
        return("hebrew8");
    if (strncmp("turkish8", codeset_ptr, count) == 0)
        return("turkish8");
    if (strncmp("tis620", codeset_ptr, count) == 0)
        return("thai8");

    /* should never get this far... return default */
    return("roman8");
}
