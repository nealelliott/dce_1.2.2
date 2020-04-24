/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: hp_conv.h,v $
 * Revision 1.1.4.2  1996/02/18  22:55:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:16:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:53  root]
 * 
 * Revision 1.1.2.3  1994/06/10  20:54:19  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:05  devsrc]
 * 
 * Revision 1.1.2.2  1994/04/12  16:51:56  mori_m
 * 	CR 9701:  Second submission for RPC runtime I18N support.
 * 	[1994/04/12  16:39:18  mori_m]
 * 
 * Revision 1.1.2.1  1994/02/08  21:32:22  mori_m
 * 	CR 9701:  Initial submission for RPC runtime I18N support
 * 	[1994/02/08  21:31:44  mori_m]
 * 
 * $EndLog$
 */
/*
** HP codeset / iconv name mapping table.
** Following is the locale / codeset table for HP-UX.
**
**  Locale	        :  Code set name
**  ----------------------------------
**  C                   : ROMAN8
**  american            : roman8
**  american.iso88591   : iso88591
**  arabic              : arabic8
**  arabic.iso88596     : iso88596
**  arabic-w            : arabic8
**  bulgarian           : iso88595
**  c-french            : roman8
**  c-french.iso88591   : iso88591
**  chinese-s           : gb2312
**  chinese-t           : ccdc
**  chinese-t.big5      : big5
**  config              : big5
**  czech               : iso88592
**  danish              : roman8
**  danish.iso88591     : iso88591
**  dutch               : roman8
**  dutch.iso88591      : iso88591
**  english             : roman8
**  english.iso88591    : iso88591
**  finnish             : roman8
**  finnish.iso88591    : iso88591
**  french              : roman8
**  french.iso88591     : iso88591
**  german              : roman8
**  french.german       : iso88591
**  greek               : greek8
**  greek.iso88597      : iso88597
**  hebrew              : hebrew8
**  hebrew.iso88598     : iso88598
**  hungarian           : iso88592
**  icelandic           : roman8
**  icelandic.iso88591  : iso88591
**  italian             : roman8
**  italian.iso88591    : iso88591
**  japanese            : sjis
**  japanese.euc        : euc
**  korean              : ksc5601
**  norwegian           : roman8
**  norwegian.iso88591  : iso88591
**  polish              : iso88592
**  portuguese          : roman8
**  portuguese.iso88591 : iso88591
**  rumanian            : iso88592
**  russian             : iso88595
**  serbocroatian       : iso88592
**  slovene             : iso88592
**  spanish             : roman8
**  spanish.iso88591    : iso88591
**  swedish             : roman8
**  swedish.iso88591    : iso88591
**  thai                : tis620
**  turkish             : turkish8
**  turkish.iso88599    : iso88599
*/

#define		CODESET_ICONV_TABLE_ENTRY	20


typedef struct codeset_iconv_name_s_t {
	char	*codeset_name;
	char	*iconv_name;
} codeset_iconv_name_t;


/*
 * The table has to be sorted by codeset_name.  Binary search is used
 * to retrieve the iconv_name.
 */
static codeset_iconv_name_t	codeset_iconv_table[] = {
	{ "ROMAN8",	"roma8" },
	{ "arabic8",	"arab8" },
	{ "big5",	"big5" },
	{ "ccdc",	"roc15" },
	{ "euc",	"ujis" },
	{ "gb2312",	"ERROR" },	/* No iconv converters available */
	{ "greek8",	"gree8" },
	{ "hebrew8",	"hebr8" },
	{ "iso88591",	"iso81" },
	{ "iso88592",	"iso82" },
	{ "iso88595",	"iso85" },
	{ "iso88596",	"iso86" },
	{ "iso88597",	"iso87" },
	{ "iso88598",	"iso88" },
	{ "iso88599",	"iso89" },
	{ "ksc5601",	"kore5" },
	{ "roman8",	"roma8" },
	{ "sjis",	"japa5" },
	{ "tis620",	"thai8" },
	{ "turkish8",	"turk8" },
};

/*
** HP iconv name retreival routine.  HP needs this special routine
** because their code set name and iconv name does not match.
**/

INTERNAL void rpc__cs_get_iconv_name (
#ifdef IDL_PROTOTYPES
    /* [in] */	unsigned32		tag,
    /* [out] */	char			**iconv_name,
    /* [out] */	error_status_t		*status
#endif
);
