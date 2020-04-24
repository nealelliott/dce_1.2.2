!++
!!  @OSF_COPYRIGHT@
!!
!!  Copyright (c) 1989 by
!!      Hewlett-Packard Company, Palo Alto, Ca. &
!!      Digital Equipment Corporation, Maynard, Mass.
!!
!!  NAME:
!!
!!      nidlmsg.msg
!!
!!  FACILITY:
!!
!!      Interface Definition Language (IDL) Compiler
!!
!!  ABSTRACT:
!!
!!  RPC IDL Compiler messages.
!!
!!  %a%private_begin
!!
!!  MODIFICATION HISTORY:
!!
!!  26-Aug-92   N. Hodgman      Marked DEC-only messages by appending _DEC
!!                              to [begin_msg] and [end_msg]. For messages
!!                              affected by conformant lower bounds, added
!!                              duplicate message marked with the _DEC text
!!                              and changed the description to include
!!                              the [min_is] attribute.
!!  18-Sep-91   D.Annicchiarico Add msg to disallow indirection [attr(*field)]
!!  09-Aug-91   D.Sherman       Added { to messages containing info on
!!                              function pointers and unique attribute
!!  17-Jul-91   A.I.Hinxman     Changes from JUly document review
!!  28-Jun-91   J.Harrow        Update FUNTYPDCL message and explanation.
!!  18-Jun-91   D.Annicchiarico Warn when all operations are [nocode]
!!  14-Jun-91   D.Annicchiarico Make must be int msgs clear that can't be hyper
!!  05-Jun-91   D.Annicchiarico Add OPTNOVAL
!!  03-Jun-91   D.Annicchiarico Add SYSERRMSG for system error message;
!!                              Separate message for open read/open write
!!  31-May-91   D.Annicchiarico Change SYSIDLNAME from info to warning
!!  29-May-91   J.Harrow        Add Catalog access errors so text is all
!!                              in one place, and remove unused errors
!!  21-May-91   Al Simons       Remove SYMTBLPOP--no longer used
!!  21-May-91   D.Annicchiarico Disallow function type declarations
!!  16-May-91   D.Annicchiarico Add COMPABORT; Make INCOMPATV1 error again
!!  16-May-91   J.Harrow        Fix HANDLEPTR error declaration
!!  09-May-91   D.Annicchiarico Add CLOSECPP warning
!!  06-May-91   J.Harrow        Add SYSIDLNAME for reserved system IDL files
!!  02-May-91   D.Sherman       Changing all <variable> and <keyword> tags
!!                              to <v> and <kw>
!!  25-Apr-91   J.Harrow        Remove unused messages.
!!  19-Apr-91   D.Sherman       Editorial and doc changes
!!  19-Apr-91   J.Harrow        Add INTDIVBY0
!!  09-Apr-91   D.Annicchiarico Add NONPORTCHAR warning
!!  05-Apr-91   D.Annicchiarico Issue warning when represent_as type is not
!!                              defined in IDL and ACF has no includes.
!!  03-Apr-91   D.Annicchiarico Issue warning mixing max_is/length_is or
!!                              size_is/last_is.
!!  17-Mar-91   tbl             Added lots of messages for X/Open NLS.
!!  08-Mar-91   D.Annicchiarico Make INCOMPATV1 a warning (not error) for now.
!!  04-Mar-91   D.Annicchiarico Transmissible type cannot contain full pointer.
!!                              V1 attributes are incompatible with this type.
!!  14-Jan-91   D.Annicchiarico Add NAMEREFAT accompany message.
!!  09-Jan-91   D.Annicchiarico Add STRV1ARRAY and STRARRAYV1.
!!  04-Jan-91   D.Annicchiarico Add XMITASREP.
!!  02-Jan-91   D.Annicchiarico Add FLOATPROM info for float passed by value.
!!  13-Dec-90   J. Harrow       add error for [ref] in xmit/repas types
!!  06-Dec-90   J. Harrow       expression messages
!!  04-Dec-90   D.Annicchiarico Generalize comm_status messages.
!!  04-Dec-90   J. Harrow       Add unsupported [align] error message.
!!  26-Nov-90   J. Harrow       Add EXTRAPUNCT warning message.
!!  15-Nov-90   D.Annicchiarico CTXXMITAS obsoleted by XMITTYPEATTRS.
!!  12-Nov-90   J. Harrow       Add POSSAUTOHAN informational message.
!!  06-Nov-90   D.Annicchiarico Change LINENONSCAL from error to warning.
!!  01-Nov-90   D.Annicchiarico Change text and explanation of PTRCTXHAN.
!!  16-Oct-90   D.Annicchiarico Remove SMALLSIZELEN, INTATTREQ, HANPRMREQ.
!!                              Add DEFAUTOHAN.
!!  16-Oct-90   J.Harrow        DCE IDLize messages
!!  01-Oct-90   D.Annicchiarico Remove ambiguous uses of "open" wrt arrays.
!!  24-Sep-90   D.Annicchiarico Add PTRATTRPTR and ARRPTRPRM.
!!  06-Sep-90   D.Annicchiarico Reword parameter messages to include function
!!                              result, add CMDERR info msg on command error.
!!  04-Sep-90   D.Annicchiarico Add ERRSTATDEF for missing error_status_t type.
!!  21-Aug-90   J.Harrow        Add MESSAGE_VERSION.  This integer should be kept
!!                              in synch with the .LITERAL value
!!                              MESSAGE_VERSION_NEEDED.
!!  20-Aug-90   D.Annicchiarico Roll CFMTARRIN, OUTREFCFMTARR, CFMTSTROUT into
!!                              OUTCFMTARR; strengthen language of CFMTUNION.
!!  16-Aug-90   J.Harrow        Add NOSEMCHECK error when checker isn't called
!!  08-Aug-90   D.Annicchiarico Added OUTDIRIGN and added param to ARRSIZEINFO.
!!  24-Jul-90   J.Harrow        Add message for illegal character literal.
!!  17-Jul-90   J.Harrow        Add messages for out of bounds version number.
!!  12-Jul-90   D.Annicchiarico Change verbose messages, add OLDUUID.
!!  26-Jun-90   M.Showman       Fix formatting problems in message documentation
!!  25-Jun-90   D.Annicchiarico Add more transmit_as restrictions.
!!  22-Jun-90   D.Annicchiarico Change STRARRAY, STRCHARBYTE, STRV1FIXED.
!!  20-Jun-90   D.Annicchiarico Documentation fixes; Add BROADPIPE.
!!  18-Jun-90   D.Annicchiarico Remove SMALLFIXED; Add SMALLMINFIRST, CFMTSTROUT
!!  08-Jun-90   D.Annicchiarico Add rep_as msg and info msgs for verbose option.
!!  06-Jun-90   J.Harrow        Remove unused alignment warnings, and update misc messages
!!  05-Jun-90   D.Annicchiarico Add messages for new semantic restrictions.
!!  13-May-90   J.Harrow        Add message for invalid constants
!!  30-Apr-90   D.Annicchiarico Incorporate Al's comments from review pass.
!!  25-Apr-90   D.Annicchiarico Add REFFUNRES, UNIQFUNRES.
!!  24-Apr-90   D.Annicchiarico Remove OUTNONREF; Add OUTUNIQUE and UNIQCTXHAN.
!!  02-Apr-90   D.Annicchiarico Several additions/changes for handles, pipes.
!!  22-Mar-90   D.Annicchiarico Remove ACFHANREQ, ACFAUTOHAN, ACFIMPLHAN.
!!                              Remove or change [v1_string] messages.
!!  22-Mar-90   D.Annicchiarico Add templates for message descriptions for doc.
!!  20-Mar-90   B. Martin       Add ALIGNSTRUCT warning.
!!  20-Mar-90   D.Annicchiarico Remove ERRUNKIND, HANDLEINV; Add DUPCASELBL.
!!  13-Mar-90   B. Martin       Add INTERNAL_ERROR and REFUNIQUE errors.
!!  13-Mar-90   D.Annicchiarico Add messages for new ptr restrictions.
!!  08-Mar-90   B. Martin       Add PRMBYREF error.
!!  06-Mar-90   B. Martin       Add CONFLICTATTR.
!!  26-Feb-90   D.Annicchiarico Change OPENFLDLAST to CFMTFLDLAST.
!!  16-Feb-90   D.Annicchiarico More arm checks and other instance checks.
!!  08-Feb-90   J. Harrow       Add messages for illegal attribute usage.
!!  06-Feb-90   B. Martin       Add SIZEARRTYPE.
!!  31-Jan-90   D.Annicchiarico Changed [npb], [v1_array] msgs, added 2 new msgs;
!!                              removed many obsolete msgs; changed some text.
!!  15-Jan-90   D.Annicchiarico Add a bunch of semantic checking messages.
!!  11-Jan-90   B. Martin       Add CONSTTYPE.
!!  21-Dec-89   D.Annicchiarico Added messages related to ACF processing.
!!  15-Dec-89   D.Annicchiarico Put in registered facility number.
!!  10-Nov-89   D.Annicchiarico Added IDTOOLONG and MULATTRDEF.
!!  18-Oct-89   D.Annicchiarico Original version.
!!
!!  %a%private_end
!!
!--

.FACILITY   IDL,1091 /PREFIX=NIDL_


.SEVERITY   SUCCESS

!+
! These two symbols must have the same integer value.
! NOTE: Add new messages only to the bottom of this file.
!-
.LITERAL NIDL_MESSAGE_VERSION_USED=3077
    MESSAGE_VERSION <3077>/FAO_COUNT=0


.SEVERITY   INFORMATIONAL

    CMDERR          <Use !AZ for list of command options> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CMDERR
!     [msg_text]Use <v>(command-option) for list of command options
!     {severity_level}INFO
!     [msg_exp]Errors on the command line prevent the IDL compiler from
!     [msg_exp]executing.
!     [msg_act]Invoke the IDL compiler with the indicated command option to
!     [msg_act]get a list of valid command options, then correct the error.
!     [end_msg]

    DEFAUTOHAN      <Operation !AZ has no binding handle parameter; [auto_handle] assumed> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]DEFAUTOHAN
!     [msg_text]Operation <v>(name) has no binding handle parameter; <kw>([auto_handle]) assumed
!     {severity_level}INFO
!     [msg_exp]When an operation uses parametric binding, its first parameter
!     [msg_exp]is a handle parameter that determines the location of a
!     [msg_exp]server of the interface.  Operations whose first parameter
!     [msg_exp]is not of type <kw>(handle_t) and does not have the <kw>([handle])
!     [msg_exp]attribute are assumed to use nonparametric binding
!     [msg_exp]known as <kw>(auto_handle), whereby a binding is automatically
!     [msg_exp]established by the RPC runtime library.
!     [msg_act]See the documentation on the various binding techniques
!     [msg_act]to determine the best method for your application.
!     [end_msg]

    FILESOURCE      <File !AZ, line !UL: !AZ> /FAO_COUNT=3

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]FILESOURCE
!     [msg_text]File <v>(filename), line <v>(number): <v>(source text)
!     {severity_level}INFO
!     [msg_exp]Identifies the filename, source line number, and source
!     [msg_exp]text associated with an error.
!     [msg_act]None.  This is an informational message.
!     [end_msg]

    FLOATPROM       <Must compile stubs with ANSI C compiler to avoid promotion of float to double in operation !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]FLOATPROM
!     [msg_text]Must compile stubs with ANSI C compiler to avoid promotion of float to double
!     {severity_level}INFO
!     [msg_exp]The operation contains a <kw>(float) parameter passed by value.
!     [msg_exp]Non-ANSI C compilers that do not support function prototypes,
!     [msg_exp]automatically promote <kw>(float) to <kw>(double).  Function
!     [msg_exp]prototypes within generated C stubs are conditional on the
!     [msg_exp]IDL_PROTOTYPES preprocessor symbol.  When you compile a
!     [msg_exp]stub with a non-ANSI C compiler, IDL_PROTOTYPES must not be
!     [msg_exp]defined (causing the non-prototyped definitions to compile)
!     [msg_exp]and thus causing <kw>(float) to <kw>(double) promotion.  This breaks
!     [msg_exp]the marshalling logic in the stub, that expects a <kw>(float), not
!     [msg_exp]a <kw>(double).
!     [msg_act]Problems do not occur
!     [msg_act]as long as you compile stubs with an ANSI C compiler.  However,
!     [msg_act]to maximize portability, it is highly recommended that you
!     [msg_act]modify any <kw>(float) parameter passed
!     [msg_act}by value to either a <kw>(double)
!     [msg_act]passed by value or a <kw>(float) passed by reference. This
!     [msg_act]message is informational.
!     [end_msg]

    IMPORTIDL       <Importing IDL file !AZ> /FAO_COUNT=1

    INCLCREATE      <Creating include file !AZ> /FAO_COUNT=1

    LEGALVALS       <Legal values are:>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]LEGALVALS
!     [msg_text]Legal values are:
!     {severity_level}INFO
!     [msg_exp]Identifies a list of legal values for a command option.
!     [msg_act]Re-enter the option using one of the legal values.
!     [end_msg]

    LINEFILE        <File !AZ, line !UL> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]LINEFILE
!     [msg_text]File <v>(filename), line <v>(number)
!     {severity_level}INFO
!     [msg_exp]Identifies the filename and source line number
!     [msg_exp]associated with an error.
!     [msg_act]None. This is an informational message.
!     [end_msg]

    NAMEDECLAT      <Name !AZ is declared in file !AZ, line !UL> /FAO_COUNT=3

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]NAMEDECLAT
!     [msg_text]Name <v>(identifier) is declared in file <v>(filename), line <v>(number)
!     {severity_level}INFO
!     [msg_exp]Gives the source file and line
!     [msg_exp]number of source text to help you diagnose the
!     [msg_exp]previous error.
!     [msg_act]None. This is an informational message.
!     [end_msg]

    NAMEREFAT       <Name is referenced in file !AZ, line !UL> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]NAMEDECLAT
!     [msg_text]Name is referenced in file <v>(filename), line <v>(number)
!     {severity_level}INFO
!     [msg_exp]Gives the source file and line
!     [msg_exp]number of source text to help you diagnose the
!     [msg_exp]previous error.
!     [msg_act]None. This is an informational message.
!     [end_msg]

    NEWUUID         <!AZ> /FAO_COUNT=1


    OPTIONSTABLE    <Options table:> /FAO_COUNT=0

!     {engineer_name}Tom
!     {audience}programmer
!     [begin_msg]
!     [ident]OPTIONSTABLE
!     [msg_text]Options table:
!     {severity_level}INFO
!     [msg_exp]Caption for options table printed in usage message
!     [msg_epx]or in response to -<kw>(confirm) option.
!     [msg_act]None. This is an informational message.
!     [end_msg]

    PROCESSACF      <Processing attribute configuration file !AZ> /FAO_COUNT=1

    RUNCPP          <Running C preprocessor !AZ> /FAO_COUNT=1

    STUBCOMPILE     <Compiling stub module !AZ> /FAO_COUNT=1

    STUBCREATE      <Creating stub module !AZ> /FAO_COUNT=1

    STUBDELETE      <Deleting stub module !AZ> /FAO_COUNT=1

    TYPEREPAS       <Type !AZ has a represent_as data type !AZ> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]TYPEREPAS
!     [msg_text]Type <v>(name1) has a <kw>(represent_as) data type <v>(name2)
!     {severity_level}INFO
!     [msg_exp]IDL does not allow a data type that has a <kw>([represent_as]) type
!     [msg_exp]to itself be used as a <kw>([represent_as]) type in another type
!     [msg_exp]definition.
!     [msg_act]The message gives
!     [msg_act]information to help you locate the problem.
!     [end_msg]

    USAGE           <Usage: idl filename [options]>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]USAGE
!     [msg_text]Usage: idl filename [options]
!     {severity_level}INFO
!     [msg_exp]Lists the format of the <kw>(idl) command
!     [msg_exp]that invokes the IDL compiler.
!     [msg_act]Use the <kw>(-confirm) option to see a list of all
!     [msg-act]valid options. Study the available options
!     [msg_act]to determine those you need.
!     [end_msg]

    VERSION         <DCE IDL compiler version !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]VERSION
!     [msg_text]DCE IDL compiler version <v>(version)
!     {severity_level}INFO
!     [msg_exp]Identifies the current version of the IDL compiler.
!     [msg_act]Use this version number when submitting bug reports.
!     [end_msg]


.SEVERITY   WARNING

    DUPPROTOCOL     <Duplicate Protocol specification in endpoint list; "!AZ" ignored>/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]DUPPROTOCOL
!     [msg_text]Duplicate protocol specification in endpoint list; "<v>(endpoint)" ignored
!     {severity_level}WARNING
!     [msg_exp]Each protocol can be listed at most once in the endpoint list.  The specified
!     [msg_exp]endpoint specification duplicates another
!     [msg_exp]endpoint specification and is ignored.
!     [msg_act]Use each protocol at most once in the endpoint list.
!     [end_msg]

    ENDPOINTSYNTAX    <Syntax error in endpoint specification "!AZ">/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]DUPPROTOCOL
!     [msg_text]Syntax error in endpoint specification <v>(endpoint)
!     {severity_level}WARNING
!     [msg_exp]The syntax of the string describing the endpoint must be of the
!     [msg_exp]form <v>(protocol:[endpoint]).
!     [msg_exp]Although not in the expected format, the string <v>(endpoint)
!     [msg_exp]is assumed to be the desired endpoint specification
!     [msg_exp]and is put into the generated stubs.
!     [msg_act]Correct the syntax of the <kw>(endpoint) specification.
!     [end_msg]

    EXTRAPUNCT <Extraneous punctuation character "!AZ" ignored >/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]EXTRAPUNCT
!     [msg_text]Extraneous punctuation character <v>(character) ignored
!     {severity_level}WARNING
!     [msg_exp]A punctuation character was found in a location
!     [msg_exp]where it is not allowed in the IDL language syntax.
!     [msg_act]If it is only an extra puncutation character,
!     [msg_act]remove it.  If the character appears correct, then the
!     [msg_act]error may be caused by another nearby syntax error.
!     [msg_act]Correct the other syntax errors and recompile the interface.
!     [end_msg]

    FPHANATTR       <The [handle] attribute of a parameter in a function pointer declaration is ignored>

!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]FPHANATTR
!     {msg_text]The <kw>([handle]) attribute of a parameter in a function pointer declaration is ignored
!     {severity_level}WARNING
!     {msg_exp}A parameter of a function specified in a function pointer
!     {msg_exp}declaration is of a data type that has the <kw>([handle]) attribute.
!     {msg_exp}The <kw>([handle]) attribute is not meaningful in this context,
!     {msg_exp}and therefore is ignored.
!     {msg_act}This is a warning, so no action is required.  If you want to
!     {msg_act}remove the warning message, define a new data type without the
!     {msg_act}<kw>([handle]) attribute to use in your parameter declaration.
!     {end_msg]

    IDTOOLONG       <Identifier: !AZ too long; maximum is !UL chars> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]IDTOOLONG
!     [msg_text]Identifier: <v>(name) too long; maximum is <v>(number) chars
!     {severity_level}WARNING
!     [msg_exp]The length identifier exceeds the maximum number of characters
!     [msg_exp]allowed by IDL for portability.
!     [msg_act]Shorten the identifier name to within the allowed limit.
!     [msg_act]Make sure the shortened identifier name is unique.
!     [end_msg]

    INCLUDEXT       <include filename must not contain a file extension>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INCLUDEXT
!     [msg_text]<kw>(include) filename must not contain a file extension
!     {severity_level}WARNING
!     [msg_exp]The filename specified in an <kw>(include) statement in the
!     [msg_exp]Attribute Configuration File (ACF) must not contain a file
!     [msg_exp]extension.  In a future IDL version, you will be able to
!     [msg_exp]specify a programming language, and the IDL compiler will
!     [msg_exp]automatically append the appropriate extension.
!     [msg_act]Remove the file extension.  The current IDL compiler
!     [msg_act]always assumes an <kw>(.h) extension.
!     [end_msg]

    INCLTYPE        <ACF include statement advised for definition of type !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INCLTYPE
!     [msg_text]ACF <kw>(include) statement advised for definition of type <v>(name)
!     {severity_level}WARNING
!     [msg_exp]An Attribute Configuration File (ACF) declares a type that is
!     [msg_exp]not defined in an IDL file.  The type is referenced in the
!     [msg_exp]generated stub code, thus its definition needs to be included
!     [msg_exp]into the stub code as well.
!     [msg_act]Place an <kw>(include) statement in the ACF to cause your module
!     [msg_act]that defines the type to be included into the generated
!     [msg_act]header file.
!     [end_msg]

    INTSIZEREQ      <A size specifier is required; long assumed>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]INTSIZEREQ
!     [msg_text]A size specifier is required; <kw>(long) assumed
!     {severity_level}WARNING
!     [msg_exp]A size specifier is required when specifying the <kw>(int) data type.
!     [msg_act]Place a size specifier -- <kw>(short, small, long,)
!     [msg_act]or <kw>(hyper) -- before the <kw>(int) keyword.
!     [end_msg]

    LINENONSCAL     <The attributes [in_line] and [out_of_line] apply only to non-scalar types>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]LINENONSCAL
!     [msg_text]The attributes <kw>([in_line]) and <kw>([out_of_line]) apply only to non-scalar types
!     {severity_level}WARNING
!     [msg_exp]Scalar data types are always marshalled in-line, since there
!     [msg_exp]is performance degradation if they are
!     [msg_exp]marshalled out-of-line.
!     [msg_act]Do not use the <kw>([in_line]) and <kw>([out_of_line]) attributes
!     [msg_act]on scalar types.
!     [end_msg]

    MISSPTRCLASS    <Missing pointer class for !AZ; [ptr], [ref], or [unique] required>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]MISSPTRCLASS
!     [msg_text]Missing pointer class for <v>(name); <kw>([ptr]), <kw>([ref]), or <kw>([unique]) required
!     {severity_level}WARNING
!     [msg_exp]A pointer declaration does not have pointer class attribute
!     [msg_exp]and no <kw>([pointer_default]) attribute was  specified on the
!     [msg_exp]interface.
!     [msg_act]All pointers must have one of the pointer class attributes:
!     [msg_act]<kw>([ptr]), <kw>([ref]), or <kw>([unique]).
!     [msg_act]Either add one of these attributes
!     [msg_act]at the location of the pointer declaration, or add the
!     [msg_act]<kw>([pointer_default]) attribute to the interface to specify the
!     [msg_act]class to be used as the default for the module.
!     [end_msg]

    MIXEDARRATTR    <The attributes [max_is,length_is] or [size_is,last_is] used together>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MIXEDARRATTR
!     [msg_text]The attributes <kw>([max_is,length_is]) or <kw>([size_is,last_is]) used together
!     {severity_level}WARNING
!     [msg_exp]The <kw>([max_is]) attribute specifies the array's upper bound while
!     [msg_exp]the <kw>([size_is]) attribute specifies the total number of array
!     [msg_exp]elements.  Similarly, <kw>([last_is]) specifies the upper data
!     [msg_exp]limit while <kw>([length_is]) specifies the
!     [msg_exp]number of valid data elements.
!     [msg_act]Mixing the attributes can be incorrect.
!     [msg_act]Make sure the declaration is correct and change if necessary.
!     [end_msg]

    MULATTRDEF      <An attribute is defined multiple times>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]MULATTRDEF
!     [msg_text]An attribute is defined multiple times
!     {severity_level}WARNING
!     [msg_exp]An attribute is repeated multiple times in an attribute list.
!     [msg_act]Remove all but one occurrence of the offending attribute.
!     [end_msg]

    NAMETOOLONG     <Generated name too long at line !UL.>  /FAO_COUNT=1

!     {engineer_name}Tom
!     {audience}programmer
!     [begin_msg]
!     [ident]NAMETOOLONG
!     [msg_text]Generated name too long at line <v>(number).
!     {severity_level}WARNING
!     [msg_exp]The compiler generated an identifier that is too long
!     [msg_exp]for some standard C compiler implementations.
!     [msg_act]Shorten the name at the specified line.
!     [end_msg]

    NOCODEOPS       <At least one operation must have the [code] attribute>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]NOCODEOPS
!     [msg_text]At least one operation must have the <kw>([code]) attribute
!     {severity_level}WARNING
!     [msg_exp]All of the operations in the interface are marked so
!     [msg_exp]no code is generated for them in the stub modules.
!     [msg_act]If you used the ACF <kw>([nocode]) attribute on the interface,
!     [msg_act]be sure to specify <kw>([code]) on at least one operation.
!     [msg_act]To disable stub generation entirely use a command line option.
!     [end_msg]

    NOENDPOINT  <No endpoint can be found in the endpoint specification "!AZ">/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]NOENDPOINT
!     [msg_text]No endpoint can be found in the endpoint specification <v>(endpoint specification)
!     {severity_level}WARNING
!     [msg_exp]The syntax specified in the <kw>(endpoint) specification
!     [msg_exp]string does not contain
!     [msg_exp]an <kw>(endpoint) specification.
!     [msg_exp]The entire string is assumed to be the protocol name
!     [msg_exp]and the <kw>(endpoint) remains unspecified in the generated stubs.
!     [msg_act]Correct the syntax of the <kw>(endpoint) specification.
!     [end_msg]

    NONPORTCHAR     <Character constant cannot be portable across character sets>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]NONPORTCHAR
!     [msg_text]Character constant cannot be portable across character sets
!     {severity_level}WARNING
!     [msg_exp]You used an integer value to specify a character. This
!     [msg_exp]cannot be portable across different character sets
!     [msg_exp](ASCII and EBCDIC).
!     [msg_act]Consider your portability requirements.
!     [end_msg]

    NOSEMCHECK <Semantic checking prevented by other reported errors>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]NOSEMCHECK
!     [msg_text]Semantic checking prevented by other reported errors
!     {severity_level}WARNING
!     [msg_exp]One or more of the reported errors prevented the compiler from
!     [msg_exp]completing the semantic checking of the interface.  This
!     [msg_exp]can cause some semantic errors in the interface to go unreported.
!     [msg_act]Correct the reported errors and compile the interface again.
!     [end_msg]

    OLDUUID         <Old UUID format must be replaced with new format:>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OLDUUID
!     [msg_text]Old UUID format must be replaced with new format
!     {severity_level}WARNING
!     [msg_exp]The UUID specified in the interface attribute list is in an
!     [msg_exp]old format.
!     [msg_act]Although the old format is compatible with the new
!     [msg_act]format, we recommend that you switch to the new format.
!     [msg_act]If the input is an NCS Version 1 NIDL source file, process it with
!     [msg_act]the translator utility (the <kw>(nidl_to_idl) command) that
!     [msg_act]converts an NCS Version 1 NIDL source file into the
!     [msg_act]format expected by the IDL compiler.
!     [msg_act]You can also use the <kw>(uuidgen) command to convert the
!     [msg_act]old format UUID to the new format.
!     [msg_act]When you have the new format for the UUID,
!     [msg_act]compile the translated source file.
!     [end_msg]

    OUTDIRIGN       <Object file for !AZ placed in current working directory> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OUTDIRIGN
!     [msg_text]Object file for <v>(source file) placed in current working directory
!     {severity_level}WARNING
!     [msg_exp]A command option specifies a directory other than the current
!     [msg_exp]working directory in which to place a generated C source and
!     [msg_exp]object file.
!     [msg_exp]The IDL compiler does not place the file in the requested
!     [msg_exp]directory if you use the <kw>(-cc_cmd) argument on the command line.
!     [msg_exp]Also, the IDL compiler does not place the file in the requested
!     [msg_exp]directory on systems on which the mechanism it usually uses
!     [msg_exp]for the placement does not work.
!     [msg_act]IDL provides a command option that allows you to pass
!     [msg_act]command options to the C compiler.  Use the appropriate
!     [msg_act]C compiler option to assure that the object files are placed
!     [msg_act]in the correct directory.
!     [end_msg]

    REFUNIQUE       <The attribute [!AZ] applied on type with [!AZ] set, [!AZ] assumed>

!     {engineer_name}Beth
!     {audience}programmer
!     {begin_msg]
!     {ident]REFUNIQUE
!     {msg_text]The attribute <v>([name]) applied on type with <v>([name]) set, <v>([name]) assumed
!     {severity_level}WARNING
!     {msg_exp]The <kw>([ref]) and <kw>([unique]) attribute applied on a type have
!     {msg_exp]conflicting meanings.  The <kw>([ref]) attribute is assumed.
!     {msg_act]No action is required, but we suggest that you modify the
!     {msg_act]interface module and select the attribute that is desired.
!     {end_msg]

    SRVNOCODE       <The attribute [nocode] does not apply to server stub>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SRVNOCODE
