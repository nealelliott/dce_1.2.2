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
 * $Log: csrcgram.y,v $
 * Revision 1.1.4.2  1996/03/09  23:25:40  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:41:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:11:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:55  root]
 * 
 * Revision 1.1.2.4  1994/06/10  20:53:31  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:59:41  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  18:36:39  mori_m
 * 	CR 9702  Corrected the previous fix.  Token tIntArray should be optional.
 * 	[1994/05/10  18:31:18  mori_m]
 * 
 * Revision 1.1.2.2  1994/05/06  20:59:04  mori_m
 * 	CR 9702: Made character set(s) a required item.
 * 	[1994/05/06  20:55:09  mori_m]
 * 
 * Revision 1.1.2.1  1994/02/08  21:47:11  mori_m
 * 	CR 9702:  Initial submission for RPC runtime I18N support
 * 	[1994/02/08  21:46:55  mori_m]
 * 
 * $EndLog$
 */
%{
#include <csrc.h>
#include <dce/dce_error.h>
#include <dcecsrmsg.h>

/*
**  Build a msgfield_t from its parts.
*/
#define Makemsgfield_t(f, t, s, i, l)	\
    (f).Token = (t), (f).StrValue = (s), (f).IntValue = (i), (f).LongValue = (l)

/*
**  The fields we're collecting in the current record.  Using globals are 
**  not preferrable, but this is much more efficient than building up a 
**  linked list and using that.
*/
static int              CountMsgFields;
static msgfield_t       MsgFields[10];

%}

%union {
	unsigned int		Number; 
	unsigned long		LongNumber; 
	unsigned int		IntArray; 
	char			*String;
	struct	msgfield_s_t	Field;
}

%token  tCodeSet tCharSets tCodeName tNumber tName tIntArray tLongNumber
%token  tMaxBytes tSTART tEND

%type	<Field>		field array
%type	<Number>	tCodeSet tCharSets tNumber tMaxBytes
%type	<String>	tName
%type	<IntArray>	tIntArray
%type	<LongNumber>	tLongNumber

%%

file		: records
		;

records		: record
		| records record
		;

record		: tSTART l_field tEND {
			ValidateRecord();
		}
		| tSTART error tEND
		;

l_field		: field {
			MsgFields[0] = $1;
			CountMsgFields = 1;
		}
		| l_field field {
			if (CountMsgFields > COUNTOF(MsgFields) -1)
				yyerror("Too many fields in record");
			else
				if ($2.Token != tIntArray)
					MsgFields[CountMsgFields++] = $2;
		}
		;

field		: tCodeSet tLongNumber {
			Makemsgfield_t($$, tCodeSet, NULL, 0, $2);
		}
		| tCodeName tName {
			Makemsgfield_t($$, tCodeName, $2, 0, 0);
        	}
		| tCharSets tNumber {
			Makemsgfield_t($$, tCharSets, NULL, $2, 0);
        	}
		| tMaxBytes tNumber {
			Makemsgfield_t($$, tMaxBytes, NULL, $2, 0);
        	}
		| array 
		;

array 		: tCharSets tIntArray {
			Makemsgfield_t($$, tCharSets, NULL, $2, 0);
			MsgFields[CountMsgFields++] = $$;
		}
		| array tIntArray {
			Makemsgfield_t($$, tIntArray, NULL, $2, 0);
			MsgFields[CountMsgFields++] = $$;
		}
		;

%%

/*
** These are the records for each message.
*/
static recfield_t	RecordFields[] = {
	{ tCodeSet,	"Code Set ID",		FALSE,	0 },
	{ tCharSets,	"Character Set ID(s)",	FALSE,	0 },
	{ tCodeName,	"Code Set Name",	FALSE,	0 },
	{ tIntArray,	"Character Sets",	TRUE,	0 },
	{ tMaxBytes,	"Max number of Bytes",	FALSE,	0 },
};

/*
** Add a entry record to the in-core array.
*/
static void
StoreRecord (
	void
)
{
	static entry_t		nullentry;
	entry_t			*ep;
	msgfield_t		*f;
	int			i, j;
	char			*buff;

	/* Get more space if needed */
	if (csrc_g_entry_count >= csrc_g_recsize - 1) 
	{
		csrc_g_recsize += REC_CHUNK;
		csrc_g_entries = realloc(csrc_g_entries, csrc_g_recsize * sizeof *csrc_g_entries);
	}

	/*
	** Get Record, fill in known and default fields.
	*/
	ep = &csrc_g_entries[csrc_g_entry_count++];
	csrc_g_valid_entry++;
	*ep = nullentry;

	/* 
	** Loop over fields; store each one in right place in entry
	*/
	for (i = CountMsgFields, f = MsgFields, ep->char_sets_num = 0; --i >= 0; f++)
	{
		switch (f->Token) {
		default:
			buff = (char *)dce_sprintf(csrc_s_internal_unknown_type, f->Token);
			yyerror(buff);
			break;

		case tCodeSet:
			ep->code_set = f->LongValue;
			break;

		case tCharSets:
			ep->char_sets[0] = f->IntValue;
			ep->char_sets_num = 1;
			break;

		case tIntArray:
			ep->char_sets[ep->char_sets_num++] = f->IntValue;
			break;

		case tCodeName:
			ep->code_set_name = f->StrValue;
			ep->code_name_len = strlen(ep->code_set_name);
			if ((j = strcoll(ep->code_set_name, "NONE")) == 0)
				csrc_g_valid_entry--;
			break;

		case tMaxBytes:
			ep->max_bytes = f->IntValue;
			break;
		}
	/* Fill the default value to 'priority' field */
	ep->priority = 255;
	}
}

/*
** Validate a record, making sure each field appears exactly once.
*/
static void
ValidateRecord (
	void
)
{
	recfield_t	*rp;
	msgfield_t	*f;
	int		i;
	char		*buff;
	int		bad;

	/* Reset counts */
	for (rp = RecordFields; rp < ENDOF(RecordFields); rp++)
		rp->Count = 0;

	/* Make sure each field is a valid record */
	for (bad = FALSE, i = CountMsgFields, f = MsgFields; --i >= 0; f++) 
	{
		for (rp = RecordFields; rp < ENDOF(RecordFields); rp++)
		{
			if (f->Token == rp->Token) 
			{
				if (rp->Token != tIntArray && rp->Count++ > 0) 
				{
				y2error(dce_sprintf(csrc_s_duplicate_field, rp->Name));
					bad = TRUE;
				}
				break;
			}

			if (rp == ENDOF(RecordFields)) 
			{
				buff = (char *)dce_sprintf(csrc_s_internal_unknown_type, f->Token);
				yyerror(buff);
				bad = TRUE;
			}
		}
	}

	/*
	** Make sure all fields were found
	*/
	for (rp = RecordFields; rp < ENDOF(RecordFields); rp++) 
	{
		if (rp->Count != 0 || rp->Optional)
			continue;
		y2error(dce_sprintf(csrc_s_missing_field, rp->Name));
		bad = TRUE;
	}

	if (!bad)
		StoreRecord();
}

