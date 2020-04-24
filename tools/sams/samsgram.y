%{
#define YYDEBUG 1
/*
 *  @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: samsgram.y,v $
 * Revision 1.1.6.2  1996/03/09  23:32:16  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:47:21  marty]
 *
 * Revision 1.1.6.1  1995/12/08  20:55:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:37  root]
 * 
 * Revision 1.1.4.7  1994/09/30  07:56:09  rsalz
 * 	Implement DCE RFC 24.2 (OT CR 11929)
 * 	[1994/09/30  07:44:20  rsalz]
 * 
 * Revision 1.1.4.6  1994/06/10  20:53:25  devsrc
 * 	cr10872- fix copyright
 * 	[1994/06/10  16:38:11  devsrc]
 * 
 * Revision 1.1.4.5  1994/04/17  22:39:12  rsalz
 * 	Check number of % sequences for XPG4 (OT CR 10337)
 * 	[1994/04/17  22:38:49  rsalz]
 * 
 * Revision 1.1.4.4  1994/04/06  15:01:20  rsalz
 * 	Work-around strange malloc interaction (OT CR 10305).
 * 	[1994/04/06  14:54:13  rsalz]
 * 
 * Revision 1.1.4.3  1994/03/11  21:09:23  rsalz
 * 	Add -f flag.
 * 	Fix segfault (was freeing wrong pointer) (OT CR 10122).
 * 	[1994/03/11  14:14:20  rsalz]
 * 
 * Revision 1.1.4.2  1994/03/10  21:09:30  rsalz
 * 	Add collections and conditional text (OT CR 9697).
 * 	[1994/03/10  20:45:47  rsalz]
 * 
 * Revision 1.1.4.1  1993/12/14  21:47:35  rsalz
 * 	Remove #include <dce/dce_error.h>
 * 	[1993/12/14  21:47:27  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  17:41:59  rsalz
 * 	Initial release
 * 	[1993/08/16  17:41:13  rsalz]
 * 
 * $EndLog$
 */
#include <sams.h>


/*
**  This is defined in <dce/dce_error.h> but we want to build sams
**  before that header file is installed, so we just copy the constant.
*/
#define DCE_ERROR_STRING_SIZE 160 


/*
**  Build a msgfield_t from its parts.
*/
#define Makemsgfield_t(f, t, v, n)	\
	(f).Token = (t), (f).Value = (v), (f).Number = (n)
#define Dummymsgfield_t(f, t, v)	\
	(f).Token = (t), (f).Value = NULL, free((v)), (f).Number = NONUM


/*
**  A header field; comes at the start of the file.
*/
typedef struct headfield_s_t {
    int		Token;
    char	*Name;
    char	**Valptr;
    int		*Intptr;
    int		Seen;
} headfield_t;


/*
**  A record field; occurs within each message definition.
*/
typedef struct recfield_s_t {
    int		Token;
    char	*Name;
    int		Optional;
    int		Count;
} recfield_t;

/*
**  The fields we're collecting in the current record.  I don't like using
**  globals, but this is much more efficient than building up a linked
**  list and using that.
*/
static int		CountMsgFields;
static msgfield_t	MsgFields[20];
%}

%union {
    int			Number;
    char		*String;
    struct msgfield_s_t	Field;
    struct svcfield_s_t	Svc;
}

%token	tACTION tADMINISTRATOR tATTRIBUTES tBIT tCODE tCOLLECTION tCOMPONENT
%token	tDEFAULT tEND tENGINEER tEXPLANATION tHANDLE tID tNOT tNOTES tNUMBER
%token	tOPERATOR tPROGRAMMER tRESPONSE tSERVICEABILITY tSET tSEVERITY tSIZE
%token  tSTART tSUBCOMP tSYSTEM tTABLE tTABLES tTECHNOLOGY tTEXT tUSER
%token	tVALUE tVENDOR