!     [msg_text]The attribute <kw>([nocode]) does not apply to server stub
!     {severity_level}WARNING
!     [msg_exp]A server for an interface must support all of the routines in
!     [msg_exp]that interface, therefore, the <kw>([nocode]) ACF attribute is not
!     [msg_exp]valid when you use IDL to generate only server stub code.
!     [msg_act]This is a warning, so no action is required.  If you want to
!     [msg_act]remove the warning message, create a new ACF for the
!     [msg_act]interface that does not use the <kw>([nocode]) attribute.  Or
!     [msg_act]change the command line used so stub generation is not
!     [msg_act]limited to the server stub.
!     [end_msg]

    SYSIDLNAME      <File !AZ is a system-defined IDL filename>/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]SYSIDLNAME
!     [msg_text]File <v>(filename) is a system-defined IDL filename
!     {severity_level}WARNING
!     [msg_exp]The file <v>(filename) has the same name as an RPC-supplied
!     [msg_exp]system IDL file.  This can cause unexpected compilation errors,
!     [msg_exp]such as missing declarations of IDL predefined types.
!     [msg_act]Rename <v>(filename) so it does not conflict with any
!     [msg_act]of the system-defined IDL filenames.
!     [end_msg]


.SEVERITY   ERROR

!    ALIGNBYTEARRAY <The attribute [align] is valid only on an array of bytes>
!
!     {engineer_name}Jerry
!     {audience}programmer
!     {begin_msg]
!     {ident]ALIGNBYTEARRAY
!     {msg_text]The attribute <kw>([align]) is valid only on an array of bytes
!     {severity_level}ERROR
!     {msg_exp]The <kw>([align]) attribute is specified in a definition of a type that is not an array of bytes.
!     {msg_act]Remove the <kw>([align]) attribute from the declaration.
!     {msg_act]The <kw>([align]) attribute is used force arbitrary alignment for an array of bytes.  An
!     {msg_act]array of bytes is the IDL data type that is uninterpreted data.  If the data
!     {msg_act]inside the array of bytes requires alignment greater than one byte, specifying the <kw>([align]) attribute
!     {msg_act]forces the required alignment when the data reaches the remote node.
!     {end_msg]

    REQCMDOPT    <!AZ requires the !AZ command line option> /FAO_COUNT=2

!     {engineer_name}Ed
!     {audience}programmer
!     [begin_msg]
!     [ident]REQCMDOPT
!     [msg_text]<v>(name) requires the <v>(name) commandline option
!     {severity_level}ERROR
!     [msg_exp]There are particular options and constructs
!     [msg_exp]that are extensions to the language, these
!     [msg_exp]require a particular commandline option.
!     [msg_act]Add the required option to the command line
!     [end_msg]

    ACFINIDL   <ACF attributes in the IDL file, require the -app_config command line option>

!     {engineer_name}Ed
!     {audience}programmer
!     [begin_msg]
!     [ident]ACFINIDL
!     [msg_text]ACF attributes in the IDL file, require the -app_config command line option
!     {severity_level}ERROR
!     [msg_exp]Placing ACF attributes in the IDL file are
!     [msg_exp]extensions to the language, and require
!     [msg_exp]the -app_config command line option.
!     [msg_act]Add -app_config to the command line
!     [end_msg]

    ANONPIPE    <Pipes must be defined with a typedef> /FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]ANONPIPE
!     [msg_text]Pipes must be defined with a <kw>(typedef)
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler constructs routine names
!     [msg_exp]that are referenced by generated stub code
!     [msg_exp]from the name of any <kw>(pipe) data type.
!     [msg_exp]Therefore the type cannot be anonymous.
!     [msg_act]Declare the <kw>(pipe) data type with a <kw>(typedef)
!     [msg_act]so the <kw>(pipe) has a name associated with it.
!     [end_msg]

    ANONTYPE    <Declaration of !AZ cannot contain an anonymous type> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ANONTYPE
!     [msg_text]Declaration of <v>(name) cannot contain an anonymous type
!     {severity_level}ERROR
!     [msg_exp]The type specification for the parameter, field or <kw>(union) arm
!     [msg_exp]<v>(name) contains a data type that is an anonymous <kw>(struct) or
!     [msg_exp]<kw>(union).  Since such a type is unique
!     [msg_exp]and not compatible with any other type,
!     [msg_exp]the generated stubs cannot generate code to access the type.
!     [msg_act]Move the <kw>(struct) or <kw>(union) declaration into
!     [msg_act]a <kw>(typedef)  and modify the
!     [msg_act]declaration of <v>(name) to use the newly created type name.
!     [end_msg]

    ARMREFPTR       <An arm of a union cannot be or contain a [ref] pointer>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARMREFPTR
!     [msg_text]An arm of a <kw>(union) cannot be or contain a <kw>([ref]) pointer
!     {severity_level}ERROR
!     [msg_exp]IDL-generated server stub code must allocate storage for
!     [msg_exp]objects pointed to by <kw>([ref]) pointers.  IDL does not allow
!     [msg_exp]a <kw>([ref]) pointer within a <kw>(union) because the valid
!     [msg_exp]arm of the <kw>(union), and therefore the
!     [msg_exp]object for which storage is allocated, is not
!     [msg_exp]known at compile time.
!     [msg_act]Change the pointer within the <kw>(union) declaration to a full pointer.
!     [end_msg]

    ARRELEMCFMT     <Array elements cannot be conformant arrays or conformant structures>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRELEMCFMT
!     [msg_text]Array elements cannot be conformant arrays or structures
!     {severity_level}ERROR
!     [msg_exp]Array elements must be of fixed size.
!     [msg_act]If the element type is an array, make sure that the array has
!     [msg_act]fixed bounds.  If the element type is a structure, make sure
!     [msg_act]that any arrays in the structure have fixed bounds.
!     [end_msg]

    ARRELEMCTX      <Array elements cannot be context handles>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRELEMCTX
!     [msg_text]Array elements cannot be context handles
!     {severity_level}ERROR
!     [msg_exp]Arrays of context handles are not allowed.
!     [msg_act]Change the definition of the array so its elements are
!     [msg_act]not context handles.  If you want to declare
!     [msg_act]an array of context handles, you must use alternative means.
!     [end_msg]

    ARRELEMPIPE     <Array elements cannot be pipes>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRELEMPIPE
!     [msg_text]Arrays of pipes are not allowed
!     {severity_level}ERROR
!     [msg_exp]Array elements cannot be pipes.
!     [msg_act]Change the definition of the array so its elements are
!     [msg_act]not of a <kw>(pipe) data type.  If you want to declare
!     [msg_act]an array of pipes, you must use alternative means.
!     [end_msg]

    ARRSIZEINFO     <Array size information required for !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRSIZEINFO
!     [msg_text]Array size information required for <v>(identifier)
!     {severity_level}ERROR
!     [msg_exp]The IDL code uses an array with bounds that are not fixed
!     [msg_exp]and does not specify the appropriate size attributes.
!     [msg_act]Modify the array declaration to include the appropriate
!     [msg_act]<kw>([max_is]) or <kw>([size_is]) attribute.  
!     [msg_act]If the array is a parameter, use an additional parameter to
!     [msg_act]specify size information.  If the array is a field
!     [msg_act]in a structure, use an additional field to
!     [msg_act]specify size information.
!     [end_msg]
!
!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]ARRSIZEINFO
!     [msg_text]Array size information required for <v>(identifier)
!     {severity_level}ERROR
!     [msg_exp]The IDL code uses an array with bounds that are not fixed 
!     [msg_exp]and does not specify the appropriate size attributes.
!     [msg_act]Modify the array declaration to include the appropriate
!     [msg_act]<kw>([min_is]), <kw>([max_is]), or <kw>([size_is]) attribute.  
!     [msg_act]If the array is a parameter, use an additional parameter to
!     [msg_act]specify the missing size information.  If the array is a field
!     [msg_act]in a structure, use an additional field to
!     [msg_act]specify the missing size information.
!     [end_msg_DEC]

    ARRCFMTDIM      <Arrays can be conformant in the first dimension only>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRCFMTDIM
!     [msg_text]Arrays can be conformant in the first dimension only
!     {severity_level}ERROR
!     [msg_exp]IDL only allows an array to be conformant in its first dimension.
!     [msg_act]Modify the array declaration so dimensions other than
!     [msg_act]the first have fixed bounds.
!     [end_msg]

    ARRPTRPRM       <An array with a pointer attribute is valid only as a parameter>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRPTRPRM
!     [msg_text]An array with a pointer attribute is valid only as a parameter
!     {severity_level}ERROR
!     [msg_exp]An array that is a parameter in an operation can have a pointer
!     [msg_exp]attribute because arrays are implicitly passed by reference
!     [msg_exp]pointer.  Otherwise, an array cannot have a pointer attribute.
!     [msg_act]Remove the pointer attribute from the parameter or type
!     [msg_act]definition, or define a new type without a pointer attribute.
!     [end_msg]

    ARRSYNTAX       <Use array syntax to declare multidimensional arrays>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRSYNTAX
!     [msg_text]Use array syntax to declare multidimensional arrays
!     {severity_level}ERROR
!     [msg_exp]An IDL declaration attempts to use mixed pointer (<kw>(*)) and
!     [msg_exp]array (<kw>([])) syntax to define a multidimensional array.
!     [msg_exp]The interpretation that an asterisk (<kw>(*)) is used
!     [msg_exp]to represent an array
!     [msg_exp]is due to the presence of one or more of the
!     [msg_exp]<kw>([max_is]), or <kw>([size_is]) attributes.  It is
!     [msg_exp]ambiguous whether size attributes apply to the pointer or the
!     [msg_exp]array, thus IDL does not allow mixing pointer and array syntax
!     [msg_exp]when size attributes are present.
!     [msg_act]The IDL compiler requires that you make such declarations
!     [msg_act]using only array <kw>([]) syntax.  If you do not want to
!     [msg_act]declare a multidimensional array, you used the
!     [msg_act]<kw>([max_is]), or <kw>([size_is]) attribute incorrectly.  They apply
!     [msg_act]only to arrays with bounds that are not fixed, and the declared
!     [msg_act]array has fixed bounds.
!     [end_msg]

    ARRVARYDIM      <Arrays can be varying in the first dimension only>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRVARYDIM
!     [msg_text]Arrays can be varying in the first dimension only
!     {severity_level}ERROR
!     [msg_exp]IDL only allows an array to be varying in its first dimension.
!     [msg_act]Remove the <kw>([first_is]), <kw>([last_is]),
!     [msg_act]or <kw>([length_is])
!     [msg_act]attributes that refer to dimensions other than the first.
!     [end_msg]

    ARRXMITOPEN     <Arrays with [transmit_as] cannot be conformant or varying>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRXMITOPEN
!     [msg_text]Arrays with <kw>([transmit_as]) cannot be conformant or varying
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow a conformant or varying array with the
!     [msg_exp]<kw>([transmit_as]) attribute.
!     [msg_act]Change the declaration so the array is of fixed size and
!     [msg_act]has no data limit attributes, or remove the <kw>([transmit_as]) attribute.
!     [end_msg]

    ATTRTRANS       <The attribute is no longer allowed in this context; use translator>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]ATTRTRANS
!     [msg_text]The attribute is no longer allowed in this context; use translator
!     {severity_level}ERROR
!     [msg_exp]An attribute list is encountered in an invalid context.
!     [msg_exp]In NCS Version 1 NIDL, attributes are allowed in this position.
!     [msg_exp]You may be compiling an NCS Version 1 NIDL source file.
!     [msg_act]If the input is an NCS Version 1 NIDL source file, process it with the
!     [msg_act]<kw>(nidl_to_idl) translator utility that
!     [msg_act]converts an NCS Version 1 NIDL source file into the format expected by
!     [msg_act]the IDL compiler.  Then compile the translated source file.
!     [msg_act]Otherwise, move the attributes to a valid location.
!     [end_msg]

    ATTRVALIND      <An attribute variable cannot be an indirect field reference>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ATTRVALIND
!     [msg_text]An attribute variable cannot be an indirect field reference
!     {severity_level}ERROR
!     [msg_exp]An indirect field reference is used in a field attribute;
!     [msg_exp]indirection is not allowed in this context.
!     [msg_act]Remove the indirection.  For example, if the referenced field was
!     [msg_act]<kw>(long *sp;) and the attribute expression was
!     [msg_act]<kw>([size_is(*sp)]), then change the field to
!     [msg_act]<kw>(long s;) and the attribute expression to
!     [msg_act]<kw>([size_is(s)]).
!     [end_msg]

    BADTAGREF       <Invalid use of tag !AZ> /FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]BADTAGREF
!     [msg_text]Invalid use of tag <v>(name)
!     {severity_level}ERROR
!     [msg_exp]You used a previously defined tag name in a declaration of a different type.
!     [msg_exp]Tag names can be used for
!     [msg_exp]a <kw>(struct) or <kw>(union) declaration, but not both.
!     [msg_act]Either make sure that each use of the tag name specifies the same type, or
!     [msg_act]use different tag names with each type.
!     [end_msg]

    BROADPIPE       <The attribute [broadcast] is not valid on an operation with pipes>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]IDEMPIPE
!     [msg_text]The attribute <kw>([broadcast]) is not valid on an operation with pipes
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow pipes to be used in <kw>([broadcast])
!     [msg_exp]operations.
!     [msg_act]Remove the <kw>([broadcast]) attribute from the operation,
!     [msg_act]or remove the <kw>(pipe) parameter from the operation.
!     [end_msg]

    CASEDISCTYPE    <Case label type does not agree with discriminator type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CASEDISCTYPE
!     [msg_text]Case label type does not agree with discriminator type
!     {severity_level}ERROR
!     [msg_exp]The value of a <kw>(case) expression in a discriminated <kw>(union)
!     [msg_exp]is not the same data type as the discriminator variable
!     [msg_exp]in the <kw>(switch) clause.
!     [msg_act]Change the discriminator declaration or the <kw>(case) expression
!     [msg_act]so the data types match.
!     [end_msg]

    CASECONENUM     <Case label must be a constant from the enumeration of the discriminant>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CASECONENUM
!     [msg_text]Case label must be a <kw>(constant) from the enumeration of the discriminant
!     {severity_level}ERROR
!     [msg_exp]The value of a <kw>(case) expression in a discriminated <kw>(union)
!     [msg_exp]is from a different enumeration data type than the enumeration
!     [msg_exp]data type of the discriminator variable in the <kw>(switch) clause.
!     [msg_act]Change the discriminator declaration or the <kw>(case) expression
!     [msg_act]so the enumeration data types match.
!     [end_msg]

    CFMTARRREF      <Structures containing conformant arrays must be passed by reference>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CFMTARRREF
!     [msg_text]Structures containing conformant arrays must be passed by reference
!     {severity_level}ERROR
!     [msg_exp]The size of a structure that contains a conformant array is
!     [msg_exp]not a compile-time constant.  It is not possible to pass such
!     [msg_exp]a structure by value.
!     [msg_act]Change the parameter declaration to pass the structure by
!     [msg_act]reference by adding an asterisk (<kw>(*)) to the
!     [msg_act]left of the parameter name.
!     [end_msg]

    CFMTBASETYP     <The base type of a pipe cannot be a conformant type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CFMTBASETYP
!     [msg_text]The base type of a <kw>(pipe) cannot be a conformant type
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow pipes of any data type of size that is not fixed.
!     [msg_act]Change your <kw>(pipe) type definition to use a fixed array or
!     [msg_act]some other construct of fixed size.
!     [end_msg]

    CFMTFLDLAST     <A conformant field must be the last field in the structure>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CFMTFLDLAST
!     [msg_text]A conformant field must be the last field in the structure
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler restricts arrays with bounds that are not fixed
!     [msg_exp]and that occur in structures to only the last field
!     [msg_exp]in the structure.
!     [msg_act]If the structure contains more than one
!     [msg_act]conformant array, remove all but one of them.
!     [msg_act]Move the conformant array so it is the last field in the
!     [msg_act]structure definition.
!     [end_msg]

    CFMTUNION       <Conformant array or structure is invalid within a union>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CFMTUNION
!     [msg_text]Conformant array or structure is invalid within a <kw>(union)
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler does not allow an array with bounds  that are not fixed
!     [msg_exp]in a discriminated <kw>(union).  Each member of a <kw>(union) must
!     [msg_exp]be of fixed size.
!     [msg_act]It is possible to have a <kw>(union) case that is a full <kw>([ptr])
!     [msg_act]pointer to a conformant array or conformant structure.
!     [msg_act]Change the declaration and
!     [msg_act]associated code accordingly.
!     [end_msg]

    CONFHANATTR     <Interface attributes [auto_handle] and [implicit_handle] cannot occur together>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CONFHANATTR
!     [msg_text]Interface attributes <kw>([auto_handle]) and <kw>([implicit_handle]) cannot occur together
!     {severity_level}ERROR
!     [msg_exp]The <kw>([auto_handle]) and <kw>([implicit_handle]) attributes
!     [msg_exp]are two distinct
!     [msg_exp]mechanisms for managing the binding between client and server
!     [msg_exp]at runtime.  You cannot specify both of these attributes.
!     [msg_act]See the documentation to determine which binding mechanism
!     [msg_act]to use.  Then remove one of the conflicting attributes
!     [msg_act]from the interface attribute list in the Attribute
!     [msg_act]Configuration File (ACF).
!     [end_msg]

    CONFLINEATTR    <The attributes [in_line] and [out_of_line] cannot occur together>

!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]CONFLINEATTR
!     {msg_text]The attributes <kw>([in_line]) and <kw>([out_of_line]) cannot occur together
!     {severity_level}ERROR
!     {msg_exp]Not currently used.  { in place of [ cause MSG.COM to ignore.
!     {msg_exp]
!     {msg_act]
!     {msg_act]
!     {end_msg]

    CONFLICTATTR    <The attributes [!AZ] and [!AZ] cannot occur together>

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]CONFLICTATTR
!     [msg_text]The attributes <v>([attribute1]) and <v>([attribute2]) cannot occur together
!     {severity_level}ERROR
!     [msg_exp]The specified attributes are conflicting and only one or the other
!     [msg_exp]can be specified.
!     [msg_act]Remove one of the attributes.
!     [end_msg]

    CONFREPRTYPE    <The [represent_as] type !AZ conflicts with previously defined !AZ> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CONFREPRTYPE
!     [msg_text]The <kw>([represent_as]) type <v>(name) conflicts with previously defined <v>(name)
!     {severity_level}ERROR
!     [msg_exp]The attribute configuration file (ACF) contains conflicting
!     [msg_exp]<kw>([represent_as]) clauses for the same data type.
!     [msg_act]Remove one of the <kw>([represent_as]) clauses for that data type.
!     [end_msg]

    CONSTNOTFND     <Constant name: !AZ not found> /FAO_COUNT=1

!     {engineer_name}Beth
!     {audience}programmer
!     [begin_msg]
!     [ident]CONSTNOTFND
!     [msg_text]Constant name: <v>(name) not found
!     {severity_level}ERROR
!     [msg_exp]The named <kw>(constant) is not defined.
!     [msg_act]Modify the interface and specify a known constant.
!     [end_msg]

    CONSTTYPE       <Constant type does not agree with constant expression>

!     {engineer_name}Beth
!     {audience}programmer
!     [begin_msg]
!     [ident]CONSTTYPE
!     [msg_text]Constant type does not agree with constant expression
!     {severity_level}ERROR
!     [msg_exp]You used a constant expression that is inappropriate
!     [msg_exp]for the constant type.
!     [msg_act]Modify the constant definition so the expression
!     [msg_act]matches the type.
!     [end_msg]

    HYPERCONST      <A hyper constant type is not allowed>/FAO_COUNT=0

!     {engineer_name}Beth
!     {audience}programmer
!     [begin_msg]
!     [ident]HYPERCONST
!     [msg_text]A <kw>(hyper) constant type is not allowed
!     {severity_level}ERROR
!     [msg_exp]<kw>(hyper) constant is not allowed.
!     [msg_act]Modify the <kw>(constant) type to one of the supported types:
!     [msg_act]<kw>(long, char, boolean, void *,) or <kw>(char *).
!     [end_msg]

    MISSONINTER     <Missing "}" on interface declaration>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]MISSONINTER
!     [msg_text]Missing <kw>(}) on interface declaration
!     {severity_level}ERROR
!     [msg_exp]The closing brace on the interface declaration was not found.
!     [msg_exp]It may have been omitted, the nesting of other braces may be incorrect, or
!     [msg_exp]some other syntax error may cause IDL to overlook a brace.
!     [msg_act]Add the closing brace if missing, otherwise make sure all other braces
!     [msg_act]are specified in pairs, and there are no other syntax errors.
!     [end_msg]


    MISSONATTR      <Missing "]" on attribute list>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]MISSONATTR
!     [msg_text]Missing <kw>(]) on attribute list
!     {severity_level}ERROR
!     [msg_exp]The closing bracket on an attribute list was not found.
!     [msg_exp]It may have been omitted, or
!     [msg_exp]some other syntax error may cause IDL to overlook the bracket.
!     [msg_act]Add the closing bracket if it is missing, otherwise make sure
!     [msg_act]there are no other syntax errors.
!     [end_msg]

    MISSONARR       <Missing "]" on array bound>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]MISSONARR
!     [msg_text]Missing <kw>(]) on array bound
!     {severity_level}ERROR
!     [msg_exp]The closing bracket on an array bound specification was not found.
!     [msg_exp]It may have been omitted, or
!     [msg_exp]some other syntax error may cause IDL to overlook the bracket.
!     [msg_act]Add the closing bracket if it is missing, otherwise make sure
!     [msg_act]there are no other syntax errors.
!     [end_msg]

    MISSONOP        <Missing result type on operation declaration>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]MISSONOP
!     [msg_text]Missing result type on operation declaration
!     {severity_level}ERROR
!     [msg_exp]No data type was found for the result of an operation
!     [msg_exp]while processing what appeared to be an
!     [msg_exp]operation declaration.  All operations must have an explicit result type.
!     [msg_act]If the result type of an operation is omitted, explicitly
!     [msg_act]specify it. If no result is returned from the operation,
!     [msg_act]specify <kw>(void) as the result type.
!     [msg_act]This error can also occur due to a previous syntax error.
!     [end_msg]

    MISSONPARAM     <Missing type on parameter declaration>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]MISSONPARAM
!     [msg_text]Missing type on parameter declaration
!     {severity_level}ERROR
!     [msg_exp]No data type is found for the parameter.
!     [msg_act]If the parameter type is omitted, explicitly specify it.
!     [msg_act]This error can also occur due to a previous syntax error.
!     [end_msg]

    CTXBASETYP      <The base type of a pipe cannot be a [context_handle] type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CTXBASETYP
!     [msg_text]The base type of a <kw>(pipe) cannot be a <kw>([context_handle]) type
!     {severity_level}ERROR
!     [msg_exp]A <kw>([context_handle]) type cannot be used as the base type
!     [msg_exp]of a <kw>(pipe).
!     [msg_act]Remove the invalid declaration, or change it so the
!     [msg_act]base type of the <kw>(pipe) is not a <kw>([context_handle]) type.
!     [end_msg]

    CTXPTRVOID      <Attribute [context_handle] only applies to void * types>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CTXPTRVOID
!     [msg_text]Attribute <kw>([context_handle]) only applies to <kw>(void *) types
!     {severity_level}ERROR
!     [msg_exp]The attribute <kw>([context_handle]) indicates that the data is a
!     [msg_exp]pointer-sized object that serves as the handle, or method
!     [msg_exp]of accessing, some object.  To the called operation, the data is often
!     [msg_exp]an address of a structure in memory, although it need not be.
!     [msg_exp]A context handle is opaque to, and must never be written by,
!     [msg_exp]the caller code.  To emphasize this, IDL previously required that
!     [msg_exp]context handles be defined as type <kw>(void *).
!     [msg_exp]IDL has since been relaxed to also allow a context handle type
!     [msg_exp]to be defined as a pointer to a structure type by tag name, e.g.
!     [msg_exp]<v>(typedef [context_handle] struct opaque_struct * opaque_ch_t).
!     [msg_act]Change the declaration with the <kw>([context_handle]) attribute
!     [msg_act]to data type <kw>(void *) or to a <kw>(struct *) data type
!     [msg_act]similar to the example above.
!     [end_msg]

    CTXSTRFLD       <Context handles are not valid as structure fields>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CTXSTRFLD
!     [msg_text]Context handles are not valid as structure fields
!     {severity_level}ERROR
!     [msg_exp]A context handle is not allowed as a field of a structure.
!     [msg_act]Pass a context handle as a separate parameter to an operation,
!     [msg_act]rather than embedding it as a field of a structure.
!     [end_msg]

    CTXUNIMEM       <Context handle is not valid as a member of a union>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CTXUNIMEM
!     [msg_text]Context handle is not valid as a member of a <kw>(union)
!     {severity_level}ERROR
!     [msg_exp]A context handle cannot be a member of a <kw>(union).
!     [msg_act]Pass a context handle as a separate parameter to an operation,
!     [msg_act]rather than embedding it as a member of a <kw>(union).
!     [end_msg]

    DEFNOTCOMP  <Definition of tag !AZ is not yet complete> /FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]DEFNOTCOMP
!     [msg_text]Definition of tag <v>(name) is not yet complete
!     {severity_level}ERROR
!     [msg_exp]The tag <v>(name) is forward referenced in a context that requires it to be
!     [msg_exp]completely defined.  References
!     [msg_exp]to <kw>(struct) or <kw>(union) types by means of tag
!     [msg_exp]name before the <kw>(struct) or <kw>(union) is
!     [msg_exp]completely defined is only allowed in
!     [msg_exp]contexts in which the size is not needed (a pointer to the type or
!     [msg_exp]in a <kw>(typedef)).  Although IDL can process
!     [msg_exp]such references, the resulting
!     [msg_exp]stub cannot be compiled by most C compilers.
!     [msg_act]Move the declaration of the <kw>(struct) or <kw>(union) prior to this
!     [msg_act]reference.
!     [end_msg]

    DUPCASEVAL      <Duplicate case label value>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]DUPCASEVAL
!     [msg_text]Duplicate <kw>(case) label value
!     {severity_level}ERROR
!     [msg_exp]A discriminated <kw>(union) contains more than one <kw>(case) label
!     [msg_exp]with the same value.  Each <kw>(case) label value in a discriminated
!     [msg_exp]<kw>(union) can be used at most once.
!     [msg_act]Remove one of the conflicting <kw>(case) labels.
!     [end_msg]

    EOF             "Unexpected end-of-file" /FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]EOF
!     [msg_text]Unexpected end-of-file
!     {severity_level}ERROR
!     [msg_exp]The end of the source file was encountered before
!     [msg_exp]the end of the interface definition.
!     [msg_exp]The source file may be incomplete.
!     [msg_act]Complete the interface definition.
!     [msg_act]
!     [end_msg]

    EOFNEAR         "Unexpected end-of-file near '!AD'" /FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]EOFNEAR
!     [msg_text]Unexpected end-of-file near <v>(source text)
!     {severity_level}ERROR
!     [msg_exp]The end of the source file was encountered before
!     [msg_exp]the end of the interface definition.
!     [msg_exp]The source file may be incomplete.
!     [msg_act]Complete the interface definition.
!     [end_msg]

    ERRINATTR       <Syntax error in attribute list>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]ERRINATTR
!     [msg_text]Syntax error in attribute list
!     {severity_level}ERROR
!     [msg_exp]The syntax of the attribute list is not correct.  It must contain a list of
!     [msg_exp]attributes separated by commas.
!     [msg_act]Make sure the attribute list contains only
!     [msg_act]valid attribute names and values, that
!     [msg_act]their spellings are correct, and that they form a valid list.
!     [end_msg]

    ERRSTATDEF      <Data type !AZ must be defined in !AZ> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ERRSTATDEF
!     [msg_text]Data type <v>(name) must be defined in <v>(file)
!     {severity_level}ERROR
!     [msg_exp]You used a feature that requires a predefined data type that is
!     [msg_exp]normally present by default, but the data type was not found.
!     [msg_exp]An examples is a <kw>([comm_status]) or <kw>([fault_status])
!     [msg_exp]parameter, which requires the data type <kw>(error_status_t).
!     [msg_act]The compiler expected to find the type in the indicated file
!     [msg_act](usually <kw>(nbase.idl), which is automatically imported by
!     [msg_act]IDL).  Check that you are using the proper header files that were
!     [msg_act]installed with DCE.
!     [end_msg]

    FILENOTDIR      <File is not a directory: !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]FILENOTDIR
