/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sams.c,v $
 * Revision 1.1.11.4  1996/12/14  19:03:26  rsalz
 * 	Fix OT CR 13683
 * 	[1996/12/14  18:51:51  rsalz]
 *
 * Revision 1.1.11.3  1996/12/05  08:54:41  rsalz
 * 	Write PDG output in SGML, not SML.
 * 	[1996/12/02  19:23:56  rsalz]
 * 
 * Revision 1.1.11.2  1996/10/03  15:17:58  arvind
 * 	Conditionalize XPG fix for field width specifier so that it is not applied to AIX to avoid problems caused
 * 	by AIX's lack of support for it.
 * 	[1996/08/13  15:46 UTC  psn  /main/psn_bugfix/4]
 * 
 * 	Try backing out XPG4 fix.
 * 	[1996/07/31  20:15 UTC  psn  /main/psn_bugfix/3]
 * 
 * Revision 1.1.11.1  1996/08/09  12:16:58  arvind
 * 	Try backing out XPG4 fix.
 * 	[1996/07/19  14:43 UTC  psn  /main/psn_bugfix/2]
 * 
 * 	Fix OT 12826.
 * 	[1996/07/17  19:23 UTC  psn  /main/psn_bugfix/1]
 * 
 * 	Fix OT 12806.
 * 	[1996/02/17  23:36:06  marty  1.1.9.2]
 * 
 * Revision 1.1.9.2  1996/02/17  23:36:06  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:52:21  marty]
 * 
 * Revision 1.1.9.1  1995/12/08  20:55:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:34  root]
 * 
 * Revision 1.1.4.26  1994/09/30  07:56:05  rsalz
 * 	Implement DCE RFC 24.2 (OT CR 11929)
 * 	[1994/09/30  07:44:17  rsalz]
 * 
 * Revision 1.1.4.25  1994/09/26  16:38:36  bowe
 * 	Fix "smallest" in writing dceXXXmsg.h file. [CR 12336]
 * 	[1994/09/26  16:38:27  bowe]
 * 
 * Revision 1.1.4.24  1994/09/16  20:47:19  bowe
 * 	Formatting improvements (mostly to tighten up the spacing).
 * 	Handle \*'s correctly (was making them \e). [CR 11977]
 * 	[1994/09/16  20:41:58  bowe]
 * 
 * Revision 1.1.4.23  1994/08/26  17:54:22  bowe
 * 	Exit if unlink() fails.  [CR 11584]
 * 	[1994/08/26  17:52:23  bowe]
 * 
 * Revision 1.1.4.22  1994/08/26  12:15:03  bowe
 * 	Check the return value of unlink() (from last change).  [CR 11584]
 * 	[1994/08/26  12:14:32  bowe]
 * 
 * Revision 1.1.4.21  1994/08/25  17:52:27  bowe
 * 	Before a writing .cat file, unlink it.  [CR 11584]
 * 	[1994/08/25  17:34:35  bowe]
 * 
 * Revision 1.1.4.20  1994/07/25  18:56:20  bowe
 * 	Change \" to " and \ to \e when writing troff) files.  Renamed output
 * 	file from xxx.sgm to xxx.sml, since that's what really is.  [CR 11258]
 * 	[1994/07/25  18:53:33  bowe]
 * 
 * Revision 1.1.4.19  1994/07/25  15:26:44  bowe
 * 	Check both 's' & 'S' for calling ValidateSvcRecords() [CR 10914]
 * 	[1994/07/25  15:26:03  bowe]
 * 
 * Revision 1.1.4.18  1994/07/12  18:31:56  rsalz
 * 	Replace \" with " in PDG text field output (OT CR 11258).
 * 	[1994/07/12  18:29:56  rsalz]
 * 
 * Revision 1.1.4.17  1994/06/10  20:53:22  devsrc
 * 	cr10872- fix copyright
 * 	[1994/06/10  16:38:09  devsrc]
 * 
 * Revision 1.1.4.16  1994/05/23  23:13:29  sommerfeld
 * 	back out change on message catalog.
 * 	[1994/05/23  19:29:25  sommerfeld]
 * 
 * Revision 1.1.4.15  1994/05/20  22:03:41  sommerfeld
 * 	special case message zero for message catalogs.
 * 	[1994/05/20  21:42:22  sommerfeld]
 * 
 * 	Fix for clearcase clean build environment:
 * 	- generate one output file at a time
 * 	- allow cleaner flags parsing.
 * 	[1994/05/17  19:12:22  sommerfeld]
 * 
 * Revision 1.1.4.14  1994/05/10  18:40:26  rsalz
 * 	Fix filenames if -m used (OT CR 10453).
 * 	[1994/05/10  18:39:48  rsalz]
 * 
 * Revision 1.1.4.13  1994/05/05  15:21:15  rsalz
 * 	XPG/4 syntax is origin-1, not origin-0 (OT CR 10544)
 * 	[1994/05/05  15:18:43  rsalz]
 * 
 * Revision 1.1.4.12  1994/04/27  18:53:26  rsalz
 * 	Tweak output format of -ox (OT CR 10453).
 * 	[1994/04/27  18:52:18  rsalz]
 * 
 * Revision 1.1.4.11  1994/04/25  19:05:04  rsalz
 * 	Add -m and x to -o flag (OT CR 10453).
 * 	[1994/04/25  18:39:08  rsalz]
 * 
 * Revision 1.1.4.10  1994/04/22  16:23:38  rsalz
 * 	Did not understand %u printf format (OT CR 10441).
 * 	[1994/04/22  16:23:23  rsalz]
 * 
 * Revision 1.1.4.9  1994/04/17  22:39:10  rsalz
 * 	Check number of % sequences for XPG4 (OT CR 10337)
 * 	[1994/04/17  22:38:47  rsalz]
 * 
 * Revision 1.1.4.8  1994/03/18  17:15:43  rsalz
 * 	Error if try to generate .cat file and not .msg file (OT CR 10162).
 * 	[1994/03/18  17:10:43  rsalz]
 * 
 * Revision 1.1.4.7  1994/03/15  16:34:38  rsalz
 * 	Have msg.h include IDL header file if -i used (OT CR 10138).
 * 	[1994/03/15  16:27:44  rsalz]
 * 
 * Revision 1.1.4.6  1994/03/11  21:09:21  rsalz
 * 	Add -f flag.
 * 	Fix segfault (was freeing wrong pointer) (OT CR 10122).
 * 	[1994/03/11  14:14:18  rsalz]
 * 
 * Revision 1.1.4.5  1994/03/10  21:09:27  rsalz
 * 	Merged with changes from 1.1.4.4
 * 	[1994/03/10  21:09:14  rsalz]
 * 
 * 	Add collections and conditional text (OT CR 9697).
 * 	[1994/03/10  20:45:45  rsalz]
 * 
 * Revision 1.1.4.4  1994/03/08  19:01:38  sekhar
 * 	[OT 10092]  don't put local attribute in IDL files.
 * 	[1994/03/08  18:27:00  sekhar]
 * 
 * Revision 1.1.4.3  1994/02/09  17:21:11  rsalz
 * 	Output "" not 0 for no-arg convenience macros (OT CR 9913).
 * 	[1994/02/09  17:19:17  rsalz]
 * 
 * Revision 1.1.4.2  1994/02/08  20:25:37  rsalz
 * 	Add trailing \n to message catalog notes (OT CR 9894).
 * 	[1994/02/07  15:57:45  rsalz]
 * 
 * Revision 1.1.4.1  1993/12/20  17:00:23  rsalz
 * 	Write PDG file in SML, not SGML.
 * 	[1993/12/20  17:00:04  rsalz]
 * 
 * Revision 1.1.2.6  1993/11/30  16:55:05  rsalz
 * 	Put all values in msg.h, even if using tables.
 * 	[1993/11/30  16:54:51  rsalz]
 * 
 * Revision 1.1.2.5  1993/11/30  16:32:57  rsalz
 * 	Put MessageID's directly into svc.c file so that it doesn't
 * 	need the msg.h file (which causes problems when using tables).
 * 	[1993/11/30  16:32:38  rsalz]
 * 
 * Revision 1.1.2.4  1993/11/23  20:23:51  rsalz
 * 	Dont use XPG %1$ notation in .msg files unless there
 * 	is more than one %-format in the input.
 * 	[1993/11/23  20:23:33  rsalz]
 * 
 * Revision 1.1.2.3  1993/11/16  18:03:39  rsalz
 * 	Add -i flag to build an IDL file.
 * 	[1993/11/16  18:03:29  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  17:41:50  rsalz
 * 	Initial release
 * 	[1993/08/16  17:41:04  rsalz]
 * 
 * $EndLog$
 */
#define EXTERN	/* NULL */
#include <sams.h>
#include <unistd.h>
#include <time.h>


#define WANT(c)		(output == NULL || strchr(output, c) != NULL)


int		CollectionSize = 100;	/* Default collection size	*/
int		FilterText = 1;

static int	CodeMask;		/* Space for the actual code	*/
static int	Multiples;		/* One file/message?		*/
static int	SortMode = 'a';		/* Sorting mode			*/
static char	*Directory;		/* Destination directory	*/
static char	*Gencat = "gencat";	/* Generating catalogs		*/
static char	*Input;			/* Input file name		*/
static char	*IDL;			/* Name of generated IDL file	*/
static char	*Name;			/* Base output file name	*/
static char	*Now;			/* Current time string		*/


#if	defined(NEED_STRDUP)
/*
**  Return an allocated copy of a string.
*/
char *
strdup(
    const char		*p
)
{
    return strcpy(malloc(strlen(p) + 1), p);
}
#endif	/* defined(NEED_STRDUP) */


/*
**  Qsort predicate to sort message records into alphabetic order by name.
*/
static int
alphasort(
    const void		*p1,
    const void		*p2
)
{
    const record_t	*r1;
    const record_t	*r2;

    r1 = (const record_t *)p1;
    r2 = (const record_t *)p2;
    return strcmp(r1->Code, r2->Code);
}


/*
**  Qsort predicate to sort message records into numeric order by value.
*/
int
numbasort(
    const void		*p1,
    const void		*p2
)
{
    const record_t	*r1;
    const record_t	*r2;

    r1 = (const record_t *)p1;
    r2 = (const record_t *)p2;
    return r1->Number - r2->Number;
}


/*
**  Qsort predicate to sort message records into alphabetic order by text.
*/
static int
textsort(
    const void		*p1,
    const void		*p2
)
{
    const record_t	*r1;
    const record_t	*r2;

    r1 = (const record_t *)p1;
    r2 = (const record_t *)p2;
    return strcmp(r1->DocText, r2->DocText);
}

/*
**  Build C Pre-process multi-#include-prevention predicate name.
**  (Whew!)
*/
static void
buildcpp(
    char		*buff,
    const char		*prolog
)
{
    char		*p;

    (void)sprintf(buff, "_%s_%s%s_", TechHeader, Name, prolog);
    for (p = buff; *p; p++)
	if (islower(*p))
	    *p = toupper(*p);
}


/*
**  Build filename.
*/
static void
buildname(
    char		*buff,
    const char		*extension,
    const int		shortname
)
{
    if (Directory)
	(void)sprintf(buff, "%s/%s%s%s",
	    Directory,
	    Name, shortname ? "" : "msg", extension);
    else
	(void)sprintf(buff, "%s%s%s",
	    Name, shortname ? "" : "msg", extension);
}


/*
**  Build filename and open file for writing.  Exit on failure.
*/
static FILE *
xfopen(
    const char		*extension,
    const int		shortname
)
{
    char		buff[FNBUF];
    FILE		*F;

    buildname(buff, extension, shortname);
    if (unlink(buff) < 0 && errno != ENOENT)
	(void)fprintf(stderr, "Warning: Can't remove %s, %s\n",
		buff, strerror(errno));
    if ((F = fopen(buff, "w")) == NULL) {
	(void)fprintf(stderr, "Can't open %s for output, %s.\n",
		buff, strerror(errno));
	exit(1);
    }
    return F;
}


/*
**  Close an open file or die trying.
*/
static void
xfclose(
    FILE		*F,
    const char		*extension
)
{
    if (ferror(F) || fclose(F) == EOF) {
	(void)fprintf(stderr, "Can't close \"%s\" file, %s.\n",
		extension, strerror(errno));
	exit(1);
    }
}


/*
**  Write the message catalog file.
*/
static void
WriteMsgFile(
    void
)
{
    FILE		*F;
    record_t		*rp;
    char		*p;
    int			i;
    int			d;
    int			needxpg;

    F = xfopen(".msg", TRUE);
    (void)fprintf(F, "$ Generated from %s on %s\n", Input, Now);
    (void)fprintf(F, "$ Do not edit!\n");

    for (i = RecCount, rp = Records; --i >= 0; rp++) {
	if (MT_SKIP(rp->Flags) || (rp->Flags & MTincatalog) == 0)
	    continue;
	(void)fprintf(F, "%lu ", rp->Number & CodeMask);
	for (d = 0, p = rp->MesgText; *p; p++)
	    if (*p == '%' && p[1] != '%')
		d++;
	for (needxpg = d > 1, d = 0, p = rp->MesgText; *p; p++)
	    switch (*p) {
	    default:
		putc(*p, F);
		break;
	    case '%':
		putc('%', F);
		if (p[1] == '%') {
		    p++;
		    putc('%', F);
		}
		else if (needxpg)
		    fprintf(F, "%d$", ++d);
		break;
#ifndef AIX32
            /*
             * This code converts the format string pattern ".*s"
             * (in .sams file) to "%1$.*2$s". This would then require
             * corresponding arguments "string,string_width" in printf like
             * commands. It is assummed that each such occurance of
             * ".*s" is paired with arguments "string,string_width"
             *
             * Since AIX 3.2.x is missing support for this its excluded
             * from the fix.
             */
	    case '.':
		putc('.', F);
		if (p[1] == '*') {
		    p++;
		    putc('*', F);
		    if (needxpg)
		        fprintf(F, "%d$", ++d);
		}
		break;
#endif /* AIX32 */
	    case '\n':
		putc('\\', F);
		putc('\n', F);
		break;
	    }
	(void)fprintf(F, "\n");
	if (rp->Notes) {
	    putc('$', F);
	    putc(' ', F);
	    for (p = rp->Notes; *p; p++) {
		putc(*p, F);
		if (*p == '\n') {
		    putc('$', F);
		    putc(' ', F);
		}
	    }
	    putc('\n', F);
	}
    }

    xfclose(F, "msg");
}

static void
WriteCatFile(
    void
)
{
    char		msgfile[FNBUF];
    char		catfile[FNBUF];
    char		command[FNBUF];

    int i;
    
    buildname(msgfile, ".msg", TRUE);
    buildname(catfile, ".cat", TRUE);
    if (unlink(catfile) < 0 && errno != ENOENT) {
	perror("Cannot remove old message catalog");
	exit(1);
    }
    (void)sprintf(command, "%s %s %s", Gencat, catfile, msgfile);
    i = system(command);
    if (i)
	(void)fprintf(stderr, "Warning:  %s exit status %d\n", Gencat, i);
}


/*
**  Write the serviceability table header file and table file.
*/
static void
WriteSvcHdr(
    void
)
{
    FILE		*F;
    svcfield_t		*sp;
    int			i;
    char		cpp[SMBUF];

    buildcpp(cpp, "svc");
    F = xfopen("svc.h", TRUE);
    (void)fprintf(F, "/* Generated from %s on %s */\n", Input, Now);
    (void)fprintf(F, "/* Do not edit! */\n");
    (void)fprintf(F, "#if\t!defined(%s)\n", cpp);
    (void)fprintf(F, "#define %s\n", cpp);

    for (sp = Service, i = 0; i < SvcCount; i++, sp++)
	(void)fprintf(F, "#define %-31s %d\n", sp->Subcomp, i);
    (void)fprintf(F, "\n");
    (void)fprintf(F, "#if\tdefined(_DCE_SVC_H)\n");
    (void)fprintf(F, "extern dce_svc_subcomp_t\t%s[%d];\n",
	    ServiceTable, SvcCount + 1);
    (void)fprintf(F, "extern dce_svc_handle_t\t\t%s;\n", ServiceHandle);
    (void)fprintf(F, "#endif\t/* defined(_DCE_SVC_H) */\n");
    (void)fprintf(F, "#endif\t/* !defined(%s) */\n", cpp);
    xfclose(F, "svc.h");
}

static void
WriteSvcSrc(
    void
)
{
    FILE		*F;
    svcfield_t		*sp;
    int			i;
    char		cpp[SMBUF];

    buildcpp(cpp, "svc");
    F = xfopen("svc.c", TRUE);
    (void)fprintf(F, "/* Generated from %s on %s */\n", Input, Now);
    (void)fprintf(F, "/* Do not edit! */\n");
    (void)fprintf(F, "#include <dce/dce.h>\n");
    (void)fprintf(F, "dce_svc_subcomp_t %s[%d] = {\n",
	    ServiceTable, SvcCount + 1);
    for (sp = Service, i = SvcCount; --i >= 0; sp++) {
	(void)fprintf(F, "    /* %s */\n", sp->Subcomp);
	(void)fprintf(F, "    { (idl_char *)\"%s\",\n", sp->sc_name);
	(void)fprintf(F, "\t(idl_char *)\"%s\",\n", sp->sc_descr);
	(void)fprintf(F, "\t0x%8.8lx /* %s */, svc_c_debug_off },\n\n",
	    sp->Number, sp->Code);
    }
    (void)fprintf(F, "    { 0 }\n");
    (void)fprintf(F, "};\n");
    xfclose(F, "svc.c");
}


/*
**  Parse a printf format and return the minimal string that will
**  specify all the arguments.  Returns pointer to private space that
**  will be re-used on subsequent calls.
*/
static char *
StripArgs(
    char		*in,
    char		*Code
)
{
    static int		size;
    static char		*base;
    char		*dest;
    int			i;
    int			done;

    /* Get return buffer, worst-case size. */
    i = strlen(in);
    if (size == 0) {
	size = i + 1;
	base = malloc(size);
    }
    else if (size < i) {
	size = i + 1;
	base = realloc(base, size);
    }
    dest = base;

    /* The flow of control here is not obvious. */
    while ((in = strchr(in, '%')) != NULL) {
	if (dest == base)
	    *dest++ = '"';
	for (*dest++ = '%', done = FALSE; !done; )
	    switch (*++in) {
	    case '\0':
		(void)fprintf(stderr, "Bad format string for %s\n", Code);
		exit(1);
	    case 'b':		/* Special BINFILE extension. */
	    case 'd': case 'o': case 'x':
	    case 'f':
	    case 'c':
	    case 's':
	    case 'u':
		done = TRUE;
		/* FALLTHROUGH */
	    case 'l':
		*dest++ = *in;
		break;
	    }
    }
    if (dest == base)
	return "\"\"";
    *dest++ = '"';
    *dest = '\0';
    return base;
}


/*
**  Write the serviceabilty convenience macros header file.
*/
static void
WriteUtilFile(
    void
)
{
    FILE		*F;
    record_t		*rp;
    int			i;
    char		*p;
    char		buff[SMBUF];
    char		cpp[SMBUF];

    buildcpp(cpp, "mac");
    F = xfopen("mac.h", TRUE);
    (void)fprintf(F, "/* Generated from %s on %s */\n", Input, Now);
    (void)fprintf(F, "/* Do not edit! */\n");
    (void)fprintf(F, "#if\t!defined(%s)\n", cpp);
    (void)fprintf(F, "#define %s\n", cpp);

    for (i = RecCount, rp = Records; --i >= 0; rp++) {
	if (MT_SKIP(rp->Flags) || rp->Attributes == NULL)
	    continue;
	(void)sprintf(buff, "%s_MSG", rp->Code);
	for (p = buff; *p; p++)
	    if (islower(*p))
		*p = toupper(*p);
	(void)fprintf(F, "#define %s \\\n", buff);
	(void)fprintf(F, "\tDCE_SVC(%s, %s), %s, \\\n",
		ServiceHandle, StripArgs(rp->MesgText, rp->Code), rp->Subcomp);
	(void)fprintf(F, "\t%s, %s\n", rp->Attributes, rp->Code);
    }
    (void)fprintf(F, "#endif\t/* !defined(%s) */\n", cpp);
    xfclose(F, "mac.h");
}


/*
**  Write a troff string - change \" to just ", and \ to \e.
*/
static void
print_troff_string(
    FILE	*F,
    char	*p,
    int		add_newline
)
{
    while (*p) {
	if (*p == '\\') {
	    p++;
	    if (*p != '"') {
		/* backslash to backslash-e */
		putc('\\', F);
		if (*p != '*')
		    putc('e', F);
	    }
	}
	if (*p) {
	    putc(*p, F);
	    p++;
	}
    }
    if (add_newline)
	putc('\n', F);
}

/*
**  Write the manpage subset file.
*/

static void
WriteManFile(
    void
)
{
    FILE		*F;
    record_t		*rp;
    int			i;
    char		*p;

    F = xfopen(".man", FALSE);
    (void)fprintf(F, "...\\\" Generated from %s on %s\n", Input, Now);
    (void)fprintf(F, "...\\\" Do not edit!\n");

    for (i = RecCount, rp = Records; --i >= 0; rp++) {
	if (MT_SKIP(rp->Flags) || (rp->Flags & MTnodoc) != 0)
	    continue;
	(void)fprintf(F, ".TP\n");
	(void)fprintf(F, "%s (0x%8.8lx)\n", rp->Code, (long)rp->Number);
	print_troff_string(F, rp->DocText, 1);
	(void)fprintf(F, ".br\n");
	print_troff_string(F, rp->Explanation, 1);
    }

    xfclose(F, "man");
}


/*
**  Write the DCE message table file.
*/
static void
WriteTableFile(
    int			count
)
{
    FILE		*F;
    record_t		*rp;
    char		*p;
    int			i;
    int			dirty;

    F = xfopen(".c", FALSE);
    (void)fprintf(F, "/* Generated from %s on %s */\n", Input, Now);
    (void)fprintf(F, "/* Do not edit! */\n");
    (void)fprintf(F, "#include <dce/dce.h>\n");
    (void)fprintf(F, "#include <dce/dce_msg.h>\n");

    if (count) {
	(void)fprintf(F, "dce_msg_table_t %s[%d] = {\n", TableHeader, count);
	for (dirty = 0, i = RecCount, rp = Records; --i >= 0; rp++) {
	    if (MT_SKIP(rp->Flags) || (rp->Flags & MTintable) == 0)
		continue;
	    if (dirty)
		(void)fprintf(F, ",\n");
	    else
		dirty = 1;
	    (void)fprintf(F, "    { 0x%8.8lx, \"", (long)rp->Number);
	    for (p = rp->MesgText; *p; p++) {
		if (*p == '\n')
		    putc('\\', F);
		putc(*p, F);
	    }
	    (void)fprintf(F, "\" }");
	}
	(void)fprintf(F, "\n};\n");
    }

    xfclose(F, "c");
}


/*
**  Parse an "attributes" string and map it into a standard severity.
*/
static char *
AttributesToSeverity(
    char		*attrib
)
{
    if (attrib == NULL)
	return "Varies";
    if (strstr(attrib, "svc_c_sev_fatal") != NULL)
	return "Fatal";
    if (strstr(attrib, "svc_c_sev_error") != NULL)
	return "Error";
    if (strstr(attrib, "svc_c_sev_warning") != NULL)
	return "Warning";
    if (strstr(attrib, "svc_c_sev_notice") != NULL)
	return "Notice";
    if (strstr(attrib, "svc_c_sev_notice_verbose") != NULL)
	return "Verbose notice";
    return "Unknown";
}

/*
**  Write an SGML string - various changes
*/
#define	_(c)	(putc((c), F))

static void
print_sgml_string(
    FILE	*F,
    char	*p
)
{
    for ( ; *p; p++)
	switch (*p) {
	default:
	    putc(*p, F);
	    break;
	case '&':
	    _('&'); _('A'); _('M'); _('P'); _(';');		break;
	case '<':
	    _('&'); _('L'); _('T'); _(';');			break;
	case '>':
	    _('&'); _('G'); _('T'); _(';');			break;
	case '\\':
	    if (p[1] == '*')
		putc(*p, F);
	    else {
		/* "Backward solidus" -- who makes up these terms? */
		_('&'); _('B'); _('S'); _('O'); _('L'); _(';');
	    }
	    break;
	}
    _('\n');
}
#undef	_

/*
**  Write the Problem Determination Guide file.
*/

static void
WritePDGFile(
    void
)
{
    FILE		*F;
    record_t		*rp;
    int			i;
    char		docfile[FNBUF];
    char		*p;

    if (!Multiples) {
	F = xfopen(".sgml", FALSE);
	(void)fprintf(F, "<!--  Generated from %s on %s -->\n", Input, Now);
	(void)fprintf(F, "<!--  Do not edit! -->\n");
    }

    for (i = RecCount, rp = Records; --i >= 0; rp++) {
	if (MT_SKIP(rp->Flags) || (rp->Flags & MTnodoc) != 0)
	    continue;
	if (Multiples) {
	    if (Directory)
		(void)sprintf(docfile, "%s/%s", Directory, rp->Code);
	    else
		(void)strcpy(docfile, rp->Code);
	    if ((F = fopen(docfile, "w")) == NULL) {
		(void)fprintf(stderr, "Can't open %s for output, %s.\n",
			docfile, strerror(errno));
		exit(1);
	    }
	    (void)fprintf(F, "<!--  Generated from %s on %s -->\n", Input, Now);
	    (void)fprintf(F, "<!--  Do not edit! -->\n");
	}
	(void)fprintf(F, "<!-- -->\n");
	(void)fprintf(F, "<MsgEntry>\n");
	(void)fprintf(F, "<Msg><Title>0x%8.8lx</Title>\n", (long)rp->Number);
	(void)fprintf(F, "<MsgMain><Title>%s</Title>\n", rp->Code);
	(void)fprintf(F, "<MsgText><para>\n");
	print_sgml_string(F, rp->DocText);
	(void)fprintf(F, "</para></MsgText>\n");
	(void)fprintf(F, "</MsgMain>\n");
	(void)fprintf(F, "</Msg>\n");
	(void)fprintf(F, "<!-- -->\n");
	(void)fprintf(F, "<MsgInfo>\n");
	(void)fprintf(F, "<MsgLevel>%s</MsgLevel>\n",
	    AttributesToSeverity(rp->Attributes));
	if (rp->Subcomp)
	    (void)fprintf(F, "<MsgOrig>%s/%s</MsgOrig>\n",
		    CodeHeader ? "n/a" : CompHeader, rp->Subcomp);
	else
	    (void)fprintf(F, "<MsgOrig>%s</MsgOrig>\n",
		    CodeHeader ? "n/a" : CompHeader);
	(void)fprintf(F, "</MsgInfo>\n");
	(void)fprintf(F, "<!-- -->\n");
	(void)fprintf(F, "<MsgExplan><Title>Explanation:</Title>\n");
	(void)fprintf(F, "<para>\n");
	print_sgml_string(F, rp->Explanation);
	(void)fprintf(F, "</para></MsgExplan>\n");
	(void)fprintf(F, "<MsgExplan><Title>Action:</Title>\n");
	(void)fprintf(F, "<para>\n");
	print_sgml_string(F, rp->Action);
	(void)fprintf(F, "</para></MsgExplan>\n");
	(void)fprintf(F, "</MsgEntry>\n");
	(void)fprintf(F, "<!-- -->\n\n");

	if (Multiples)
	    xfclose(F, rp->Code);
    }

    if (!Multiples)
	xfclose(F, "sgml");
}


/*
**  Write the three-field index file.
*/
static void
WriteIDXFile(
    void
)
{
    FILE		*F;
    record_t		*rp;
    int			i;
    char		*p;

    F = xfopen(".idx", FALSE);
    for (i = RecCount, rp = Records; --i >= 0; rp++) {
	if (MT_SKIP(rp->Flags) || (rp->Flags & MTnodoc) != 0)
	    continue;
	fprintf(F, "%d@%s@0x%8.8lx@", rp->Code, rp->Code, (long)rp->Number);
	for (p = rp->MesgText; *p; p++)
	    if (*p != '\\')
		putc(*p, F);
	    else if (*++p == '\n')
		putc(' ', F);
	    else
		putc(*p, F);
	putc('\n', F);
    }
    xfclose(F, "idx");
}


/*
**  Write an IDL input file.
*/
static void
WriteIDLFile(
    void
)
{
    FILE		*F;
    record_t		*rp;
    int			i;
    char		buff[FNBUF];

    /* Open IDL file. */
    if (Directory)
	(void)sprintf(buff, "%s/%s.idl", Directory, IDL);
    else
	(void)sprintf(buff, "%s.idl", IDL);
    if (unlink(buff) < 0 && errno != ENOENT)
	(void)fprintf(stderr, "Warning: Can't remove %s, %s\n",
		buff, strerror(errno));
    if ((F = fopen(buff, "w")) == NULL) {
	(void)fprintf(stderr, "Can't open %s for output, %s.\n",
		buff, strerror(errno));
	exit(1);
    }
    (void)fprintf(F, "/* Generated from %s on %s */\n", Input, Now);
    (void)fprintf(F, "/* Do not edit! */\n");
    (void)fprintf(F, "interface %s\n{\n", IDL);
    for (i = RecCount, rp = Records; --i >= 0; rp++) {
	if (rp->Flags == MTsetvalue)
	    continue;
	if (MT_SKIP(rp->Flags))
	    continue;
	if (Table != NULL && (rp->Flags & MTintable) == 0)
	    continue;
	(void)fprintf(F, "    const unsigned32 %-31s = 0x%8.8lx;\n",
		rp->Code, (long)rp->Number);
    }
    (void)fprintf(F, "}\n");

    xfclose(F, "h");
}


/*
**  Write the DCE Message identifier header file.
*/
static void
WriteHeaderFile(
    int			count
)
{
    FILE		*F;
    record_t		*rp;
    MSGID		biggest;
    MSGID		smallest;
    int			havebig;
    int			havesmall;
    int			i;
    char		buff[SMBUF];
    char		cpp[SMBUF];

    buildcpp(cpp, "msg");
    F = xfopen(".h", FALSE);
    (void)fprintf(F, "/* Generated from %s on %s */\n", Input, Now);
    (void)fprintf(F, "/* Do not edit! */\n");
    (void)fprintf(F, "#if\t!defined(%s)\n", cpp);
    (void)fprintf(F, "#define %s\n", cpp);
    if (IDL != NULL)
	(void)fprintf(F, "#include <dce/%s.h>\n\n", IDL);

    havebig = havesmall = 0;
    for (i = RecCount, rp = Records; --i >= 0; rp++) {
	if (rp->Flags == MTsetvalue)
	    continue;
	if (rp->Flags != MTsetvalue && rp->Flags != MTcollect) {
	    if (!havebig || rp->Number > biggest) {
		biggest = rp->Number;
		havebig = 1;
	    }
	    if (!havesmall || rp->Number < smallest) {
		smallest = rp->Number;
		havesmall = 1;
	    }
	}
	if (MT_SKIP(rp->Flags) || IDL != NULL)
	    continue;
#if	0
	if (Table != NULL && (rp->Flags & MTintable) == 0)
	    continue;
#endif
	(void)fprintf(F, "#define %-31s 0x%8.8lx\n",
		rp->Code, (long)rp->Number);
    }

    (void)fprintf(F, "\n");
    (void)sprintf(buff, "smallest_%s_message_id", CompHeader);
    (void)fprintf(F, "#define %-31s 0x%8.8lx\n", buff, (long)smallest);
    (void)sprintf(buff, "biggest_%s_message_id", CompHeader);
    (void)fprintf(F, "#define %-31s 0x%8.8lx\n", buff, (long)biggest);
    (void)fprintf(F, "\n");
    if (count) {
	(void)fprintf(F, "#if\tdefined(_DCE_MSG_H)\n");
	(void)fprintf(F, "extern dce_msg_table_t %s[%d];\n",
		TableHeader, count);
	(void)fprintf(F, "#endif\t/* defined(_DCE_MSG_H) */\n");
    }
    (void)fprintf(F, "#endif\t/* !defined(%s) */\n", cpp);
    xfclose(F, "h");
}


/*
**  Make sure that the serviceability table and messages that use it
**  are all cross-linked.
*/
static void
ValidateSvcRecords(
    void
)
{
    record_t		*rp;
    svcfield_t		*sp;
    char		*p;
    int			i;
    int			j;
    char		c;
    int			bad;

    /* Make sure all subcomponents have messages. */
    bad = FALSE;
    for (i = SvcCount, sp = Service; --i >= 0; sp++) {
	for (p = sp->Code, c = *p, j = RecCount, rp = Records; --j >= 0; rp++)
	    if (c == *rp->Code && strcmp(p, rp->Code) == 0) {
		sp->sc_descr = rp->MesgText;
		sp->Number = rp->Number;
		break;
	    }
	if (j < 0) {
	    (void)fprintf(stderr,
		    "Subcomponent %s has unknown message %s.\n",
		    sp->sc_name, sp->Code);
	    bad = TRUE;
	    continue;
	}
    }

    /* All records that have a Subcomp field must have a Subcomp that
     * is in the serviceability table. */
    for (i = RecCount, rp = Records; --i >= 0; rp++) {
	if ((p = rp->Subcomp) == NULL)
	    continue;
	for (c = *p, j = SvcCount, sp = Service; --j >= 0; sp++)
	    if (c == *sp->Subcomp && strcmp(p, sp->Subcomp) == 0)
		break;
	if (j < 0) {
	    (void)fprintf(stderr, "Message %s has unknown index %s.\n",
		    rp->Code, p);
	    bad = TRUE;
	    continue;
	}
    }
    if (bad)
	exit(1);
}


/*
**  Map a technology to a number; return -1 on error.
*/
int
Technology(
    const char		*p
)
{
    if (strcmp(p, "dce") == 0)
	return 1;
    if (strcmp(p, "dfs") == 0)
	return 2;
    return -1;
}


/*
**  Print a usage message and exit.
*/
static void
Usage(
    const char		*p
)
{
    if (p)
	(void)fprintf(stderr, "%s\n", p);
    (void)fprintf(stderr, "Usage error.\n");
    exit(1);
}


int
main(
    int			ac,
    char		*av[]
)
{
    MSGID		base;
    MSGID		idx;
    record_t		*rp;
    time_t		t;
    struct tm		*tm;
    char		*rad;
    char		*output;
    char		*p;
    char		buff[SMBUF];
    int			i;
    int			new;
    int			count;
    int			oldcoll;

    /* Parse JCL. */
    output = NULL;
    Name = NULL;
    Table = NULL;
    while ((i = getopt(ac, av, "d:fg:i:mn:o:s:t:x")) != EOF)
	switch (i) {
	default:
	    Usage(NULL);
	    /* NOTREACHED */
	case 'd':
	    Directory = optarg;
	    break;
	case 'f':
	    FilterText = 0;
	    break;
	case 'g':
	    Gencat = optarg;
	    break;
	case 'i':
	    IDL = optarg;
	    break;
	case 'm':
	    Multiples = 1;
	    break;
	case 'n':
	    Name = optarg;
	    break;
	case 'o':
	    for (p = output = optarg; *p; p++)
		switch (*p) {
		default:
		    Usage("Bad output option");
		    break;
		case 'c': case 'd': case 'h': case 'm':
		case 'p': case 's': case 't': case 'u':
		case 'x':
		case 'i':
		case 'S':
		    break;
		}
	    break;
	case 's':
	    switch (optarg[0]) {
	    default:
		Usage("Bad sort option");
	    case 'a': case 'n': case 't':
		SortMode = optarg[0];
		break;
	    }
	    break;
	case 't':
	    Table = optarg;
	    break;
	case 'x':
	    XPGCheck = TRUE;
	    break;
	}
    ac -= optind;
    av += optind;
    if (ac != 1)
	Usage("One input file not specified");
    Input = av[0];

    /* Get space to store the records. */
    RecCount = 0;
    RecSize = REC_CHUNK;
    Records = (record_t *)malloc(RecSize * sizeof *Records);
    SvcSize = REC_CHUNK;
    Service = (svcfield_t *)malloc(SvcSize * sizeof *Service);

    /* Parse the input. */
    ValueHeader = 1;
    TechHeader = "dce";
    DefaultHeader = MTintable | MTincatalog;
    yyopen(Input);
    (void)yyparse();
    if (Errors) {
	(void)fprintf(stderr, "No output generated.\n");
	exit(1);
    }
    if (Name == NULL) {
	Name = malloc(strlen(TechHeader) + strlen(CompHeader) + 1);
	sprintf(Name, "%s%s", TechHeader, CompHeader);
    }

    /* Calculate base number. */
    if (CodeHeader) {
	/* We know it was written using %d and not shifted (see samsgram.y). */
	base = strtol(CodeHeader, (char **)NULL, 10) << ISV_COMP_SHIFT;
	CodeMask = ISV_CODE_MASK;
    }
    else {
	rad = RAD40ALPHABET;
	idx = strchr(rad, CompHeader[0]) - rad;
	idx *= 40;
	idx += strchr(rad, CompHeader[1]) - rad;
	idx *= 40;
	idx += strchr(rad, CompHeader[2]) - rad;
	idx <<= COMP_SHIFT;
	idx &= COMP_MASK;
	base = Technology(TechHeader) << TECH_SHIFT;
	base |= idx;
	CodeMask = CODE_MASK;
    }

    /* Assign numbers to all records. */
    oldcoll = 0;
    for (idx = ValueHeader, i = RecCount, rp = Records; --i >= 0; rp++) {
	if (rp->Flags == MTsetvalue) {
	    if (rp->Number < idx)
		(void)fprintf(stderr, "Warning: re-used counter at %ld.\n",
		    rp->Number);
	    idx = rp->Number;
	    continue;
	}
	if (rp->Flags == MTcollect) {
	    new = rp->Number * CollectionSize;
	    if (new < idx) {
		(void)fprintf(stderr,
    "Error: collection %d overflow (was at message %ld, collection %ld).\n",
		    rp->Number, idx, oldcoll);
		exit(1);
	    }
	    if (rp->Number <= oldcoll) {
		(void)fprintf(stderr, "Error: collection %d reused.\n",
		    rp->Number);
		exit(1);
	    }
	    oldcoll = rp->Number;
	    idx = new;
	    continue;
	}
	if (!rp->Set) {
	    if (idx > CodeMask) {
		(void)fprintf(stderr, "Error: Overflowed counter at %s.\n",
			rp->Code);
		exit(1);
	    }
	    rp->Number = base | idx++;
	}
    }

    /* Count actual messages that will be in the table. */
    for (count = 0, i = RecCount, rp = Records; --i >= 0; rp++) {
	if (MT_SKIP(rp->Flags) || (rp->Flags & MTintable) == 0)
	    continue;
	count++;
    }

    /* Check serviceability stuff.  This does some nested linear searches
     * that at some point we might want to replace with, e.g., hashing. */
    if (WANT('s') || WANT('S')) {
	if (SvcCount == 0) {
	    (void)fprintf(stderr, "No serviceability table defined.\n");
	    exit(1);
	}

	ValidateSvcRecords();
    }

    /* Get time as a convenient string. */
    (void)time(&t);
    tm = gmtime(&t);
    (void)sprintf(buff, "%4d-%2.2d-%2.2d-%2.2d:%2.2d:%2.2d.000",
	tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
	tm->tm_hour, tm->tm_min, tm->tm_sec);
    Now = strdup(buff);

    /* Write files. */
    qsort(Records, (size_t)RecCount, sizeof *Records, numbasort);
    if (WANT('t'))
	WriteTableFile(count);
    if (WANT('h'))
	WriteHeaderFile(count);
    if (WANT('m'))
	WriteMsgFile();
    if (WANT('c'))
	WriteCatFile();
    if (WANT('s'))
	WriteSvcHdr();
    if (WANT('S'))
	WriteSvcSrc();
    if (WANT('u'))
	WriteUtilFile();
    if (WANT('i')) {
	if (IDL)
	    WriteIDLFile();
	else if (output != NULL) {
	    fprintf(stderr, "Must use -i flag to generate IDL file\n");
	    exit(1);
	}
    }
    if (WANT('x'))
	WriteIDXFile();
    if (WANT('p') || WANT('d')) {
	if (SortMode == 'a')
	    qsort(Records, (size_t)RecCount, sizeof *Records, alphasort);
	else if (SortMode == 't')
	    qsort(Records, (size_t)RecCount, sizeof *Records, textsort);
	if (WANT('p'))
	    WritePDGFile();
	if (WANT('d'))
	    WriteManFile();
    }

    /* That's all she wrote. */
    return 0;
}