%type	<Field>		field
%type	<Number>	tBIT tNUMBER bits o_bits tables
%type	<String>	tID
%type	<Svc>		e_svc

%%

file	: headers records
	| headers svc records
	;

svc	: tSERVICEABILITY tTABLE tID tHANDLE tID tSTART l_svc tEND {
	    ServiceTable = $3;
	    ServiceHandle = $5;
	}
	;

l_svc	: e_svc {
	    if (SvcCount >= SvcSize - 1) {
		SvcSize += REC_CHUNK;
		Service = realloc(Service, SvcSize * sizeof *Service);
	    }
	    Service[SvcCount++] = $1;
	}
	| l_svc e_svc {
	    if (SvcCount >= SvcSize - 1) {
		SvcSize += REC_CHUNK;
		Service = realloc(Service, SvcSize * sizeof *Service);
	    }
	    Service[SvcCount++] = $2;
	}
	;

e_svc	: tSUBCOMP tID tID tID {
	    $$.Subcomp = $2;
	    $$.sc_name = $3;
	    $$.Code = $4;
	}
	;

headers	: l_head {
	    ValidateHeaders();
	}
	;

l_head	: header
	| header l_head
	| error l_head {
	    YYABORT;
	}
	;

header	: tCOMPONENT tCODE tNUMBER {
	    char	buff[SMBUF];

	    sprintf(buff, "%d", $3);
	    SetHeader(tCODE, strdup(buff), 0);
	}
	| tCOLLECTION tSIZE tNUMBER {
	    SetHeader(tCOLLECTION, NULL, $3);
	}
	| tCOMPONENT tID {
	    SetHeader(tCOMPONENT, $2, 0);
	}
	| tDEFAULT bits {
	    SetHeader(tDEFAULT, NULL, $2);
	}
	| tTECHNOLOGY tID {
	    SetHeader(tTECHNOLOGY, $2, 0);
	}
	| tVALUE tNUMBER {
	    SetHeader(tVALUE, NULL, $2);
	}
	| tTABLE tID {
	    SetHeader(tTABLE, $2, 0);
	}
	;

records	: record
	| records record
	;

record	: tSTART o_bits l_field tEND {
	    ValidateRecord($2);
	}
	| tSTART error tEND
	| tSET tVALUE '=' tNUMBER {
	    StoreChangeRecord($4);
	}
	| tSTART tCOLLECTION tNUMBER {
	    if ($3 <= 0 || $3 > CODE_MASK / CollectionSize)
		yyerror("Bad collection number");
	    else
		StoreCollectionRecord($3);
	}
	;

o_bits	: /* NULL */ {
	    $$ = DefaultHeader;
	}
	| bits {
	    $$ = $1;
	}
	;

bits	: tBIT {
	    $$ = $1;
	}
	| tNOT tBIT {
	    $$ = 0;
	}
	| bits tBIT {
	    $$ = $1 | $2;
	}
	| bits tNOT tBIT {
	    $$ = $1 & ~$3;
	}
	;

l_field	: field {
	    MsgFields[0] = $1;
	    CountMsgFields = 1;
	}
	| l_field field {
	    if (CountMsgFields > COUNTOF(MsgFields) - 1)
		yyerror("Too many fields in record");
	    else
		MsgFields[CountMsgFields++] = $2;
	}
	;