!     [msg_text]File is not a directory: <v>(filename)
!     {severity_level}ERROR
!     [msg_exp]A file that is not a directory is specified in a context
!     [msg_exp]where a directory is required; for example, as the command
!     [msg_exp]line option that specifies an output directory.
!     [msg_act]Specify a filename that is a directory.
!     [end_msg]

    FILENOTFND      <File !AZ not found> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]FILENOTFND
!     [msg_text]File <v>(filename) not found
!     {severity_level}ERROR
!     [msg_exp]A file, specified by the user either in the IDL source or
!     [msg_exp]ACF file, does not exist.
!     [msg_act]Check the filenames you specified.  Determine
!     [msg_act]the location of any imported or included files that are needed to
!     [msg_act]compile the IDL source file specified. If any are not in the
!     [msg_act]directories that the IDL compiler searches by default, you
!     [msg_act]must use a command line option to specify additional
!     [msg_act]directories to search.
!     [end_msg]

    FIRSTINATTR     <The [first_is] parameter must have the [in] attribute>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]FIRSTINATTR
!     [msg_text]The <kw>([first_is]) parameter must have the <kw>([in]) attribute
!     {severity_level}ERROR
!     [msg_exp]The <kw>([first_is]) attribute specifies a parameter that contains
!     [msg_exp]the lower data limit of a varying array.  Since the array has the
!     [msg_exp]<kw>([in]) attribute, the lower data limit
!     [msg_exp]parameter must also be <kw>([in])
!     [msg_exp]so the number of array elements to send from client to
!     [msg_exp]server is known.
!     [msg_act]Change the lower data limit parameter referenced by the
!     [msg_act]<kw>([first_is]) clause to have the <kw>([in]) attribute.
!     [end_msg]

    FIRSTYPEINT     <A [first_is] variable must be a small, short, or long integer>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]FIRSTYPEINT
!     [msg_text]A <kw>([first_is]) variable must be a <kw>(small), <kw>(short), or <kw>(long) integer
!     {severity_level}ERROR
!     [msg_exp]The <kw>([first_is]) attribute specifies a field
!     [msg_exp]or parameter that contains
!     [msg_exp]the lower data limit of a varying array.  Array data limits
!     [msg_exp]must be integers which are not <kw>(hyper).
!     [msg_act]Change the data limit field or parameter referenced by the
!     [msg_act]<kw>([first_is]) clause to integer data type.
!     [end_msg]

    FLOATCONSTNOSUP <Floating point constants not supported> /FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]FLOATCONSTNOSUP
!     [msg_text]Floating point constants not supported
!     {severity_level}ERROR
!     [msg_exp]Floating point constants cannot be specified in IDL.
!     [msg_act]Remove the <kw>(float) constant specification.
!     [end_msg]

    FPCFMTARR       <Function pointers are not valid as elements of conformant arrays>

!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg}
!     {ident}FPCFMTARR
!     {msg_text}Function pointers are not valid as elements of conformant arrays
!     {severity_level}ERROR
!     {msg_exp}Pointers to functions are not valid as elements of arrays
!     {msg_exp}with bounds  that are not fixed.
!     {msg_act}Modify the declaration of the array of function pointers
!     {msg_act}so the array has fixed bounds.
!     {end_msg}

    FPLOCINT        <Function pointers are allowed only in local interfaces>

!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]FPLOCINT
!     {msg_text]Function pointers are allowed only in local interfaces
!     {severity_level}ERROR
!     {msg_exp]The IDL compiler allows function pointers only in
!     {msg_exp]<kw>([local]) interfaces.
!     {msg_act]Remove the function pointer declarations, and consider
!     {msg_act]alternative ways to achieve that goal.
!     {end_msg]

    FPHANPRM        <Function pointer parameters cannot be of type handle_t>

!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]FPHANPRM
!     {msg_text]Function pointer parameters cannot be of type <kw>(handle_t)
!     {severity_level}ERROR
!     {msg_exp]The IDL source declares an object that is a pointer to a function,
!     {msg_exp]and one of the parameters to this function is of data type
!     {msg_exp]<kw>(handle_t).  When a server calls the function whose address
!     {msg_exp]is given by the function pointer parameter, it is always
!     {msg_exp]back to the original caller.  A binding parameter is
!     {msg_exp]thus invalid in this context.
!     {msg_act]Remove the <kw>(handle_t) parameter from the function pointer
!     {msg_act]declaration.
!     {end_msg]

    FPINPRM         <Function pointers in an operation must be [in] parameters>

!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg}
!     {ident}FPINPRM
!     {msg_text}Function pointers in an operation must be <kw>([in]) parameters
!     {severity_level}ERROR
!     {msg_exp}A parameter that is a pointer to a function can only be
!     {msg_exp}used as an <kw>([in]) parameter.  It cannot be an <kw>([out]) parameter
!     {msg_exp}or the operation result.
!     {msg_act}If the function pointer is a parameter, make sure it is <kw>([in])
!     {msg_act}only.  An operation result cannot be a function pointer.
!     {end_msg}

    FPPIPEBASE      <The base type of a pipe cannot be a function pointer>

!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]FPPIPEBASE
!     {msg_text]The base type of a <kw>(pipe) cannot be a function pointer
!     {severity_level}ERROR
!     {msg_exp]Function pointers are not valid as the base type of a <kw>(pipe).
!     {msg_act]Consider alternative ways to achieve the result.
!     {end_msg]

    FPSTRFLD        <Function pointers are not valid as structure fields>

!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]FPSTRFLD
!     {msg_text]Function pointers are not valid as structure fields
!     {severity_level}ERROR
!     {msg_exp]Function pointers are not allowed as fields of structures.
!     {msg_act]Consider alternative ways to achieve the result,
!     {msg_act]such as passing function pointers as separate parameters
!     {msg_act]to operations.
!     {end_msg]

    FPUNIMEM        <Function pointers are not valid as members of unions>

!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]FPUNIMEM
!     {msg_text]Function pointers are not valid as members of <kw>(union)s
!     {severity_level}ERROR
!     {msg_exp]A member of a discriminated <kw>(union) cannot be a function
!     {msg_exp]pointer.
!     {msg_act]Consider alternative ways to achieve the result,
!     {msg_act]such as passing function pointers as separate parameters
!     {msg_act]to operations.
!     {end_msg]

    FUNTYPDCL       <Function types are allowed only in local interfaces>
!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]FUNTYPDCL
!     {msg_text]Function types are allowed only in local interfaces
!     {severity_level}ERROR
!     {msg_exp]IDL does not allow the declaration of an item whose data type
!     {msg_exp]is a function except in interfaces with the <kw>([local])
!     {msg_exp]attribute.
!     {msg_act]Either add the <kw>([local]) attribute to the interface,
!     {msg_act]or remove the function type declaration and consider
!     {msg_act]alternative ways to achieve that goal.
!     {end_msg]

    HANARRELEM      <Array elements cannot be of type handle_t>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]HANARRELEM
!     [msg_text]Array elements cannot be of type <kw>(handle_t)
!     {severity_level}ERROR
!     [msg_exp]Data type <kw>(handle_t) is only meaningful when used as the data
!     [msg_exp]type of the first parameter in an operation.  It is used to
!     [msg_exp]establish a binding to a server of the interface.
!     [msg_act]Do not declare arrays of type <kw>(handle_t).
!     [end_msg]

    HANDLEIN        <A [handle] binding parameter must be [in] or [in,out]>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]HANDLEIN
!     [msg_text]A <kw>([handle]) binding parameter must be <kw>([in]) or <kw>([in,out])
!     {severity_level}ERROR
!     [msg_exp]A parameter of a data type with the <kw>([handle]) attribute
!     [msg_exp]as the first parameter
!     [msg_exp]in an operation is used to
!     [msg_exp]establish a binding to a server of the interface.
!     [msg_exp]It must be an <kw>([in]) or <kw>([in,out]) parameter.
!     [msg_act]Place the <kw>([in]) attribute on the <kw>([handle]) parameter.
!     [end_msg]

    HANDLEPTR       <Pointers to type handle_t are valid only in parameter declarations>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]HANDLEPTR
!     [msg_text]Pointers to type <kw>(handle_t) are valid only in parameter declarations
!     {severity_level}ERROR
!     [msg_exp]Data type <kw>(handle_t) is only meaningful when used as the data
!     [msg_exp]type of the first parameter in an operation.  It is used to
!     [msg_exp]establish a binding to a server of the interface.
!     [msg_act]Do not declare a pointer to data type <kw>(handle_t), except
!     [msg_act]as a parameter.
!     [end_msg]

    HANFIRSTPRM     <A handle_t as other than the first parameter requires a [transmit_as]>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]HANFIRSTPRM
!     [msg_text]A <kw>(handle_t) as other than the first parameter requires a <kw>([transmit_as])
!     {severity_level}ERROR
!     [msg_exp]Data type <kw>(handle_t) is only meaningful when used as the data
!     [msg_exp]type of the first parameter in an operation.  A bound handle
!     [msg_exp]is used to deliver the call to the correct server of the
!     [msg_exp]interface.  A bound <kw>(handle_t) parameter itself
!     [msg_exp]is not transmittable.  When the <kw>(handle_t) type is used
!     [msg_exp]in other than the first parameter, it must have a
!     [msg_exp]<kw>([transmit_as]) clause to convert it to a transmittable type.
!     [msg_act]Use data type <kw>(handle_t) only as the data
!     [msg_act]type of the first parameter in an operation,
!     [msg_act]unless the defined data type has a <kw>([transmit_as]) clause.
!     [end_msg]

    HANPIPEBASE     <The base type of a pipe cannot be handle_t>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]HANPIPEBASE
!     [msg_text]The base type of a <kw>(pipe) cannot be <kw>(handle_t)
!     {severity_level}ERROR
!     [msg_exp]Data type <kw>(handle_t) is only meaningful when used as the data
!     [msg_exp]type of the first parameter in an operation.  It is used to
!     [msg_exp]establish a binding to a server of the interface.
!     [msg_act]Do not declare pipes whose base type is <kw>(handle_t).
!     [end_msg]

    HANPRMIN        <A handle_t binding parameter must be an [in] parameter>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]HANPRMIN
!     [msg_text]A <kw>(handle_t) binding parameter must be an <kw>([in]) parameter
!     {severity_level}ERROR
!     [msg_exp]A parameter of type <kw>(handle_t) as the first parameter
!     [msg_exp]in an operation
!     [msg_exp]establishes a binding to a server of the interface.
!     [msg_exp]It must be an <kw>([in]) parameter only.
!     [msg_act]Remove the <kw>([out]) attribute from the <kw>(handle_t) parameter.
!     [end_msg]

    HANSTRFLD       <Structure fields cannot be of type handle_t>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]HANSTRFLD
!     [msg_text]Structure fields cannot be of type <kw>(handle_t)
!     {severity_level}ERROR
!     [msg_exp]Data type <kw>(handle_t) is only meaningful when used as the data
!     [msg_exp]type of the first parameter in an operation.  It is used to
!     [msg_exp]establish a binding to a server of the interface.
!     [msg_act]Do not declare structure fields of data type <kw>(handle_t).
!     [end_msg]

    HANUNIMEM       <Members of unions cannot be of type handle_t>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]HANUNIMEM
!     [msg_text]Members of <kw>(union)s cannot be of type <kw>(handle_t)
!     {severity_level}ERROR
!     [msg_exp]Data type <kw>(handle_t) is only meaningful when used as the data
!     [msg_exp]type of the first parameter in an operation.  It is used to
!     [msg_exp]establish a binding to a server of the interface.
!     [msg_act]Do not declare <kw>(union) members of data type <kw>(handle_t).
!     [end_msg]

    HANXMITAS       <A handle_t first parameter must not have [transmit_as] type>
!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]HANXMITAS
!     [msg_text]A <kw>(handle_t) first parameter must not have <kw>([transmit_as]) type
!     {severity_level}ERROR
!     [msg_exp]A parameter of type <kw>(handle_t), when used as the data
!     [msg_exp]type of the first parameter in an operation, is used to
!     [msg_exp]establish a binding to a server of the interface.
!     [msg_exp]It is not sent over the wire, therefore it is incorrect
!     [msg_exp]for it to have a <kw>([transmit_as]) data type.
!     [msg_act]Remove the <kw>([transmit_as]) clause from the type's declaration.
!     [end_msg]

    IDEMPIPE        <The attribute [idempotent] is not valid on an operation with pipes>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]IDEMPIPE
!     [msg_text]The attribute <kw>([idempotent]) is not valid on an operation with pipes
!     {severity_level}ERROR
!     [msg_exp]An operation that uses pipes cannot be idempotent, because the
!     [msg_exp]<kw>(pipe) data stream can break at unpredictable times.
!     [msg_act]Remove the <kw>([idempotent]) attribute from the operation,
!     [msg_act]or remove the <kw>(pipe) parameter from the operation.
!     [end_msg]

    IGNARRELEM      <The attribute [ignore] on array elements is not allowed>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]IGNARRELEM
!     [msg_text]The attribute  <kw>([ignore]) on array elements is not allowed
!     {severity_level}ERROR
!     [msg_exp]The <kw>([ignore]) attribute is not allowed on array elements
!     [msg_act]Remove the <kw>([ignore]) attribute from the relevant declaration.
!     [end_msg]

    IGNATTRPTR      <The attribute [ignore] is valid only for pointers>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]IGNATTRPTR
!     [msg_text]The attribute <kw>([ignore]) is valid only for pointers
!     {severity_level}ERROR
!     [msg_exp]The <kw>([ignore]) attribute is valid only for pointers.
!     [msg_act]Remove the <kw>([ignore]) attribute from the declaration.
!     [end_msg]

    ILLFIELDATTR    <The attribute [!AZ] on a field is not allowed>/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]ILLFIELDATTR
!     [msg_text]The attribute <v>([attribute-name]) on a field is not allowed
!     {severity_level}ERROR
!     [msg_exp]The specified attribute is not valid on a field.
!     [msg_act]Remove the attribute.
!     [end_msg]

    ILLPARAMATTR    <The attribute [!AZ] on a parameter is not allowed>/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]ILLPARAMATTR
!     [msg_text]The attribute <v>([attribute-name]) on a parameter is not allowed
!     {severity_level}ERROR
!     [msg_exp]The specified attribute is not valid on a parameter declaration.
!     [msg_act]Remove the attribute.
!     [end_msg]

    ILLTYPEATTR     <The attribute [!AZ] on a type is not allowed>/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]ILLTYPEATTR
!     [msg_text]The attribute <v>([attribute-name]) on a type is not allowed
!     {severity_level}ERROR
!     [msg_exp]The specified attribute is not valid on a type declaration.
!     [msg_act]Remove the attribute.
!     [end_msg]

    ILLOPATTR       <The attribute [!AZ] on an operation is not allowed>/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]ILLOPATTR
!     [msg_text]The attribute <v>([attribute-name]) on an operation is not allowed
!     {severity_level}ERROR
!     [msg_exp]The specified attribute is not valid on an operation.
!     [msg_act]Remove the attribute.
!     [end_msg]

    ILLINTATTR      <The attribute [!AZ] on an interface is not allowed>/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]ILLINTATTR
!     [msg_text]The attribute <v>([attribute-name]) on an interface is not allowed
!     {severity_level}ERROR
!     [msg_exp]The specified attribute is not valid on a interface declaration.
!     [msg_act]Remove the attribute.
!     [end_msg]

    ILLMEMATTR      <The attribute [!AZ] on a union member is not allowed>/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]ILLMEMATTR
!     [msg_text]The attribute <v>([attribute-name]) on a <kw>(union) member is not allowed
!     {severity_level}ERROR
!     [msg_exp]The specified attribute is not valid on a <kw>(union) member declaration.
!     [msg_act]Remove the attribute.
!     [end_msg]

    IMPHANVAR       <An [implicit_handle] variable must either be of type handle_t or have the handle attribute>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]IMPHANVAR
!     [msg_text]An <kw>([implicit_handle]) variable must either be of type <kw>(handle_t) or have the <kw>([handle]) attribute
!     {severity_level}ERROR
!     [msg_exp]The <kw>([implicit_handle]) variable declared in the ACF must be a
!     [msg_exp]handle, either a primitive handle of data type <kw>(handle_t);
!     [msg_exp]or a customized handle, a data type with the <kw>([handle]) attribute.
!     [msg_act]Specify a valid handle data type for the <kw>([implicit_handle]).
!     [end_msg]

    IMPORTLOCAL     <A nonlocal interface cannot import a local interface>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]IMPORTLOCAL
!     [msg_text]A nonlocal interface cannot import a local interface
!     {severity_level}ERROR
!     [msg_exp]The <kw>([local]) interface attribute implies that the interface
!     [msg_exp]is not part of an RPC application, but used only to generate
!     [msg_exp]header files.  This causes IDL to suppress any
!     [msg_exp]errors specific to RPC that the interface uses as
!     [msg_exp]part of an RPC application.
!     [msg_act]Remove the <kw>([local]) attribute from the imported interface
!     [msg_act]definition.  The imported interface does not need a UUID
!     [msg_act]unless the interface defines an operation and you compile it
!     [msg_act]independently.
!     [end_msg]

    INCOMPATV1      <V1 attributes are incompatible with this type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INCOMPATV1
!     [msg_text]V1 attributes are incompatible with this type
!     {severity_level}ERROR
!     [msg_exp]The version 1 migration attributes are provided for compatibility for
!     [msg_exp]applications that were originally written with NCS Version 1 NIDL.
!     [msg_exp]These attributes
!     [msg_exp]are <kw>([v1_string]), <kw>([v1_array]), <kw>([v1_enum]),
!     [msg_exp]and <kw>([v1_struct]).  In later versions of IDL, strings,
!     [msg_exp]conformant and varying arrays, enumerations,
!     [msg_exp]and structures are enhanced at
!     [msg_exp]the network representation level.  A data type with any of
!     [msg_exp]the V1 migration attributes cannot contain one of these data types
!     [msg_exp]that does not have the corresponding V1 attribute.
!     [msg_act]Change the data type declaration so all relevant
!     [msg_act]contained types have the corresponding V1 attribute, or remove
!     [msg_act]V1 attributes if compatibility with NCS Version 1 NIDL is unnecessary.
!     [end_msg]

    INTCODEATTR     <Interface attributes [code] and [nocode] cannot occur together>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INTCODEATTR
!     [msg_text]Interface attributes <kw>([code]) and <kw>([nocode]) cannot occur together
!     {severity_level}ERROR
!     [msg_exp]The <kw>([code]) or <kw>([nocode]) attribute on an interface
!     [msg_exp]establishes the default for operations in the interface.
!     [msg_exp]You can specify only one of these attributes.
!     [msg_act]Remove one of the conflicting attributes.
!     [end_msg]

    INTDIVBY0     <Integer division by zero>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]INTDIVBY0
!     [msg_text]Integer division by zero
!     {severity_level}ERROR
!     [msg_exp]The evaluation of an integer constant expression resulted
!     [msg_exp]in an integer divided by zero.
!     [msg_act]Correct the expression so a division by zero does
!     [msg_act]not occur.
!     [end_msg]

    INTLINEATTR     <Interface attributes [in_line] and [out_of_line] cannot occur together>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INTLINEATTR
!     [msg_text]Interface attributes <kw>([in_line]) and <kw>([out_of_line]) cannot occur together
!     {severity_level}ERROR
!     [msg_exp]The <kw>([in_line]) or <kw>([out_of_line]) attribute on an interface
!     [msg_exp]establishes the default for types in the interface.
!     [msg_exp]You can specify only one of these attributes.
!     [msg_act]Remove one of the conflicting attributes.
!     [end_msg]

    INTNAMDIF       <ACF interface name !AZ differs from IDL interface name !AZ> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INTNAMDIF
!     [msg_text]ACF interface name <v>(name) differs from IDL interface name <v>(name)
!     {severity_level}ERROR
!     [msg_exp]The interface name in an interface's Attribute Configuration
!     [msg_exp]File must be identical to the interface name in the IDL source.
!     [msg_act]Change the ACF to use the correct interface name.
!     [end_msg]

    INTOVERFLOW     <Integer value overflows !AZ>/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]INTOVERFLOW
!     [msg_text]Integer value overflows <v>(size)
!     {severity_level}ERROR
!     [msg_exp]The integer value specified is too large to be contained in
!     [msg_exp]the destination of size <v>(size).
!     [msg_act]Either reduce the size of the integer value, or increase the
!     [msg_act]size of the destination declaration.
!     [end_msg]

    INTCONSTINVAL    <Integer constant !AZ is invalid>/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]INTCONSTINVAL
!     [msg_text]Integer constant <v>(integer) is invalid
!     {severity_level}ERROR
!     [msg_exp]The integer contains incorrect characters.
!     [msg_act]Correct the specification of the integer.
!     [end_msg]

    INTUUIDREQ      <Interface UUID must be specified>

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]INTUUIDREQ
!     [msg_text]Interface UUID must be specified
!     {severity_level}ERROR
!     [msg_exp]The interface does not have the <kw>([uuid]) attribute required for
!     [msg_exp]the complete definition of the interface.
!     [msg_act]Either add the <kw>([local]) attribute to the interface, or generate
!     [msg_act]a UUID with the <kw>(uuidgen) utility supplied with IDL and specify
!     [msg_act]the UUID value in the interface <kw>([uuid]) attribute.
!     [end_msg]

    INVARRIND       <Invalid array bound type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INVARRIND
!     [msg_text]Invalid array bound type
!     {severity_level}ERROR
!     [msg_exp]Array indices must be integers.
!     [msg_act]Change the array declaration so the fixed bounds are integer
!     [msg_act]constants.
!     [end_msg]

    INVCASETYP      <Invalid case label type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INVCASETYP
!     [msg_text]Invalid <kw>(case) label type
!     {severity_level}ERROR
!     [msg_exp]The values in <kw>(case) clauses in
!     [msg_exp]discriminated <kw>(union)s must be
!     [msg_exp]integer, enumeration, boolean, or character values that match
!     [msg_exp]the data type of the <kw>(union) discriminator.
!     [msg_act]Modify the invalid clause to use a value of the correct
!     [msg_act]data type.
!     [end_msg]

    INVCHARLIT <Invalid character literal>

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]INVCHARLIT
!     [msg_text]Invalid character literal
!     {severity_level}ERROR
!     [msg_exp]The specification of the character literal is invalid.
!     [msg_act]Character literals can be specified as a single printing
!     [msg_act]character or one of the following escape sequeces:
!     [msg_act]<literal>(\)n, <literal>(\)t, <literal>(\)v, <literal>(\)b,
!     [msg_act]<literal>(\)r, <literal>(\)f, <literal>(\)a,
!     [msg_act]<literal>(\)<literal>(\), <literal>(\)?, <literal>(\)',
!     [msg_act]<literal>(\)", <literal>(\)<v>(ooo), <literal>(\)x<v>(hh)
!     [msg_act](where <v>(ooo) are octal digits, and
!     [msg_act]<v>(hh) are hexadecimal digits).
!     [end_msg]

    INVOCTDIGIT      <Invalid octal digit in !AZ> /FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]INVOCTDIGIT
!     [msg_text]Invalid octal digit in <v>(integer)
!     {severity_level}ERROR
!     [msg_exp]An octal integer contains a digit that is not valid.
!     [msg_exp]Integer constants with a leading zero are interpreted as
!     [msg_exp]octal values.  Only the digits 0 through 7 are valid in octal value.
!     [msg_act]If the integer is not intended to be specified in octal, remove
!     [msg_act]the leading zeros. Otherwise correct the value to contain only
!     [msg_act]octal digits.
!     [end_msg]

    INVOKECPP       <Could not invoke the C preprocessor>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INVOKECPP
!     [msg_text]Could not invoke the C preprocessor
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler is unable to invoke the C preprocessor
!     [msg_exp]to preprocess the IDL file before compiling it.
!     [msg_act]If you do not need the C preprocessor, you can use a command
!     [msg_act]line option to prevent the
!     [msg_act]IDL compiler from calling it.  If you specified
!     [msg_act]the C preprocessor to invoke with a command line option,
!     [msg_act]check to make sure you gave the correct file specification.
!     [msg_act]If you do not specify a C preprocessor, the IDL compiler
!     [msg_act]looks in a default directory for it.  The C
!     [msg_act]preprocessor may not be installed in the expected directory.
!     [end_msg]

    INVOOLPRM       <The attribute [out_of_line] is not allowed on parameters>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INVOOLPRM
!     [msg_text]The attribute <kw>([out_of_line]) is not allowed on parameters
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow the <kw>([out_of_line])
!     [msg_exp]attribute on parameters.
!     [msg_act]Apply the <kw>([out_of_line]) attribute to the type definition
!     [msg_act]rather than the parameter.  If you do not want all
!     [msg_act]instances of the type <kw>([out_of_line]), define two separate
!     [msg_act]types, one of which has the <kw>([out_of_line]) attribute.
!     [end_msg]

    INVOPTION       <Command option -!AZ !AZ is not valid> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INVOPTION
!     [msg_text]Command option -<v>(option-name) <v>(option-value) is not valid
!     {severity_level}ERROR
!     [msg_exp]The <v>(option-value) specified is not valid for the option
!     [msg_exp]with which it is used.
!     [msg_act]See the documentation for the correct command line syntax.
!     [end_msg]

    INVPARAMS       <Invalid parameters on command line:>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INVPARAMS
!     [msg_text]Invalid parameters on command line:
!     {severity_level}ERROR
!     [msg_exp]There are extra parameters on the command line.
!     [msg_act]You may have attempted to use a value on a option that does
!     [msg_act]not take a value, or attempted to compile more than one
!     [msg_act]interface at once.
!     [msg_act]See the documentation for the proper command line syntax.
!     [end_msg]

    INVPTRCTX       <Pointers to context handles are valid only in parameter declarations>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INVPTRCTX
!     [msg_text]Pointers to context handles are valid only in parameter declarations
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow pointers to context handles,
!     [msg_exp]except in parameter declarations.
!     [msg_act]Do not declare a pointer to a <kw>(context_handle), except as a parameter.
!     [end_msg]

    INVPTRPIPE      <Pointers to pipes are valid only in parameter declarations>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INVPTRPIPE
!     [msg_text]Pointers to pipes are valid only in parameter declarations
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow a pointer to a <kw>(pipe),
!     [msg_exp]except in a parameter declaration.
!     [msg_act]Do not declare a pointer to a <kw>(pipe), except as a parameter.
!     [end_msg]

    LASTINATTR      <The [last_is] parameter must have the [in] attribute>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]LASTINATTR
!     [msg_text]The <kw>([last_is]) parameter must have the <kw>([in]) attribute
!     {severity_level}ERROR
!     [msg_exp]The <kw>([last_is]) attribute specifies a parameter that contains
!     [msg_exp]the upper data limit of a varying array.  Since the array has the
!     [msg_exp]<kw>([in]) attribute, the upper data
!     [msg_exp]limit parameter must also be <kw>([in])
!     [msg_exp]so the number of array elements to send from client to
!     [msg_exp]server is known.
!     [msg_act]Change the data limit parameter referenced by the
!     [msg_act]<kw>([last_is]) clause to have the <kw>([in]) attribute.
!     [end_msg]

    LASTLENCONF     <The attributes [last_is] and [length_is] cannot occur together>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]LASTLENCONF
!     [msg_text]The attributes <kw>([last_is]) and <kw>([length_is]) cannot occur together
!     {severity_level}ERROR
!     [msg_exp]The <kw>([last_is]) and <kw>([length_is]) attributes both specify a
!     [msg_exp]field or parameter used to determine the
!     [msg_exp]data limit of a varying array.
!     [msg_exp]You cannot use both.
!     [msg_act]The <kw>([last_is]) attribute specifies the
!     [msg_act]index of the last valid element.
!     [msg_act]The <kw>([length_is]) attribute specifies the
!     [msg_act]total number of elements in the
!     [msg_act]array. Remove one of these attributes.
!     [end_msg]

    LASTYPEINT      <A [last_is] variable must be a small, short, or long integer>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]LASTYPEINT
!     [msg_text]A <kw>([last_is]) variable must be a <kw>(small), <kw>(short), or <kw>(long) integer
!     {severity_level}ERROR
!     [msg_exp]The <kw>([last_is]) attribute specifies a field or parameter that contains
!     [msg_exp]the upper data limit of a varying array.  Array data limits
!     [msg_exp]must be integers which are not <kw>(hyper).
!     [msg_act]Change the upper data limit field or parameter referenced by the
!     [msg_act]<kw>([last_is]) clause to be of integer data type.
!     [end_msg]

    LBLESSUB        <The lower bound must not be greater than the upper bound>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]LBLESSUB
