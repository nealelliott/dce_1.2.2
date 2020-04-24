/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nidlmsg.h,v $
 * Revision 1.1.15.2  1996/02/18  23:45:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:26  marty]
 *
 * Revision 1.1.15.1  1995/12/07  22:21:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:25:40  root]
 * 
 * Revision 1.1.11.2  1994/05/03  18:50:40  tom
 * 	From DEC: increment version for extended string support.
 * 	[1994/05/03  18:45:57  tom]
 * 
 * Revision 1.1.11.1  1994/02/14  16:00:43  rico
 * 	Add message to warn when [fault_status] used on an IDL Encoding Services
 * 	operation.
 * 	[1994/02/14  15:52:38  rico]
 * 
 * Revision 1.1.8.1  1993/10/14  12:40:35  hinxman
 * 	CR 8898 Warn user if IDL compiler encounters CPP directive
 * 	CR 8899 Improve error reporting of missing required type
 * 	CR 8900 Warn that anonymous enum is not portable
 * 	[1993/10/14  12:40:08  hinxman]
 * 
 * Revision 1.1.6.3  1993/07/26  20:10:44  hinxman
 * 	OT 8309 - Check [cs_*tag] parameters have correct dimensionality
 * 	[1993/07/26  20:06:43  hinxman]
 * 
 * Revision 1.1.6.2  1993/07/07  20:03:06  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:34:14  ganni]
 * 
 * $EndLog$
 */