field	: tACTION tID {
	    Makemsgfield_t($$, tACTION, $2, NONUM);
	}
	| tCODE tID {
	    Makemsgfield_t($$, tCODE, $2, NONUM);
	}
	| tCODE tID '=' tNUMBER {
	    if ($4 == NONUM)
		yyerror("Using reserved message ID");
	    Makemsgfield_t($$, tCODE, $2, $4);
	}
	| tENGINEER tID {
	    Makemsgfield_t($$, tENGINEER, $2, NONUM);
	}
	| tEXPLANATION tID {
	    Makemsgfield_t($$, tEXPLANATION, $2, NONUM);
	}
	| tNOTES tID {
	    Makemsgfield_t($$, tNOTES, $2, NONUM);
	}
	| tTABLES '(' tables ')' {
	    Makemsgfield_t($$, tTABLES, strdup(""), $3);
	}
	| tTEXT tID {
	    CheckTextField($2);
	    Makemsgfield_t($$, tTEXT, $2, NONUM);
	}
	| tATTRIBUTES tID {
	    Makemsgfield_t($$, tATTRIBUTES, $2, NONUM);
	}
	| tSUBCOMP tID {
	    Makemsgfield_t($$, tSUBCOMP, $2, NONUM);
	}
	| tADMINISTRATOR tRESPONSE tID {
	    Dummymsgfield_t($$, tADMINISTRATOR, $3);
	}
	| tOPERATOR tRESPONSE tID {
	    Dummymsgfield_t($$, tOPERATOR, $3);
	}
	| tPROGRAMMER tRESPONSE tID {
	    Dummymsgfield_t($$, tPROGRAMMER, $3);
	}
	| tSEVERITY tID {
	    Dummymsgfield_t($$, tSEVERITY, $2);
	}
	| tSYSTEM tRESPONSE tID {
	    Dummymsgfield_t($$, tSYSTEM, $3);
	}
	| tUSER tRESPONSE tID {
	    Dummymsgfield_t($$, tUSER, $3);
	}
	| tVENDOR tID tID {
	    Dummymsgfield_t($$, tVENDOR, $2);
	    free($3);
	}
	;

tables	: tID {
	    $$ = Table == NULL || strcmp($1, Table) == 0;
	}
	| tables tID {
	    $$ = $1 || Table == NULL || strcmp($2, Table) == 0;
	}
	;
%%


/*
**  These are the headers that must be at the start of the file.
*/
static headfield_t	Headers[] = {
    {	tCODE,		"code",		&CodeHeader,	NULL		},
    {	tCOLLECTION,	"collection",	NULL,		&CollectionSize	},
    {	tCOMPONENT,	"component",	&CompHeader,	NULL		},
    {	tDEFAULT,	"default",	NULL,		&DefaultHeader	},
    {	tTECHNOLOGY,	"technology",	&TechHeader,	NULL		},
    {	tVALUE,		"value",	NULL,		&ValueHeader	},
    {	tTABLE,		"table",	&TableHeader,	NULL		},
};

/*
**  These are the records for each message.
*/
static recfield_t	RecordFields[] = {
    {	tACTION,	"action",		FALSE,	0	},
    {	tATTRIBUTES,	"attributes",		TRUE,	0	},
    {	tCODE,		"code",			FALSE,	0	},
    {	tENGINEER,	"engineer",		TRUE,	0	},
    {	tEXPLANATION,	"explanation",		FALSE,	0	},
    {	tSUBCOMP,	"sub-component",	TRUE,	0	},
    {	tNOTES,		"notes",		TRUE,	0	},
    {	tTABLES,	"tables",		TRUE,	0	},
    {	tTEXT,		"text",			FALSE,	0	},
    {	tADMINISTRATOR,	"admin. response",	TRUE,	0	},
    {	tOPERATOR,	"operator response",	TRUE,	0	},
    {	tPROGRAMMER,	"programmer response",	TRUE,	0	},
    {	tSEVERITY,	"severity",		TRUE,	0	},
    {	tSYSTEM,	"system response",	TRUE,	0	},
    {	tUSER,		"user response",	TRUE,	0	},
};