!     [msg_text]The lower bound must not be greater than the upper bound
!     {severity_level}ERROR
!     [msg_exp]The syntax of an array requires the lower bound to
!     [msg_exp]precede the upper bound.
!     [msg_act]Modify the array declaration so the lower bound is first.
!     [end_msg]

    LENINATTR       <The [length_is] parameter must have the [in] attribute>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]LENINATTR
!     [msg_text]The <kw>([length_is]) parameter must have the <kw>([in]) attribute
!     {severity_level}ERROR
!     [msg_exp]The <kw>([length_is]) attribute specifies a parameter used
!     [msg_exp]to determine the data length of a varying array.
!     [msg_exp]Since the array has the
!     [msg_exp]<kw>([in]) attribute, the <kw>(length_is) parameter
!     [msg_exp]must also be <kw>([in])
!     [msg_exp]so the number of array elements to send from client to
!     [msg_exp]server is known.
!     [msg_act]Change the parameter referenced by the
!     [msg_act]<kw>([length_is]) clause to have the <kw>([in]) attribute.
!     [end_msg]

    LENTYPEINT      <A [length_is] variable must be a small, short, or long integer>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]LENTYPEINT
!     [msg_text]A <kw>([length_is]) variable must be a <kw>(small), <kw>(short), or <kw>(long) integer
!     {severity_level}ERROR
!     [msg_exp]The <kw>([length_is]) attribute specifies a field
!     [msg_exp]or parameter that contains
!     [msg_exp]the data length of a varying array.  The field or parameter
!     [msg_exp]must be integers which are not <kw>(hyper).
!     [msg_act]Change the field or parameter referenced by the
!     [msg_act]<kw>([length_is]) clause to be of integer data type.
!     [end_msg]

    MAJORTOOLARGE <The major version number is too large; the maximum is !UL>

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]MAJORTOOLARGE
!     [msg_text]The major version number is too large; the maximum is <v>(number)
!     {severity_level}ERROR
!     [msg_exp]The major version number is too large.
!     [msg_act]Replace the major version number (the portion
!     [msg_act]to the left of the period) with a value
!     [msg_act]less than <v>(number).
!     [end_msg]

    MAXCFMTYPE      <The attribute [max_is] cannot be applied to dimension !UL; upper bound is not dynamic> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXCFMTYPE
!     [msg_text]The attribute <kw>([max_is]) cannot be applied to dimension <v>(n); upper bound is not dynamic
!     {severity_level}ERROR
!     [msg_exp]A <kw>([max_is]) variable is only valid for array dimensions that
!     [msg_exp]have an upper bound that is not fixed.
!     [msg_act]If the array is multidimensional, you may have specified the
!     [msg_act]<kw>([max_is]) variable for the wrong dimension.  See the
!     [msg_act]documentation for the proper syntax.  If the array has only
!     [msg_act]one dimension, the <kw>([max_is]) clause is invalid, since the
!     [msg_act]array's upper bound is fixed.  You may have meant to use a
!     [msg_act]<kw>([last_is]) clause.  See the documentation for the
!     [msg_act]distinctions between conformant and varying arrays
!     [msg_act]and their attributes.
!     [end_msg]

    MAXIDINTF       <Maximum identifier length for interface name is !UL characters> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXIDINTF
!     [msg_text]Maximum identifier length for interface name is <v>(number) characters
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler constructs new identifier names
!     [msg_exp]that are referenced by generated stub code
!     [msg_exp]from the interface name.
!     [msg_exp]Therefore, the number of characters allowed
!     [msg_exp]in an interface name is
!     [msg_exp]less than for other identifiers.
!     [msg_act]Shorten the interface name.
!     [end_msg]

    MAXIDTYPTA      <Maximum identifier length for type with [transmit_as] is !UL characters> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXIDTYPTA
!     [msg_text]Maximum identifier length for type with <kw>([transmit_as]) is <v>(number) characters
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler constructs new identifier names
!     [msg_exp]that are referenced by generated stub code
!     [msg_exp]from the names of any <kw>([transmit_as]) data types.
!     [msg_exp]Therefore, the number of characters allowed
!     [msg_exp]in such a data type is
!     [msg_exp]less than for other identifiers.
!     [msg_act]Shorten the data type name.
!     [end_msg]

    MAXIDTYPHAN     <Maximum identifier length for [handle] type is !UL characters> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXIDTYPHAN
!     [msg_text]Maximum identifier length for <kw>([handle]) type is <v>(number) characters
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler constructs new identifier names
!     [msg_exp]that are referenced by generated stub code
!     [msg_exp]from the names of any <kw>([handle]) data types.
!     [msg_exp]Therefore, the number of characters allowed
!     [msg_exp]in such a data type is
!     [msg_exp]less than for other identifiers.
!     [msg_act]Shorten the data type name.
!     [end_msg]

    MAXIDTYPCH      <Maximum identifier length for [context_handle] type is !UL characters> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXIDTYPCH
!     [msg_text]Maximum identifier length for <kw>([context_handle]) type is <v>(number) characters
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler constructs new identifier names
!     [msg_exp]that are referenced by generated stub code
!     [msg_exp]from the names of any <kw>([context_handle]) data types.
!     [msg_exp]Therefore, the number of characters allowed
!     [msg_exp]in such a data type is
!     [msg_exp]less than for other identifiers.
!     [msg_act]Shorten the data type name.
!     [end_msg]

    MAXIDTYPPT      <Maximum identifier length for pointed-to type is !UL characters> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXIDTYPPT
!     [msg_text]Maximum identifier length for pointed-to type is <v>(number) characters
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler constructs new identifier names
!     [msg_exp]that are referenced by generated stub code
!     [msg_exp]from the names of any pointed-to data types.
!     [msg_exp]Therefore, the number of characters allowed
!     [msg_exp]in such a data type is
!     [msg_exp]less than for other identifiers.
!     [msg_act]Shorten the data type name.
!     [end_msg]

    MAXIDTYPPIPE    <Maximum identifier length for pipe type is !UL characters> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXIDTYPPIPE
!     [msg_text]Maximum identifier length for <kw>(pipe) type is <v>(number) characters
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler constructs new identifier names
!     [msg_exp]that are referenced by generated stub code
!     [msg_exp]from the names of any <kw>(pipe) data types.
!     [msg_exp]Therefore, the number of characters allowed
!     [msg_exp]in such a data type is
!     [msg_exp]less than for other identifiers.
!     [msg_act]Shorten the data type name.
!     [end_msg]

    MAXIDTYPRA      <Maximum identifier length for [represent_as] type is !UL characters> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXIDTYPRA
!     [msg_text]Maximum identifier length for <kw>([represent_as]) type is <v>(number) characters
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler constructs new identifier names
!     [msg_exp]that are referenced by generated stub code
!     [msg_exp]from the names of any <kw>([represent_as]) data types.
!     [msg_exp]Therefore, the number of characters allowed
!     [msg_exp]in such a data type is
!     [msg_exp]less than for other identifiers.
!     [msg_act]Shorten the data type name.
!     [end_msg]

    MAXIDTYPOOL     <Maximum identifier length for [out_of_line] type is !UL characters> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXIDTYPOOL
!     [msg_text]Maximum identifier length for <kw>([out_of_line]) type is <v>(number) characters
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler constructs new identifier names
!     [msg_exp]that are referenced by generated stub code
!     [msg_exp]from the names of any <kw>([out_of_line]) data types.
!     [msg_exp]Therefore, the number of characters allowed
!     [msg_exp]in such a data type is
!     [msg_exp]less than for other identifiers.
!     [msg_act]Shorten the data type name.
!     [end_msg]

    MAXINATTR       <A [max_is] parameter must have the [in] attribute>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXINATTR
!     [msg_text]A <kw>([max_is]) parameter must have the <kw>([in]) attribute
!     {severity_level}ERROR
!     [msg_exp]The <kw>([max_is]) attribute specifies a parameter that contains
!     [msg_exp]the upper bound of a conformant array.  This must be an <kw>([in])
!     [msg_exp]attribute so the server stub code knows how much
!     [msg_exp]space to allocate for the array.
!     [msg_act]Change the upper bound parameter referenced by the
!     [msg_act]<kw>([max_is]) clause to have the <kw>([in]) attribute.
!     [end_msg]

    MAXSIZEATTR     <The attribute [max_is] or [size_is] is required>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXSIZEATTR
!     [msg_text]The attribute <kw>([max_is]) or <kw>([size_is]) is required
!     {severity_level}ERROR
!     [msg_exp]An array with an upper bound that is not fixed is used as a field of
!     [msg_exp]a structure or as a parameter of an operation.  You must
!     [msg_exp]specify a field or parameter that determines
!     [msg_exp]the allocation of the array at runtime.
!     [msg_act]Use a <kw>([max_is]) or <kw>([size_is]) attribute to specify
!     [msg_act]the field or parameter that contains the size information.
!     [end_msg]

    MAXSIZECONF     <The attributes [max_is] and [size_is] cannot occur together>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXSIZECONF
!     [msg_text]The attributes <kw>([max_is]) and <kw>([size_is]) cannot occur together
!     {severity_level}ERROR
!     [msg_exp]The <kw>([max_is]) and <kw>([size_is]) attributes both specify a
!     [msg_exp]field or parameter used to determine the
!     [msg_exp]upper bound of a conformant array.
!     [msg_exp]You cannot use both.
!     [msg_act]The <kw>([max_is]) attribute specifies
!     [msg_act]the index of the last possible array element.
!     [msg_act]The <kw>([size_is]) attribute specifies
!     [msg_act]the total number of possible elements in the
!     [msg_act]array. Remove one of the attributes.
!     [end_msg]

    MAXTYPEINT      <A [max_is] variable must be a small, short, or long integer>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXTYPEINT
!     [msg_text]A <kw>([max_is]) variable must be a <kw>(small), <kw>(short), or <kw>(long) integer
!     {severity_level}ERROR
!     [msg_exp]The <kw>([max_is]) attribute specifies a field or parameter that contains
!     [msg_exp]the upper bound of a conformant array.  Array bounds 
!     [msg_exp]must be integers which are not <kw>(hyper).
!     [msg_act]Change the upper bound field or parameter referenced by the
!     [msg_act]<kw>([max_is]) clause to be of integer data type.
!     [end_msg]

    BFINTTYPE   <A bit field specification can be applied only to simple, integral types>

!     {engineer_name}Ed
!     {audience}programmer
!     [begin_msg]
!     [ident]BFINTTYPE
!     [msg_text]A bit field specification can be applied only to simple, integral types
!     {severity_level}ERROR
!     [msg_exp]A bit field specification indicates the bit width of a simple
!     [msg_exp]integral type.  This cannot be applied to other types such as
!     [msg_exp]pointers and structs for obvious reasons.
!     [msg_act]Remove bit field specifications that appear on non-integral 
!     [msg_act]simple types.
!     [end_msg]

    MAYBEOUTPRM     <A [maybe] operation cannot have [out] parameters or a function result>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAYBEOUTPRM
!     [msg_text]A <kw>([maybe]) operation cannot have <kw>([out]) parameters or a function result
!     {severity_level}ERROR
!     [msg_exp]The <kw>([maybe]) attribute specifies that the operation's caller
!     [msg_exp]does not require and does not receive a response or fault
!     [msg_exp]indication.  Do not use an <kw>([out])
!     [msg_exp]parameter or a function result in a <kw>([maybe]) operation.
!     [msg_act]Remove the <kw>([maybe]) attribute from the operation,
!     [msg_act]or remove all <kw>([out]) parameters from the function and
!     [msg_act]declare its result type as <kw>(void).
!     [end_msg]

    MINATTREQ       <The attribute [min_is] is required>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]MINATTREQ
!     [msg_text]The attribute <kw>([min_is]) is required
!     {severity_level}ERROR
!     [msg_exp]An array with a lower bound that is not fixed is used as a field of
!     [msg_exp]a structure or as a parameter of an operation.  You must
!     [msg_exp]specify a field or parameter that determines
!     [msg_exp]the lower bound of the array at runtime.
!     [msg_act]Use a <kw>([min_is]) attribute to specify
!     [msg_act]the field or parameter that contains the lower bound.
!     [end_msg_DEC]

    MINCFMTYPE      <The attribute [min_is] cannot be applied to dimension !UL; lower bound is not dynamic> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]MINCFMTYPE
!     [msg_text]The attribute <kw>([min_is]) cannot be applied to dimension <v>(n); lower bound is not dynamic
!     {severity_level}ERROR
!     [msg_exp]A <kw>([min_is]) variable is valid only for array dimensions that
!     [msg_exp]have a lower bound that is not fixed.
!     [msg_act]If the array is multidimensional, you may have specified the
!     [msg_act]<kw>([min_is]) variable for the wrong dimension.  See the
!     [msg_act]documentation for the proper syntax.  If the array has only
!     [msg_act]one dimension, the <kw>([min_is]) clause is invalid, since the
!     [msg_act]array's lower bound is fixed.  The
!     [msg_act]<kw>([first_is]) clause is valid for single dimension
!     [msg_act]arrays.  See the documentation for the
!     [msg_act]distinctions between conformant and varying arrays
!     [msg_act]and their attributes.
!     [end_msg_DEC]

    MININATTR       <A [min_is] parameter must have the [in] attribute>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]MININATTR
!     [msg_text]A <kw>([min_is]) parameter must have the <kw>([in]) attribute
!     {severity_level}ERROR
!     [msg_exp]The <kw>([min_is]) attribute specifies a parameter that contains
!     [msg_exp]the lower bound of a conformant array.  This parameter
!     [msg_exp]must have an <kw>([in])
!     [msg_exp]attribute so the server stub code will allocate adequate
!     [msg_exp]space for the array.
!     [msg_act]Change the lower bound parameter referenced by the
!     [msg_act]<kw>([min_is]) clause to have the <kw>([in]) attribute.
!     [end_msg_DEC]

    MINORTOOLARGE <The minor version number is too large; the maximum is !UL>

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]MINORTOOLARGE
!     [msg_text]The minor version number is too large; the maximum is <v>(number)
!     {severity_level}ERROR
!     [msg_exp]The minor version number is too large.
!     [msg_act]Replace the minor version number (the portion
!     [msg_act]to the right of the period) with a value
!     [msg_act]less than <v>(number).
!     [end_msg]

    MINTYPEINT      <A [min_is] variable must be a small, short, or long integer>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]MINTYPEINT
!     [msg_text]A <kw>([min_is]) variable must be a <kw>(small), <kw>(short), or <kw>(long) integer
!     {severity_level}ERROR
!     [msg_exp]The <kw>([min_is]) attribute specifies a field or parameter that contains
!     [msg_exp]the lower bound of a conformant array.  Array bounds 
!     [msg_exp]must be integers that are not <kw>(hyper).
!     [msg_act]Change the lower bound field or parameter referenced by the
!     [msg_act]<kw>([min_is]) clause so that it is a small, short, or long
!     [msg_act]integer.
!     [end_msg_DEC]

    NAMEALRDEC      <Name already declared: !AZ> /FAO_COUNT=1

!     {engineer_name}Beth
!     {audience}programmer
!     [begin_msg]
!     [ident]NAMEALRDEC
!     [msg_text]Name already declared: <v>(name)
!     {severity_level}ERROR
!     [msg_exp]The name referenced is already declared.
!     [msg_act]Modify the interface and select a unique name
!     [msg_act]for the given item.
!     [end_msg]

    NAMENOTCONST    <Name is not a constant: !AZ> /FAO_COUNT=1

!     {engineer_name}Beth
!     {audience}programmer
!     [begin_msg]
!     [ident]NAMENOTCONST
!     [msg_text]Name is not a constant: <v>(name)
!     {severity_level}ERROR
!     [msg_exp]The name used to define a new constant is not the name of
!     [msg_exp]a <kw>(constant).
!     [msg_act]Modify the <kw>(constant) definition to use
!     [msg_act]a predefined <kw>(constant) name.
!     [end_msg]

    NAMENOTFIELD    <Name is not a field: !AZ> /FAO_COUNT=1

!     {engineer_name}Beth
!     {audience}programmer
!     [begin_msg]
!     [ident]NAMENOTFIELD
!     [msg_text]Name is not a field: <v>(name)
!     {severity_level}ERROR
!     [msg_exp]The name referenced in an array attribute is not
!     [msg_exp]declared as a field in the structure being defined.
!     [msg_act]Declare a field in the structure that can be used
!     [msg_act]in the array attribute.
!     [end_msg]

    NAMENOTFND      <Name not found: !AZ> /FAO_COUNT=1

!     {engineer_name}Beth
!     {audience}programmer
!     [begin_msg]
!     [ident]NAMENOTFND
!     [msg_text]Name not found: <v>(name)
!     {severity_level}ERROR
!     [msg_exp]The referenced name is not defined.
!     [msg_act]Modify the interface to define an appropriate type for the
!     [msg_act]specified name.
!     [end_msg]

    NAMENOTPARAM    <Name is not a parameter: !AZ> /FAO_COUNT=1

!     {engineer_name}Beth
!     {audience}programmer
!     [begin_msg]
!     [ident]NAMENOTPARAM
!     [msg_text]Name is not a parameter: <v>(name)
!     {severity_level}ERROR
!     [msg_exp]The name referenced in an array attribute is not
!     [msg_exp]declared as a parameter in the operation being defined.
!     [msg_act]Declare a parameter in the operation that can be used
!     [msg_act]in the array attribute.
!     [end_msg]

    NAMENOTTYPE     <Name is not a type: !AZ> /FAO_COUNT=1

!     {engineer_name}Beth
!     {audience}programmer
!     [begin_msg]
!     [ident]NAMENOTTYPE
!     [msg_text]Name is not a type: <v>(name)
!     {severity_level}ERROR
!     [msg_exp]The name referenced is not a type definition.
!     [msg_act]Modify the interface to specify a unique type specification
!     [msg_act]instead of the name referenced.
!     [end_msg]

    NAMEPREVDECLAT  <Name !AZ previously declared in file !AZ, line !UL> /FAO_COUNT=3

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]NAMEPREVDECLAT
!     [msg_text]Name <v>(name) previously declared in file <v>(filename), line <v>(line)
!     {severity_level}ERROR
!     [msg_exp]The name referenced was previously declared at the specified location.
!     [msg_act]Remove this declaration, if it is redundant, or change the spelling
!     [msg_act]so it does not conflict with the existing name.
!     [end_msg]

    NLSCATVER <!AZNLS message catalog version mismatch in "!AZ", Expected: "!UL", Actual: "!AZ">
    NLSWRONG  <!AZError messages may be incorrect>

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]NLSCATVER
!     [msg_text]NLS message catalog version mismatch in "<v>(filename)", Expected: "<v>(version)", Actual: "<v>(version)"
!     {severity_level}ERROR
!     [msg_exp]The error messages reported by the IDL compiler are stored in an NLS
!     [msg_exp]message catalog.  The catalog <v>(filename) is not the same version
!     [msg_exp]as the IDL compiler being invoked.  The error messages reported may
!     [msg_exp]therefore be incorrect or cause the compiler to terminate abnormally.
!     [msg_act]Check the definition of the NLS environment variable NLSPATH and verify that
!     [msg_act]it searches the appropriate directories.  The default location for the IDL
!     [msg_act]compiler message catalog is
!     [msg_act]<v>(<literal>(<)dceshared>)<kw>(/nls/msg/)<v>(LANG)<kw>(/%N)
!     [end_msg]

    NONINTEXP <Non-integer values are not allowed in expressions>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]NONINTEXP
!     [msg_text]Non-integer values are not allowed in expressions
!     {severity_level}ERROR
!     [msg_exp]One of the values in the expression is not
!     [msg_exp]an integer (or one that can be promoted to integer).
!     [msg_act]Correct the expression to contain only integer values.
!     [end_msg]

    NYSALIGN        <The [align] attribute is not yet supported>
!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]NYSALIGN
!     {msg_text]The <kw>([align]) attribute is not yet supported
!     {severity_level}ERROR
!     {msg_exp]The feature is not supported by IDL.
!     {msg_act]Do not depend upon the alignment of opaque data.
!     {end_msg]

    NYSINSHAPE      <The attribute [in(shape)] is not yet supported>
!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]NYSINSHAPE
!     {msg_text]The attribute <kw>([in(shape)]) is not yet supported
!     {severity_level}ERROR
!     {msg_exp]The feature is not supported by IDL.
!     {msg_act]Use unqualified <kw>([in]) as an interim solution.
!     {end_msg]

    NYSNONZEROLB    <Arrays with a nonzero lower bound are not yet supported>

!     {engineer_name}Rico
!     {audience}obsolete_placeholder
!     {begin_msg]
!     {ident]NYSNONZEROLB
!     {msg_text]Arrays with a nonzero lower bound are not yet supported
!     {severity_level}ERROR
!     {msg_exp]The IDL compiler does not support arrays with either
!     {msg_exp]a fixed nonzero lower bound or a
!     {msg_exp]lower bound that is not fixed.
!     {msg_act]Use <kw>(-standard) <kw>(extended) if you want to use this 
!     {msg_act]construct.
!     {end_msg]

    NYSOUTSHAPE     <The attribute [out(shape)] is not yet supported>
!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]NYSOUTSHAPE
!     {msg_text]The attribute <kw>([out(shape)]) is not yet supported
!     {severity_level}ERROR
!     {msg_exp]The feature is not supported by IDL.
!     {msg_act]Use unqualified <kw>([out]) as an interim solution.
!     {end_msg]

    NYSUNIQUE       <Attribute [unique] is not yet supported>
!     {engineer_name}Rico
!     {audience}obsolete_placeholder
!     {begin_msg]
!     {ident]NYSUNIQUE
!     {msg_text]Attribute <kw>([unique]) is not yet supported
!     {severity_level}ERROR
!     {msg_exp]This feature is not supported by IDL.
!     {msg_act]Use <kw>(-standard) <kw>(extended) if you want to use this 
!     {msg_act]construct.
!     {end_msg]

    OPCODEATTR      <Operation attributes [code] and [nocode] cannot occur together>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OPCODEATTR
!     [msg_text]Operation attributes <kw>([code]) and <kw>([nocode]) cannot occur together
!     {severity_level}ERROR
!     [msg_exp]The <kw>([code]) and <kw>([nocode]) attributes have directly opposite
!     [msg_exp]meanings.  Do not use both.
!     [msg_act]Remove one of the conflicting attributes.
!     [end_msg]

    OPENREAD        <Unable to open !AZ for read access> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OPENREAD
!     [msg_text]Unable to open <v>(filename) for read access
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler is unable to open a file for processing.
!     [msg_act]Make sure the file exists, and that it has the proper access
!     [msg_act]protection for the IDL compiler.
!     [end_msg]

    OPENWRITE       <Unable to open !AZ for write access> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OPENWRITE
!     [msg_text]Unable to open <v>(filename) for write access
!     {severity_level}ERROR
!     [msg_exp]The IDL compiler is unable to create a file.
!     [msg_act]Make sure the directory to contain the file exists,
!     [msg_act]and that it has the proper access
!     [msg_act]protection for the IDL compiler.
!     [end_msg]

    OPNOTDEF        <Operation name !AZ referenced in ACF is not defined> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OPNOTDEF
!     [msg_text]Operation name <v>(name) referenced in ACF is not defined
!     {severity_level}ERROR
!     [msg_exp]Any operation referenced in an Attribute Configuration File
!     [msg_exp](ACF) must be defined in the corresponding IDL file.
!     [msg_act]Check for typographical errors in either file.
!     [end_msg]

    OUTCFMTARR      <An [out] conformant array must be a top-level parameter or under a non-[ref] pointer>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OUTCFMTARR
!     [msg_text]An <kw>([out]) conformant array must be a top-level parameter or under a non-<kw>([ref]) pointer
!     {severity_level}ERROR
!     [msg_exp]An array with unspecified lower and/or upper bounds requires
!     [msg_exp]size information.  If the array is a top-level parameter,
!     [msg_exp]the size information must be in additional <kw>([in]) parameters.
!     [msg_exp]If the array is contained within a structure, the size
!     [msg_exp]information must be in additional fields of the structure,
!     [msg_exp]and the parameter containing the structure must be <kw>([in]) or
!     [msg_exp]<kw>([in,out]).  In both cases, the size
!     [msg_exp]information must be <kw>([in])
!     [msg_exp]so the server stub knows how much storage to allocate
!     [msg_exp]for the array.  The exception to this latter case is any <kw>([out])
!     [msg_exp]conformant array that is pointed to by a full <kw>([ptr]) pointer
!     [msg_exp]or a <kw>([unique]) pointer
!     [msg_exp]or that indirectly lies under a <kw>([ptr])
!     [msg_exp]or <kw>([unique]) pointer.  In this
!     [msg_exp]case the semantics are that the user-written manager code
!     [msg_exp]either allocates and/or manages the
!     [msg_exp]storage for the array and any other data below the <kw>([ptr])
!     [msg_exp]or <kw>([unique])
!     [msg_exp]pointer or returns a <kw>(NULL) value for
!     [msg_exp]the <kw>([ptr]) or <kw>([unique]) pointer.
!     [msg_act]Change the parameter to be <kw>([in,out]), or pass the array as a
!     [msg_act]separate <kw>([out]) parameter with <kw>([in]) size information
!     [msg_act]parameters, or change the data structure and the code that
!     [msg_act]manipulates it so the conformant array is under a
!     [msg_act]<kw>([ptr]) or <kw>([unique]) pointer.
!     [end_msg]

    OUTPRMREF       <Output parameters must be passed by reference>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OUTPRMREF
!     [msg_text]Output parameters must be passed by reference
!     {severity_level}ERROR
!     [msg_exp]Parameters with the <kw>([out]) or <kw>([in,out]) attributes must be
!     [msg_exp]passed by reference, so the changed value of the
!     [msg_exp]parameter can be reflected back to the caller.
!     [msg_act]Add a <kw>(*) to the parameter declaration to indicate
!     [msg_act]pass-by-reference calling mechanism.
!     [end_msg]

    OUTPTRPRM       <An [out,ptr] parameter is not valid>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OUTPTRPRM
!     [msg_text]An <kw>([out,ptr]) parameter is not valid
!     {severity_level}ERROR
!     [msg_exp]The <kw>([ptr]) parameter attribute implies that the value of the
!     [msg_exp]pointer may be <kw>(NULL).  It is invalid on an <kw>([out]) only
!     [msg_exp]parameter, since the possible NULL-ness is not shipped
!     [msg_exp]to the server (for example, in the <kw>([in]) direction).
!     [msg_act]If the pointer cannot be <kw>(NULL)
!     [msg_act]remove the <kw>([ptr]) attribute.
!     [msg_act]Otherwise, make the parameter an <kw>([in,out,ptr]) parameter.
!     [end_msg]

    OUTUNIQUE       <An [out] parameter or operation result cannot contain [unique] pointers>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]OUTUNIQUE
!     [msg_text]An <kw>([out]) parameter or operation result cannot contain <kw>([unique]) pointers
!     {severity_level}ERROR
!     [msg_exp]Operation results and parameters that are <kw>([out]) only cannot
!     [msg_exp]have <kw>([unique]) pointers.  Unique pointers may be <kw>(NULL),
!     [msg_exp]but there is no way to express the <kw>(NULL) value to the server
!     [msg_exp]unless the pointer is part of an <kw>([in]) parameter.
!     [msg_act]Change your type declarations so no types contained in
!     [msg_act]the parameter have the <kw>([unique]) attribute, or, if the pointer
!     [msg_act]is part of a parameter, add the <kw>([in]) attribute to the parameter.
!     [end_msg_DEC]

    OPRESPIPE       <An operation result cannot be a pipe>
!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OPRESPIPE
!     [msg_text]An operation result cannot be a <kw>(pipe)
!     {severity_level}ERROR
!     [msg_exp]Pipes are not allowed as operation results.
!     [msg_act]Change the operation so the <kw>(pipe) is an extra <kw>([out])
!     [msg_act]parameter instead of the operation result.
!     [end_msg]

    OUTSTAR         <Output parameters require an explicit top-level *>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OUTSTAR