/*++                                                                          */
/*!  @OSF_COPYRIGHT@                                                          */
/*!                                                                           */
/*!  Copyright (c) 1989 by                                                    */
/*!      Hewlett-Packard Company, Palo Alto, Ca. &                            */
/*!      Digital Equipment Corporation, Maynard, Mass.                        */
/*!                                                                           */
/*!  NAME:                                                                    */
/*!                                                                           */
/*!      nidlmsg.msg                                                          */
/*!                                                                           */
/*!  FACILITY:                                                                */
/*!                                                                           */
/*!      Interface Definition Language (IDL) Compiler                         */
/*!                                                                           */
/*!  ABSTRACT:                                                                */
/*!                                                                           */
/*!  RPC IDL Compiler messages.                                               */
/*!                                                                           */
/*!  %a%private_begin                                                         */
/*!                                                                           */
/*!  MODIFICATION HISTORY:                                                    */
/*!                                                                           */
/*!  26-Aug-92   N. Hodgman      Marked DEC-only messages by appending _DEC   */
/*!                              to [begin_msg] and [end_msg]. For messages   */
/*!                              affected by conformant lower bounds, added   */
/*!                              duplicate message marked with the _DEC text  */
/*!                              and changed the description to include       */
/*!                              the [min_is] attribute.                      */
/*!  18-Sep-91   D.Annicchiarico Add msg to disallow indirection [attr(*field */
/*!  09-Aug-91   D.Sherman       Added { to messages containing info on       */
/*!                              function pointers and unique attribute       */
/*!  17-Jul-91   A.I.Hinxman     Changes from JUly document review            */
/*!  28-Jun-91   J.Harrow        Update FUNTYPDCL message and explanation.    */
/*!  18-Jun-91   D.Annicchiarico Warn when all operations are [nocode]        */
/*!  14-Jun-91   D.Annicchiarico Make must be int msgs clear that can't be hy */
/*!  05-Jun-91   D.Annicchiarico Add OPTNOVAL                                 */
/*!  03-Jun-91   D.Annicchiarico Add SYSERRMSG for system error message;      */
/*!                              Separate message for open read/open write    */
/*!  31-May-91   D.Annicchiarico Change SYSIDLNAME from info to warning       */
/*!  29-May-91   J.Harrow        Add Catalog access errors so text is all     */
/*!                              in one place, and remove unused errors       */
/*!  21-May-91   Al Simons       Remove SYMTBLPOP--no longer used             */
/*!  21-May-91   D.Annicchiarico Disallow function type declarations          */
/*!  16-May-91   D.Annicchiarico Add COMPABORT; Make INCOMPATV1 error again   */
/*!  16-May-91   J.Harrow        Fix HANDLEPTR error declaration              */
/*!  09-May-91   D.Annicchiarico Add CLOSECPP warning                         */
/*!  06-May-91   J.Harrow        Add SYSIDLNAME for reserved system IDL files */
/*!  02-May-91   D.Sherman       Changing all <variable> and <keyword> tags   */
/*!                              to <v> and <kw>                              */
/*!  25-Apr-91   J.Harrow        Remove unused messages.                      */
/*!  19-Apr-91   D.Sherman       Editorial and doc changes                    */
/*!  19-Apr-91   J.Harrow        Add INTDIVBY0                                */
/*!  09-Apr-91   D.Annicchiarico Add NONPORTCHAR warning                      */
/*!  05-Apr-91   D.Annicchiarico Issue warning when represent_as type is not  */
/*!                              defined in IDL and ACF has no includes.      */
/*!  03-Apr-91   D.Annicchiarico Issue warning mixing max_is/length_is or     */
/*!                              size_is/last_is.                             */
/*!  17-Mar-91   tbl             Added lots of messages for X/Open NLS.       */
/*!  08-Mar-91   D.Annicchiarico Make INCOMPATV1 a warning (not error) for no */
/*!  04-Mar-91   D.Annicchiarico Transmissible type cannot contain full point */
/*!                              V1 attributes are incompatible with this typ */
/*!  14-Jan-91   D.Annicchiarico Add NAMEREFAT accompany message.             */
/*!  09-Jan-91   D.Annicchiarico Add STRV1ARRAY and STRARRAYV1.               */
/*!  04-Jan-91   D.Annicchiarico Add XMITASREP.                               */
/*!  02-Jan-91   D.Annicchiarico Add FLOATPROM info for float passed by value */
/*!  13-Dec-90   J. Harrow       add error for [ref] in xmit/repas types      */
/*!  06-Dec-90   J. Harrow       expression messages                          */
/*!  04-Dec-90   D.Annicchiarico Generalize comm_status messages.             */
/*!  04-Dec-90   J. Harrow       Add unsupported [align] error message.       */
/*!  26-Nov-90   J. Harrow       Add EXTRAPUNCT warning message.              */
/*!  15-Nov-90   D.Annicchiarico CTXXMITAS obsoleted by XMITTYPEATTRS.        */
/*!  12-Nov-90   J. Harrow       Add POSSAUTOHAN informational message.       */
/*!  06-Nov-90   D.Annicchiarico Change LINENONSCAL from error to warning.    */
/*!  01-Nov-90   D.Annicchiarico Change text and explanation of PTRCTXHAN.    */
/*!  16-Oct-90   D.Annicchiarico Remove SMALLSIZELEN, INTATTREQ, HANPRMREQ.   */
/*!                              Add DEFAUTOHAN.                              */
/*!  16-Oct-90   J.Harrow        DCE IDLize messages                          */
/*!  01-Oct-90   D.Annicchiarico Remove ambiguous uses of "open" wrt arrays.  */
/*!  24-Sep-90   D.Annicchiarico Add PTRATTRPTR and ARRPTRPRM.                */
/*!  06-Sep-90   D.Annicchiarico Reword parameter messages to include functio */
/*!                              result, add CMDERR info msg on command error */
/*!  04-Sep-90   D.Annicchiarico Add ERRSTATDEF for missing error_status_t ty */
/*!  21-Aug-90   J.Harrow        Add MESSAGE_VERSION.  This integer should be */
/*!                              in synch with the .LITERAL value             */
/*!                              MESSAGE_VERSION_NEEDED.                      */
/*!  20-Aug-90   D.Annicchiarico Roll CFMTARRIN, OUTREFCFMTARR, CFMTSTROUT in */
/*!                              OUTCFMTARR; strengthen language of CFMTUNION */
/*!  16-Aug-90   J.Harrow        Add NOSEMCHECK error when checker isn't call */
/*!  08-Aug-90   D.Annicchiarico Added OUTDIRIGN and added param to ARRSIZEIN */
/*!  24-Jul-90   J.Harrow        Add message for illegal character literal.   */
/*!  17-Jul-90   J.Harrow        Add messages for out of bounds version numbe */
/*!  12-Jul-90   D.Annicchiarico Change verbose messages, add OLDUUID.        */
/*!  26-Jun-90   M.Showman       Fix formatting problems in message documenta */
/*!  25-Jun-90   D.Annicchiarico Add more transmit_as restrictions.           */
/*!  22-Jun-90   D.Annicchiarico Change STRARRAY, STRCHARBYTE, STRV1FIXED.    */
/*!  20-Jun-90   D.Annicchiarico Documentation fixes; Add BROADPIPE.          */
/*!  18-Jun-90   D.Annicchiarico Remove SMALLFIXED; Add SMALLMINFIRST, CFMTST */
/*!  08-Jun-90   D.Annicchiarico Add rep_as msg and info msgs for verbose opt */
/*!  06-Jun-90   J.Harrow        Remove unused alignment warnings, and update */
/*!  05-Jun-90   D.Annicchiarico Add messages for new semantic restrictions.  */
/*!  13-May-90   J.Harrow        Add message for invalid constants            */
/*!  30-Apr-90   D.Annicchiarico Incorporate Al's comments from review pass.  */
/*!  25-Apr-90   D.Annicchiarico Add REFFUNRES, UNIQFUNRES.                   */
/*!  24-Apr-90   D.Annicchiarico Remove OUTNONREF; Add OUTUNIQUE and UNIQCTXH */
/*!  02-Apr-90   D.Annicchiarico Several additions/changes for handles, pipes */
/*!  22-Mar-90   D.Annicchiarico Remove ACFHANREQ, ACFAUTOHAN, ACFIMPLHAN.    */
/*!                              Remove or change [v1_string] messages.       */
/*!  22-Mar-90   D.Annicchiarico Add templates for message descriptions for d */
/*!  20-Mar-90   B. Martin       Add ALIGNSTRUCT warning.                     */
/*!  20-Mar-90   D.Annicchiarico Remove ERRUNKIND, HANDLEINV; Add DUPCASELBL. */
/*!  13-Mar-90   B. Martin       Add INTERNAL_ERROR and REFUNIQUE errors.     */
/*!  13-Mar-90   D.Annicchiarico Add messages for new ptr restrictions.       */
/*!  08-Mar-90   B. Martin       Add PRMBYREF error.                          */
/*!  06-Mar-90   B. Martin       Add CONFLICTATTR.                            */
/*!  26-Feb-90   D.Annicchiarico Change OPENFLDLAST to CFMTFLDLAST.           */
/*!  16-Feb-90   D.Annicchiarico More arm checks and other instance checks.   */
/*!  08-Feb-90   J. Harrow       Add messages for illegal attribute usage.    */
/*!  06-Feb-90   B. Martin       Add SIZEARRTYPE.                             */
/*!  31-Jan-90   D.Annicchiarico Changed [npb], [v1_array] msgs, added 2 new  */
/*!                              removed many obsolete msgs; changed some tex */
/*!  15-Jan-90   D.Annicchiarico Add a bunch of semantic checking messages.   */
/*!  11-Jan-90   B. Martin       Add CONSTTYPE.                               */
/*!  21-Dec-89   D.Annicchiarico Added messages related to ACF processing.    */
/*!  15-Dec-89   D.Annicchiarico Put in registered facility number.           */
/*!  10-Nov-89   D.Annicchiarico Added IDTOOLONG and MULATTRDEF.              */
/*!  18-Oct-89   D.Annicchiarico Original version.                            */
/*!                                                                           */
/*!  %a%private_end                                                           */
/*!                                                                           */
/*--                                                                          */
/*+                                                                           */
/* These two symbols must have the same integer value.                        */
/*-                                                                           */
#define NIDL_MESSAGE_VERSION_USED 3077