/*
**  Make sure that each header field is legitimate.
*/
static void
ValidateHeaders(
    void
)
{
    static char		OPTIONAL[] = "$";
    headfield_t		*hp;
    long		l;
    char		*p;
    char		buff[SMBUF];

    /* Set defaults. */
    if (CodeHeader == NULL && CompHeader == NULL) {
	yyerror("Must have \"component\" or \"component code\" header");
	return;
    }
    if (CodeHeader != NULL && CompHeader != NULL) {
	yyerror("Cannot have both \"component\" and \"component code\" header");
	return;
    }

    /* Make sure code header is valid. */
    if (CodeHeader == NULL) {
	for (p = CompHeader; *p; p++)
	    if (strchr(RAD40ALPHABET, *p) == NULL) {
		(void)sprintf(buff,
			"Illegal character `%c' in \"component\"", *p);
		yyerror(buff);
	    }
	if (strlen(CompHeader) != 3)
	    yyerror("\"component\" header must have 3 characters");
	CodeHeader = OPTIONAL;
    }
    else {
	l = strtol(CodeHeader, &p, 0);
	if (l <= 0 || *p)
	    yyerror("Illegal value for \"component code\" header");
	if (l != (l & (TECH_MASK | ISV_COMP_MASK)))
	    yyerror("\"component code\" header value masked off");
	if ((l & TECH_MASK) != ISV1_TECH && (l & TECH_MASK) != ISV2_TECH)
	    yyerror("Illegal value for \"component code\" header");
	sprintf(buff, "%6.6lx", l >> ISV_COMP_SHIFT);
	CompHeader = strdup(buff);
	CodeHeader = strdup(buff);
    }

    if (TableHeader == NULL) {
	if (CodeHeader == OPTIONAL)
	    (void)sprintf(buff, "%s_msg_table", CompHeader);
	else
	    (void)sprintf(buff, "dce_%s_msg_table", CompHeader);
	TableHeader = strdup(buff);
    }


    /* Make sure they're all there. */
    for (hp = Headers; hp < ENDOF(Headers); hp++)
	if (hp->Valptr && *hp->Valptr == NULL)
	    y2error("Missing \"%s\" header", hp->Name);

    if (CodeHeader == OPTIONAL)
	CodeHeader = NULL;

    /* Make sure technology header is valid. */
    if (TechHeader && Technology(TechHeader) < 0)
	y2error("Ilegal value `%s' for \"technology\" header", TechHeader);
    if (CodeHeader != NULL && strcmp(TechHeader, "dce") != 0)
	yyerror("\"technology\" header must be \"dce\"");

    /* Make sure value header is valid. */
    l = CompHeader != NULL ? CODE_MASK : ISV_CODE_MASK;
    if (ValueHeader > l) {
	(void)sprintf(buff, "Value `%d' too big for \"value\" header",
		ValueHeader);
	yyerror(buff);
    }
}


/*
**  Store a header.
*/
static void
SetHeader(
    int			t,
    char		*value,
    int			Number
)
{
    headfield_t		*hp;
    char		buff[SMBUF];

    for (hp = Headers; hp < ENDOF(Headers); hp++)
	if (hp->Token == t) {
	    if (hp->Seen)
		y2error("Duplicate \"%s\" header", hp->Name);
	    else if (value)
		*hp->Valptr = value;
	    else
		*hp->Intptr = Number;
	    hp->Seen = TRUE;
	    return;
	}
    (void)sprintf(buff, "Internal error -- unknown header %d", t);
    yyerror(buff);
}

/*
**  Add a "change counter" record to the in-core array.
*/
static void
StoreChangeRecord(
    int			i
)
{
    record_t		*rp;

    /* Get more space if needed. */
    if (RecCount >= RecSize - 1) {
	RecSize += REC_CHUNK;
	Records = realloc(Records, RecSize * sizeof *Records);
    }

    /* Get record, fill in fields. */
    rp = &Records[RecCount++];
    rp->Flags = MTsetvalue;
    rp->Number = i;
    rp->Code = "";
}


/*
**  Add a "collection" record to the in-core array.
*/
static void
StoreCollectionRecord(
    int			i
)
{
    record_t		*rp;

    /* Get more space if needed. */
    if (RecCount >= RecSize - 1) {
	RecSize += REC_CHUNK;
	Records = realloc(Records, RecSize * sizeof *Records);
    }

    /* Get record, fill in fields. */
    rp = &Records[RecCount++];
    rp->Flags = MTcollect;
    rp->Number = i;
    rp->Code = "";
}