!     [msg_text]Output parameters require an explicit top-level <kw>(*)
!     {severity_level}ERROR
!     [msg_exp]Parameters with the <kw>([out]) or <kw>([in,out]) attributes must be
!     [msg_exp]passed by reference, so the changed value of the
!     [msg_exp]parameter can be reflected back to the caller.
!     [msg_exp]IDL does not allow a <kw>(*) in a type definition to serve as
!     [msg_exp]a passing mechanism <kw>(*).
!     [msg_act]Change the declaration of the parameter so it
!     [msg_act]contains an explicit <kw>(*).
!     [end_msg]

    OUTUNIQPRM      <An [out,unique] parameter is not valid>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]OUTUNIQPRM
!     [msg_text]An <kw>([out,unique]) parameter is not valid
!     {severity_level}ERROR
!     [msg_exp]The <kw>([unique]) parameter attribute implies that the value of the
!     [msg_exp]pointer may be <kw>(NULL).  The <kw>(NULL) value is
!     [msg_exp]invalid on an <kw>([out]) only
!     [msg_exp]parameter, since the <kw>(NULL) value was not sent
!     [msg_exp]to the server as an <kw>([in]) parameter.
!     [msg_act]If the pointer cannot be <kw>(NULL), remove the
!     [msg_act]<kw>([unique]) attribute.
!     [msg_act]Otherwise, make the parameter an <kw>([in,out,unique]) parameter.
!     [end_msg_DEC]

    PIPEBASETYP     <The base type of a pipe cannot be a pipe type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PIPEBASETYP
!     [msg_text]The base type of a <kw>(pipe) cannot be a <kw>(pipe) type
!     {severity_level}ERROR
!     [msg_exp]A <kw>(pipe) type cannot be used in the definition of
!     [msg_exp]another <kw>(pipe) type.
!     [msg_act]Remove the invalid declaration, or change it so the
!     [msg_act]base type of the <kw>(pipe) is not another <kw>(pipe) type.
!     [end_msg]

    PIPESTRFLD      <Pipes are not valid as structure fields>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PIPESTRFLD
!     [msg_text]Pipes are not valid as structure fields
!     {severity_level}ERROR
!     [msg_exp]Pipes are not allowed as fields of structures.
!     [msg_act]Pass a <kw>(pipe) as a separate parameter to an operation,
!     [msg_act]rather than embedding it as a field of a structure.
!     [end_msg]

    PIPEUNIMEM      <Pipes are not valid as members of unions>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PIPEUNIMEM
!     [msg_text]Pipes are not valid as members of <kw>(union)s
!     {severity_level}ERROR
!     [msg_exp]Pipes are not allowed as members of <kw>(union)s.
!     [msg_act]Pass a <kw>(pipe) as a separate parameter to an operation,
!     [msg_act]rather than embedding it as a member of a <kw>(union).
!     [end_msg]

    PIPEXMITAS      <A pipe cannot have a [transmit_as] type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PIPEXMITAS
!     [msg_text]A <kw>(pipe) cannot have a <kw>([transmit_as]) type
!     {severity_level}ERROR
!     [msg_exp]A pipe is a specialized object handled by RPC that,
!     [msg_exp]by itself, is not a transmittable object.
!     [msg_exp]Thus, a <kw>([transmit_as]) clause is invalid on
!     [msg_exp]a <kw>(pipe) definition.
!     [msg_act]Remove the <kw>([transmit_as]) attribute.
!     [end_msg]

    PRMBYREF        <A parameter with [!AZ] cannot be passed by value>

!     {engineer_name}Beth
!     {audience}programmer
!     [begin_msg]
!     [ident]PRMBYREF
!     [msg_text]A parameter with <v>([attribute-name]) cannot be passed by value
!     {severity_level}ERROR
!     [msg_exp]The <v>([attribute-name]) attribute can only be applied
!     [msg_exp]to a parameter if that parameter has an explicit <kw>(*).
!     [msg_act]Modify the parameter signature to have a top-level <kw>(*).
!     [end_msg]

    PRMINOROUT      <A parameter must have either or both the [in] and [out] attributes>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PRMINOROUT
!     [msg_text]A parameter must have either or both the <kw>([in]) and <kw>([out]) attributes
!     {severity_level}ERROR
!     [msg_exp]The direction of a parameter must be declared.  The
!     [msg_exp]<kw>([in]) attribute tells IDL that the parameter is passed from
!     [msg_exp]client to server.  The
!     [msg_exp]<kw>([out]) attribute tells IDL that the parameter is passed from
!     [msg_exp]server to client.
!     [msg_act]Add the <kw>([in]), <kw>([out]), or <kw>([in,out]) attribute
!     [msg_act]to the parameter.
!     [end_msg]

    PRMLINEATTR     <Parameter attributes [in_line] and [out_of_line] cannot occur together>

!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]PRMLINEATTR
!     {msg_text]Parameter attributes <kw>([in_line]) and <kw>([out_of_line]) cannot occur together
!     {severity_level}ERROR
!     {msg_exp]These attributes not currently implemented for parameters.
!     {msg_exp] { in place of [ cause MSG.COM to ignore.
!     {msg_act]
!     {msg_act]
!     {end_msg]

    PRMNOTDEF       <Parameter name !AZ referenced in ACF operation !AZ is not defined> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PRMNOTDEF
!     [msg_text]Parameter name <v>(name) referenced in ACF operation <v>(name) is not defined
!     {severity_level}ERROR
!     [msg_exp]Any parameter referenced within an operation declaration in an
!     [msg_exp]Attribute Configuration File (ACF) must be defined in the
!     [msg_exp]corresponding operation definition in the IDL file.
!     [msg_act]Check for typographical errors in either file.
!     [end_msg]

    PTRATTRHAN      <The attribute [ptr] is invalid on a binding handle parameter>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PTRATTRHAN
!     [msg_text]The attribute <kw>([ptr]) is invalid on a binding <kw>(handle) parameter
!     {severity_level}ERROR
!     [msg_exp]The first parameter in an operation is a <kw>(handle) parameter,
!     [msg_exp]either of type <kw>(handle_t) or a type with
!     [msg_exp]the <kw>([handle]) attribute.
!     [msg_exp]It cannot have the <kw>([ptr]) attribute, since it
!     [msg_exp]cannot be <kw>(NULL).
!     [msg_act]Remove the <kw>([ptr]) attribute from the parameter.
!     [end_msg]

    PTRATTRPTR      <The attribute [ptr] is valid only for pointer types or array parameter types>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PTRATTRPTR
!     [msg_text]The attribute <kw>([ptr]) is valid only for pointer types or array parameter types
!     {severity_level}ERROR
!     [msg_exp]The <kw>([ptr]) attribute modifies the default behavior of any
!     [msg_exp]pointer type or an array parameter data type.
!     [msg_exp]It is not valid on other data types.
!     [msg_exp]Note that a <kw>(void *) object is not considered a pointer.
!     [msg_act]There may be a missing <kw>(*) in your declaration.
!     [msg_act]If not, remove the <kw>([ptr]) attribute.
!     [end_msg]

    PTRBASETYP      <The base type of a pipe cannot be or contain a pointer>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PTRBASETYP
!     [msg_text]The base type of a <kw>(pipe) cannot be, or contain, a pointer
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow the base type of a <kw>(pipe) to be a pointer
!     [msg_exp]or any data type that contains pointers.
!     [msg_act]Change your <kw>(pipe) type definition so the base type of
!     [msg_act]the <kw>(pipe) is not a pointer and does not contain any pointers.
!     [end_msg]

    PTRCFMTARR      <Incorrect syntax for pointer to conformant array>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PTRCFMTARR
!     [msg_text]Incorrect syntax for pointer to conformant array
!     {severity_level}ERROR
!     [msg_exp]An IDL declaration attempts to use mixed pointer <kw>(*) and
!     [msg_exp]array <kw>([]) syntax to declare a pointer to an array that has
!     [msg_exp]bounds that are not fixed.  It is
!     [msg_exp]ambiguous whether size attributes apply to the pointer or the
!     [msg_exp]array, thus IDL does not allow mixing pointer and array syntax
!     [msg_exp]when size attributes are present.
!     [msg_act]If the declaration is a parameter declaration and the
!     [msg_act]top-level <kw>(*) is a pointer to an array, that <kw>(*) is
!     [msg_act]unnecessary.  This is because arrays are implicitly passed
!     [msg_act]by reference, as in C.  Remove the top-level <kw>(*).
!     [msg_act]If the declaration is a field of a structure, change the
!     [msg_act]declaration to use arrayified pointer syntax.  A field that is
!     [msg_act]a pointer to a type and also has a conformant array attribute
!     [msg_act]represents a pointer to an array of that type.
!     [msg_act]If the declaration is an [out] parameter, see the description
!     [msg_act]of the message "An [out] conformant array must be a top-level
!     [msg_act]parameter or under a full pointer."
!     [end_msg]

    PTRCTXHAN       <Full pointers to context handles are not allowed>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PTRCTXHAN
!     [msg_text]Full pointers to context handles are not allowed
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow full pointers 
!     [msg_exp]to context handles.
!     [msg_act]If the declaration has the <kw>([ptr]) attribute, either explicitly
!     [msg_act]or by default, change it to a <kw>([ref]) pointer.
!     [msg_act]However, if the
!     [msg_act]declaration is an operation result, it cannot be a <kw>([ref])
!     [msg_act]pointer and an operation resulting in a pointer to a
!     [msg_act]context handle is not possible.
!     [end_msg]

    PTRPIPE         <The attribute [ptr] is not valid on pipe parameters>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PTRPIPE
!     [msg_text]The attribute <kw>([ptr]) is not valid on <kw>(pipe) parameters
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow the <kw>([ptr]) attribute
!     [msg_exp]on <kw>(pipe) parameters.
!     [msg_act]Remove the <kw>([ptr]) attribute from the parameter.
!     [end_msg]

    PTRV1ENUM       <Pointers to [v1_enum] types are not allowed>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PTRV1ENUM
!     [msg_text]Pointers to <kw>([v1_enum]) types are not allowed
!     {severity_level}ERROR
!     [msg_exp]An enumeration type with the <kw>([v1_enum]) attribute follows
!     [msg_exp]NCS Version 1 NIDL semantics, and does not allow pointers to
!     [msg_exp]enumerations except as a reference passing mechanism on a
!     [msg_exp]parameter.
!     [msg_act]If compatibility with NCS Version 1 NIDL is not necessary,
!     [msg_act]remove the <kw>([v1_enum]) attribute.
!     [end_msg]

    PTRVARYARR      <Pointers to varying arrays are not allowed>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PTRVARYARR
!     [msg_text]Pointers to varying arrays are not allowed
!     {severity_level}ERROR
!     [msg_exp]An IDL declaration attempts to use mixed pointer <kw>(*) and
!     [msg_exp]array <kw>([]) syntax to declare a pointer to an array that has
!     [msg_exp]varying bounds.  Usually, it is
!     [msg_exp]ambiguous whether size attributes apply to the pointer or the
!     [msg_exp]array, thus IDL does not allow mixing pointer and array syntax
!     [msg_exp]when size attributes are present.
!     [msg_act]If the declaration is a parameter declaration and the
!     [msg_act]top-level <kw>(*) is a pointer to an array, that <kw>(*) is
!     [msg_act]unnecessary.  This is because arrays are implicitly passed
!     [msg_act]by reference, as in C.  Remove the top-level <kw>(*).
!     [msg_act]If the declaration is more complex,
!     [msg_act]investigate alternative ways to achieve
!     [msg_act]results, such as using a pointer to a
!     [msg_act]structure that has the required information.
!     [end_msg]

    PTRVOIDCTX      <void * must be used in conjunction with the [context_handle] attribute>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PTRVOIDCTX
!     [msg_text]<kw>(void *) must be used in conjunction with the <kw>([context_handle]) attribute
!     {severity_level}ERROR
!     [msg_exp]The only valid use of a <kw>(void *) data type in an interface
!     [msg_exp]definition is on an item with the <kw>([context_handle]) attribute.
!     [msg_act]Change the data type in the declaration, or add the
!     [msg_act]<kw>([context_handle]) attribute.
!     [end_msg]

    REFATTRPTR      <The attribute [ref] is valid only for pointer types or array parameter types>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]REFATTRPTR
!     [msg_text]The attribute <kw>([ref]) is valid only for pointer types or array parameter types
!     {severity_level}ERROR
!     [msg_exp]The <kw>([ref]) attribute modifies the default behavior of any
!     [msg_exp]pointer type or an array parameter data type.
!     [msg_exp]It is not valid on other data types.
!     [msg_exp]Note that a <kw>(void *) object is not considered a pointer.
!     [msg_act]There may be a missing <kw>(*) in your declaration.
!     [msg_act]If not, remove the <kw>([ref]) attribute.
!     [end_msg]

    REFFUNRES       <A [ref] function result is not valid>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]REFFUNRES
!     [msg_text]A <kw>([ref]) function result is not valid
!     {severity_level}ERROR
!     [msg_exp]Unlike <kw>([out]) parameters, there is never any pre-existing
!     [msg_exp]storage in the caller for pointer valued function results.
!     [msg_exp]The pointer always indicates new storage.  This is the
!     [msg_exp]capability provided by full pointers only, not
!     [msg_exp]<kw>([ref]) pointers.
!     [msg_act]Remove the <kw>([ref]) attribute from the type definition
!     [msg_act]of the data type of the function result.
!     [end_msg]

    RENAMEFAILED    <Rename of !AZ to !AZ failed>/FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]RENAMEFAILED
!     [msg_text]Rename of <v>(filename1) to <v>(filename2) failed
!     {severity_level}ERROR
!     [msg_exp]The call to rename <v>(filename1) to <v>(filename2) returned
!     [msg_exp]a failing status.
!     [msg_act]Make sure the filenames and paths are valid, you have privilege
!     [msg_act]to perform the rename operation, and there is enough free space on the
!     [msg_act]destination device.
!     [end_msg]

    REPASNEST       <Types with [represent_as] cannot be nested>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]REPASNEST
!     [msg_text]Types with <kw>([represent_as]) cannot be nested
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow a data type that has a <kw>([represent_as]) type
!     [msg_exp]to itself be used as a <kw>([represent_as]) type in another type
!     [msg_exp]definition.
!     [msg_act]Do not nest <kw>([represent_as]) types.
!     [end_msg]

    SCOPELVLS       <Too many scoping levels>

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]SCOPELVLS
!     [msg_text]Too many scoping levels
!     {severity_level}ERROR
!     [msg_exp]The input source is too complicated to be parsed.  This
!     [msg_exp]occurs because of deeply
!     [msg_exp]nested <kw>(struct) or <kw>(union) declarations.
!     [msg_act]Simplify the input source by using <kw>(typedef)s to represent the
!     [msg_act]nested <kw>(struct) or <kw>(union) declarations, and
!     [msg_act]building the more
!     [msg_act]complicated <kw>(struct) or <kw>(union) declarations
!     [msg_act]by referencing the
!     [msg_act]named types instead of in-line specification of the <kw>(struct)
!     [msg_act]or <kw>(union) declaration.
!     [end_msg]

    SIZEARRTYPE     <Size attributes can only be applied to array types>

!     {engineer_name}Beth
!     {audience}programmer
!     [begin_msg]
!     [ident]SIZEARRTYPE
!     [msg_text]Size attributes can only be applied to array types
!     {severity_level}ERROR
!     [msg_exp]Array bound attributes are only valid when used on array types or pointers
!     [msg_exp]used as an array.
!     [msg_act]Remove the array bound attribute or correct the type to be
!     [msg_act]an array.
!     [end_msg]

    SIZECFMTYPE     <The attribute [size_is] cannot be applied to dimension !UL; upper bound is not dynamic> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SIZECFMTYPE
!     [msg_text]The attribute <kw>([size_is]) cannot be applied to dimension <v>(n); upper bound is not dynamic
!     {severity_level}ERROR
!     [msg_exp]A <kw>(size_is) variable is only valid for array dimensions that
!     [msg_exp]have an upper bound that is not fixed.
!     [msg_act]If the array is multidimensional, you may have specified the
!     [msg_act]<kw>(size_is) variable for the wrong dimension.  See the
!     [msg_act]documentation for the proper syntax.  If the array has only
!     [msg_act]one dimension, the <kw>(size_is) clause is invalid, since the
!     [msg_act]array's upper data limit is fixed.  You may have meant to use a
!     [msg_act]<kw>([length_is]) clause.  See the documentation for the
!     [msg_act]distinctions between conformant and varying arrays
!     [msg_act]and their attributes.
!     [end_msg]

    SIZEINATTR      <A [size_is] parameter must have the [in] attribute>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SIZEINATTR
!     [msg_text]A <kw>([size_is]) parameter must have the <kw>([in]) attribute
!     {severity_level}ERROR
!     [msg_exp]The <kw>([size_is]) attribute specifies a parameter that contains
!     [msg_exp]size information of a conformant array.  This must be an <kw>([in])
!     [msg_exp]attribute so the server stub code knows how much
!     [msg_exp]space to allocate for the array.
!     [msg_act]Change the size parameter referenced by the
!     [msg_act]<kw>([size_is]) clause to have the <kw>([in]) attribute.
!     [end_msg]

    SIZEMISMATCH    <The size attributes do not match the array dimension>

!     {engineer_name}Beth
!     {audience}programmer
!     [begin_msg]
!     [ident]SIZEMISMATCH
!     [msg_text]The size attributes do not match the array dimension
!     {severity_level}ERROR
!     [msg_exp]There are too many attribute references for the number of
!     [msg_exp]dimensions defined for the array.
!     [msg_act]Modify the array bound attribute, specifying one reference
!     [msg_act]for each dimension of the array.
!     [end_msg]

    SIZEPRMPTR      <The array size attribute variable !AZ cannot have the [ptr] or [unique] attributes> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SIZEPRMPTR
!     [msg_text]The array size attribute variable <v>(name) cannot have the <kw>([ptr]) or <kw>([unique]) attributes
!     {severity_level}ERROR
!     [msg_exp]An array size attribute clause in the source IDL specifies
!     [msg_exp]its value by dereferencing a pointer parameter.
!     [msg_exp]This is invalid if the pointer variable has either of the
!     [msg_exp]<kw>([ptr]) or <kw>([unique]) attributes, since both
!     [msg_exp]allow the pointer
!     [msg_exp]to be <kw>(NULL).  If the pointer is <kw>(NULL), there is no
!     [msg_exp]way to determine the size of the array.
!     [msg_act]Either redeclare the size variable as a scalar rather
!     [msg_act]than a pointer, or change the size variable to a <kw>([ref])
!     [msg_act]pointer.
!     [end_msg]

    SIZETYPEINT     <A [size_is] variable must be a small, short, or long integer>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SIZETYPEINT
!     [msg_text]A <kw>([size_is]) variable must be a <kw>(small), <kw>(short), or <kw>(long) integer
!     {severity_level}ERROR
!     [msg_exp]The <kw>([size_is]) attribute specifies a field or parameter that contains
!     [msg_exp]size information of a conformant array.  Array bounds, and
!     [msg_exp]thus array size,
!     [msg_exp]must be integers which are not <kw>(hyper).
!     [msg_act]Change the size information field or parameter referenced by the
!     [msg_act]<kw>([size_is]) clause to be of integer data type.
!     [end_msg]

    SIZEVARREPAS    <A size attribute variable must not have a represent_as type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SIZEVARREPAS
!     [msg_text]A size attribute variable must not have a <kw>([represent_as]) type
!     {severity_level}ERROR
!     [msg_exp]The parameter or field referenced
!     [msg_exp]in a <kw>([max_is]),
!     [msg_exp]<kw>([size_is]), <kw>([first_is]), <kw>([last_is]),
!     [msg_exp]or <kw>([length_is]) clause must
!     [msg_exp]not be of a data type that is declared to have a
!     [msg_exp]<kw>([represent_as]) type.
!     [msg_act]Change either the referenced parameter or field data type,
!     [msg_act]or the data type definition itself.
!     [end_msg]
!
!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]SIZEVARREPAS
!     [msg_text]A size attribute variable must not have a <kw>([represent_as]) type
!     {severity_level}ERROR
!     [msg_exp]The parameter or field referenced
!     [msg_exp]in a <kw>([min_is]), <kw>([max_is]),
!     [msg_exp]<kw>([size_is]), <kw>([first_is]), <kw>([last_is]),
!     [msg_exp]or <kw>([length_is]) clause must
!     [msg_exp]not be of a data type that is declared to have a
!     [msg_exp]<kw>([represent_as]) type.
!     [msg_act]Change either the referenced parameter or field data type,
!     [msg_act]or the data type definition itself.
!     [end_msg_DEC]

    SIZEVARXMITAS   <A size attribute variable must not have a transmit_as type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SIZEVARXMITAS
!     [msg_text]A size attribute variable must not have a <kw>([transmit_as]) type
!     {severity_level}ERROR
!     [msg_exp]The parameter or field referenced
!     [msg_exp]in a <kw>([max_is]),
!     [msg_exp]<kw>([size_is]), <kw>([first_is]), <kw>([last_is]),
!     [msg_exp]or <kw>([length_is]) clause must
!     [msg_exp]not be of a data type that is declared to have a
!     [msg_exp]<kw>([transmit_as]) type.
!     [msg_act]Change either the referenced parameter or field data type,
!     [msg_act]or the data type definition itself.
!     [end_msg]
!
!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]SIZEVARXMITAS
!     [msg_text]A size attribute variable must not have a <kw>([transmit_as]) type
!     {severity_level}ERROR
!     [msg_exp]The parameter or field referenced
!     [msg_exp]in a <kw>([min_is]), <kw>([max_is]),
!     [msg_exp]<kw>([size_is]), <kw>([first_is]), <kw>([last_is]),
!     [msg_exp]or <kw>([length_is]) clause must
!     [msg_exp]not be of a data type that is declared to have a
!     [msg_exp]<kw>([transmit_as]) type.
!     [msg_act]Change either the referenced parameter or field data type,
!     [msg_act]or the data type definition itself.
!     [end_msg_DEC]

    SMALLARRSYN     <The attribute [v1_array] must be in array, not pointer, syntax>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SMALLARRSYN
!     [msg_text]The attribute <kw>([v1_array]) must be in array, not pointer, syntax
!     {severity_level}ERROR
!     [msg_exp]The <kw>([v1_array]) attribute is for compatibility with
!     [msg_exp]NCS Version 1 arrays.  NCS Version 1 NIDL did
!     [msg_exp]not allow pointers to represent arrays.
!     [msg_act]Define the array using array syntax.
!     [end_msg]

    SMALLCFMT       <A conformant [v1_array] must also be varying>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SMALLCFMT
!     [msg_text]A conformant <kw>([v1_array]) must also be varying
!     {severity_level}ERROR
!     [msg_exp]An array with the <kw>([v1_array]) attribute that has a
!     [msg_exp]conformant upper bound that is not fixed must also be varying (it must
!     [msg_exp]have a <kw>([last_is]) or <kw>([length_is]) attribute in
!     [msg_exp]addition to the <kw>([max_is]) or <kw>([size_is])
!     [msg_exp]attribute).
!     [msg_act]If compatibility with NCS Version 1 NIDL is not necessary, remove the
!     [msg_act]<kw>([v1_array]) attribute.
!     [msg_act]Otherwise, add a <kw>([last_is]) or <kw>([length_is])
!     [msg_act]attribute to the declaration.
!     [end_msg]

    SMALLINV        "The attribute [v1_array] is invalid for an array with more than 65535 elements"

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SMALLINV
!     [msg_text]The attribute <kw>([v1_array]) is invalid for an array with more than 65535 elements
!     {severity_level}ERROR
!     [msg_exp]An array with the <kw>([v1_array]) attribute is limited to a total
!     [msg_exp]of 65535 or fewer elements.  The declared array has too many
!     [msg_exp]elements to be <kw>([v1_array]).
!     [msg_act]Remove the <kw>([v1_array]) attribute from the array declaration.
!     [end_msg]

    SMALLMINFIRST   <The attribute [v1_array] cannot occur with the [min_is] or [first_is] attributes>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SMALLMINFIRST
!     [msg_text]The attribute <kw>([v1_array]) cannot occur with the <kw>([first_is]) attribute
!     {severity_level}ERROR
!     [msg_exp]The only valid array attributes for the
!     [msg_exp]<kw>([v1_array]) attribute are 
!     [msg_exp]<kw>([max_is]), <kw>([last_is]), and 
!     [msg_exp]<kw>([length_is]). 
!     [msg_act]If you require compatibility with NCS 
!     [msg_act]Version 1 NIDL, the lower bound
!     [msg_act]of the array must be fixed; therefore,
!     [msg_act]remove the 
!     [msg_act]<kw>([first_is]) attribute and change the array declaration.
!     [msg_act]Otherwise, remove the <kw>([v1_array]) attribute.
!     [end_msg]
!
!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]SMALLMINFIRST
!     [msg_text]The attribute <kw>([v1_array]) cannot occur with the <kw>([min_is]) or <kw>([first_is]) attributes
!     {severity_level}ERROR
!     [msg_exp]The only valid array attributes for the
!     [msg_exp]<kw>([v1_array]) attribute are 
!     [msg_exp]<kw>([max_is]), <kw>([size_is]), <kw>([last_is]), and 
!     [msg_exp]<kw>([length_is]). 
!     [msg_act]If you require compatibility with NCS 
!     [msg_act]Version 1 NIDL, the lower bound
!     [msg_act]of the array must be fixed; therefore,
!     [msg_act]remove the <kw>([min_is]) or 
!     [msg_act]<kw>([first_is]) attribute and change the array declaration.
!     [msg_act]Otherwise, remove the <kw>([v1_array]) attribute.
!     [end_msg_DEC]

    SMALLMULTID     <A [v1_array] can be conformant or varying in first dimension only>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SMALLMULTID
!     [msg_text]A <kw>([v1_array]) can be conformant or varying in first dimension only
!     {severity_level}ERROR
!     [msg_exp]An array with the <kw>([v1_array]) attribute is limited to
!     [msg_exp]a conformant or varying upper data limit
!     [msg_exp]in the first dimension only.
!     [msg_act]Remove the <kw>([v1_array]) attribute from the array declaration.
!     [end_msg]

    SMALLOPENLB     <A [v1_array] cannot have a conformant lower bound>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SMALLOPENLB
!     [msg_text]A <kw>([v1_array]) cannot have a conformant lower bound
!     {severity_level}ERROR
!     [msg_exp]A <kw>([v1_array]) specifies that the array be handled in a
!     [msg_exp]manner compatible with NCS Version 1 NIDL, which did not
!     [msg_exp]support non-fixed lower bounds for arrays.
!     [msg_act]If compatibility with NCS Version 1 is required, the array
!     [msg_act]must not have a conformant lower bound.  Otherwise, remove
!     [msg_act]the <kw>([v1_array]) attribute.
!     [end_msg]

    STRARRAY        <Arrays of strings are not allowed>

!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]STRARRAY
!     {msg_text]Arrays of strings are not allowed
!     {severity_level}ERROR
!     {msg_exp]IDL does not allow arrays of objects with the <kw>([string])
!     {msg_exp]attribute.
!     {msg_act]Place the <kw>([string]) data into a structure, and define an array
!     {msg_act]of that structure (IDL does allow arrays of structures).
!     {end_msg]

    STRARRAYV1      <The attribute [string] cannot be applied to a [v1_array]>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]STRARRAYV1
!     [msg_text]The attribute <kw>([string]) cannot be applied to a <kw>([v1_array])
!     {severity_level}ERROR
!     [msg_exp]A string that is compatible with the NCS Version 1 NIDL <kw>(string0) data
!     [msg_exp]type must have the <kw>([v1_array]) and <kw>([v1_string]) attributes.
!     [msg_exp]The <kw>([string]) attribute only applies
!     [msg_exp]without the <kw>([v1_array])
!     [msg_exp]attribute.  A <kw>([v1_array]) is for
!     [msg_exp]compatibility with NCS Version 1 NIDL.
!     [msg_act]If compatibility with NCS Version 1 NIDL is required,
!     [msg_act]place the <kw>([v1_string]) attribute on the declaration.
!     [msg_act]Otherwise, remove the <kw>([v1_array]) attribute from the
!     [msg_act]array declaration.
!     [end_msg]

    STRCHARBYTE     <The attribute [string] is valid only for one-dimensional array of valid base type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]STRCHARBYTE