/* nidlmsg.h - Error message literals for IDL */

#define CAT_SET  1
#define CAT_FILE "idl.cat"

#define NIDL_MESSAGE_VERSION 1
#define NIDL_CMDERR 2
#define NIDL_DEFAUTOHAN 3
#define NIDL_FILESOURCE 4
#define NIDL_FLOATPROM 5
#define NIDL_IMPORTIDL 6
#define NIDL_INCLCREATE 7
#define NIDL_LEGALVALS 8
#define NIDL_LINEFILE 9
#define NIDL_NAMEDECLAT 10
#define NIDL_NAMEREFAT 11
#define NIDL_NEWUUID 12
#define NIDL_OPTIONSTABLE 13
#define NIDL_PROCESSACF 14
#define NIDL_RUNCPP 15
#define NIDL_STUBCOMPILE 16
#define NIDL_STUBCREATE 17
#define NIDL_STUBDELETE 18
#define NIDL_TYPEREPAS 19
#define NIDL_USAGE 20
#define NIDL_VERSION 21
#define NIDL_DUPPROTOCOL 22
#define NIDL_ENDPOINTSYNTAX 23
#define NIDL_EXTRAPUNCT 24
#define NIDL_FPHANATTR 25
#define NIDL_IDTOOLONG 26
#define NIDL_INCLUDEXT 27
#define NIDL_INCLTYPE 28
#define NIDL_INTSIZEREQ 29
#define NIDL_LINENONSCAL 30
#define NIDL_MISSPTRCLASS 31
#define NIDL_MIXEDARRATTR 32
#define NIDL_MULATTRDEF 33
#define NIDL_NAMETOOLONG 34
#define NIDL_NOCODEOPS 35
#define NIDL_NOENDPOINT 36
#define NIDL_NONPORTCHAR 37
#define NIDL_NOSEMCHECK 38
#define NIDL_OLDUUID 39
#define NIDL_OUTDIRIGN 40
#define NIDL_REFUNIQUE 41
#define NIDL_SRVNOCODE 42
#define NIDL_SYSIDLNAME 43
#define NIDL_REQCMDOPT 44
#define NIDL_ACFINIDL 45
#define NIDL_ANONPIPE 46
#define NIDL_ANONTYPE 47
#define NIDL_ARMREFPTR 48
#define NIDL_ARRELEMCFMT 49
#define NIDL_ARRELEMCTX 50
#define NIDL_ARRELEMPIPE 51
#define NIDL_ARRSIZEINFO 52
#define NIDL_ARRCFMTDIM 53
#define NIDL_ARRPTRPRM 54
#define NIDL_ARRSYNTAX 55
#define NIDL_ARRVARYDIM 56
#define NIDL_ARRXMITOPEN 57
#define NIDL_ATTRTRANS 58
#define NIDL_ATTRVALIND 59
#define NIDL_BADTAGREF 60
#define NIDL_BROADPIPE 61
#define NIDL_CASEDISCTYPE 62
#define NIDL_CASECONENUM 63
#define NIDL_CFMTARRREF 64
#define NIDL_CFMTBASETYP 65
#define NIDL_CFMTFLDLAST 66
#define NIDL_CFMTUNION 67
#define NIDL_CONFHANATTR 68
#define NIDL_CONFLINEATTR 69
#define NIDL_CONFLICTATTR 70
#define NIDL_CONFREPRTYPE 71
#define NIDL_CONSTNOTFND 72
#define NIDL_CONSTTYPE 73
#define NIDL_HYPERCONST 74
#define NIDL_MISSONINTER 75
#define NIDL_MISSONATTR 76
#define NIDL_MISSONARR 77
#define NIDL_MISSONOP 78
#define NIDL_MISSONPARAM 79
#define NIDL_CTXBASETYP 80
#define NIDL_CTXPTRVOID 81
#define NIDL_CTXSTRFLD 82
#define NIDL_CTXUNIMEM 83
#define NIDL_DEFNOTCOMP 84
#define NIDL_DUPCASEVAL 85
#define NIDL_EOF 86
#define NIDL_EOFNEAR 87
#define NIDL_ERRINATTR 88
#define NIDL_ERRSTATDEF 89
#define NIDL_FILENOTDIR 90
#define NIDL_FILENOTFND 91
#define NIDL_FIRSTINATTR 92
#define NIDL_FIRSTYPEINT 93
#define NIDL_FLOATCONSTNOSUP 94
#define NIDL_FPCFMTARR 95
#define NIDL_FPLOCINT 96
#define NIDL_FPHANPRM 97
#define NIDL_FPINPRM 98
#define NIDL_FPPIPEBASE 99
#define NIDL_FPSTRFLD 100
#define NIDL_FPUNIMEM 101
#define NIDL_FUNTYPDCL 102
#define NIDL_HANARRELEM 103
#define NIDL_HANDLEIN 104
#define NIDL_HANDLEPTR 105
#define NIDL_HANFIRSTPRM 106
#define NIDL_HANPIPEBASE 107
#define NIDL_HANPRMIN 108
#define NIDL_HANSTRFLD 109
#define NIDL_HANUNIMEM 110
#define NIDL_HANXMITAS 111
#define NIDL_IDEMPIPE 112
#define NIDL_IGNARRELEM 113
#define NIDL_IGNATTRPTR 114
#define NIDL_ILLFIELDATTR 115
#define NIDL_ILLPARAMATTR 116
#define NIDL_ILLTYPEATTR 117
#define NIDL_ILLOPATTR 118
#define NIDL_ILLINTATTR 119
#define NIDL_ILLMEMATTR 120
#define NIDL_IMPHANVAR 121
#define NIDL_IMPORTLOCAL 122
#define NIDL_INCOMPATV1 123
#define NIDL_INTCODEATTR 124
#define NIDL_INTDIVBY0 125
#define NIDL_INTLINEATTR 126
#define NIDL_INTNAMDIF 127
#define NIDL_INTOVERFLOW 128
#define NIDL_INTCONSTINVAL 129
#define NIDL_INTUUIDREQ 130
#define NIDL_INVARRIND 131
#define NIDL_INVCASETYP 132
#define NIDL_INVCHARLIT 133
#define NIDL_INVOCTDIGIT 134
#define NIDL_INVOKECPP 135
#define NIDL_INVOOLPRM 136
#define NIDL_INVOPTION 137
#define NIDL_INVPARAMS 138
#define NIDL_INVPTRCTX 139
#define NIDL_INVPTRPIPE 140
#define NIDL_LASTINATTR 141
#define NIDL_LASTLENCONF 142
#define NIDL_LASTYPEINT 143
#define NIDL_LBLESSUB 144
#define NIDL_LENINATTR 145
#define NIDL_LENTYPEINT 146
#define NIDL_MAJORTOOLARGE 147
#define NIDL_MAXCFMTYPE 148
#define NIDL_MAXIDINTF 149
#define NIDL_MAXIDTYPTA 150
#define NIDL_MAXIDTYPHAN 151
#define NIDL_MAXIDTYPCH 152
#define NIDL_MAXIDTYPPT 153
#define NIDL_MAXIDTYPPIPE 154
#define NIDL_MAXIDTYPRA 155
#define NIDL_MAXIDTYPOOL 156
#define NIDL_MAXINATTR 157
#define NIDL_MAXSIZEATTR 158
#define NIDL_MAXSIZECONF 159
#define NIDL_MAXTYPEINT 160
#define NIDL_BFINTTYPE 161
#define NIDL_MAYBEOUTPRM 162
#define NIDL_MINATTREQ 163
#define NIDL_MINCFMTYPE 164
#define NIDL_MININATTR 165
#define NIDL_MINORTOOLARGE 166
#define NIDL_MINTYPEINT 167
#define NIDL_NAMEALRDEC 168
#define NIDL_NAMENOTCONST 169
#define NIDL_NAMENOTFIELD 170
#define NIDL_NAMENOTFND 171
#define NIDL_NAMENOTPARAM 172
#define NIDL_NAMENOTTYPE 173
#define NIDL_NAMEPREVDECLAT 174
#define NIDL_NLSCATVER 175
#define NIDL_NLSWRONG 176
#define NIDL_NONINTEXP 177
#define NIDL_NYSALIGN 178
#define NIDL_NYSINSHAPE 179
#define NIDL_NYSNONZEROLB 180
#define NIDL_NYSOUTSHAPE 181
#define NIDL_NYSUNIQUE 182
#define NIDL_OPCODEATTR 183
#define NIDL_OPENREAD 184
#define NIDL_OPENWRITE 185
#define NIDL_OPNOTDEF 186
#define NIDL_OUTCFMTARR 187
#define NIDL_OUTPRMREF 188
#define NIDL_OUTPTRPRM 189
#define NIDL_OUTUNIQUE 190
#define NIDL_OPRESPIPE 191
#define NIDL_OUTSTAR 192
#define NIDL_OUTUNIQPRM 193
#define NIDL_PIPEBASETYP 194
#define NIDL_PIPESTRFLD 195
#define NIDL_PIPEUNIMEM 196
#define NIDL_PIPEXMITAS 197
#define NIDL_PRMBYREF 198
#define NIDL_PRMINOROUT 199
#define NIDL_PRMLINEATTR 200
#define NIDL_PRMNOTDEF 201
#define NIDL_PTRATTRHAN 202
#define NIDL_PTRATTRPTR 203
#define NIDL_PTRBASETYP 204
#define NIDL_PTRCFMTARR 205
#define NIDL_PTRCTXHAN 206
#define NIDL_PTRPIPE 207
#define NIDL_PTRV1ENUM 208
#define NIDL_PTRVARYARR 209
#define NIDL_PTRVOIDCTX 210
#define NIDL_REFATTRPTR 211
#define NIDL_REFFUNRES 212
#define NIDL_RENAMEFAILED 213
#define NIDL_REPASNEST 214
#define NIDL_SCOPELVLS 215
#define NIDL_SIZEARRTYPE 216
#define NIDL_SIZECFMTYPE 217
#define NIDL_SIZEINATTR 218
#define NIDL_SIZEMISMATCH 219
#define NIDL_SIZEPRMPTR 220
#define NIDL_SIZETYPEINT 221
#define NIDL_SIZEVARREPAS 222
#define NIDL_SIZEVARXMITAS 223
#define NIDL_SMALLARRSYN 224
#define NIDL_SMALLCFMT 225
#define NIDL_SMALLINV 226
#define NIDL_SMALLMINFIRST 227
#define NIDL_SMALLMULTID 228
#define NIDL_SMALLOPENLB 229
#define NIDL_STRARRAY 230
#define NIDL_STRARRAYV1 231
#define NIDL_STRCHARBYTE 232
#define NIDL_STRUCTXMITCFMT 233
#define NIDL_STRUNTERM 234
#define NIDL_STRV1ARRAY 235
#define NIDL_STRV1FIXED 236
#define NIDL_STRVARY 237
#define NIDL_STSATTRONCE 238
#define NIDL_STSPRMOUT 239
#define NIDL_STSRETVAL 240
#define NIDL_STSVARTYPE 241
#define NIDL_SYNTAXERR 242
#define NIDL_SYNTAXNEAR 243
#define NIDL_SYNTAXUUID 244
#define NIDL_SYSERRMSG 245
#define NIDL_TOOMANYELEM 246
#define NIDL_TOOMANYPORT 247
#define NIDL_TYPENOTFND 248
#define NIDL_TYPLINEATTR 249
#define NIDL_TYPNOTDEF 250
#define NIDL_UNBALPARENS 251
#define NIDL_UNBALBRACKETS 252
#define NIDL_UNBALBRACES 253
#define NIDL_UNIDISCTYP 254
#define NIDL_UNIQATTRHAN 255
#define NIDL_UNIQATTRPTR 256
#define NIDL_UNIQCTXHAN 257
#define NIDL_UNIQFUNRES 258
#define NIDL_UNKNOWNATTR 259
#define NIDL_USETRANS 260
#define NIDL_UUIDINV 261
#define NIDL_VARDECLNOSUP 262
#define NIDL_VOIDOPPTR 263
#define NIDL_XMITASREP 264
#define NIDL_XMITCFMTARR 265
#define NIDL_XMITPIPEBASE 266
#define NIDL_XMITPTR 267
#define NIDL_XMITTYPEATTRS 268
#define NIDL_BUGNOBUG 269
#define NIDL_CONFLICTOPT 270
#define NIDL_COMMENTEOF 271
#define NIDL_COMPABORT 272
#define NIDL_INTERNAL_ERROR 273
#define NIDL_INVBUG 274
#define NIDL_INVNOBUG 275
#define NIDL_MAXWARN 276
#define NIDL_OPTNOVAL 277
#define NIDL_OUTOFMEM 278
#define NIDL_SRCFILELEN 279
#define NIDL_SRCFILEREQ 280
#define NIDL_UNKFLAG 281
#define NIDL_NOPORTNEU 282
#define NIDL_NOPORTUNIQUE 283
#define NIDL_DISCRIMIN 284
#define NIDL_DISCRIMOUT 285
#define NIDL_DISCRIMREPAS 286
#define NIDL_DISCRIMXMITAS 287
#define NIDL_EUMEMATTR 288
#define NIDL_NEUARRAY 289
#define NIDL_NEUREPAS 290
#define NIDL_NEUREPTYPE 291
#define NIDL_NEUSWATTR 292
#define NIDL_NEUSWPTR 293
#define NIDL_NEUSWTYPE 294
#define NIDL_NEUXMITAS 295
#define NIDL_NEUXMITYPE 296
#define NIDL_PTRNEUNION 297
#define NIDL_SWATTRNEU 298
#define NIDL_SWDATATYPE 299
#define NIDL_SWTYPENEU 300
#define NIDL_FLDXMITCFMT 301
#define NIDL_TYPEREPCFMT 302
#define NIDL_ARMUNIQUEPTR 303
#define NIDL_LANGCFMTSTRCT 304
#define NIDL_LANGCFMTSTRNG 305
#define NIDL_LANGMAPCHAR 306
#define NIDL_LANGPIPESNS 307
#define NIDL_LANGUNSFNRS 308
#define NIDL_LANGMAXDIM 309
#define NIDL_LANGMIXCASE 310
#define NIDL_LANGATTRNS 311
#define NIDL_ARRFUNRES 312
#define NIDL_ATTRPTRVOID 313
#define NIDL_HANATTRTRAN 314
#define NIDL_FWDTAGREF 315
#define NIDL_ATTRUSEMULT 316
#define NIDL_TYPEATTRUSE 317
#define NIDL_NOPORTNZLB 318
#define NIDL_NOPORTCFMT 319
#define NIDL_NOPORTVARY 320
#define NIDL_ENCNOPARAMS 321
#define NIDL_DECNOPARAMS 322
#define NIDL_OPREFDELIN 323
#define NIDL_ENCOUTONLY 324
#define NIDL_DECINONLY 325
#define NIDL_ENCDECDIR 326
#define NIDL_ENCDECPIPE 327
#define NIDL_ENCDECBIND 328
#define NIDL_EXCNOTDEF 329
#define NIDL_INVFILESPEC 330
#define NIDL_FILESPECREQ 331
#define NIDL_OPNOTAGS 332
#define NIDL_TYPENEST 333
#define NIDL_DUPPRMATTR 334
#define NIDL_OPINCSCHAR 335
#define NIDL_OPOUTCSCHAR 336
#define NIDL_ARRATTRSHR 337
#define NIDL_ATTROUTCFMTCS 338
#define NIDL_CSARRSYN 339
#define NIDL_ARRPTRUNIQ 340
#define NIDL_XMITCTYPE 341
#define NIDL_TAGBEFDATA 342
#define NIDL_TAGAFTDATA 343
#define NIDL_HANCTYPE 344
#define NIDL_PIPECTYPE 345
#define NIDL_ARRMULTID 346
#define NIDL_ARRTYPATTR 347
#define NIDL_TYPECTYPE 348
#define NIDL_TYPEOFBYTES 349
#define NIDL_TAGPRMTYPE 350
#define NIDL_MAXIDTYPE 351
#define NIDL_NOPORTEXCEP 352
#define NIDL_NOPORTATTR 353
#define NIDL_PRMDEPATTR 354
#define NIDL_CPPCMDOPT 355
#define NIDL_NOPORTANON 356
#define NIDL_NOFLTPARAM 357
#define NIDL_CONSTRUCTOR 358
#define NIDL_INTREFNOTALO 359
#define NIDL_PTRATTBIGN 360
#define NIDL_STATICIGN 361
#define NIDL_LOOKUPNOTVAL 362
#define NIDL_NEWNOTVAL 363
#define NIDL_INHERITNOTVAL 364
#define NIDL_INHERITNOTDEF 365
#define NIDL_DELEGATENOTVAL 366