/*
**  Turn "foo <%s|file> 3\<5" into "foo \*Lfile\*O 3<5" for documentation text.
*/
static char *
DocumentationFilter(
    char	*p
)
{
    char	*save;
    char	*dest;
    int		i;

    /* Get worst-case amount of space. */
    for (i = strlen(p) + 1, dest = p; *dest; dest++)
	if (*dest == '<')
	    /* Add space for \*L and \*O */
	    i += 3 + 3;
    save = dest = malloc(i);

    while (*p) {
	if (*p != '<') {
	    if (*p == '\\' && p[1] == '<') {
		*dest++ = '<';
		p += 2;
	    }
	    else
		*dest++ = *p++;
	    continue;
	}

	while (*++p && *p != '|')
	    continue;
	if (*p != '|') {
	    yyerror("Missing | in <a|b> construct");
	    break;
	}
	*dest++ = '\\';
	*dest++ = '*';
	*dest++ = 'L';
	while (*++p && *p != '>')
	    *dest++ = *p;
	if (*p != '>') {
	    yyerror("Missing > in <a|b> construct");
	    break;
	}
	*dest++ = '\\';
	*dest++ = '*';
	*dest++ = 'O';
	p++;
    }
    *dest = '\0';

    return save;
}

/*
**  Turn "foo <%s|file> 3\<5" into "foo %s 3<5" for program text.
*/
static char *
ProgramFilter(
    char	*p
)
{
    char	*save;
    char	*dest;

    /* Get worst-case amount of output space. */
    dest = save = malloc(strlen(p) + 1);

    while (*p) {
	if (*p != '<') {
	    if (*p == '\\' && p[1] == '<') {
		*dest++ = '<';
		p += 2;
	    }
	    else
		*dest++ = *p++;
	    continue;
	}

	while (*++p && *p != '|')
	    *dest++ = *p;
	if (*p != '|') {
	    yyerror("Missing | in <a|b> construct");
	    break;
	}
	while (*++p && *p != '>')
	    continue;
	if (*p != '>') {
	    yyerror("Missing > in <a|b> construct");
	    break;
	}
	p++;
    }
    *dest = '\0';

    return save;
}


/*
**  Add a message record to the in-core array.
*/
static void
StoreRecord(
    int			Flags
)
{
    static record_t	nullrec;
    record_t		*rp;
    msgfield_t		*f;
    int			i;
    char		buff[SMBUF];

    /* Get more space if needed. */
    if (RecCount >= RecSize - 1) {
	RecSize += REC_CHUNK;
	Records = realloc(Records, RecSize * sizeof *Records);
    }

    /* Get record, fill in known and default fields. */
    rp = &Records[RecCount++];
    *rp = nullrec;
    rp->Flags = Flags;
    rp->TableFlag = NONUM;

    /* Loop over fields; store each one in right place in record. */
    for (i = CountMsgFields, f = MsgFields; --i >= 0; f++)
	switch (f->Token) {
	default:
	    (void)sprintf(buff, "Internal error storing unknown type %d",
		    f->Token);
	    yyerror(buff);
	    break;
	/* Skip ignored fields. */
	case tADMINISTRATOR:
	case tOPERATOR:
	case tPROGRAMMER:
	case tSEVERITY:
	case tSYSTEM:
	case tUSER:
	case tVENDOR:
	    break;
	case tACTION:
	    rp->Action = f->Value;
	    break;
	case tATTRIBUTES:
	    rp->Attributes = f->Value;
	    break;
	case tCODE:
	    rp->Code = f->Value;
	    if (f->Number != NONUM) {
		rp->Set = TRUE;
		rp->Number = f->Number;
	    }
	    break;
	case tENGINEER:
	    break;
	case tEXPLANATION:
	    rp->Explanation = f->Value;
	    break;
	case tSUBCOMP:
	    rp->Subcomp = f->Value;
	    break;
	case tNOTES:
	    rp->Notes = f->Value;
	    break;
	case tTABLES:
	    rp->TableFlag = f->Number;
	    break;
	case tTEXT:
	    if (FilterText) {
		rp->DocText = DocumentationFilter(f->Value);
		rp->MesgText = ProgramFilter(f->Value);
	    }
	    else {
		rp->DocText = strdup(f->Value);
		rp->MesgText = strdup(f->Value);
	    }
	    free(f->Value);
	    break;
	}

    if ((rp->Flags & MTlongtext) == 0
     && strlen(rp->MesgText) > DCE_ERROR_STRING_SIZE)
	yyerror("Text is too long");
    if (rp->Attributes != NULL && rp->Subcomp == NULL)
	yyerror("Have \"attributes\" but not \"sub-component\" field");
    if (rp->Attributes == NULL && rp->Subcomp != NULL)
	yyerror("Have \"sub-component\" but not \"attributes\" field");

    /* If we got a "tables" field and the record could be in the
     * table, then only put it in the table if it is in the table
     * that we are generating. */
    if (rp->TableFlag == FALSE && (rp->Flags & MTintable) != 0)
	rp->Flags &= ~MTintable;
}