! [msg_text]The attribute <kw>([string]) is valid only for one-dimensional array of valid base type
!     {severity_level}ERROR
!     [msg_exp]The <kw>([string]) attribute flags that an array is a
!     [msg_exp]string.  Strings must be arrays whose base type is one of the
!     [msg_exp]following: char, byte, unsigned short, unsigned long,
!     [msg_exp]or a structure whose fields are all scalar byte fields.
!     [msg_act]Remove the <kw>([string]) attribute, or change the data type
!     [msg_act]so it is a valid string.
!     [end_msg]

    STRUCTXMITCFMT  <Structures with [transmit_as] or [represent_as] cannot be conformant>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]STRUCTXMITCFMT
!     [msg_text]Structures with <kw>([transmit_as]) or <kw>([represent_as]) cannot be conformant
!     {severity_level}ERROR
!     [msg_exp]IDL does not support a conformant structure with the
!     [msg_exp]<kw>([transmit_as]) or <kw>([represent_as]) attribute.
!     [msg_act]Either change the declaration so the structure fields are of
!     [msg_act]a fixed size and have none of the conformant array attributes
!     [msg_act]<kw>([max_is]) or <kw>([size_is]),
!     [msg_act]or remove the <kw>([transmit_as]) or <kw>([represent_as]) attribute.
!     [end_msg]
!
!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]STRUCTXMITCFMT
!     [msg_text]Structures with <kw>([transmit_as]) or <kw>([represent_as]) cannot be conformant
!     {severity_level}ERROR
!     [msg_exp]IDL does not support a conformant structure with the
!     [msg_exp]<kw>([transmit_as]) or <kw>([represent_as]) attribute.
!     [msg_act]Either change the declaration so the structure fields are of
!     [msg_act]a fixed size and have none of the conformant array attributes
!     [msg_act]<kw>([min_is]), <kw>([max_is]), or <kw>([size_is]),
!     [msg_act]or remove the <kw>([transmit_as]) or <kw>([represent_as]) attribute.
!     [end_msg_DEC]

    STRUNTERM       <Unterminated string literal>

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]STRUNTERM
!     [msg_text]Unterminated string literal
!     {severity_level}ERROR
!     [msg_exp]There is a missing closing double quote (<kw>(")) on the string literal.
!     [msg_act]Make sure the the closing quote for the string literal is on
!     [msg_act]the same source line as the starting quote.  Also verify that
!     [msg_act]any double quote characters internal to the string are
!     [msg_act]preceded by the escape character (<literal>(\)).
!     [end_msg]

    STRV1ARRAY      <The [v1_string] attribute can only be applied to a [v1_array]>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]STRV1ARRAY
!     [msg_text]The <kw>([v1_string]) attribute can only be applied to a <kw>([v1_array])
!     {severity_level}ERROR
!     [msg_exp]A <kw>([v1_string]) is compatible with
!     [msg_exp]the NCS Version 1 NIDL <kw>(string0) data
!     [msg_exp]type.  It can be applied only to an array
!     [msg_exp]compatible with NCS Version 1. For example,
!     [msg_exp]an array with the <kw>([v1_array]) attribute.
!     [msg_act]If compatibility with NCS Version 1 NIDL is required,
!     [msg_act]place the <kw>([v1_array]) attribute on the array.
!     [msg_act]Otherwise, change the <kw>([v1_string]) attribute to <kw>([string]),
!     [msg_act]that is a generalization of <kw>([v1_string]).
!     [end_msg]

    STRV1FIXED      <A [v1_string] must be an array of char with fixed bounds>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]STRV1FIXED
!     [msg_text]A <kw>([v1_string]) must be an array of <kw>(char) with fixed bounds
!     {severity_level}ERROR
!     [msg_exp]A <kw>([v1_string]) is compatible with
!     [msg_exp]the NCS Version 1 NIDL <kw>(string0) data
!     [msg_exp]type.  It must be a single dimensioned array of <kw>(char) with
!     [msg_exp]a zero lower bound and a fixed upper bound.
!     [msg_act]If compatibility with NCS Version 1 NIDL is required,
!     [msg_act]change the declaration accordingly.
!     [msg_act]Otherwise, see the documentation regarding the <kw>([string])
!     [msg_act]attribute.
!     [end_msg]

    STRVARY         <A [string] array cannot have varying array attributes>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]STRVARY
!     [msg_text]A <kw>([string]) array cannot have varying array attributes
!     {severity_level}ERROR
!     [msg_exp]The <kw>([string]) attribute says that the length of a string is
!     [msg_exp]an intrinsic property of the string itself; for example, C
!     [msg_exp]strings are zero-terminated.
!     [msg_act]Remove any <kw>([first_is]), <kw>([last_is]),
!     [msg_act]or <kw>([length_is]) clauses
!     [msg_act]from the string declaration.
!     [end_msg]

    STSATTRONCE     <The attribute [!AZ] can occur at most once per operation> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]STSATTRONCE
!     [msg_text]The attribute <v>([attribute-name]) can occur at most once per operation
!     {severity_level}ERROR
!     [msg_exp]A <kw>([comm_status]) or <kw>([fault_status]) parameter or operation
!     [msg_exp]result is used to return an error code if a certain type of
!     [msg_exp]error occurs during execution of an operation.  Do not use
!     [msg_exp]either attribute more than once per operation.
!     [msg_act]Remove the redundant usages of the attribute
!     [msg_act]from the interface's Attribute Configuration File (ACF).
!     [end_msg]

    STSPRMOUT       <A parameter with [!AZ] must be an [out] parameter> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]STSPRMOUT
!     [msg_text]A parameter with <v>([attribute-name]) must be an <kw>([out]) parameter
!     {severity_level}ERROR
!     [msg_exp]A <kw>([comm_status]) or <kw>([fault_status]) parameter
!     [msg_exp]is used to return
!     [msg_exp]an error code if a certain type of error occurs during
!     [msg_exp]execution of an operation.  Thus, it must be an <kw>([out])
!     [msg_exp]parameter.
!     [msg_act]Place the <kw>([out]) attribute on the parameter.
!     [end_msg]

    STSRETVAL       <An operation with [!AZ] must return a value of type error_status_t> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]STSRETVAL
!     [msg_text]An operation with <v>([attribute-name]) must return a value of type <kw>(error_status_t)
!     {severity_level}ERROR
!     [msg_exp]The <kw>([comm_status]) or <kw>([fault_status]) attribute
!     [msg_exp]on an operation
!     [msg_exp]signifies that the return value of the operation is used to
!     [msg_exp]return an error code if a certain type of error occurs during
!     [msg_exp]execution of an operation.  Thus, the operation must be
!     [msg_exp]declared to deliver a result of data type <kw>(error_status_t).
!     [msg_act]Declare the operation result to be of data type <kw>(error_status_t).
!     [end_msg]

    STSVARTYPE      <A parameter with [!AZ] must be of type error_status_t> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]STSVARTYPE
!     [msg_text]A parameter with <v>([attribute-name]) must be of type <kw>(error_status_t)
!     {severity_level}ERROR
!     [msg_exp]A <kw>([comm_status]) or <kw>([fault_status]) parameter
!     [msg_exp]or function result
!     [msg_exp]is used to return an error code if a certain type of error
!     [msg_exp]occurs during execution of an operation.  The data type of the
!     [msg_exp]variable must be <kw>(error_status_t).
!     [msg_act]Declare the variable to be of data type <kw>(error_status_t).
!     [end_msg]

    SYNTAXERR       <Syntax error>

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]SYNTAXERR
!     [msg_text]Syntax error
!     {severity_level}ERROR
!     [msg_exp]The source input is incorrect and no interpretation
!     [msg_exp]can be made.
!     [msg_act]Examine the source input on and near the source listed
!     [msg_act]and correct any errors.
!     [end_msg]

    SYNTAXNEAR      <Syntax error near "!AD"> /FAO_COUNT=3

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]SYNTAXNEAR
!     [msg_text]Syntax error near <v>(source-text)
!     {severity_level}ERROR
!     [msg_exp]The source input is incorrect and no interpretation
!     [msg_exp]can be made for <v>(source-text).
!     [msg_act]Examine the source input on and near the source listed
!     [msg_act]and correct any errors.
!     [end_msg]

    SYNTAXUUID      <Syntax error in UUID format>/FAO_COUNT=0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]SYNTAXUUID
!     [msg_text]Syntax error in UUID format
!     {severity_level}ERROR
!     [msg_exp]The UUID specified in the <kw>([uuid]) attribute of an interface
!     [msg_exp]is not in a valid format.
!     [msg_act]Make sure the UUID is transcribed correctly and
!     [msg_act]contains the required punctuation.
!     [end_msg]

    SYSERRMSG       <System error message: !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SYSERRMSG
!     [msg_text]System error message: <v>(message-text)
!     {severity_level}ERROR
!     [msg_exp]Identifies the cause of the
!     [msg_exp]previous error.
!     [msg_act]See the system documentation for an explanation
!     [msg_act]of the error message.
!     [end_msg]

    TOOMANYELEM     <Too many elements in !AZ> /FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]TOOMANYELEM
!     [msg_text]Too many elements in <v>(item)
!     {severity_level}ERROR
!     [msg_exp]The <kw>(enum) <v>(item) contains more than 32767 elements.
!     [msg_act]Reduce the number of elements included in the
!     [msg_act]<kw>(enum).
!     [end_msg]

    TOOMANYPORT     <Too many endpoint specifications; Maximum is !UL> /FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]TOOMANYPORT
!     [msg_text]Too many endpoint specifications; Maximum is <v>(number)
!     {severity_level}ERROR
!     [msg_exp]There are more endpoint specifications than IDL can process.
!     [msg_act]Remove some of the endpoint specifications listed
!     [msg_act]in the <kw>([endpoint]) attribute.
!     [msg_act]Specify at most <v>(number) endpoint specifications for an interface.
!     [end_msg]

    TYPENOTFND      <Type name: !AZ not found> /FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]TYPENOTFND
!     [msg_text]Type name: <v>(name) not found
!     {severity_level}ERROR
!     [msg_exp]The type <v>(name) is not declared at this point in the
!     [msg_exp]interface declaration.
!     [msg_act]Correct the spelling of <v>(name) if incorrect, and make sure
!     [msg_act]that a definition of <v>(name) precedes the reference.
!     [end_msg]

    TYPLINEATTR     <Type attributes [in_line] and [out_of_line] cannot occur together>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]TYPLINEATTR
!     [msg_text]Type attributes <kw>([in_line]) and <kw>([out_of_line]) cannot occur together
!     {severity_level}ERROR
!     [msg_exp]The <kw>([in_line]) and <kw>([out_of_line]) attributes are
!     [msg_exp]opposite in meaning.
!     [msg_exp]You can specify at most one of these attributes on a type.
!     [msg_act]Remove one of the conflicting attributes.
!     [end_msg]

    TYPNOTDEF       <Type name !AZ referenced in ACF is not defined> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]TYPNOTDEF
!     [msg_text]Type name <v>(name) referenced in ACF is not defined
!     {severity_level}ERROR
!     [msg_exp]The context in which a type name is used in the Attribute
!     [msg_exp]Configuration File (ACF) requires that the type be defined
!     [msg_exp]in the corresponding IDL file.
!     [msg_act]See if the referenced type is defined in some other interface.
!     [msg_act]If so, the ACF reference belongs in the ACF for that interface.
!     [msg_act]Check for typographical errors in both the IDL and ACF files.  Check that
!     [msg_act]you have not omitted a type definition in the IDL file.
!     [end_msg]

    UNBALPARENS     <Unbalanced parentheses> /FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]UNBALPARENS
!     [msg_text]Unbalanced parentheses.
!     {severity_level}ERROR
!     [msg_exp]The number of left parentheses and right parentheses are not equal.
!     [msg_act]Make sure the number of left parentheses equals the number of right
!     [msg_act]parentheses and that each parenthesis is in the correct place.
!     [end_msg]

    UNBALBRACKETS    <Unbalanced brackets> /FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]UNBALBRACKETS
!     [msg_text]Unbalanced brackets.
!     {severity_level}ERROR
!     [msg_exp]The number of left brackets and right brackets are not equal.
!     [msg_act]Make sure the number of left brackets equals the number of right
!     [msg_act]brackets and that each bracket is in the correct place.
!     [end_msg]

    UNBALBRACES     <Unbalanced braces> /FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]UNBALBRACES
!     [msg_text]Unbalanced braces.
!     {severity_level}ERROR
!     [msg_exp]The number of left braces and right braces are not equal.
!     [msg_act]Make sure the number of left braces equals the number of right
!     [msg_act]braces and that each brace is in the correct place.
!     [end_msg]

    UNIDISCTYP      <A union discriminator type must be small, short, long, char, boolean, or enum>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]UNIDISCTYP
!     [msg_text]A <kw>(union) discriminator type must be <kw>(small, short, long, char, boolean,) or <kw>(enum)
!     {severity_level}ERROR
!     [msg_exp]The discriminator of a <kw>(union), that determines which case of
!     [msg_exp]the <kw>(union) is used, is restricted to integer, character,
!     [msg_exp]enumeration, and boolean data types.  Also, <kw>(hyper int)
!     [msg_exp]discriminators are not allowed.
!     [msg_act]Change the discriminator to one of the valid data types.
!     [msg_act]Make sure the data type of the <kw>(constant) values
!     [msg_act]in the <kw>(case)
!     [msg_act]clauses within the <kw>(union) agree with the discriminator data type.
!     [end_msg]

    UNIQATTRHAN     <The attribute [unique] is invalid on a binding handle parameter>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]UNIQATTRHAN
!     [msg_text]The attribute <kw>([unique]) is invalid on a binding <kw>(handle) parameter
!     {severity_level}ERROR
!     [msg_exp]The first parameter in an operation is a <kw>(handle) parameter,
!     [msg_exp]which is either of type <kw>(handle_t) or a type
!     [msg_exp]with the <kw>([handle]) attribute.
!     [msg_exp]A binding handle parameter cannot have the <kw>([unique]) 
!     [msg_exp]attribute because it cannot be <kw>(NULL).
!     [msg_act]Remove the <kw>([unique]) attribute from the parameter.
!     [end_msg_DEC]

    UNIQATTRPTR     <The attribute [unique] is valid only for pointer types or array parameter types>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]UNIQATTRPTR
!     [msg_text]The attribute <kw>([unique]) is valid only for pointer types or array parameter types
!     {severity_level}ERROR
!     [msg_exp]The <kw>([unique]) attribute modifies the default behavior 
!     [msg_exp]of any pointer type or an array parameter data type.
!     [msg_exp]It is not valid on other data types.
!     [msg_exp]Note that a <kw>(void *) object is not considered a pointer.
!     [msg_act]There may be a missing asterisk (<kw>(*)) in your declaration.
!     [msg_act]If not, remove the <kw>([unique]) attribute.
!     [end_msg_DEC]

    UNIQCTXHAN      <[unique] pointers to context handles are not allowed>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]UNIQCTXHAN
!     [msg_text]<kw>([unique]) pointers to context handles are not allowed
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow <kw>([unique]) pointers to context handles.
!     [msg_act]If the declaration has the <kw>([unique]) attribute, either
!     [msg_act]explicitly or by default, change it to a <kw>([ref]) pointer.
!     [msg_act]However, if the
!     [msg_act]declaration is an operation result, it cannot be a <kw>([ref])
!     [msg_act]pointer and an operation resulting in a pointer to a
!     [msg_act]context handle is not valid.
!     [end_msg_DEC]

    UNIQFUNRES      <A [unique] function result is not valid>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]UNIQFUNRES
!     [msg_text]A <kw>([unique]) function result is not valid
!     {severity_level}ERROR
!     [msg_exp]Unlike <kw>([out]) parameters, there is never any 
!     [msg_exp]preexisting storage in the caller for
!     [msg_exp]pointer-valued function results.
!     [msg_exp]The pointer always points to new storage.
!     [msg_exp]This is the
!     [msg_exp]capability provided only by full pointers, not by
!     [msg_exp]<kw>([unique]) pointers.
!     [msg_act]Remove the <kw>([unique]) attribute from the type definition
!     [msg_act]of the data type for the function result.
!     [end_msg_DEC]

    UNKNOWNATTR     <Unknown attribute [!AZ]>/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]UNKNOWNATTR
!     [msg_text]Unknown attribute <v>([attribute-name])
!     {severity_level}ERROR
!     [msg_exp]The attribute specified is not a valid IDL attribute.
!     [msg_act]Check for spelling errors or misplaced syntax.
!     [end_msg]

    USETRANS <Obsolete feature encountered; Use translator>

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]USETRANS
!     [msg_text]Obsolete feature encountered; Use translator
!     {severity_level}ERROR
!     [msg_exp]A feature of NCS Version 1 NIDL was encountered that is not supported in
!     [msg_exp]the IDL compiler.  You may be compiling an NCS Version 1 NIDL source file.
!     [msg_act]If the input is a NCS Version 1 NIDL source file, process it with
!     [msg_act]the translator utility (the <kw>(nidl_to_idl) command) that
!     [msg_act]converts an NCS Version 1 NIDL source file into the format expected by
!     [msg_act]the IDL compiler.  Then compile the translated source file.  Otherwise, remove
!     [msg_act]the reference to the obsolete feature.
!     [end_msg]

    UUIDINV         <The attribute [uuid] is invalid when the [local] attribute is specified>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]UUIDINV
!     [msg_text]The attribute <kw>([uuid]) is invalid when the <kw>([local]) attribute is specified
!     {severity_level}ERROR
!     [msg_exp]The <kw>([local]) interface attribute flags that an interface is not
!     [msg_exp]for RPC use, but is only a local header generation
!     [msg_exp]mechanism.  The <kw>([uuid]) attribute is only used for
!     [msg_exp]RPC interfaces.
!     [msg_act]Remove one of the conflicting attributes.
!     [end_msg]

    VARDECLNOSUP <Variable declarations are not supported; declaration of !AZ ignored>

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]VARDECLNOSUP
!     [msg_text]Variable declarations are not supported; declaration of <v>(variable) ignored
!     {severity_level}ERROR
!     [msg_exp]IDL does not support the declaration of variables.  The types
!     [msg_exp]of data that can be declared in IDL are; constants, types, and
!     [msg_exp]functions.  The declaration of <v>(variable) is not a function
!     [msg_exp]and is incorrect.
!     [msg_act]If intended as a function declaration,
!     [msg_act]correct the declaration so the top-level declaration
!     [msg_act]is a function. Otherwise, remove the declaration.
!     [end_msg]

    VOIDOPPTR       <void is valid only in an operation or a context handle pointer declaration>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]VOIDOPPTR
!     [msg_text]<kw>(void) is valid only in an operation or a pointer declaration
!     {severity_level}ERROR
!     [msg_exp]The <kw>(void) keyword is only valid as the return value of an
!     [msg_exp]operation, or as part of a <kw>(void *) declaration.  Note also
!     [msg_exp]that in RPC interfaces, <kw>(void *) is only valid if the
!     [msg_exp]<kw>([context_handle]) attribute is also specified on the data item.
!     [msg_act]There may be a missing <kw>(*) in your declaration.
!     [msg_act]If not, change the <kw>(void) reference to some valid data type.
!     [end_msg]

    XMITASREP       <A type used in a [transmit_as] clause cannot have a [represent_as] type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]XMITASREP
!     [msg_text]A type used in a <kw>([transmit_as]) clause cannot have a <kw>([represent_as]) type
!     {severity_level}ERROR
!     [msg_exp]The type used in a <kw>([transmit_as]) clause specifies an alternative
!     [msg_exp]type that a base IDL type is translated to or from and before or after
!     [msg_exp]transmission over the network.
!     [msg_exp]The type used in a <kw>([represent_as]) clause is an
!     [msg_exp]alternative type that a base IDL type is translated from or to at
!     [msg_exp]the application code-to-stub code interface.
!     [msg_exp]Going from base IDL type to transmittable type and then to
!     [msg_exp]a <kw>([represent_as]) type (that may not be transmittable) is
!     [msg_exp]invalid.
!     [msg_act]You may have meant to put a <kw>([represent_as]) clause on the base IDL
!     [msg_act]type, not its transmittable type.
!     [end_msg]

    XMITCFMTARR     <A [transmit_as] or [represent_as] type cannot be a conformant array>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]XMITCFMTARR
!     [msg_text]A <kw>([transmit_as]) or <kw>([represent_as]) type cannot be a conformant array
!     {severity_level}ERROR
!     [msg_exp]A type specified in a <kw>([transmit_as]) clause or a type with
!     [msg_exp]the <kw>([transmit_as]) or <kw>([represent_as]) attribute cannot be a
!     [msg_exp]conformant array, since there is nowhere to attach the size
!     [msg_exp]information attribute that is needed to transmit the data.
!     [msg_act]Define a structure that contains a conformant array and
!     [msg_act]a size information field instead of using a conformant array
!     [msg_act]directly.
!     [end_msg]

    XMITPIPEBASE    <The base type of a pipe cannot be a [transmit_as] type>
!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]XMITPIPEBASE
!     [msg_text]The base type of a <kw>(pipe) cannot be a <kw>([transmit_as]) type
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow the base type of
!     [msg_exp]a <kw>(pipe) to have the <kw>([transmit_as]) attribute.
!     [msg_act]Declare the base type of the <kw>(pipe) to be the data type
!     [msg_act]in the <kw>([transmit_as]) clause.
!     [end_msg]

    XMITPTR         <A translated transmittable type cannot contain [!AZ] pointers> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]XMITPTR
!     [msg_text]A translated transmittable type cannot contain <v>([attribute-name]) pointers
!     {severity_level}ERROR
!     [msg_exp]A transmittable type that is derived by calling a translation
!     [msg_exp]routine, for example, a <kw>(from_local) translation
!     [msg_exp]of a <kw>([represent_as])
!     [msg_exp]type or a <kw>(to_xmit) translation of a <kw>([transmit_as]) type,
!     [msg_exp]cannot contain pointers of the indicated type.
!     [msg_act]Define a transmittable type that does not contain invalid pointers.
!     [msg_act]If using <kw>([represent_as]), reconsider using it.
!     [end_msg]

    XMITTYPEATTRS   <A type with [transmit_as] cannot have other type attributes>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]XMITTYPEATTRS
!     [msg_text]A type with <kw>([transmit_as]) cannot have other type attributes
!     {severity_level}ERROR
!     [msg_exp]A data type with the <kw>([transmit_as]) attribute
!     [msg_exp](the presented type) has an
!     [msg_exp]associated transmittable type, and you must write routines
!     [msg_exp]to convert the presented type to or from the transmittable type.
!     [msg_exp]Only the transmittable type is relevant to the RPC mechanism,
!     [msg_exp]thus it is incorrect for the presented type
!     [msg_exp]to have additional RPC-specific attributes.
!     [msg_act]Remove any type attributes other than <kw>([transmit_as]).
!     [end_msg]


.SEVERITY   FATAL
    BUGNOBUG        <Command option -bug !UL conflicts with -no_bug !UL> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]BUGNOBUG
!     [msg_text]Command option <kw>(-bug) <v>(number) conflicts with <kw>(-no_bug) <v>(number)
!     {severity_level}FATAL
!     [msg_exp]The command line options conflict with each other.
!     [msg_act]Remove one of the conflicting options.
!     [end_msg]

.SEVERITY   FATAL
    CONFLICTOPT     <Command option !AZ conflicts with !AZ> /FAO_COUNT=2

!     {engineer_name}Ed
!     {audience}programmer
!     [begin_msg]
!     [ident]CONFLICTOPT
!     [msg_text]Command option <v>(name) conflicts with <v>(name)
!     {severity_level}FATAL
!     [msg_exp]The command line options conflict with each other.
!     [msg_act]Remove one of the conflicting options.
!     [end_msg]

    COMMENTEOF      "Unterminated comment, end-of-file encountered"/FAO_COUNT = 0

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]COMMENTEOF
!     [msg_text]Unterminated comment, end-of-file encountered
!     {severity_level}FATAL
!     [msg_exp]During the processing of a multiline comment,
!     [msg_exp]the end of the source
!     [msg_exp]file was reached before the termination characters for
!     [msg_exp]the comment.
!     [msg_act]Make sure that all multiline comments are correctly terminated.
!     [end_msg]

    COMPABORT       <Compilation aborted>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]COMPABORT
!     [msg_text]Compilation aborted
!     {severity_level}FATAL
!     [msg_exp]Previous errors caused the compilation to abort.
!     [msg_act]Correct the errors and recompile.
!     [end_msg]

    INTERNAL_ERROR  <Internal IDL compiler error: Module !AZ, Line !UL> /FAO_COUNT=2

!     {engineer_name}Beth
!     {audience}programmer
!     [begin_msg]
!     [ident]INTERNAL_ERROR
!     [msg_text]Internal IDL compiler error
!     {severity_level}FATAL
!     [msg_exp]An internal IDL compiler error was found.
!     [msg_act]Submit a Software Performance Report and notify
!     [msg_act]your software support representative so the appropriate
!     [msg_act]action can be taken.
!     [end_msg]

    INVBUG          <Command option -bug !UL is not known> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INVBUG
!     [msg_text]Command option <kw>(-bug) <v>(number) is not known
!     {severity_level}FATAL
!     [msg_exp]The documentation defines all the valid integers that can
!     [msg_exp]accompany the <kw>(-bug) and <kw>(-no_bug) options.  You have chosen
!     [msg_exp]an invalid value.
!     [msg_act]See the documentation for the correct number.
!     [end_msg]

    INVNOBUG        <Command option -no_bug !UL is not known> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INVNOBUG
!     [msg_text]Command option <kw>(-no_bug) <v>(number) is not known
!     {severity_level}FATAL
!     [msg_exp]The documentation defines all the valid integers that can
!     [msg_exp]accompany the <kw>(-bug) and <kw>(-no_bug) options.  You have chosen
!     [msg_exp]an invalid value.
!     [msg_act]See the documentation for the correct number.
!     [end_msg]

    MAXWARN         <Reached maximum of !UL warnings; exiting> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXWARN
!     [msg_text]Reached maximum of <v>(number) warnings; exiting
!     {severity_level}FATAL
!     [msg_exp]The IDL compiler has a predefined maximum number of warnings
!     [msg_exp]it produces before aborting the compilation.  This feature
!     [msg_exp]suppresses what may be the same warning
!     [msg_exp]over and over.
!     [msg_act]Fix the warnings you get, or recompile the IDL file or
!     [msg_act]specify the <kw>(-no_warn) option.
!     [end_msg]

    OPTNOVAL        <Command option !AZ does not take a value> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OPTNOVAL
!     [msg_text]Command option <v>(option) does not take a value
!     {severity_level}FATAL
!     [msg_exp]You specified a value with a command line option that
!     [msg_exp]does not take a value.
!     [msg_act]If the value is meant to be the source file, it must
!     [msg_act]be separated from the command option with a space.
!     [end_msg]

    OUTOFMEM        <Out of memory>

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]OUTOFMEM
!     [msg_text]Out of memory
!     {severity_level}FATAL
!     [msg_exp]The system-defined limit of memory available to the compiler was exceeded.
!     [msg_act]Either have the system-defined limit of memory raised, or
!     [msg_act]simplify the source file being compiled by breaking it into multiple modules or
!     [msg_act]eliminating unnecessary imports.
!     [end_msg]

    SRCFILELEN      <Length of source filename and stub suffix exceed 8 chars>

!     {engineer_name}Rico
!     {audience}programmer
!     {begin_msg]
!     {ident]SRCFILELEN
!     {msg_text]Length of source filename and stub suffix exceed 8 chars
!     {severity_level}FATAL
!     {msg_exp]This is a MS-DOS only message.  Not our job (yet, anyway).
!     {msg_exp] { in place of [ cause MSG.COM to ignore.
!     {msg_act]
!     {msg_act]
!     {end_msg]

    SRCFILEREQ      <A source IDL filename is required>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]SRCFILEREQ
!     [msg_text]A source IDL filename is required
!     {severity_level}FATAL
!     [msg_exp]The command line does not contain a source IDL file to compile.
!     [msg_act]You must specify a source IDL file.  You may have erroneously
!     [msg_act]specified the source IDL file when
!     [msg_act]actually you gave a value to a command option.
!     [end_msg]

    UNKFLAG         <Unknown command line option: !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]UNKFLAG
!     [msg_text]Unknown command line option: <v>(option)
!     {severity_level}FATAL
!     [msg_exp]You entered an invalid command line option.
!     [msg_act]Check the command line for typographical errors.
!     [msg_act]See the documentation for proper command line syntax.
!     [end_msg]


    .SEVERITY   WARNING
    NOPORTNEU <Use of a nonencapsulated union requires !AZ>/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]NOPORTNEU
!     [msg_text]Use of a nonencapsulated union requires <v>(option)
!     {severity_level}WARNING
!     [msg_exp]A nonencapsulated union is not portable to all implementations of DCE V1.0.
!     [msg_act]Use an encapsulated union instead or specify the extended standard
!     [msg_act]option on the compiler command.
!     [end_msg]

    .SEVERITY   WARNING
    NOPORTUNIQUE <Use of [unique] pointers requires !AZ>/FAO_COUNT=1

!     {engineer_name}Jerry
!     {audience}programmer
!     [begin_msg]
!     [ident]NOPORTUNIQUE
!     [msg_text]Use of <kw>([unique]) pointers requires <v>(option)
!     {severity_level}WARNING
!     [msg_exp]A <kw>([unique]) pointer is not portable to all implementations of DCE V1.0.
!     [msg_act]Use a <kw>([ref]) or <kw>([ptr]) pointer instead or specify the extended standard
!     [msg_act]option on the compiler command.
!     [end_msg]

    .SEVERITY   ERROR
    DISCRIMIN       <An [in] or [in,out] union must have an [in] discriminator>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]DISCRIMIN
!     [msg_text]An <kw>([in]) or <kw>([in,out]) union must have an <kw>([in]) discriminator
!     {severity_level}ERROR
!     [msg_exp]A nonencapsulated union parameter that has the <kw>([in])
!     [msg_exp]attribute must have a discriminator, specified in a
!     [msg_exp]<kw>([switch_is]) clause, that also has the <kw>([in])
!     [msg_exp]attribute, since the client stub code must know which member
!     [msg_exp]of the union to send to the server.
!     [msg_act]Modify the parameter referenced in the <kw>([switch_is]) clause
!     [msg_act]to have the <kw>([in]) attribute.
!     [end_msg_DEC]

    DISCRIMOUT      <An [in,out] or [out] union must have an [out] discriminator>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]DISCRIMOUT
!     [msg_text]An <kw>([in,out]) or <kw>([out]) union must have an <kw>([out]) discriminator
!     {severity_level}ERROR
!     [msg_exp]A nonencapsulated union parameter that has the <kw>([out])
!     [msg_exp]attribute must have a discriminator, specified in a
!     [msg_exp]<kw>([switch_is]) clause, that also has the <kw>([out])
!     [msg_exp]attribute, since the server stub code must know which member
!     [msg_exp]of the union to send to the client.
!     [msg_act]Modify the parameter referenced in the <kw>([switch_is]) clause
!     [msg_act]to have the <kw>([out]) attribute.
!     [end_msg_DEC]

    DISCRIMREPAS    <A [switch_is] variable must not have a [represent_as] type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]DISCRIMREPAS
!     [msg_text]A <kw>([switch_is]) variable must not have a <kw>([represent_as]) type
!     {severity_level}ERROR
!     [msg_exp]The parameter or field referenced in a <kw>([switch_is]) clause
!     [msg_exp]must not be of a data type that has been declared to have a
!     [msg_exp]<kw>([represent_as]) type.
!     [msg_act]Change the referenced parameter or field data type,
!     [msg_act]or the data type definition itself.
!     [end_msg_DEC]

    DISCRIMXMITAS   <A [switch_is] variable must not have a [transmit_as] type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]DISCRIMXMITAS
!     [msg_text]A <kw>([switch_is]) variable must not have a <kw>([transmit_as]) type
!     {severity_level}ERROR
!     [msg_exp]The parameter or field referenced in a <kw>([switch_is]) clause
!     [msg_exp]must not be of a data type that has been declared to have a
!     [msg_exp]<kw>([transmit_as]) type.
!     [msg_act]Change the referenced parameter or field data type,
!     [msg_act]or the data type definition itself.
!     [end_msg_DEC]

    .SEVERITY   ERROR
    EUMEMATTR       <An encapsulated union member cannot have the [!AZ] attribute> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]EUMEMATTR
!     [msg_text]An encapsulated union member cannot have the <v>([attribute]) attribute
!     {severity_level}ERROR
!     [msg_exp]The attribute used is valid only on nonencapsulated unions,
!     [msg_exp]but was used on an encapsulated union.
!     [msg_act]Remove the attribute.  Consult the documentation for the
!     [msg_act]differences between encapsulated and nonencapsulated unions.
!     [end_msg_DEC]

    .SEVERITY   ERROR
    NEUARRAY        <Arrays of nonencapsulated unions are not allowed>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]NEUARRAY
!     [msg_text]Arrays of nonencapsulated unions are not allowed
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow arrays of nonencapsulated unions since there
!     [msg_exp]is no way to apply a different discriminator to each element.
!     [msg_act]Use an array of encapsulated unions.
!     [end_msg_DEC]

    NEUREPAS        <A [represent_as] type cannot be a nonencapsulated union>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]NEUREPAS
!     [msg_text]A <kw>([represent_as]) type cannot be a nonencapsulated union
!     {severity_level}ERROR
!     [msg_exp]A type referenced in a <kw>([represent_as]) clause cannot be a
!     [msg_exp]nonencapsulated union, since there is no defined way to
!     [msg_exp]communicate the union discriminator to/from <kw>([represent_as])
!     [msg_exp]data translation routines.
!     [msg_act]Use an encapsulated union for the local representation type.
!     [end_msg_DEC]

    NEUREPTYPE      <A nonencapsulated union cannot have a [represent_as] type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]NEUREPTYPE
!     [msg_text]A nonencapsulated union cannot have a <kw>([represent_as]) type
!     {severity_level}ERROR
!     [msg_exp]A nonencapsulated union type cannot be defined to have a
!     [msg_exp]local representation type, since there is no defined way to
!     [msg_exp]communicate the union discriminator to/from <kw>([represent_as])
!     [msg_exp]data translation routines.
!     [msg_act]Use an encapsulated union, or reconsider the need for a
!     [msg_act]different local representation type for the union.
!     [end_msg_DEC]

    NEUSWATTR       <A nonencapsulated union declaration must have a [switch_is] attribute>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]NEUSWATTR
!     [msg_text]A nonencapsulated union declaration must have a <kw>([switch_is]) attribute
!     {severity_level}ERROR
!     [msg_exp]A <kw>([switch_is]) attribute is required on an instance of a
!     [msg_exp]nonencapsulated union type to identify the union discriminator
!     [msg_exp]variable.
!     [msg_act]Provide a <kw>([switch_is]) clause that specifies a field or
!     [msg_act]parameter that is the union discriminator, or use an
!     [msg_act]encapsulated union, where the union discriminator variable is
!     [msg_act]encapsulated as a field within the union type.
!     [end_msg_DEC]

    NEUSWPTR    <The union switch variable !AZ cannot be a [ptr] or [unique] pointer> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]NEUSWPTR
!     [msg_text]The union switch variable <v>(name) cannot be a <kw>([ptr]) or <kw>([unique]) pointer
!     {severity_level}ERROR
!     [msg_exp]A <kw>([switch_is]) attribute clause in the source IDL specifies
!     [msg_exp]its value by dereferencing a pointer field or parameter.
!     [msg_exp]This is invalid if the pointer variable has either of the
!     [msg_exp]<kw>([ptr]) or <kw>([unique]) attributes, since both allow the
!     [msg_exp]pointer to be <kw>(NULL).  If the pointer were <kw>(NULL), there
!     [msg_exp]would be no way to determine the union discriminator.
!     [msg_act]Either redeclare the <kw>([switch_is]) variable to be a scalar
!     [msg_act]rather than a pointer, or change the <kw>([switch_is]) variable
!     [msg_act]to be a <kw>([ref]) pointer.
!     [end_msg_DEC]

    NEUSWTYPE       <A nonencapsulated union type must have a [switch_type] attribute>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]NEUSWTYPE
!     [msg_text]A nonencapsulated union type must have a <kw>([switch_type]) attribute
!     {severity_level}ERROR
!     [msg_exp]A <kw>([switch_type]) attribute is required on a definition of a
!     [msg_exp]nonencapsulated union type to identify the data type of the
!     [msg_exp]union discriminator.
!     [msg_act]Provide a <kw>([switch_type]) clause that specifies a valid
!     [msg_act]union discriminator data type.
!     [end_msg_DEC]

    NEUXMITAS       <A [transmit_as] type cannot be a nonencapsulated union>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]NEUXMITAS
!     [msg_text]A <kw>([transmit_as]) type cannot be a nonencapsulated union
!     {severity_level}ERROR
!     [msg_exp]A type referenced in a <kw>([transmit_as]) clause cannot be a
!     [msg_exp]nonencapsulated union, since there is no defined way to
!     [msg_exp]communicate the union discriminator to/from <kw>([transmit_as])
!     [msg_exp]data translation routines.
!     [msg_act]Use an encapsulated union for the transmissible type.
!     [end_msg_DEC]

    NEUXMITYPE      <A nonencapsulated union cannot have a [transmit_as] type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]NEUXMITYPE
!     [msg_text]A nonencapsulated union cannot have a <kw>([transmit_as]) type
!     {severity_level}ERROR
!     [msg_exp]A nonencapsulated union type cannot be defined to have a
!     [msg_exp]transmissible type, since there is no defined way to
!     [msg_exp]communicate the union discriminator to/from <kw>([transmit_as])
!     [msg_exp]data translation routines.
!     [msg_act]Use an encapsulated union, or reconsider the need for a
!     [msg_act]different transmissible type for the union.
!     [end_msg_DEC]

    .SEVERITY   ERROR
    PTRNEUNION      <Cannot have more than one level of indirection to a nonencapsulated union>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]PTRNEUNION
!     [msg_text]Cannot have more than one level of indirection to a nonencapsulated union
!     {severity_level}ERROR
!     [msg_exp]IDL allows only a single level of pointer to a nonencapsulated
!     [msg_exp]union type.
!     [msg_act]Remove the extra levels of indirection, or use an encapsulated
!     [msg_act]union type instead.
!     [end_msg_DEC]

    .SEVERITY   ERROR
    SWATTRNEU       <A [switch_is] attribute is only valid on a nonencapsulated union>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]SWATTRNEU
!     [msg_text]A <kw>([switch_is]) attribute is only valid on a nonencapsulated union
!     {severity_level}ERROR
!     [msg_exp]A <kw>([switch_is]) attribute is used on a union type to
!     [msg_exp]identify the union discriminator variable.  It is not valid
!     [msg_exp]on a non-union type.  It is also not valid on an encapsulated
!     [msg_exp]union type, where the union discriminator variable is
!     [msg_exp]encapsulated as a field within the union type.
!     [msg_act]Use the <kw>([switch_is]) attribute only on instances of
!     [msg_act]nonencapsulated union types.
!     [end_msg_DEC]

    SWDATATYPE      <Data type of [switch_is] variable !AZ does not agree with [switch_type] !AZ> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]SWDATATYPE
!     [msg_text]Data type of <kw>([switch_is]) variable <v>(name) does not agree with <kw>([switch_type]) <v>(type)
!     {severity_level}ERROR
!     [msg_exp]The data type of the variable referenced in the <kw>([switch_is])
!     [msg_exp]clause must be the same as the data type referenced in the
!     [msg_exp]<kw>([switch_type]) clause of the item's type definition.
!     [msg_act]Make sure that all instances of a type that has the
!     [msg_act]<kw>([switch_type]) attribute have a <kw>([switch_is]) clause
!     [msg_act]whose data type agrees with the <kw>([switch_type]).
!     [end_msg_DEC]

    SWTYPENEU       <A [switch_type] attribute is only valid on a nonencapsulated union type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]SWTYPENEU
!     [msg_text]A <kw>([switch_type]) attribute is only valid on a nonencapsulated union type
!     {severity_level}ERROR
!     [msg_exp]A <kw>([switch_type]) attribute is used on a union type to
!     [msg_exp]identify the union discriminator type.  It is not valid
!     [msg_exp]on a non-union type.  It is also not valid on an encapsulated
!     [msg_exp]union type, where the union discriminator type and variable
!     [msg_exp]must be specified in a <kw>(switch) clause.
!     [msg_act]Use the <kw>([switch_type]) attribute only on definitions of
!     [msg_act]nonencapsulated union types.
!     [end_msg_DEC]

    .SEVERITY ERROR
    FLDXMITCFMT     <A structure field cannot be of a presented type for which the transmitted type is conformant>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]FLDXMITCFMT
!     [msg_text]A structure field cannot be of a presented type for which the transmitted type is conformant
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow a structure field whose type has a
!     [msg_exp]<kw>([transmit_as]) attribute where the transmissible type
!     [msg_exp]is conformant.
!     [msg_act]Use a valid IDL construct.
!     [end_msg]

    .SEVERITY ERROR
    TYPEREPCFMT     <A type with the [represent_as] attribute cannot be conformant>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]TYPEREPCFMT
!     [msg_text]A type with the <kw>([represent_as]) attribute cannot be conformant
!     {severity_level}ERROR
!     [msg_exp]IDL does not allow a conformant network type to have a local
!     [msg_exp]represented type.
!     [msg_act]Use a valid IDL construct.
!     [end_msg]

    ARMUNIQUEPTR       <An arm of a union cannot be or contain a [unique] pointer>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]ARMUNIQUEPTR
!     [msg_text]An arm of a <kw>(union) cannot be or contain a <kw>([unique]) pointer
!     {severity_level}ERROR
!     [msg_exp]IDL-generated server stub code must allocate storage for
!     [msg_exp]objects pointed to by <kw>([unique]) pointers.  IDL does not allow
!     [msg_exp]a <kw>([unique]) pointer within a <kw>(union) because the valid
!     [msg_exp]arm of the <kw>(union), and therefore the
!     [msg_exp]object for which storage is allocated, is not
!     [msg_exp]known at compile time.
!     [msg_act]Change the pointer within the <kw>(union) declaration to a full pointer.
!     [end_msg_DEC]

    .SEVERITY WARNING
    LANGCFMTSTRCT   <Conformant structures not supported in target language>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]LANGCFMTSTRCT
!     [msg_text]Conformant structures not supported in target language
!     {severity_level}WARNING
!     [msg_exp]An IDL conformant structure does not have a source
!     [msg_exp]representation in the target language selected.
!     [msg_act]The IDL conformant array will be declared as a one-element array
!     [msg_act]in the generated language header file.  Your application code
!     [msg_act]might be able to use the address of this array to access other
!     [msg_act]array elements, depending on the target language.
!     [msg_act]Alternatives are to write application code that uses the
!     [msg_act]construct in another programming language, or to change the IDL
!     [msg_act]source to use a construct that is supported by the target
!     [msg_act]language.
!     [end_msg_DEC]

    .SEVERITY WARNING
    LANGCFMTSTRNG   <Conformant strings not supported in target language>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]LANGCFMTSTRNG
!     [msg_text]Conformant strings not supported in target language
!     {severity_level}WARNING
!     [msg_exp]An IDL conformant string does not have a source
!     [msg_exp]representation in the target language selected.
!     [msg_act]The IDL conformant array will be declared as a one-element array
!     [msg_act]in the generated language header file.  Your application code
!     [msg_act]might be able to use the address of this array to access other
!     [msg_act]array elements, depending on the target language.
!     [msg_act]Alternatives are to write application code that uses the
!     [msg_act]construct in another programming language, or to change the IDL
!     [msg_act]source to use a construct that is supported by the target
!     [msg_act]language.
!     [end_msg_DEC]

    .SEVERITY WARNING
    LANGMAPCHAR     <Character doesn't map to target language>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]LANGMAPCHAR
!     [msg_text]Character doesn't map to target language
!     {severity_level}WARNING
!     [msg_exp]A character in an IDL character or string constant does not
!     [msg_exp]have a source representation in the target language selected.
!     [msg_act]The character will be translated to a '?' in the generated
!     [msg_act]language header file.  Edit the file manually as necessary.
!     [end_msg_DEC]

    .SEVERITY WARNING
    LANGPIPESNS     <Pipes not supported for target language>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]LANGPIPESNS
!     [msg_text]Pipes not supported for target language
!     {severity_level}WARNING
!     [msg_exp]The data structures necessary to manage IDL pipes do not
!     [msg_exp]have a source representation in the target language selected.
!     [msg_act]Pipe objects will have no corresponding definition in the
!     [msg_act]generated language header file.  Application code for operations
!     [msg_act]with pipes should be written in another programming language.
!     [end_msg_DEC]

    .SEVERITY WARNING
    LANGUNSFNRS     <Non-scalar function result converted to trailing [out] parameter>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]LANGUNSFNRS
!     [msg_text]Non-scalar function result converted to trailing <kw>([out]) parameter
!     {severity_level}WARNING
!     [msg_exp]The target language selected does not allow the function result
!     [msg_exp]argument of a routine to be a non-scalar type.  The IDL compiler
!     [msg_exp]converts such a construct to a trailing <kw>([out]) parameter.
!     [msg_act]No action is required.  You might want to edit your IDL source
!     [msg_act]and change the declaration from a function result to an <kw>([out])
!     [msg_act]parameter in order to map naturally to the target language.
!     [end_msg_DEC]

    .SEVERITY ERROR
    LANGMAXDIM      <Target language supports maximum of !UL array dimensions> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]LANGMAXDIM
!     [msg_text]Target language supports maximum of <v>(number) array dimensions
!     {severity_level}ERROR
!     [msg_exp]The target language selected does not support arrays with more
!     [msg_exp]than the indicated number of dimensions.
!     [msg_act]Such arrays will have no corresponding declaration in the
!     [msg_act]generated language header file.  Application code using such
!     [msg_act]arrays should be written in another programming language.
!     [end_msg_DEC]

    .SEVERITY WARNING
    LANGMIXCASE     <Name !AZ differs from a previous name only in case> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]LANGMIXCASE
!     [msg_text]Name <v>(name) differs from a previous name only in case
!     {severity_level}WARNING
!     [msg_exp]The target language selected is not case-sensitive with respect
!     [msg_exp]to identifiers; that is, any two identifiers that differ only in
!     [msg_exp]case are considered to be identical.  The IDL source contains
!     [msg_exp]two distinct identifiers that differ only in case, but they
!     [msg_exp]conflict with eachother in the generated language header file.
!     [msg_act]Edit the IDL source code to remove the conflict.
!     [end_msg_DEC]

    .SEVERITY WARNING
    LANGATTRNS      <Attribute [!AZ] not supported for target language> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg_DEC]
!     [ident]LANGATTRNS
!     [msg_text]Attribute <v>([attribute]) not supported for target language
!     {severity_level}WARNING
!     [msg_exp]IDL does not support the indicated attribute for the
!     [msg_exp]target language selected.
!     [msg_act]In some cases the attribute can be removed.  For example,
!     [msg_act]the <kw>(v1_) attributes are only necessary if interoperation
!     [msg_act]with NCS Version 1 is required.
!     [end_msg_DEC]

    .SEVERITY ERROR
    ARRFUNRES       <Array function results are not allowed>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRFUNRES
!     [msg_text]Array function results are not allowed
!     {severity_level}ERROR
!     [msg_exp]Because array function results are not supported in the C
!     [msg_exp]languages, they are not allowed in IDL.
!     [msg_act]Return the required array as an <v>([out]) parameter.
!     [end_msg]

    .SEVERITY ERROR
    ATTRPTRVOID     <Attribute [!AZ] cannot be applied to a void * type> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ATTRPTRVOID
!     [msg_text]Attribute <v>([attribute]) cannot be applied to a <kw>(void *) type
!     {severity_level}ERROR
!     [msg_exp]The indicated attribute is not valid on a <kw>(void *) type.
!     [msg_exp]The only attribute that is meaningful on a <kw>(void *) type
!     [msg_exp]is <v>([context_handle]).
!     [msg_act]Edit the IDL source code to remove the conflict.
!     [end_msg]

    .SEVERITY ERROR
    HANATTRTRAN     <The [handle] attribute is valid only on transmittable types>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]HANATTRTRAN
!     [msg_text]The <v>([handle]) attribute is valid only on transmittable types
!     {severity_level}ERROR
!     [msg_exp]The <v>([handle]) attribute was used on a type that is not
!     [msg_exp]transmittable, such as <v>(handle_t).  Types with <v>([handle])
!     [msg_exp]are sent from client to server and must have a concrete
!     [msg_exp]definition so that they are transmittable.
!     [msg_act]Remove the <v>([handle]) attribute or change the base type to
!     [msg_act]which it is applied.
!     [end_msg]

    .SEVERITY WARNING
    FWDTAGREF       <Forward tag reference !AZ in this declaration is not ANSI C compliant> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]FWDTAGREF
!     [msg_text]Forward tag reference <v>(name) in this declaration is not ANSI C compliant
!     {severity_level}WARNING
!     [msg_exp]A tag reference, such as <kw>(struct tagname), was used before
!     [msg_exp]the actual definition of the tag.  Such references are not
!     [msg_exp]ANSI C compliant when used in certain scopes, such as in a
!     [msg_exp]parameter declaration.  Thus the C source modules generated by
!     [msg_exp]the IDL compiler might not compile successfuly.
!     [msg_act]Define the type that is referenced by tag name before
!     [msg_act]referencing it in a parameter declaration.
!     [end_msg]

    .SEVERITY ERROR
    ATTRUSEMULT     <Attribute cannot be used more than once>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ATTRUSEMULT
!     [msg_text]Attribute cannot be used more than once
!     {severity_level}ERROR
!     [msg_exp]An attribute is repeated multiple times in an attribute list.
!     [msg_act]Remove all but one occurrence of the offending attribute.
!     [end_msg]

    .SEVERITY ERROR
    TYPEATTRUSE     <Type with [!AZ] cannot be used in definition of type with [!AZ]> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]TYPEATTRUSE
!     [msg_text]Type with <v>([attribute1]) cannot be used in definition of type with <v>([attribute2])
!     {severity_level}ERROR
!     [msg_exp]IDL restricts the use of the two attributes such that the base
!     [msg_exp]type of a type that has the first attribute must not contain
!     [msg_exp]the second attribute in its definition.
!     [msg_act]Refer to the IDL documentation on the two attributes for more
!     [msg_act]information.
!     [end_msg]

    .SEVERITY   WARNING
    NOPORTNZLB      <Use of nonzero array lower bound requires !AZ>/FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]NOPORTNZLB
!     [msg_text]Use of nonzero array lower bound requires <v>(option)
!     {severity_level}WARNING
!     [msg_exp]A nonzero array lower bound is not portable to all
!     [msg_exp]implementations of DCE V1.0.
!     [msg_act]Use an array with lower bound zero instead or specify the
!     [msg_act]extended standard option on the compiler command.
!     [end_msg]


    .SEVERITY   WARNING
    NOPORTCFMT      <Use of conformant minor array dimension requires !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]NOPORTCFMT
!     [msg_text]Use of conformant minor array dimension requires <v>(option)
!     {severity_level}WARNING
!     [msg_exp]An array that is conformant in a minor dimension, i.e. other than
!     [msg_exp]the first dimension, is not portable to all
!     [msg_exp]implementations of DCE V1.0.
!     [msg_act]Use an array that is conformant in only the first dimension,
!     [msg_act]or specify the extended standard option on the compiler command.
!     [end_msg]

    .SEVERITY   WARNING
    NOPORTVARY      <Use of varying minor array dimension requires !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]NOPORTVARY
!     [msg_text]Use of varying minor array dimension requires <v>(option)
!     {severity_level}WARNING
!     [msg_exp]An array that is varying in a minor dimension, i.e. other than
!     [msg_exp]the first dimension, is not portable to all
!     [msg_exp]implementations of DCE V1.0.
!     [msg_act]Use an array that is varying in only the first dimension,
!     [msg_act]or specify the extended standard option on the compiler command.
!     [end_msg]


    .SEVERITY ERROR
    ENCNOPARAMS     <Operation with [encode] must have at least one [in] or [in,out] parameter>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ENCNOPARAMS
!     [msg_text]Operation with <kw>([encode]) must have at least one <kw>([in]) or <kw>([in,out]) parameter
!     {severity_level}ERROR
!     [msg_exp]The client stub for an operation with the <kw>([encode]) ACF
!     [msg_exp]attribute encodes the operation's <kw>([in]) and <kw>([in,out])
!     [msg_exp]parameters into a data stream.  The <kw>([encode]) attribute
!     [msg_exp]is not meaningful on an operation with only <kw>([out])
!     [msg_exp]parameters (excluding the binding handle parameter).
!     [msg_act]Perhaps you meant to use the <kw>([decode]) attribute on the
!     [msg_act]operation.  If not, either remove the <kw>([encode]) attribute
!     [msg_act]or add the <kw>([in]) attribute to one or more parameters.
!     [end_msg]

    .SEVERITY ERROR
    DECNOPARAMS     <Operation with [decode] must have at least one [out] or [in,out] parameter>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]DECNOPARAMS
!     [msg_text]Operation with <kw>([decode]) must have at least one <kw>([out]) or <kw>([in,out]) parameter
!     {severity_level}ERROR
!     [msg_exp]The client stub for an operation with the <kw>([decode]) ACF
!     [msg_exp]attribute decodes a data stream into the operation's <kw>([out])
!     [msg_exp]and <kw>([in,out]) parameters.  The <kw>([decode]) attribute
!     [msg_exp]is not meaningful on an operation with only <kw>([in])
!     [msg_exp]parameters.
!     [msg_act]Perhaps you meant to use the <kw>([encode]) attribute on the
!     [msg_act]operation.  If not, either remove the <kw>([decode]) attribute
!     [msg_act]or add the <kw>([out]) attribute to one or more parameters.
!     [end_msg]

    .SEVERITY WARNING
    OPREFDELIN      <Operation with [reflect_deletions] has no [in] or [in,out] full pointers>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OPREFDELIN
!     [msg_text]Operation with <kw>([reflect_deletions]) has no <kw>([in]) or <kw>([in,out]) full pointers
!     {severity_level}WARNING
!     [msg_exp]Reflection of node deletions from server to client is only
!     [msg_exp]applicable to operations that have <kw>([in]) or <kw>([in,out])
!     [msg_exp]full (<kw>([ptr])) pointers.  The <kw>([reflect_deletions])
!     [msg_exp]attribute thus has no effect on the indicated operation.
!     [msg_act]Remove the <kw>([reflect_deletions]) attribute from the
!     [msg_act]operation to avoid this warning message.
!     [end_msg]

    .SEVERITY   WARNING
    ENCOUTONLY      <Operation with [encode] has [out]-only parameter !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ENCOUTONLY
!     [msg_text]Operation with <kw>([encode]) has <kw>([out])-only parameter <v>(name)
!     {severity_level}WARNING
!     [msg_exp]The client stub for an operation with the <kw>([encode])
!     [msg_exp]attribute encodes all of the operation parameters with the
!     [msg_exp]<kw>([in]) attribute.  Parameters with only the <kw>([out])
!     [msg_exp]attribute are ignored.
!     [msg_act]If any client will decode the encoded information by
!     [msg_act]applying the <kw>([decode]) attribute to the operation, the
!     [msg_act]operation must contain all <kw>([in,out]) parameters except for
!     [msg_act]the binding handle parameter.
!     [end_msg]

    .SEVERITY WARNING
    DECINONLY       <Operation with [decode] has [in]-only parameter !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]DECINONLY
!     [msg_text]Operation with <kw>([decode]) has <kw>([in])-only parameter <v>(name)
!     {severity_level}WARNING
!     [msg_exp]The client stub for an operation with the <kw>([decode])
!     [msg_exp]attribute decodes encoded data into operation parameters with the
!     [msg_exp]<kw>([out]) attribute.  Parameters with only the <kw>([in])
!     [msg_exp]attribute are ignored.
!     [msg_act]If any client will encode the data by
!     [msg_act]applying the <kw>([encode]) attribute to the operation, the
!     [msg_act]operation must contain all <kw>([in,out]) parameters except for
!     [msg_act]the binding handle parameter.
!     [end_msg]

    .SEVERITY ERROR
    ENCDECDIR       <Operation with [encode,decode] must have all [in,out] parameters>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ENCDECDIR