/*
**  Validate a record, making sure each field appears exactly once.
*/
static void
ValidateRecord(
    int			Flags
)
{
    recfield_t		*rp;
    msgfield_t		*f;
    int			i;
    char		*p;
    char		buff[SMBUF];
    int			bad;

    /* Reset counts. */
    for (rp = RecordFields; rp < ENDOF(RecordFields); rp++)
	rp->Count = 0;

    /* Make sure each field is a valid record. */
    for (bad = FALSE, i = CountMsgFields, f = MsgFields; --i >= 0; f++) {
	for (rp = RecordFields; rp < ENDOF(RecordFields); rp++)
	    if (f->Token == rp->Token) {
		if (rp->Count++ > 0) {
		    y2error("Duplicate \"%s\" field", rp->Name);
		    bad = TRUE;
		}
		break;
	    }
	/* Record not found in field list; this should not happen since
	 * yacc should not have recognized it! */
	if (rp == ENDOF(RecordFields)) {
	    (void)sprintf(buff, "Internal error -- unknown header %d",
		    f->Token);
	    yyerror(buff);
	    bad = TRUE;
	}
    }

    /* Make sure all fields were found. */
    for (rp = RecordFields; rp < ENDOF(RecordFields); rp++) {
	if (rp->Count != 0 || rp->Optional)
	    continue;
	if ((Flags & MTnodoc) != 0
	 && (rp->Token == tACTION || rp->Token == tEXPLANATION))
	    continue;
	y2error("Missing \"%s\" field", rp->Name);
	bad = TRUE;
    }

    if (!bad)
	StoreRecord(Flags);
}


static void
CheckTextField(
    char		*text
)
{
    static char		digits[] = "0123456789";
    char		buff[SMBUF];
    char		*p;
    char		*q;
    int			count;

    /* Don't require it if only one "%". */
    for (count = 0, p = text; *p; )
	if (*p++ == '%' && *p++ != '%')
	    count++;
    if (count < 2)
	return;

    /* Too many parameters for XPG4? */
    if (count > NL_ARGMAX) {
	sprintf(buff, "More then NL_ARGMAX (%d) parameters in text", NL_ARGMAX);
	yyerror(buff);
    }

    if (XPGCheck) {
	/* For all "%" make sure it is "%d$" where "d" is a digit. */
	for (count = 0, p = text; (p = strchr(p, '%')) != NULL; p++) {
	    q = strchr(digits, p[1]);
	    if (q == NULL || q - digits > NL_ARGMAX || p[2] != '$') {
		yyerror("Illegal XPG/printf %d$ format.");
		break;
	    }
	}
    }
}