!     [msg_text]Operation with <kw>([encode,decode]) must have all <kw>([in,out]) parameters
!     {severity_level}ERROR
!     [msg_exp]The client stub for an operation with the <kw>([encode,decode])
!     [msg_exp]attributes can encode the operation's parameters into a data
!     [msg_exp]stream and subsequently decode the data back into parameters.
!     [msg_exp]This only makes sense if the decoded data, i.e. the <kw>([out]
!     [msg_exp]parameters, agrees with the originally encoded <kw>([in])
!     [msg_exp]parameters.
!     [msg_act]The operation must contain all <kw>([in,out]) parameters except
!     [msg_act]for the binding handle parameter.
!     [end_msg]

    .SEVERITY ERROR
    ENCDECPIPE      <Operation with [encode] or [decode] may not contain pipe parameters>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ENCDECPIPE
!     [msg_text]Operation with <kw>([encode]) or <kw>([decode]) may not contain pipe parameters
!     {severity_level}ERROR
!     [msg_exp]The IDL encoding services do not support pipes.
!     [msg_act]Use arrays to represent the data, or utilize multiple
!     [msg_act]procedure calls to encode or decode the data.
!     [end_msg]

    .SEVERITY ERROR
    ENCDECBIND      <Operation with [encode] or [decode] must use explicit binding>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ENCDECBIND
!     [msg_text]Operation with <kw>([encode]) or <kw>([decode]) must use explicit binding
!     {severity_level}ERROR
!     [msg_exp]An operation with either of the <kw>([encode]) or <kw>([decode])
!     [msg_exp]attributes may not use automatic, customized, or implicit
!     [msg_exp]binding.
!     [msg_act]Edit the IDL operation definition to have an item of type
!     [msg_act]<v>(handle_t) as its first argument, or edit the ACF operation
!     [msg_act]declaration to include the <kw>([explicit_handle]) attribute.
!     [end_msg]

    .SEVERITY ERROR
    EXCNOTDEF       <Exception name !AZ referenced in ACF is not defined> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]EXCNOTDEF
!     [msg_text]Exception name <v>(name) referenced in ACF is not defined
!     {severity_level}ERROR
!     [msg_exp]An exception name is referenced in an Attribute Configuration
!     [msg_exp]File (ACF) but is not defined in the corresponding IDL file.
!     [msg_exp]Exceptions are not imported, so in order to reference an
!     [msg_exp]exception name in the ACF for a particular interface it must
!     [msg_exp]be defined in the IDL file for that same interface.
!     [msg_act]Check for typographical errors in both the IDL and ACF files.
!     [msg_act]If necessary, add the exception name to the source IDL file
!     [msg_act]using the [exceptions] interface attribute, or move the ACF
!     [msg_act]reference to the ACF file corresponding to the interface in which
!     [msg_act]the exception name is defined.
!     [end_msg]

    .SEVERITY FATAL
    INVFILESPEC     <Invalid file specification: !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]INVFILESPEC
!     [msg_text]Invalid file specification: <v>(filename)
!     {severity_level}FATAL
!     [msg_exp]An invalid file specification was given.
!     [msg_act]Check the files names specified on the command line for
!     [msg_act]proper syntax.
!     [end_msg]

    .SEVERITY FATAL
    FILESPECREQ     <File specification required for !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]FILESPECREQ
!     [msg_text]File specification required for <v>(option)
!     {severity_level}FATAL
!     [msg_exp]The indicated IDL command option requires a file specification.
!     [msg_act]Supply a file specification following the option.
!     [end_msg]

    .SEVERITY WARNING
    OPNOTAGS    <Operation with [cs_tag_rtn] contains no codeset tag parameters>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OPNOTAGS
!     [msg_text]Operation with <kw>([cs_tag_rtn]) contains no codeset tag parameters
!     {severity_level}WARNING
!     [msg_exp]The <kw>([cs_tag_rtn]) attribute normally appears on an
!     [msg_exp]operation only if it has least one parameter with a
!     [msg_exp]<kw>([cs_stag]), <kw>([cs_drtag]), or <kw>([cs_rtag]) attribute.
!     [msg_act]Make sure the ACF declaration for the operation has one or more
!     [msg_act]of the above tags applied to parameter(s) in the operation.
!     [end_msg]

    .SEVERITY ERROR
    TYPENEST    <Types with the [!AZ] attribute cannot be nested> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]TYPENEST
!     [msg_text]Types with the <v>([name]) attribute cannot be nested
!     {severity_level}ERROR
!     [msg_exp]A type with the indicated attribute cannot include
!     [msg_exp]another type with the same attribute.
!     [msg_act]Do not nest types with the attribute.
!     [end_msg]

    .SEVERITY ERROR
    DUPPRMATTR  <The [!AZ] attribute cannot be duplicated in the same parameter list> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]DUPPRMATTR
!     [msg_text]The <v>([name]) attribute cannot be duplicated in the same parameter list
!     {severity_level}ERROR
!     [msg_exp]The named attribute cannot be applied twice in the same
!     [msg_exp]parameter list.
!     [msg_act]Make sure that only one parameter in the operation's parameter
!     [msg_act]list has the attribute applied to it.  The attribute is
!     [msg_act]specified either in the source IDL or source ACF file.
!     [end_msg]

    .SEVERITY ERROR
    OPINCSCHAR  <Operation with [in] [cs_char] data requires [cs_stag] parameter>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OPINCSCHAR
!     [msg_text]Operation with <kw>([in]) <kw>([cs_char]) data requires <kw>([cs_stag]) parameter
!     {severity_level}ERROR
!     [msg_exp]If any <kw>([in]) parameter, or part of an <kw>([in]) parameter,
!     [msg_exp]has a <kw>([cs_char]) type, there must be a parameter in the
!     [msg_exp]same operation to which <kw>([cs_stag]) is applied.
!     [msg_act]Make sure the ACF declaration for the operation has the
!     [msg_act]<kw>([cs_stag]) attribute applied to one of the parameters in
!     [msg_act]the operation.
!     [end_msg]

    .SEVERITY ERROR
    OPOUTCSCHAR <Operation with [out] [cs_char] data requires [cs_rtag] parameter>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]OPOUTCSCHAR
!     [msg_text]Operation with <kw>([out]) <kw>([cs_char]) data requires <kw>([cs_rtag]) parameter
!     {severity_level}ERROR
!     [msg_exp]If any <kw>([out]) parameter, or part of an <kw>([out]) parameter,
!     [msg_exp]has a <kw>([cs_char]) type, there must be a parameter in the
!     [msg_exp]same operation to which <kw>([cs_rtag]) is applied.
!     [msg_act]Make sure the ACF declaration for the operation has the
!     [msg_act]<kw>([cs_rtag]) attribute applied to one of the parameters in
!     [msg_act]the operation.
!     [end_msg]

    .SEVERITY ERROR
    ARRATTRSHR  <Array attribute variable cannot be used for both [cs_char] and non-[cs_char] arrays>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRATTRSHR
!     [msg_text]Array attribute variable cannot be used for both <kw>([cs_char]) and non-<kw>([cs_char]) arrays
!     {severity_level}ERROR
!     [msg_exp]In a structure or parameter list, any variable which specifies
!     [msg_exp]the <kw>([size_is]) or <kw>([length_is]) for a <kw>([cs_char])
!     [msg_exp]array must not be referenced in any attribute of a
!     [msg_exp]non-<kw>([cs_char]) array.
!     [msg_act]Use separate structure fields or operation parameters to
!     [msg_act]specify array attributes for the non-<kw>([cs_char]) array.
!     [end_msg]

    .SEVERITY ERROR
    ATTROUTCFMTCS   <An [in,size_is] parameter for an [out,cs_char] array cannot be used for other array attributes>

!     {engineer_name}Rico
!     {audience}obsolete_placeholder
!     {begin_msg]
!     {ident]ATTROUTCFMTCS
!     {msg_text]An <kw>([in,size_is]) parameter for an <kw>([out,cs_char]) array cannot be used for other array attributes
!     {severity_level}ERROR
!     {msg_exp]A parameter which is the <kw>([in]) <v>([size_is]) of an
!     {msg_exp]<kw>([out]) conformant array of <kw>([cs_char]) type cannot be
!     {msg_exp]referenced in any attribute of any other array.
!     {msg_act]Use separate operation parameters to specify array attributes
!     {msg_act]for other arrays in the operation.
!     {end_msg]

    .SEVERITY ERROR
    CSARRSYN    <A [size_is] or [max_is] attribute cannot be applied to a pointer to a [cs_char] type>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CSARRSYN
!     [msg_text]A <kw>([size_is]) or <kw>([max_is]) attribute cannot be applied to a pointer to a <kw>([cs_char]) type>
!     {severity_level}ERROR
!     [msg_exp]The <kw>([cs_char]) ACF attribute cannot be applied to a type
!     [msg_exp]which is the target of a pointer with the <kw>([size_is]) or
!     [msg_exp]<kw>([max_is]) attribute.
!     [msg_act]Use an array of <kw>([cs_char]) instead of pointer to array
!     [msg_act]of <kw>([cs_char]), or use a pointer to a structure which
!     [msg_act]contains a conformant array of <kw>([cs_char]).
!     [end_msg]

    .SEVERITY ERROR
    ARRPTRUNIQ  <An array with [!AZ] base type cannot have the [ptr] or [unique] attributes> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRPTRUNIQ
!     [msg_text]An array with <v>([name]) base type cannot have the <kw>([ptr]) or <kw>([unique]) attributes
!     {severity_level}ERROR
!     [msg_exp]If an array parameter has a base type with the indicated
!     [msg_exp]attribute, it cannot have <kw>([ptr]) or <kw>([unique]) as
!     [msg_exp]parameter attributes.
!     [msg_act]Remove the <kw>([ptr]) or <kw>([unique]) attribute.  This
!     [msg_exp]implies that the parameter cannot have the value NULL, i.e.
!     [msg_act]it must always point to valid array storage.
!     [end_msg]

    .SEVERITY ERROR
    XMITCTYPE   <A [transmit_as] transmitted type cannot contain a [!AZ] type> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]XMITCTYPE
!     [msg_text]A <kw>([transmit_as]) transmitted type cannot contain a <v>([name]) type
!     {severity_level}ERROR
!     [msg_exp]The indicated attribute cannot be applied to any type
!     [msg_exp]which is used as the transmitted type used in a
!     [msg_exp]<kw>([transmit_as]) attribute or any type which is
!     [msg_exp]used in the definition of the transmitted type used in a
!     [msg_exp]<kw>([transmit_as]) attribute.
!     [msg_act]Do not use the attribute on the transmitted type specified in the
!     [msg_act]<v>([transmit_as(transmitted_type)]) attribute or any other type
!     [msg_act]that is contained within that transmitted type.
!     [end_msg]

    .SEVERITY ERROR
    TAGBEFDATA  <A [cs_stag] parameter must precede any [in] [cs_char] data in a parameter list>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]TAGBEFDATA
!     [msg_text]A <kw>([cs_stag]) parameter must precede any <kw>([in]) <kw>([cs_char]) data in a parameter list
!     {severity_level}ERROR
!     [msg_exp]A <kw>([cs_stag]) parameter must occur in an operation's
!     [msg_exp]parameter list before any [in] parameters containing types
!     [msg_exp]with the [cs_char] ACF attribute.
!     [msg_act]Re-order the parameters to meet this requirement.
!     [end_msg]

    .SEVERITY ERROR
    TAGAFTDATA  <A [cs_rtag] parameter must precede any [out] [cs_char] data in a parameter list>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]TAGAFTDATA
!     [msg_text]A <kw>([cs_rtag]) parameter must precede any <kw>([out]) <kw>([cs_char]) data in a parameter list
!     {severity_level}ERROR
!     [msg_exp]A <kw>([cs_rtag]) parameter must occur in an operation's
!     [msg_exp]parameter list before any [out] parameters containing types
!     [msg_exp]with the [cs_char] ACF attribute.
!     [msg_act]Re-order the parameters to meet this requirement.
!     [end_msg]

    .SEVERITY ERROR
    HANCTYPE    <A [handle] binding parameter cannot contain a [!AZ] type> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]HANCTYPE
!     [msg_text]A <kw>([handle]) binding parameter cannot contain a <v>([name]) type
!     {severity_level}ERROR
!     [msg_exp]A customized binding handle (a type with the <kw>([handle])
!     [msg_exp]attribute used as the first parameter in an operation) cannot
!     [msg_exp]include types with the indicated attribute.
!     [msg_act]Do not use the indicated attribute with customized binding.
!     [end_msg]

    .SEVERITY ERROR
    PIPECTYPE   <The base type of a pipe cannot be or contain a [!AZ] type> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]PIPECTYPE
!     [msg_text]The base type of a pipe cannot be or contain a <v>([name]) type
!     {severity_level}ERROR
!     [msg_exp]The indicated attribute cannot be applied to the base type of a
!     [msg_exp]pipe, or a type used in constructing the base type of a pipe.
!     [msg_act]Do not use the indicated attribute with pipes.
!     [end_msg]

    .SEVERITY ERROR
    ARRMULTID   <Arrays of [!AZ] type cannot be multidimensional> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRMULTID
!     [msg_text]Arrays of <v>([name]) type cannot be multidimensional
!     {severity_level}ERROR
!     [msg_exp]The indicated attribute cannot be applied to a type if there
!     [msg_exp]is an array which has this type as base type and the array has
!     [msg_exp]more than one dimension.
!     [msg_act]Specify a different base type for the array which has more than
!     [msg_act]one dimension.
!     [end_msg]

    .SEVERITY ERROR
    ARRTYPATTR  <Arrays of [!AZ] type can only use the [size_is] and [length_is] array attributes> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]ARRTYPATTR
!     [msg_text]Arrays of <v>([name]) type can only use the <kw>([size_is]) and <kw>([length_is]) array attributes
!     {severity_level}ERROR
!     [msg_exp]The indicated attribute cannot be applied to a type if there
!     [msg_exp]is an array which has this type as base type and any of the
!     [msg_exp]attributes <kw>([min_is]), <kw>([max_is]), <kw>([first_is]),
!     [msg_exp]<kw>([last_is]), or <kw>([string]) has been applied to the array.
!     [msg_act]Use only the <kw>([size_is]) and/or <kw>([length_is])
!     [msg_act]attributes in relevant array declarations, or
!     [msg_act]specify a different base type for the array that has
!     [msg_act]the indicated attribute.
!     [end_msg]

    .SEVERITY ERROR
    TYPECTYPE   <Type with [!AZ] cannot be or contain type with [!AZ]> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]TYPECTYPE
!     [msg_text]Type with <v>([name1]) cannot be or contain type with <v>([name2])
!     {severity_level}ERROR
!     [msg_exp]A type with the first attribute cannot also have the second
!     [msg_exp]attribute, nor can it contain any type which has the second
!     [msg_exp]attribute.
!     [msg_act]The two features cannot interact on a single type.
!     [msg_act]Consider alternate ways to achieve the desired result.
!     [end_msg]

    .SEVERITY ERROR
    TYPEOFBYTES <Type with [!AZ] must resolve to byte or structure containing only byte fields> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]TYPEOFBYTES
!     [msg_text]Type with <v>([name]) must resolve to byte or structure containing only byte fields
!     {severity_level}ERROR
!     [msg_exp]A type with the indicated attribute must be defined as an IDL
!     [msg_exp]type that resolves to byte or to a structure type in which all
!     [msg_exp]of the fields have types which resolve to byte.
!     [msg_act]Change the type definition, or remove the attribute if it is
!     [msg_act]not needed.
!     [end_msg]

    .SEVERITY ERROR
    TAGPRMTYPE  <Tag parameters must have type unsigned long int passed by value or reference>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]TAGPRMTYPE
!     [msg_text]Tag parameters must have type unsigned long int passed by value or reference
!     {severity_level}ERROR
!     [msg_exp]Parameters to which tag attributes (<kw>([cs_stag]),
!     [msg_exp]<kw>([cs_drtag]), or <kw>([cs_rtag])) are attached must be
!     [msg_exp]either <v>(unsigned long) integer values or <v>(unsigned long)
!     [msg_exp]integers passed by reference.
!     [msg_act]Change the parameter types as required.
!     [end_msg]

    .SEVERITY ERROR
    MAXIDTYPE   <Maximum identifier length for [!AZ] type is !UL characters> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]MAXIDTYPE
!     [msg_text]Maximum identifier length for <v>([name]) type is <v>(number) characters
!     {severity_level}ERROR
!     [msg_exp]A type name with the indicated attribute is used to construct
!     [msg_exp]other names with a prefix and/or suffix concatenated to it.
!     [msg_exp]This further restricts the length of the name to the indicated
!     [msg_exp]value, to make sure that the maximum identifier length is not
!     [msg_exp]exceeded in the generated names.
!     [msg_act]Shorten the type name.
!     [end_msg]

    .SEVERITY   WARNING
    NOPORTEXCEP <Use of user-defined exceptions requires !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]NOPORTEXCEP
!     [msg_text]Use of user-defined exceptions requires <v>(option)
!     {severity_level}WARNING
!     [msg_exp]Use of user-defined exceptions is not interoperable with
!     [msg_exp]implementations of DCE V1.0.
!     [msg_act]Do not use the feature if interoperability with DCE V1.0
!     [msg_act]is required.  Otherwise, specify the extended standard option
!     [msg_act]on the compiler command to suppress the warning.
!     [end_msg]

    .SEVERITY   WARNING
    NOPORTATTR  <Use of [!AZ] attribute requires !AZ> /FAO_COUNT=2

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]NOPORTATTR
!     [msg_text]Use of <v>([name]) attribute requires <v>(option)
!     {severity_level}WARNING
!     [msg_exp]The indicated attribute is not supported by DCE V1.0, and
!     [msg_exp]thus should not be used if interoperability with
!     [msg_exp]implementations of DCE V1.0 is required.
!     [msg_act]Do not use the feature if interoperability with DCE V1.0
!     [msg_act]is required.  Otherwise, specify the extended standard option
!     [msg_act]on the compiler command to suppress the warning.
!     [end_msg]

    .SEVERITY   ERROR
    PRMDEPATTR  <A [!AZ] parameter must have the [!AZ] attribute>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]
!     [msg_text]A <v>([name1]) parameter must have the <v>([name2]) attribute
!     {severity_level}ERROR
!     [msg_exp]A parameter with the first attribute must also have the
!     [msg_exp]second attribute.
!     [msg_act]Add the second attribute to the parameter declaration.
!     [end_msg]

    .SEVERITY   WARNING
    CPPCMDOPT   <Use of C preprocessor directive requires !AZ> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]CPPCMDOPT
!     [msg_text]Use of C preprocessor directive requires <v>(command-option)
!     {severity_level}WARNING
!     [msg_exp]The IDL compiler detected a C preprocessor directive in a
!     [msg_exp]source IDL or ACF file.  C preprocessor directives begin in
!     [msg_exp]the first column of a source line and start with the '#'
!     [msg_exp]character.  Either you have specified an IDL command option to
!     [msg_exp]disable C preprocessing, or IDL does not invoke the C
!     [msg_exp]preprocessor by default on the platform on which you are running.
!     [msg_act]Add the indicated command option to your <kw>(idl) command
!     [msg_act]so that the IDL compiler will invoke the C preprocesor
!     [msg_act]to preprocess source IDL and ACF files before they are parsed.
!     [end_msg]

    .SEVERITY   WARNING
    NOPORTANON  <Use of anonymous !AZ may not be portable across C compilers> /FAO_COUNT=1

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]NOPORTANON
!     [msg_text]Use of anonymous <kw>(type) may not be portable across C compilers
!     {severity_level}WARNING
!     [msg_exp]The source IDL contains an IDL language construct, such as an
!     [msg_exp]<kw>(enum), declared as an unnamed type within another
!     [msg_exp]definition.  The generated stub code will compile cleanly under
!     [msg_exp]some C compilers, but generate errors from others.  The potential
!     [msg_exp]errors are due to differences in the scoping of identifiers
!     [msg_exp]across C compilers.
!     [msg_act]Use a <kw>(typedef) statement to define the base <kw>(type)
!     [msg_act]as a named type, then use the type name in any subsequent
!     [msg_act]declarations in place of an anonymous definition.
!     [end_msg]

    .SEVERITY WARNING
    NOFLTPARAM  <A [fault_status] parameter has no utility in this operation>

!     {engineer_name}Rico
!     {audience}programmer
!     [begin_msg]
!     [ident]NOFLTPARAM
!     [msg_text]A <kw>([fault_status]) parameter has no utility in this operation
!     {severity_level}WARNING
!     [msg_exp]A <kw>([fault_status]) parameter is used in an operation where no
!     [msg_exp]fault can ever occur.  An example is an encoding services
!     [msg_exp]operation, i.e. one using <kw>([encode]) or <kw>([decode]) -
!     [msg_exp]there is no remote entity from which a fault might be received.
!     [msg_act]Perhaps you meant to use a <kw>([comm_status]) attribute.
!     [msg_act]If not, the parameter can be removed from the operation or
!     [msg_act]the <kw>([fault_status]) attribute can be removed from the
!     [msg_act]parameter.
!     [end_msg]

    .SEVERITY   ERROR
    CONSTRUCTOR  <A [cxx_new] operation must return an interface pointer>

!     {engineer_name}Viv
!     {audience}programmer
!     [begin_msg]
!     [ident]CONSTRUCTOR
!     [msg_text]A <kw>([cxx_new]) operation must return an interface pointer
!     {severity_level}ERROR
!     [msg_exp]A <kw>([cxx_new]) operation attribute is used to identify a 
!     [msg_exp]remote object creator operation. A remote object creator
!     [msg_exp]operation must return a pointer to an interface.
!     [msg_act]Insure that the return value for the operation is 
!     [msg_act]an <interface-name> * type.
!     [end_msg]

    .SEVERITY   ERROR
    INTREFNOTALO    <Interface reference !AZ allowed only as target of a * or & pointer> /FAO_COUNT=1

!     {engineer_name}Wong
!     {audience}programmer
!     [begin_msg]
!     [ident]INTREFNOTALO
!     [msg_text]Interface reference <kw>(name) allowed only as target of * or & pointer
!     {severity_level}ERROR
!     [msg_exp]When an IDL inteface is compiled with <kw>(-lang cxx), the
!     [msg_exp]interface name also implicitly defines a type, similar to a C++
!     [msg_exp]class.  Pointers to or references to this interface type
!     [msg_exp]may be used elsewhere in the interface.  Any other uses of
!     [msg_exp]this type are not valid.
!     [msg_act]Change the declaration to use a * or &, or change the type
!     [msg_act]used in the declaration.
!     [end_msg]

    .SEVERITY   WARNING
    PTRATTBIGN  <Pointer attributes are ignored on an interface reference>

!     {engineer_name}Wong
!     {audience}programmer
!     [begin_msg]
!     [ident]PTRATTBIGN
!     [msg_text]Pointer attributes are ignored on an interface reference
!     {severity_level}WARNING
!     [msg_exp]Interface references are not managed in the same way as
!     [msg_exp]other pointers and references in IDL.  Thus, the pointer
!     [msg_exp]attributes <kw>([ref]), <kw>([unique]), and <kw>([ptr])
!     [msg_exp]are not meaningful on an interface reference.
!     [msg_act]No action is required, since the pointer attribute is ignored
!     [msg_act]and the IDL compiler continues processing.  To eliminate the
!     [msg_act]warning message, remove the pointer attribute from the
!     [msg_act]declaration.
!     [end_msg]

    .SEVERITY   WARNING
    STATICIGN   <Static property or ACF attribute [cxx_static] is ignored except for -lang cxx>

!     {engineer_name}Wong
!     {audience}programmer
!     [begin_msg]
!     [ident]STATICIGN
!     [msg_text]Static property or ACF attribute <kw>([cxx_static]) is ignored except for <kw>(-lang cxx)
!     {severity_level}WARNING
!     [msg_exp]The IDL <kw>(static) keyword on an operation, as well as the ACF
!     [msg_exp]<kw>([cxx_static]) attribute on an operation, only pertain to
!     [msg_exp]interfaces that are compiled with the <kw>(-lang cxx) IDL
!     [msg_exp]compiler option.
!     [msg_act]If your application code is written in C++, then you must use
!     [msg_act]the <kw>(-lang cxx) option to the IDL compiler in order to
!     [msg_act]enable generation of C++ header file and stub files.
!     [msg_act]Otherwise, no action is required, since the static property is
!     [msg_act]ignored, and the IDL compiler continues processing.  To eliminate
!     [msg_act]the warning message, remove the static property from the
!     [msg_act]declaration.
!     [end_msg]

    .SEVERITY   ERROR
    LOOKUPNOTVAL    <ACF attribute [cxx_lookup] is only valid for -lang cxx>

!     {engineer_name}Wong
!     {audience}programmer
!     [begin_msg]
!     [ident]LOOKUPNOTVAL
!     [msg_text]ACF attribute <kw>([cxx_lookup]) is only valid for <kw>(-lang cxx)
!     {severity_level}ERROR
!     [msg_exp]The ACF <kw>([cxx_lookup]) interface attribute only pertains to
!     [msg_exp]interfaces that are compiled with the <kw>(-lang cxx) IDL
!     [msg_exp]compiler option.
!     [msg_act]If your application code is written in C++, then you must use
!     [msg_act]the <kw>(-lang cxx) option to the IDL compiler in order to
!     [msg_act]enable generation of C++ header file and stub files.
!     [msg_act]Otherwise, edit the <v>(interface-name)).acf file,
!     [msg_act]and remove the <kw>([cxx_lookup]) attribute.
!     [end_msg]

    .SEVERITY   ERROR
    NEWNOTVAL   <ACF attribute [cxx_new] is only valid for -lang cxx>

!     {engineer_name}Wong
!     {audience}programmer
!     [begin_msg]
!     [ident]NEWNOTVAL
!     [msg_text]ACF attribute <kw>([cxx_new]) is only valid for <kw>(-lang cxx)
!     {severity_level}ERROR
!     [msg_exp]The ACF <kw>([cxx_new]) operation attribute only pertains to
!     [msg_exp]interfaces that are compiled with the <kw>(-lang cxx) IDL
!     [msg_exp]compiler option.
!     [msg_act]If your application code is written in C++, then you must use
!     [msg_act]the <kw>(-lang cxx) option to the IDL compiler in order to
!     [msg_act]enable generation of C++ header file and stub files.
!     [msg_act]Otherwise, edit the <v>(interface-name)).acf file,
!     [msg_act]and remove any uses of the <kw>([cxx_new]) attribute.
!     [end_msg]

    .SEVERITY   ERROR
    INHERITNOTVAL   <IDL inheritance operator ':' is only valid for -lang cxx>

!     {engineer_name}Viv
!     {audience}programmer
!     [begin_msg]
!     [ident]INHERITNOTVAL
!     [msg_text]The Inheritance operator ':' is only valid for <kw>(-lang cxx)
!     {severity_level}ERROR
!     [msg_exp]The Inheritance operator ':' is only valid for
!     [msg_exp]interfaces that are compiled with the <kw>(-lang cxx) IDL
!     [msg_exp]compiler option.
!     [msg_act]If your application code is written in C++ and using interface inheritance,
!     [msg_act]then you must use the <kw>(-lang cxx) option to the IDL compiler
!     [msg_act]in order to enable generation of C++ header file and stub files
!     [msg_act]with inheritance.
!     [msg_act]Otherwise, edit the <v>(interface-name)).idl file,
!     [msg_act]and remove any uses of the inheritance ':' operator.
!     [end_msg]

    .SEVERITY   ERROR
    INHERITNOTDEF   <Inherited interfaces must be imported: !AZ > /FAO_COUNT=1

!     {engineer_name}Viv
!     {audience}programmer
!     [begin_msg]
!     [ident]INHERITNOTDEF
!     [msg_text]The inherited interface identified by the inheritance operator must be imported.
!     {severity_level}ERROR
!     [msg_exp]No definition for the inherited interface identified by the 
!     [msg_exp]inheritance operator was found.
!     [msg_act]If your application code is using interface inheritance,
!     [msg_act]then you must import a definition for the inherited
!     [msg_act]interface.
!     [msg_act]Otherwise, edit the <v>(interface-name)).idl file,
!     [msg_act]and remove any uses of the inheritance ':' operator.
!     [end_msg]



    .SEVERITY   ERROR
    DELEGATENOTVAL    <ACF attribute [cxx_delegate] is only valid for -lang cxx>

!     {engineer_name}Viv
!     {audience}programmer
!     [begin_msg]
!     [ident]DELEGATENOTVAL
!     [msg_text]ACF attribute <kw>([cxx_delegate]) is only valid for <kw>(-lang cxx)
!     {severity_level}ERROR
!     [msg_exp]The ACF <kw>([cxx_delegate]) interface attribute only pertains to
!     [msg_exp]interfaces that are compiled with the <kw>(-lang cxx) IDL
!     [msg_exp]compiler option.
!     [msg_act]If your application code is written in C++, then you must use
!     [msg_act]the <kw>(-lang cxx) option to the IDL compiler in order to
!     [msg_act]enable generation of C++ header file and stub files.
!     [msg_act]Otherwise, edit the <v>(interface-name)).acf file,
!     [msg_act]and remove the <kw>([cxx_delegate]) attribute.
!     [end_msg]


.END
