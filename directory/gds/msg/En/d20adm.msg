$
$ @OSF_COPYRIGHT@
$ COPYRIGHT NOTICE
$ Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
$ ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
$ src directory for the full copyright text.
$
$ HISTORY
$ $Log: d20adm.msg,v $
$ Revision 1.1.774.2  1996/03/09  23:14:01  marty
$ 	Update OSF copyright year
$ 	[1996/03/09  22:31:22  marty]
$
$ Revision 1.1.774.1  1995/12/08  16:04:16  root
$ 	Submit OSF/DCE 1.2.1
$ 	[1995/12/08  15:15:35  root]
$ 
$ Revision 1.1.772.4  1994/06/21  14:47:31  marrek
$ 	June 1994 code submission.
$ 	[1994/06/16  09:42:28  marrek]
$ 
$ Revision 1.1.772.3  1994/05/10  15:56:50  marrek
$ 	Bug fixes.
$ 	[1994/04/29  09:29:49  marrek]
$ 
$ 	April 1994 code submission.
$ 	[1994/04/27  12:41:32  marrek]
$ 
$ Revision 1.1.772.2  1994/03/23  15:19:04  keutel
$ 	March 1994 code drop
$ 	[1994/03/21  16:32:25  keutel]
$ 
$ Revision 1.1.772.1  1994/02/22  17:41:13  marrek
$ 	Code drop for dce1.1.
$ 	[1994/02/08  10:56:10  marrek]
$ 
$ Revision 1.1.770.3  1993/10/19  15:02:59  keutel
$ 	CR 9188
$ 	[1993/10/19  15:02:51  keutel]
$ 
$ Revision 1.1.770.2  1993/10/14  17:35:57  keutel
$ 	October 1993 code drop
$ 	[1993/10/14  17:34:15  keutel]
$ 
$ Revision 1.1.770.1  1993/10/13  17:33:18  keutel
$ 	October 1993 code drop
$ 	[1993/10/11  09:01:10  keutel]
$ 
$ Revision 1.1.6.3  1993/08/11  15:02:51  marrek
$ 	July 1993 code drop.
$ 	[1993/08/02  13:00:50  marrek]
$ 
$ Revision 1.1.6.2  1993/07/02  15:09:20  marrek
$ 	Changes for port to HP800.
$ 	[1993/07/02  14:37:58  marrek]
$ 
$ Revision 1.1.4.7  1993/02/02  10:19:31  marrek
$ 	Any Syntax introduced.
$ 	[1993/02/02  09:40:27  marrek]
$ 
$ Revision 1.1.4.6  1993/01/29  08:40:27  marrek
$ 	Correct some messages.
$ 	[1993/01/28  10:49:08  marrek]
$ 
$ Revision 1.1.4.5  1993/01/25  10:13:11  marrek
$ 	Add comment leader
$ 	[1993/01/14  15:18:11  marrek]
$ 
$ $EndLog$
$
$ $RCSfile: d20adm.msg,v $ $Revision: 1.1.774.2 $ $Date: 1996/03/09 23:14:01 $
$
$set 1 preload                  $ /* approximate length of input values */
$                                 /* *sccsid = @(#)d20adm.msg	8.1.1.5 92/02/12 (K Sys AP 11)    */

1 200000

$set 2 preload                   $ /* input values for updatei times    */

1 %.0=0        .1=1        .2=2        .3=3        .4=4        .5=5        .\
    6=6        .7=7        .8=8        .9=9        .10=10       .11=11       .\
   12=12       .13=13       .&
2  14=14       .15=15       .16=16       .17=17       .18=18       .\
   19=19       .20=20       .21=21       .22=22       .23=23       .a
3 %.0=SUNDAY   .1=MONDAY   .2=TUESDAY  .3=WEDNESDAY.4=THURSDAY .5=FRIDAY   .6=SATURDAY .a

$ /* ************************************************************************ */
$ /* ----------------------- O P E R A T I O N S ---------------------------- */
$ /* ************************************************************************ */

$             /* A D M I N I S T R A T I O N   F U N C T I O N S */
$set 3 preload                   $ /* administration functions          */

1 Object Administration  $
2 Schema Administration  $
3 Shadow Administration  $
4 Subtree Administration $
5 Cache Update           $
6 Logon to a Specific DSA$
7 Logon                  $

$             /* B A S I C  A D M I N I S T R A T I O N */
$set 4 preload                   $ /* basic administration              */

1  $                     $
2  Add Object           $
3  Remove Object        $
4  Display Objects      $
5  Display Objects      $
6  Add Attributes       $
7  Delete Attributes    $
8  Modify Attribute     $
9  Add Alias            $
10 Modify RDN           $
11 $                     $

$             /* CACHE operations */
$set 5 preload

1 Add Client Address
2 Display Client Address
3 Delete Default DSA


$             /* S C H E M E  A D M I N I S T R A T I O N */
$set 6 preload                   $ /* scheme administration             */

1  $                     $
2  Store Schema		 $
3  Load Schema		 $
4  Display SRT		 $
5  Add SRT Entry         $
6  Delete SRT Entry      $
7  Modify SRT Entry      $
8  Display OCT           $
9  Add OCT Entry         $
10 Delete OCT Entry      $
11 Modify OCT Entry      $
12 Display AT            $
13 Add AT Entry          $
14 Delete AT Entry       $
15 Modify AT Entry       $
16 $                     $


$             /* D E L T A - U P D A T E - F U N C T I O N S            */
$set 7 preload                   $ /* delta - update - functions        */

1 Create Shadows and Shadowing Job
2 Create Shadowing Job
3 Remove Shadows and Shadowing Job
4 Remove Shadowing Job
5 Update Shadowing Job
6 Display Shadowing Jobs
7 Display Update Errors
8 Remove Update Error


$             /* T R E E - H A N D L I N G - F U N C T I O N S         */
$set 8 preload                   $ /* tree - handling - functions      */

1 Save Subtree
2 Append Subtree
3 Copy Subtree
4 Change Name / Move Subtree
5 Delete Subtree
6 Change Master
7 Modify Subtree

$set 9 preload                   $ /* CACHE/DSA functions              */

1 $ 3 Display Objects                     $
2 $ 3 Display Objects (Global Master Info)$
3 $ 4 Display Local and Default DSA                      $
4 $ 4 Display Objects (Entries in Current DSA)$
5 $ 5 Add Client Address                  $
6 $ 5 Add Attributes                      $
7 $ 6 Display Client Address              $
8 $ 6 Delete Attributes                   $
9 $ 7 Delete Default DSA                  $
10 $ 7 Modify Attribute                   $

$set 10 preload                   $ /* char *d20_2_1opt                */
$                                   /* char *d20_2_2opt                */
1 %.0=None                   .1=Changing Name Structure.a
2 %.0=None                   .1=Changing Name Structure.2=Exit                   .a

$set 11 preload                  $ /* char *d20_opclass[D20_NOPCLASS] */

1 Modify Public   $
2 Read Standard   $
3 Modify Standard $
4 Read Sensitive  $
5 Modify Sensitive$

$set 12 preload                  $ /* char *d20_E11format             */
$                                  /* char *d20_E12format             */
1 %.0=ACTIVATE UPDATES            .\
    1=DEACTIVATE UPDATES          .\
    2=MODIFY UPDATE FREQUENCY     .\
    3=ACTIVATE UPDATES IMMEDIATELY.a
2 %.0=ACTIVATE UPDATES            .\
    1=NO UPDATES                  .a

$set 13 preload                  

1 %.5=EVERY 5 MINUTES .\
   10=EVERY 10 MINUTES.\
   15=EVERY 15 MINUTES.\
   30=EVERY 30 MINUTES.a
2 %.1=EVERY HOUR      .\
    2=EVERY 2 HOURS   .\
    4=EVERY 4 HOURS   .\
    6=EVERY 6 HOURS   .\
   12=EVERY 12 HOURS  .a
3 %.99=EVERY DAY      .\
     2=ONCE A WEEK    .\
     3=TWICE A WEEK   .a


$set 14 preload                  $ /* char d20_F1_job_format[]        */
$                                  /* char d20_F2_job_format[]        */
1 %.4=ACTIVE                      .\
    5=NOT ACTIVE                  .a
2 %.4=ACTIVE                      .\
    1=DEACTIVATE UPDATES          .\
    2=MODIFY UPDATE FREQUENCY     .\
    3=ACTIVATE UPDATES IMMEDIATELY.a
3 %.5=NO UPDATES                  .\
    0=ACTIVATE UPDATES            .\
    3=ACTIVATE UPDATES IMMEDIATELY.a

$ /* ************************************************************************ */
$ /* ------------------ ERROR MESSAGE TABLE --------------------------------- */
$ /* ************************************************************************ */

$set 15 preload                  $ /* ERROR MESSAGES                    */

1 Unknown operation !
2 This operation is not allowed for local name !
3 Unknown object type !
4 Last recurring value reached !
5 Name part %d is too long !
6 Enter new attribute value !
7 Enter old attribute value !
8 Auxiliary object class %d not allowed !
9 Input not finished with ' !
10 Attribute value %d too long !
11 Attribute name %d not allowed !
12 At least 1 attribute is necessary !
13 Scheme update failed, local scheme file will be created !
14 Only %d attributes allowed !
15 Not all parts of the DSA name have been entered!
16 Old value too long !
17 New value too long !
18 Enter attribute name !
19 There are still more entries matching the pattern !
20 No objects found !
21 End of list reached !
22 Input not hexadecimal !
23 Invalid net address syntax !
24 Number of changed entries : %d !
25 Invalid net address size !
26 Please enter a filename !
27 Invalid character in net address !
28 Invalid PSAP-address syntax !
29 Invalid combination of different macro types !
30 Invalid filename !
31 To much macro parameter in net address !
32 Invalid macro parameter format !
33 Wrong input for copies update time !
34 Insufficient memory !
35 Unknown net address type !
36 Invalid selector size !
37 Invalid selector syntax !
38 Can't read macro file !
39 Last page reached !
40 Internal error !
41 Mandatory attr. appears in one of the superior object classes !
42 Cannot open/close file containing list of countries !
43 First page reached !
44 Wrong DN-interpretation for DN-%-d !
45 Wrong DN-syntax for DN-%-d !
46 At least one DN must be given !
47 Wrong DN-length for DN-%-d !
48 Problems with ACL (too long) !
49 Attribute value doesn't exist or no modify permission !
50 Not all entries could be changed !
51 Too many entries found ! Please restrict entry information !
52 No space for scheme !
53 Directory identifier unknown !
54 User credentials (name, password) unknown !
55 Illegal format of directory identifier !
56 Default DSA unknown !
57 Local scheme can't be read !
58 Invalid environment DIRINFBAS !
59 Selected object type is not allowed !
60 Schema from DSA can not be read!
61 Not all parts of the object name are defined !
62 Syntax error in specified identifier (IDI)
63 Wrong DSA name syntax !
64 Not all the mandatory attributes have been entered !
65 Unknown attribute type in scheme description information !
66 No entries in SRT !
67 Beginning of list of selected SRT entries reached !
68 Inconsistency of scheme (wrong format of TIMESTAMP/SRT/OCT/AT entry) !
69 Unknown attribute type in scheme information !
70 End of list of selected SRT entries reached !
71 While reading scheme: error value = %d !
72 On accessing local scheme files !
73 Invalid attribute input format !
74 No memory for storing object types !
75 Operation not allowed on this attribute !
76 Optional attr. appears in one of the superior object classes !
77 Target DSA should not be LOCAL DSA !
78 Object type already defined !
79 Unknown attribute type !
80 Unknown object type !
81 Wrong naming use !
82 No entries in OCT !
83 No entries in AT !
84 Beginning of list of selected OCT entries reached !
85 Beginning of list of selected AT entries reached !
86 End of list of selected OCT entries reached !
87 End of list of selected AT entries reached !
88 Attribute type already defined !
89 Invalid attribute length !
90 Object type is no leaf (object type) !
91 Unknown parent object type !
92 While reading (local) scheme !
93 Dump/Restore fails !
94 Enter DSA-name !
95 Attribute value not allowed !
96 Cache entry for default or local DSA fails
97 Use scheme administration for modifying attribute !
98 SRT/OCT/AT entry must not be deleted !
99 File Identifier: > 0 !
100 Index Priority: 0 - 10 !
101 No PSAP-Address for client entered !
102 Shadowing job can't be activated or deactivated !
103 Operation not allowed on a single valued attribute !
104 Warning: Conversion to the local string format fails ! 
105 One of the fields could not be converted to the T61 string !
106 Conv. tables for internationalization could not be inititialized !
107 Wrong Distinguished Name syntax !


$set 16 preload                  $ /* ERROR MESSAGES                    */

1 Shadowing job doesn't exist !
2 Dumping fails (Error while searching objects) !
3 Shadow creation fails !
4 Dumping fails (Error while creating dump file) !
5 Internal error while allocating memory !
6 Shadow generation fails (SEARCH operation) !
7 Shadow generation fails (ADD operation) !
8 Shadow generation fails (MODIFY operation) !
9 Shadow generation fails (DELETE operation) !
10 Error while reading dump file !
11 Dumping fails (connection to DSA) !
12 Error while updating shadowing job entry !
13 End of shadowing job file reached !
14 Overlapping shadowing jobs !
15 Illegal update operation mode !
16 Local DSA name can't be read !
17 '*' not allowed in object name !
18 Not all shadows could be removed (job removed) !
19 Error while removing shadows; (job removed) !
20 No shadowing jobs defined yet !
21 Last shadowing job displayed !
22 Error while reading shadowing jobs !
23 Error file doesn't exist !
24 Error file already locked, wait until unlocked !
25 Error while locking error file !
26 Syntax error in specified Ethernet-Address !
27 Error while removing shadows; (job  n o t  removed) !
28 Saving of subtree failed (connection to DSA) !
29 Saving of subtree failed (SEARCH operation) !
30 Saving of subtree failed (dump file creation) !
31 Saving of subtree failed !
32 Appending of subtree failed (SEARCH operation) !
33 Appending of subtree failed (ADD operation) !
34 Appending of subtree failed (MODIFY operation) !
35 Parent object doesn't exist !
36 Save file not found !
37 Error while reading save file !
38 Appending of subtree failed !
39 Deleting of subtree failed !
40 Deleting of subtree failed (SEARCH operation) !
41 Invalid service controls !
42 Directory-ID missing !
43 Unknown parameter !
44 Internet address : n1.n2.n3.n4 where n1-n4 could be 0-255 !
45 Value for Portnumber: 1024-32000 !
46 Changing of master failed !
47 Changing of master failed (SEARCH operation) !
48 Changing of master failed (MODIFY operation) !
49 Not all shadows are present on the new DSA !
50 Changing of name failed (SEARCH operation) !
51 Parent of new object does not exist !
52 Changing of name failed !
53 Top of list reached !
54 Too many object types (for a DN structure) defined !
55 Changing of ACL has an own operation !
56 Changing of Aliased-Object isn't allowed !
57 Operation only allowed in local DSA !
58 Entry does not exist !
59 Superior entry does not exist !
60 Entry already exists !
61 Object class does not exist !
62 Optional attribute does not exist !
63 Mandatory attribute does not exist !
64 Attribute is both an optional and a mandatory attribute !
65 Object identifier is not unique !
66 Too many rules which generate same DN structure !
67 This operation will leave some orphan entries !
68 OCT entry is referenced in an SRT entry !
69 AT entry referenced in an OCT/SRT entry !
70 Wrong input for file number !
71 Wrong input for acronym !
72 Wrong input for object identifier !
73 Wrong input for name !
74 Wrong input for lower/upper bound !
75 Wrong input for number of recurring attribute values !
76 Wrong input for syntax !
77 Wrong input for access class !
78 Wrong input for index level !
79 Wrong input for phonetical matching !
80 Name is not unique !
81 Too many mandatory attributes !
82 Too many optional attributes !
83 Attribute acronym too long !
84 Repetition in object-class/attribute list !
85 At least one object-class/attribute must be entered !
86 Too many naming attributes !
87 Naming attribute acronym too long !
88 Naming attribute does not exist !
89 Naming attribute does not belong to object class !
90 Naming attribute is recurring attribute !
91 Naming attribute in an SRT entry becomes invalid !
92 DEFAULT schema entry can not be deleted !
93 DEFAULT schema entry can not be modified !
94 Object identifer of DEFAULT schema entry can not be changed !
95 Superior object classes of DEFAULT schema entry can not be changed !
96 Mandatory attribute of DEFAULT schema entry can not be modified !
97 Structure rule with this rule no. can not be created !
98 Too many rules involved to generate a DN !
99 Invalid CDS-Cell value !
100 Invalid CDS-Replica value !
101 Invalid Postal-Address value !
102 Invalid Teletex-Terminal-Id value !
103 Invalid Telex-Number value !
104 Invalid Fax-Number value !
105 Invalid Search-Guide value !
106 Syntax of DEFAULT schema entry can not be changed !
107 Error opening lock file !
108 Lock file doesn't exist !
109 Error opening error file !
110 Error opening temporary file !
111 Invalid opcode encountered in error file !
112 Flushing to end of error file !
113 Error writing update to error file !
114 Error deleting temporary file !
115 End of error file reached !
116 Error updating error file !
117 Enter an alternate address !
118 Object identifier syntax incorrect !
119 Master bind not possible with existing credentials !
120 The administration can't add more than ten revoked certificates.
121 Principal name syntax incorrect !
122 
123 Date is not valid !
124 A Certificate is required !
125 Algorithm identifier incorrect !

$set 17 preload                  $ /* ERROR MESSAGES                    */

1 Unexpected error code received by administration !
2 Invalid Bind-ID !
3 Invalid Directory-ID !
4 Invalid update on a shadow entry !
5 Specified entry is no shadow entry !
6 Memory exceeded !
7 Invalid user credentials (name, password) !
8 Unknown DSA !
9 Inappropriate Author !
10 Invalid Signature !
11 Unsigned Arguement !
12 Invalid access rights !
13 Attribute not present !
14 Constraint violation !
15 Inappropriate match: attribute value not found !
16 Attribute not present !
17 Wrong attribute syntax !
18 Attribute/Attribute-value exists !
19 Object (or superior object) doesn't exist !
20 Operation not allowed on alias entry !
21 Wrong (naming) attribute syntax !
22 Alias is orphan !
23 Time limit exceeded !
24 System too busy !
25 No connection to DSA/DUA-Cache available !
26 Chaining required although prohibited by serv. cntls. !
27 DSA could not proceed in name resolution !
28 Invalid reference in operation !
29 Administrative limit exceeded !
30 Loop detected !
31 Critical extension was required but not available !
32 No referrals within requested scope !
33 Directory information tree has become inconsistent !
34 There are still subordinate entries !
35 Object already exists !
36 Operation not allowed on naming attributes !
37 Naming violation !
38 Object class violation !
39 Operation effects multiple DSAs (which is prohibited) !
40 Operation attempted to modify object class attribute !
41 Error from underlying communication layers !
42 Problem in translating DSA schema to admin. schema !
43 Operation does not exist !
44 Problem in storing admin. schema in a local file !
45 Operation can not be abandoned !
46 Operation abandoned !
47 Unexpected attribute type while reading TIMESTAMP of DSA scheme!
48 Object class with file no. -1 is not allowed !
49 Invalid syntax for a naming attribute !
50 Naming attribute must have +ve value for No. of recurring values !
51 All mandatory components must be entered !
52 Both Domain Type and Value should be entered !
53 One of the components of the mandatory group must be entered !
54 At least one of the components must be entered !
55 Wrong syntax for Directory Name !
56 There is no auxiliary object class for the selected object class !
57 Invalid structure rule no. for new object !
58 Too many super classes !
59 Super class acronym too long !
60 Too many auxiliary classes !
61 Auxiliary class acronym too long !
62 Constraint violation in super class usage !
63 Object class kind of default schema entry can not be changed !
64 Schema can not have more than one object class of kind ALIAS !
65 Invalid object class kind !
66 Too many structural object classes as super classes !
67 Schema object class can not be modified !
68 This modification generates a loop in object class hierarchy !
69 Constraint violation: Class is being used as an AUX. class !
70 Only structural kind of obj. class can have AUX. classes !
71 Auxiliary class should be of kind AUXILIARY !
72 Constraint violation: Class is being used as super class !
73 Constraint violation: Class is being used in a structure rule !
74 STRUCTURAL object class must have GTP as one of it's super classes !
75 Object class in a structure rule should be of kind STRUCTURAL !
76 Not all mandatory attributes have been entered !
77 Attribute with this syntax should be non-recurring !
78 Bad argument to an IAPL function !
79 With an anonymous authentication, do not enter a user name or a password.
80 With DCE authentication, do not enter a user name or a password.
81 With simple authentication, enter a user name and a password.
82 With strong authentication, enter a user name but no password.
83 Value for the authentication mechanism not valid.
84 Use nothing for anonymous or 2 for simple or 6 for strong or 5 for DCE.

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  1 ------------------------------ */
$ /* ************************************************************************ */

$set 18 preload                 $ /* mask 1 (login mask)                */

1   7  7 Password:# @#
2   7 36 @# @#
3   9  7 @# @#
4   9 36 @# @#
5  10  7 @# @#
6  10 36 @# @#
7  11  7 @# @#
8  11 36 @# @#
9  12  7 @# @#
10 12 36 @# @#
11 13  7 @# @#
12 13 36 @# @#
13 14  7 @# @#
14 14 36 @# @#
15 15  7 @# @#
16 15 36 @# @#
17 16  7 @# @#
18 16 36 @# @#
19 17  7 @# @#
20 17 36 @# @#
21 18  7 @# @#
22 18 36 @# @#
23 19  7 @# @#
24 19 36 @# @#
25 20  7 @# @#
26 20 36 @# @#
27 21 10 Options:# @#
28 21 19 @# %.0=Logon to the Default DSA.1=Logon to a Specific DSA .2=Logon to the DUA Cache  .3=Changing Name Structure .a#




$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  2 ------------------------------ */
$ /* ************************************************************************ */

$set 19 preload                 $ /* mask 2                             */

1   2  5 (Mask 2)             DIRECTORY SYSTEM# @#
2   2 46 @# @#
3   5  5 DSA IDENTIFICATION:# @#
4   9  7 @# @#
5   9 36 @# @#
6  10  7 @# @#
7  10 36 @# @#
8  11  7 @# @#
9  11 36 @# @#
10 12  7 @# @#
11 12 36 @# @#
12 13  7 @# @#
13 13 36 @# @#
14 14  7 @# @#
15 14 36 @# @#
16 15  7 @# @#
17 15 36 @# @#
18 16  7 @# @#
19 16 36 @# @#
20 17  7 @# @#
21 17 36 @# @#
22 18  7 @# @#
23 18 36 @# @#
24 19  7 @# @#
25 19 36 @# @#
26 20  7 @# @#
27 20 36 @# @#
28 21 10 Options:# @#
29 21 19 @# %.0=None                   .1=Changing Name Structure.a#


$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  3 ------------------------------ */
$ /* ************************************************************************ */

$set 20 preload                 $ /* mask 3                             */

1   2  5 (Mask 3)             DIRECTORY SYSTEM           Administration# @#
2   6  8 ADMINISTRATION FUNCTIONS:# @#
3   9 10 0 Exit# @#
4  11 10 1 Object Administration# @#
5  13 10 2 Schema Administration# @#
6  13 10 2 Cache Update         # @#
7  15 10 3 Shadow Administration# @#
8  17 10 4 Subtree Administration# @#
9  19  8 Current DSA# @#
10 19 20 @# @#
11 20 20 @# @#
12 23  5 Which function ?# @#
13 23 22 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  4 ------------------------------ */
$ /* ************************************************************************ */

$set 21 preload                 $ /* mask 4                             */

1   2  5 (Mask 4)             DIRECTORY SYSTEM# @#
2   6  8 OPERATIONS# @#
3   8 10 $ 0 Exit# @#
4   9 10 $ 1 Add Object# @#
5  10 10 $ 2 Remove Object# @#
6  11 10 @# @#
7  12 10 @# @#
8  13 10 $ 5 Add Attributes# @#
9  14 10 $ 6 Delete Attributes# @#
10 15 10 $ 7 Modify Attribute# @#
11 16 10 $ 8 Add Alias# @#
12 17 10 $ 9 Modify RDN# @#
13  6 40 Entry Type:# @#
14  6 52 @# %.0=MASTER.1=SHADOW.a#
15 23  5 Which operation ?# @#
16 23 23 @# @#
17  2 48 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  4a ----------------------------- */
$ /* ************************************************************************ */

$set 22 preload                 $ /* mask 4a                            */

1   2  5 (Mask 4a)            DIRECTORY SYSTEM# @#
2   2 48 Display Special DSAs# @#
3   6  8 Special DSAs# @#
4   8  9 Local:# @#
5   8 18 @# @#
6   9 18 @# @#
7  11  9 Default:# @#
8  11 18 @# @#
9  12 18 @# @#
10 14 18 @# @#
11 15 18 @# @#
12 17 18 @# @#
13 18 18 @# @#
14 20 18 @# @#
15 21 18 @# @#


$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  5 ------------------------------ */
$ /* ************************************************************************ */

$set 23 preload                 $ /* mask 5                            */

1   5  8 Structure Rule# @#
2   5  8 Structure Rule of Alias Object# @#
3   5  8 Structure Rule of Aliased Object# @#
4   5 43 Name structure# @#
5   8  5 00 ROOT# @#
6   9  5 @# @#
7   9  8 @# @#
8   9 43 @# @#
9  10  5 @# @#
10 10  8 @# @#
11 10 43 @# @#
12 11  5 @# @#
13 11  8 @# @#
14 11 43 @# @#
15 12  5 @# @#
16 12  8 @# @#
17 12 43 @# @#
18 13  5 @# @#
19 13  8 @# @#
20 13 43 @# @#
21 14  5 @# @#
22 14  8 @# @#
23 14 43 @# @#
24 15  5 @# @#
25 15  8 @# @#
26 15 43 @# @#
27 16  5 @# @#
28 16  8 @# @#
29 16 43 @# @#
30 17  5 @# @#
31 17  8 @# @#
32 17 43 @# @#
33 18  5 @# @#
34 18  8 @# @#
35 18 43 @# @#
36 19  5 @# @#
37 19  8 @# @#
38 19 43 @# @#
39 20  5 @# @#
40 20  8 @# @#
41 20 43 @# @#
42 23  5 Which structure rule ?# @#
43 23 28 @# @#
44  2  5 (Mask 5)             DIRECTORY SYSTEM# @#
45  2 46 @# @#
46  5  8 Structure Rule of New Object# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  6 ------------------------------ */
$ /* ************************************************************************ */

$set 24 reload                  $ /* mask 6                            */

1   2  5 (Mask 6)             DIRECTORY SYSTEM# @#
2   2 46 @# @#
3   5  8 Object Interpretation:# @#
4   5 36 @# %.0=SINGLE OBJECT              .1=OBJECT AND ITS SUBORDINATES.a#
5   6  8 Object Name# @#
6   6  8 Alias Name# @#
7   6  8 Aliased Name# @#
8   6  8 New Object Name# @#
9   8 10 @# @#
10  8 36 @# @#
11  9 10 @# @#
12  9 36 @# @#
13 10 10 @# @#
14 10 36 @# @#
15 11 10 @# @#
16 11 36 @# @#
17 12 10 @# @#
18 12 36 @# @#
19 13 10 @# @#
20 13 36 @# @#
21 14 10 @# @#
22 14 36 @# @#
23 15 10 @# @#
24 15 36 @# @#
25 16 10 @# @#
26 16 36 @# @#
27 17 10 @# @#
28 17 36 @# @#
29 18 10 @# @#
30 18 36 @# @#
31 19 10 @# @#
32 19 36 @# @#
33 20 6  Structural Object class:# @#
34 20 31 @# @#
35 21 6  Auxiliary object class:# @#
36 21 30 @# %.0=NO .1=YES.a#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  6a ----------------------------- */
$ /* ************************************************************************ */

$set 25 preload                  $ /* mask 6a                           */

1   2  5 (Mask 6a)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   6  8 Access rights# @#
4   8 10 Modify Public:# @#
5   8 29 @# %.0=NO .1=YES.a#
6   9 10 Read Standard:# @#
7   9 29 @# %.0=NO .1=YES.a#
8  10 10 Modify Standard:# @#
9  10 29 @# %.0=NO .1=YES.a#
10 11 10 Read Sensitive:# @#
11 11 29 @# %.0=NO .1=YES.a#
12 12 10 Modify Sensitive:# @#
13 12 29 @# %.0=NO .1=YES.a#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  6b ----------------------------- */
$ /* ************************************************************************ */

$set 26 preload                  $ /* mask 6b                           */

1   2 56 @# @#
2   6  8 Access rights# @#
3   6 25 @# @#
4   9 13 Distinguished Names:# @#
5   9 60 Interpretation# @#
6  11 13 @# @#
7  12 13 @# @#
8  12 63 @# %.0=OBJECT .1=SUBTREE.3=       .a#
9  14 13 @# @#
10 15 13 @# @#
11 15 63 @# %.0=OBJECT .1=SUBTREE.3=       .a#
12 17 13 @# @#
13 18 13 @# @#
14 18 63 @# %.0=OBJECT .1=SUBTREE.3=       .a#
15 20 13 @# @#
16 21 13 @# @#
17 21 63 @# %.0=OBJECT .1=SUBTREE.3=       .a#
18  2  5 (Mask 6b)            DIRECTORY SYSTEM# @#
19  0  0 @# @#
20  0  0 @# @#
21  0  0 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  7 ------------------------------ */
$ /* ************************************************************************ */

$set 27 preload                  $ /* mask 7                            */

1   2  5 (Mask 7)             DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   6  8 Attributes:# @#
4   8 10 Name :# @#
5   8 18 @# @#
6   9 10 Value:# @#
7   9 18 @# @#
8  10 18 @# @#
9  12 10 Name :# @#
10 12 18 @# @#
11 13 10 Value:# @#
12 13 18 @# @#
13 14 18 @# @#
14 16 10 Name :# @#
15 16 18 @# @#
16 17 10 Value:# @#
17 17 18 @# @#
18 18 18 @# @#
19  0  0 @# @#
20  0  0 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  7a ----------------------------- */
$ /* ************************************************************************ */

$set 28 preload                  $ /* mask 7a                           */


1   5  5 P-Selector:# @#
2   5 21 @# @#
3   7  5 S-Selector:# @#
4   7 21 @# @#
5   9  5 T-Selector:# @#
6   9 21 @# @#
7  12  5 NSAP-address 1:# @#
8  12 21 @# @#
9  14  5 NSAP-address 2:# @#
10 14 21 @# @#
11 16  5 NSAP-address 3:# @#
12 16 21 @# @#
13 18  5 NSAP-address 4:# @#
14 18 21 @# @#
15 20  5 NSAP-address 5:# @#
16 20 21 @# @#
17  2  5 (Mask 7a)            DIRECTORY SYSTEM# @#
18  2 53 @# @#
19  0  0 @# @#
20  0  0 @# @#
21  0  0 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  8 ------------------------------ */
$ /* ************************************************************************ */

$set 29 preload                  $ /* mask 8                            */

1   2  5 (Mask 8)             DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   7  8 Attribute:# @#
4   9 10 Name :# @#
5   9 21 @# @#
6  11 10 Old value:# @#
7  11 21 @# @#
8  12 21 @# @#
9  14 10 New value:# @#
10 14 21 @# @#
11 15 21 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  9 ------------------------------ */
$ /* ************************************************************************ */

$set 30 preload                  $ /* mask 9                            */

1   2  5 (Mask 9)             DIRECTORY SYSTEM# @#
2   2 48 @# @#
3   4  5 Operations# @#
4   6 13 0 Exit# @#
5   7 13 1 Store Schema# @#
6   8 13 2 Load Schema# @#
7   9  6 (SRT): 3 Display SRT# @#
8  10 13 4 Add SRT Entry# @#
9  11 13 5 Delete SRT Entry# @#
10 12 13 6 Modify SRT Entry# @#
11 13  6 (OCT): 7 Display OCT# @#
12 14 13 8 Add OCT Entry# @#
13 15 13 9 Delete OCT Entry# @#
14 16 12 10 Modify OCT Entry# @#
15 17  6 (AT): 11 Display AT# @#
16 18 12 12 Add AT Entry# @#
17 19 12 13 Delete AT Entry# @#
18 20 12  14 Modify AT Entry# @#
19 23  5 Which operation ?# @#
20 23 23 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  10 ----------------------------- */
$ /* ************************************************************************ */

$set 31 preload                  $ /* mask 10                           */

1   2  5 (Mask 11)            DIRECTORY SYSTEM# @#
2   2 48 @# @#
3   4  8 Object Class Acronym:# @#
4   4 43 @# @#
5   5  8 Object Class Name:# @#
6   5 43 @# @#
7   6  8 Acronyms Of Super Classes:# @#
8   6 43 @# @#
9   7 43 @# @#
10  8  8 Object Identifier:# @#
11  8 43 @# @#
12  9  8 Object Class Kind:# @#
13  9 43 @# %.0=Abstract  .1=Alias     .2=Auxiliary .3=Structural.a#
14 10  8 File Number:# @#
15 10 43 @# @#
16 11  8 Auxiliary Object Classes:# @#
17 11 43 @# @#
18 12 43 @# @#
19 13  8 Mandatory Attributes:# @#
20 13 43 @# @#
21 14 43 @# @#
22 15  8 Optional Attributes:# @#
23 15 43 @# @#
24 16 43 @# @#
25 17 43 @# @#
26 18 43 @# @#
27 19 43 @# @#
28 20 43 @# @#
29 21 43 @# @#
30  0  0 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  11 ----------------------------- */
$ /* ************************************************************************ */

$set 32 preload                  $ /* mask 11                           */

1   2  5 (Mask 12)            DIRECTORY SYSTEM# @#
2   2 48 @# @#
3   5 10 Attribute Acronym:# @#
4   5 42 @# @#
5   6 10 Attribute Name:# @#
6   6 42 @# @#
7   7 10 Object Identifier:# @#
8   7 42 @# @#
9   8 10 Lower Bound:# @#
10  8 42 @# @#
11  9 10 Upper Bound:# @#
12  9 42 @# @#
13 10 10 Number of Recurring Values:# @#
14 10 42 @# @#
15 11 10 Attribute Syntax:# @#
16 11 42 @# @# 
17 12 10 Access Class:# @#
18 12 42 @# %.0=Public    .1=Standard  .2=Sensitive .a#
19 13 10 Index Level:# @#
20 13 42 @# @#
21 14 10 Phonetic Matching:# @#
22 14 42 @# %.0=NO .1=YES.a#
23  0  0 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  12 ----------------------------- */
$ /* ************************************************************************ */

$set 33 preload                  $ /* mask 12                           */

1   2  5 (Mask 10)            DIRECTORY SYSTEM# @#
2   2 48 @# @#
3   6 10 Rule Number:# @#
4   6 45 @# @#
5   7 10 Superior Rule Number:# @#
6   7 45 @# @#
7   8 10 Acronyms of Naming Attributes:# @#
8   8 45 @# @#
9   9 10 Structural Object Class:# @#
10  9 45 @# @#
11 10 45 @# @#
12  0  0 @# @#

$ /* ***************************************************************** */
$ /* -----------------------------  M A S K  13 ---------------------- */
$ /* ***************************************************************** */

$set 34 preload                  $ /* mask 13                           */

1   2  5 (Mask 13)            DIRECTORY SYSTEM# @#
2   2 48 @# @#
3   6  8 OPERATIONS# @#
4   8 10 0 Exit# @#
5   9 10 1 Create Shadows and Shadowing Job# @#
6  10 10 2 Create Shadowing Job# @#
7  11 10 3 Remove Shadows and Shadowing Job# @#
8  12 10 4 Remove Shadowing Job# @#
9  13 10 5 Update Shadowing Job# @#
10 14 10 6 Display Shadowing Jobs# @#
11 15 10 7 Display Update Errors# @#
12 16 10 8 Remove Update Error# @#
13 23  5 Which operation ?# @#
14 23 23 @# @#


$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  14.1 -------------------- */
$ /* ****************************************************************** */

$set 35 preload                  $ /* mask 14.1                         */

1  2    5  (Mask 14)            DIRECTORY SYSTEM# @#
2  2    46 @# @#
3  4 	5  Object Name:# @#
4  4 	30 @# @#
5  5 	30 @# @#
6  7 	5  Object Interpretation:# @#
7  7 	30 @# %.0=SINGLE OBJECT .1=OBJECT AND ITS SUBORDINATES .a#
8  9 	5  Target DSA:# @#
9  9 	30 @# @#
10 10 	30 @# @#
11 12   5  Job:# @#
12 12   30  @# @#
13  0   0   @# @#



$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  14.2 -------------------- */
$ /* ****************************************************************** */

$set 36 preload                  $ /* mask 14.2                         */

1 14  5  Update Frequency:# @#
2 14  30 @# %.0=HIGH  .1=MEDIUM .2=LOW   .a#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  14a --------------------- */
$ /* ****************************************************************** */

$set 37 preload                  $ /* mask 14a                          */

1 16 5  @# @#
2 16 5  Update Times:# @#
3 16 30 @# @#
4 17 8  @# @#
5 18 8  @# @#
6 19 8  @# @#
7 20 8  @# @#
8 0  0  @# @#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  14b --------------------- */
$ /* ****************************************************************** */

$set 38 preload                  $ /* mask 14b                          */

1 17 8  @# @#
2 17 8  Hours[0-23]:# @#
3 17 30 @# @#
4 18 8  @# @#
5 19 8  @# @#
6 20 8  @# @#
7 0  0  @# @#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  14c --------------------- */
$ /* ****************************************************************** */

$set 39 preload                  $ /* mask 14c                          */

1 17 8  @# @#
2 18 8  @# @#
3 17 8  Day of Week:# @#
4 17 30 @# @#
5 18 8  Hours[0-23]:# @#
6 18 30 @# @#
7 19 8  @# @#
8 20 8  @# @#
9 0  0  @# @#


$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  16 ---------------------- */
$ /* ****************************************************************** */

$set 40 preload                  $ /* mask 16                           */

1   2  5 (Mask 16)            DIRECTORY SYSTEM# @#
2   2 48 @# @#
3   6  8 OPERATIONS# @#
4   8 10 0 Exit# @#
5   9 10 1 Save Subtree# @#
6  10 10 2 Append Subtree# @#
7  11 10 3 Copy Subtree# @#
8  12 10 4 Change Name / Move Subtree# @#
9  13 10 5 Delete Subtree# @#
10 14 10 6 Change Master# @#
11 15 10 7 Modify Subtree# @#
12 23  5 Which operation ?# @#
13 23 23 @# @#


$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  17a --------------------- */
$ /* ****************************************************************** */

$set 41 preload                  $ /* mask 17a                          */

1  2  5 (Mask 17a)           DIRECTORY SYSTEM# @#
2  2 48 @# @#
3  6 10 Source DSA:# @#
4  6 23 @# %.0=MASTER DSA(s).1=BIND DSA     .2=SPECIFIC DSA.a#
5  8 10 File name:# @#
6  8 23 @# @#
7  9 23 @# @#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  17b --------------------- */
$ /* ****************************************************************** */

$set 42 preload                  $ /* mask 17b                          */

1   2  5 (Mask 17b)           DIRECTORY SYSTEM# @#
2   2 48 @# @#
3   6 10 File name:# @#
4   6 38 @# @#
5   7 38 @# @#
6   9 10 Overwrite existing Entries:# @#
7   9 38 @# %.0=No .1=Yes.a#
8  11 10 New entries protected by:# @#
9  11 38 @# %.1=ACL of the new parent.0=Original ACL         .a#
10 13 10 Target DSA:# @#
11 13 38 @# %.1=BIND DSA    .2=SPECIFIC DSA.a#

$ /* ************************************************************************ */
$ /* ----------------------- M A S K (QUIT) --------------------------------- */
$ /* ************************************************************************ */

$set 43 preload                  $ /* mask QUIT                         */

1 23  5 @# @#
2 23  5 ERROR:# @#
3 23 12 @# @#
4 24 12 To continue press <CR> !# @#

$ /* ************************************************************************ */
$ /* ----------------------- M A S K (MESSAGE) ------------------------------ */
$ /* ************************************************************************ */

$set 44 preload                  $ /* mask MESSAGE                      */

1 23  5 @# @#
2 23 12 @# @#
3 24 12 To continue press <CR> !# @#

$ /* ************************************************************************ */
$ /* ------------------------ F R A M E   M A S K --------------------------- */
$ /* ************************************************************************ */

$set 45 preload                  $ /* FRAME mask                        */

1   1  2 @# @#
2   2  2 @# @#
3   2 79 @# @#
4   3  2 @# @#
5   4  2 @# @#
6   4 79 @# @#
7   5  2 @# @#
8   5 79 @# @#
9   6  2 @# @#
10  6 79 @# @#
11  7  2 @# @#
12  7 79 @# @#
13  8  2 @# @#
14  8 79 @# @#
15  9  2 @# @#
16  9 79 @# @#
17 10  2 @# @#
18 10 79 @# @#
19 11  2 @# @#
20 11 79 @# @#
21 12  2 @# @#
22 12 79 @# @#
23 13  2 @# @#
24 13 79 @# @#
25 14  2 @# @#
26 14 79 @# @#
27 15  2 @# @#
28 15 79 @# @#
29 16  2 @# @#
30 16 79 @# @#
31 17  2 @# @#
32 17 79 @# @#
33 18  2 @# @#
34 18 79 @# @#
35 19  2 @# @#
36 19 79 @# @#
37 20  2 @# @#
38 20 79 @# @#
39 21  2 @# @#
40 21 79 @# @#
41 22  2 @# @#

$set 46 preload 	$ /* mask18 (summary list mask) 

1   2  5 @# %.0=(Mask 18)            DIRECTORY SYSTEM.\
	      1=(Mask 6c)            DIRECTORY SYSTEM.\
	      2=(Mask 6d)            DIRECTORY SYSTEM.\
	      3=(Mask 9c)            DIRECTORY SYSTEM.\
	      4=(Mask 9b)            DIRECTORY SYSTEM.\
	      5=(Mask 9a)            DIRECTORY SYSTEM.a#
2   2 46 @# @#
3   50 0 @# @#
4   5  4 @# @#
5   6  4 @# @#
6   7  4 @# @#
7   8  4 @# @#
8   9  4 @# @#
9   10 4 @# @#
10  11 4 @# @#
11  12 4 @# @#
12  13 4 @# @#
13  14 4 @# @#
14  15 4 @# @#
15  16 4 @# @#
16  17 4 @# @#
17  18 4 @# @#
18  19 4 @# @#
19  20 4 @# @#
20  0  0 @# @#
21  0  0 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  19 ----------------------------- */
$ /* ************************************************************************ */

$set 47 preload 	$ /* mask19 

1   2  5 (Mask 6)             DIRECTORY SYSTEM# @#
2   2 48 @# @#
3   6  8 Object Name# @#
4   8 10 @# @#
5   8 36 @# @#
6   9 10 @# @#
7   9 36 @# @#
8  10 10 @# @#
9  10 36 @# @#
10 11 10 @# @#
11 11 36 @# @#
12 12 10 @# @#
13 12 36 @# @#
14 13 10 @# @#
15 13 36 @# @#
16 14 10 @# @#
17 14 36 @# @#
18 15 10 @# @#
19 15 36 @# @#
20 16 10 @# @#
21 16 36 @# @#
22 17 10 @# @#
23 17 36 @# @#
24 18 10 @# @#
25 18 36 @# @#
26 19 10 @# @#
27 19 36 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  20    -------------------------- */
$ /* ************************************************************************ */

$set 48 preload 	$ /* mask20 (error list mask) 

1   2  5 (Mask 20)          DIRECTORY SYSTEM# @#
2   2 52 @# @#
3   5  4 @# @#
4   6  4 @# @#
5   7  4 @# @#
6   8  4 @# @#
7   9  4 @# @#
8   10 4 @# @#
9   11 4 @# @#
10  12 4 @# @#
11  13 4 @# @#
12  14 4 @# @#
13  15 4 @# @#
14  16 4 @# @#
15  17 4 @# @#
16  18 4 @# @#
17  19 4 @# @#
18  20 4 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  21    -------------------------- */
$ /* ************************************************************************ */

$set 49 preload 	$ /* mask21 (dnscell) 

1   2  5 (Mask 21)          DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   6  8 @# @#
4   9 10 Namespace UUID:# @#
5   9 26 @# @#
6  12 10 Root dir UUID:# @#
7  12 26 @# @#
8  15 10 Root dir name:# @#
9  15 26 @# @#
10 20 10 Modification:# @#
11 20 26 @# %.4=Modify Value.3=Delete Value.2=Add Value   .a#
12  0  0 @# @#
13  0  0 @# @#
14  0  0 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  22    -------------------------- */
$ /* ************************************************************************ */

$set 50 preload 	$ /* mask22 (dnsreplica) 

1   2  5 (Mask 22)          DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   6  8 @# @#
4   8 10 Replica type :# @#
5   8 30 @# %.1=MASTER   .3=READ ONLY.a#
6  10 10 Clearinghouse UUID:# @#
7  10 30 @# @#
8  12 10 Clearinghouse name:# @#
9  12 30 @# @#
10 13 30 @# @#
11 14 10 Tower 1:# @#
12 14 30 @# @#
13 15 10 Tower 2:# @#
14 15 30 @# @#
15 16 10 Tower 3:# @#
16 16 30 @# @#
17 17 10 Tower 4:# @#
18 17 30 @# @#
19 18 10 Tower 5:# @#
20 18 30 @# @#
21 20 10 Modification:# @#
22 20 30 @# %.4=Modify Value.3=Delete Value.2=Add Value   .a#
23  0  0 @# @#
24  0  0 @# @#
25  0  0 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  23    -------------------------- */
$ /* ************************************************************************ */

$set 51 preload 	$ /* mask23 (ttxid) 

1   2  5 (Mask 23)             DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   6  8 @# @#
4   8 10 Terminal:# @#
5   8 24 @# @#
6  10 10 Control:# @#
7  10 24 @# @#
8  12 10 Graphic:# @#
9  12 24 @# @#
10 14 10 Miscel:# @#
11 14 24 @# @#
12 16 10 Page formats:# @#
13 16 24 @# @#
14 18 10 Private use:# @#
15 18 24 @# @#
16 20 10 Modification:# @#
17 20 24 @# %.4=Modify Value.3=Delete Value.2=Add Value   .a#
18  0  0 @# @#
19  0  0 @# @#
20  0  0 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  24    -------------------------- */
$ /* ************************************************************************ */

$set 52 preload 	$ /* mask24 (telex) 

1   2  5 (Mask 24)             DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   6  8 @# @#
4   9 10 Telex Number:# @#
5   9 24 @# @#
6  12 10 Country Code:# @#
7  12 24 @# @#
8  15 10 Answerback:# @#
9  15 24 @# @#
10 20 10 Modification:# @#
11 20 24 @# %.4=Modify Value.3=Delete Value.2=Add Value   .a#
12  0  0 @# @#
13  0  0 @# @#
14  0  0 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  25    -------------------------- */
$ /* ************************************************************************ */

$set 53 preload 	$ /* mask25 (Postal-Address) 

1   2  5 (Mask 25)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   5 10 @# @#
4   8 12 Part1:# @#
5   8 19 @# @#
6  10 12 Part2:# @#
7  10 19 @# @#
8  12 12 Part3:# @#
9  12 19 @# @#
10 14 12 Part4:# @#
11 14 19 @# @#
12 16 12 Part5:# @#
13 16 19 @# @#
14 18 12 Part6:# @#
15 18 19 @# @#
16 20 12 Modification:# @#
17 20 26 @# %.4=Modify Value.3=Delete Value.2=Add Value   .a#
18  0  0 @# @#
19  0  0 @# @#
20  0  0 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  26    -------------------------- */
$ /* ************************************************************************ */

$set 54 preload 	$ /* mask26 (Fax-Number) 

1   2  5 (Mask 26)             DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   6  8 @# @#
4   9 10 Telephone number:# @#
5   9 28 @# @#
6  11 10 A3 width:# @#
7  11 28 @# %.0=NO .1=YES.a#
8  12 10 B4 length:# @#
9  12 28 @# %.0=NO .1=YES.a#
10 13 10 B4 width:# @#
11 13 28 @# %.0=NO .1=YES.a#
12 14 10 Fine resolution:# @#
13 14 28 @# %.0=NO .1=YES.a#
14 15 10 Two dimensional:# @#
15 15 28 @# %.0=NO .1=YES.a#
16 16 10 Uncompressed:# @#
17 16 28 @# %.0=NO .1=YES.a#
18 17 10 Unlimited length:# @#
19 17 28 @# %.0=NO .1=YES.a#
20 20 10 Modification:# @#
21 20 28 @# %.4=Modify Value.3=Delete Value.2=Add Value   .a#
22  0  0 @# @#
23  0  0 @# @#
24  0  0 @# @#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  14d --------------------- */
$ /* ****************************************************************** */

$set 55 preload 	$ /* mask 14d 

1 17 8  @# @#
2 18 8  @# @#
3 19 8  @# @#
4 20 8  @# @#
5 17 8  Day of Week:# @#
6 17 30 @# @#
7 18 8  Hours[0-23]:# @#
8 18 30 @# @#
9 19 8  Day of Week:# @#
10 19 30 @# @#
11 20 8  Hours[0-23]:# @#
12 20 30 @# @#
13 0  0  @# @#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  14.3 -------------------- */
$ /* ****************************************************************** */

$set 56 preload 	$ /* mask 15 

1   2  5 (Mask 15)            DIRECTORY SYSTEM# @#
2   2 48 @# @#
3   6  5 Object Name:# @#
4   6 30 @# @#
5   7 30 @# @#
6   8  5 Target DSA:# @#
7   8 30 @# @#
8   9 30 @# @#
9  10  5 Operation:# @#
10 10 30 @# %.6=ADD       .7=REMOVE    .8=MODIFY    .9=MODIFY RDN.a#
11 11  5 Error Time:# @#
12 11 30 @# @#
13 12  5 Error Count:# @#
14 12 30 @# @#
15 13  5 Error Code:# @#
16 13 30 @# @#
17 15  5 Remove Update Error:# @#
18 15 30 @# %.0=NO .1=YES.a#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  27 ---------------------- */
$ /* ****************************************************************** */

$set 57 preload 	$ /* mask 27 (OR Address Types) 

1   2  5 (Mask 27)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   4  5 @# @#
4   5 30 O/R Address Type:# @#
5   5 48 @# %.0=Mnemonic           .1=Numeric            .2=Structured postal  .3=Unstructured postal.4=Terminal           .a#
6   0  0 @# @#
7   0  0 @# @#
8   0  0 @# @#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  28 ---------------------- */
$ /* ****************************************************************** */

$set 58 preload 	$ /* mask 28 (Mnemonic O/R Address) 

1   2  5 (Mask 28)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   4  5 @# @#
4   5 30 O/R Address Type: # @#
5   5 48 Mnemonic           # @#
6   7  3 Country Name:# @#
7   7 17 @# @#
8   7 22 ADMD Name:# @#
9   7 33 @# @#
10  7 51 PRMD Name:# @#
11  7 62 @# @#
12  8  3 Org. Name:# @#
13  8 14 @# @#
14  9  3 OU1:# @#
15  9  8 @# @#
16  9 41 OU2:# @#
17  9 46 @# @#
18 10  3 OU3:# @#
19 10  8 @# @#
20 10 41 OU4:# @#
21 10 46 @# @#
22 11  3 Com. Name:# @#
23 11 14 @# @#
24 12  3 Given Name:# @#
25 12 15 @# @#
26 12 40 Initials:# @#
27 12 50 @# @#
28 13  3 Surname:# @#
29 13 12 @# @#
30 13 54 Generation:# @#
31 13 66 @# @#
32 14  3 Domain Type1:# @#
33 14 17 @# @#
34 14 40 Domain Type2:# @#
35 14 54 @# @#
36 15  3 Domain Type3:# @#
37 15 17 @# @#
38 15 40 Domain Type4:# @#
39 15 54 @# @#
40 16  3 Domain Value1:# @#
41 16 18 @# @#
42 17  3 Domain Value2:# @#
43 17 18 @# @#
44 18  3 Domain Value3:# @#
45 18 18 @# @#
46 19  3 Domain Value4:# @#
47 19 18 @# @#
48 21 10 Modification:# @#
49 21 24 @# %.4=Modify Value.3=Delete Value.2=Add Value   .a#
50  0  0 @# @#
51  0  0 @# @#
52  0  0 @# @#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  29 ---------------------- */
$ /* ****************************************************************** */

$set 59 preload 	$ /* mask 29 (Numeric O/R Address) 

1   2  5 (Mask 29)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   4  5 @# @#
4   5 30 O/R Address Type: # @#
5   5 48 Numeric            # @#
6   7  3 Country Name:# @#
7   7 17 @# @#
8   7 22 ADMD Name:# @#
9   7 33 @# @#
10  7 51 PRMD Name:# @#
11  7 62 @# @#
12  8  3 Numeric User Identifier:# @#
13  8 28 @# @#
14  9  3 Domain Type1:# @#
15  9 17 @# @#
16  9 40 Domain Type2:# @#
17  9 54 @# @#
18 10  3 Domain Type3:# @#
19 10 17 @# @#
20 10 40 Domain Type4:# @#
21 10 54 @# @#
22 11  3 Domain Value1:# @#
23 11 18 @# @#
24 12  3 Domain Value2:# @#
25 12 18 @# @#
26 13  3 Domain Value3:# @#
27 13 18 @# @#
28 14  3 Domain Value4:# @#
29 14 18 @# @#
30 21 10 Modification:# @#
31 21 24 @# %.4=Modify Value.3=Delete Value.2=Add Value   .a#
32  0  0 @# @#
33  0  0 @# @#
34  0  0 @# @#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  30 ---------------------- */
$ /* ****************************************************************** */

$set 60 preload 	$ /* mask 30 (Structured postal O/R Address) 

1   2  5 (Mask 30)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   4  5 @# @#
4   5 30 O/R Address Type: # @#
5   5 48 Structured postal  # @#
6   7  3 Country Name:# @#
7   7 17 @# @#
8   7 22 ADMD Name:# @#
9   7 33 @# @#
10  7 51 PRMD Name:# @#
11  7 62 @# @#
12  8  3 Postal Country Name:# @#
13  8 24 @# @#
14  8 40 Postal Code:# @#
15  8 53 @# @#
16  9  3 Postal Address Details:# @#
17  9 36 @# @#
18 10  3 Postal Delivery Point Name:# @#
19 10 36 @# @#
20 11  3 Postal Delivery System Name:# @#
21 11 36 @# @#
22 12  3 Postal General Delivery Address:# @#
23 12 36 @# @#
24 13  3 Postal Locale:# @#
25 13 36 @# @#
26 14  3 Postal Office Box Number:# @#
27 14 36 @# @#
28 15  3 Postal Office Name:# @#
29 15 36 @# @#
30 16  3 Postal Office Number:# @#
31 16 36 @# @#
32 17  3 Postal Organisation Name:# @#
33 17 36 @# @#
34 18  3 Postal Patron Details:# @#
35 18 36 @# @#
36 19  3 Postal Patron Name:# @#
37 19 36 @# @#
38 20  3 Postal Street Address:# @#
39 20 36 @# @#
40 21 10 Modification:# @#
41 21 24 @# %.4=Modify Value.3=Delete Value.2=Add Value   .a#
42  0  0 @# @#
43  0  0 @# @#
44  0  0 @# @#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  31 ---------------------- */
$ /* ****************************************************************** */

$set 61 preload 	$ /* mask 31 (Unstructured postal O/R Address) 

1   2  5 (Mask 31)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   4  5 @# @#
4   5 30 O/R Address Type: # @#
5   5 48 Unstructured postal# @#
6   7  3 Country Name:# @#
7   7 17 @# @#
8   7 22 ADMD Name:# @#
9   7 33 @# @#
10  7 51 PRMD Name:# @#
11  7 62 @# @#
12  8  3 Postal Country Name:# @#
13  8 24 @# @#
14  8 40 Postal Code:# @#
15  8 53 @# @#
16  9  3 Postal Address in Full:# @#
17  9 27 @# @#
18 10  3 @# @#
19 11  3 @# @#
20 12  3 Postal Address in Lines (Part 1):# @#
21 12 37 @# @#
22 13  3 Postal Address in Lines (Part 2):# @#
23 13 37 @# @#
24 14  3 Postal Address in Lines (Part 3):# @#
25 14 37 @# @#
26 15  3 Postal Address in Lines (Part 4):# @#
27 15 37 @# @#
28 16  3 Postal Address in Lines (Part 5):# @#
29 16 37 @# @#
30 17  3 Postal Address in Lines (Part 6):# @#
31 17 37 @# @#
32 18  3 Postal Delivery System Name:# @#
33 18 32 @# @#
34 21 10 Modification:# @#
35 21 24 @#  %.4=Modify Value.3=Delete Value.2=Add Value   .a#
36  0  0 @# @#
37  0  0 @# @#
38  0  0 @# @#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  32 ---------------------- */
$ /* ****************************************************************** */

$set 62 preload 	$ /* mask 32 (Terminal O/R Address) 

1   2  5 (Mask 32)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   4  5 @# @#
4   5 30 O/R Address Type: # @#
5   5 48 Terminal           # @#
6   7  3 ISDN Number:# @#
7   7 16 @# @#
8   8  3 ISDN Subaddress:# @#
9   8 20 @# @#
10  9  3 Presentation Address:# @#
11  9 25 @# %.0=NO .1=YES.a#
12 10  3 X121 Address:# @#
13 10 17 @# @#
14 11  3 Country Name:# @#
15 11 17 @# @#
16 11 22 ADMD Name:# @#
17 11 33 @# @#
18 11 51 PRMD Name:# @#
19 11 62 @# @#
20 12  3 Terminal Identifier:# @#
21 12 24 @# @#
22 13  3 Terminal Type:# @#
23 13 18 @# %.0=Not used    .3=TELEX       .4=TELETEX     .5=G3 FACSIMILE.6=G4 FACSIMILE.7=IA5 TERMINAL.8=VIDEOTEX    .a#
24 14  3 Domain Type1:# @#
25 14 17 @# @#
26 14 40 Domain Type2:# @#
27 14 54 @# @#
28 15  3 Domain Type3:# @#
29 15 17 @# @#
30 15 40 Domain Type4:# @#
31 15 54 @# @#
32 16  3 Domain Value1:# @#
33 16 18 @# @#
34 17  3 Domain Value2:# @#
35 17 18 @# @#
36 18  3 Domain Value3:# @#
37 18 18 @# @#
38 19  3 Domain Value4:# @#
39 19 18 @# @#
40 21 10 Modification:# @#
41 21 24 @# %.4=Modify Value.3=Delete Value.2=Add Value   .a#
42  0  0 @# @#
43  0  0 @# @#
44  0  0 @# @#


$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  33 ---------------------- */
$ /* ****************************************************************** */

$set 63 preload 	$ /* mask 33 (DL permission Types) 

1   2  5 (Mask 33)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   4  5 @# @#
4   5 30 DL Submit Permission Type: # @#
5   5 57 @# %.0=Individual     .1=Member Of DL   .2=Pattern Match  .3=Member Of Group.a#
6   0  0 @# @#
7   0  0 @# @#
8   0  0 @# @#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  34 ---------------------- */
$ /* ****************************************************************** */

$set 64 preload 	$ /* mask 34 (OR Name) 

1   2  5 (Mask 34)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   4  5 @# @#
4   5 30 DL Submit Permission Type: # @#
5   5 57 @# %.0=Individual     .1=Member Of DL   .2=Pattern Match  .a#
6   8 10 Directory Name: # @#
7   8 26 @# @#
8   9 26 @# @#
9  11 10 O/R Address: # @#
10 11 23 @# %.0=NO .1=YES.a#
11 21 10 Modification: # @#
12 21 24 @# %.4=Modify Value.3=Delete Value.2=Add Value   .a#
13  0  0 @# @#
14  0  0 @# @#
15  0  0 @# @#

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  35 ---------------------- */
$ /* ****************************************************************** */

$set 65 preload 	$ /* mask 35 (Member Of Group) 

1   2  5 (Mask 35)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   4  5 @# @#
4   5 30 DL Submit Permission Type: Member Of Group# @#
5   8 10 Member Of Group: # @#
6   8 27 @# @#
7   9 27 @# @#
8  21 10 Modification: # @#
9  21 24 @# %.4=Modify Value.3=Delete Value.2=Add Value   .a#
10  0  0 @# @#
11  0  0 @# @#
12  0  0 @# @#

$ /* ****************************************************************** */
$ /* ------------------ masks for security package follow ------------- */
$ /* ****************************************************************** */

$ /* ****************************************************************** */
$ /* ------------------------------  M A S K  36 ---------------------- */
$ /* ----------------------- Certificate ------------------------------ */
$ /* ****************************************************************** */

$set 66 preload 	$ /* mask 36 (Certificate) 

1   2  5 (Mask 36)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   4 10 @# @#
4   4 65 @# %.0=Forward.1=Reverse.a#
5   6  5 Version:# @#
6   6 14 @# @#
7   6 50 Serial Number:# @#
8   6 65 @# @#
9   7  5 Signature# @#
10  8  6 Algorithm:# @#
11  8 17 @# %.0=rsa           .1=sqMod-n       .2=sqMod-nWithRSA.a#
12  8 50 Parameter:# @#
13  8 61 @# @#
14  9  5 Issuer:# @#
15  9 25 @# @#
16 10 25 @# @#
17 11  5 Validity# @#
18 12  6 Not Before# @#
19 12 18 Year:# @#
20 12 24 @# @#
21 12 27 Month:# @#
22 12 34 @# @#
23 12 37 Day:# @#
24 12 42 @# @#
25 12 45 Hours:# @#
26 12 52 @# @#
27 12 55 Minutes:# @#
28 12 64 @# @#
29 12 67 Seconds:# @#
30 12 76 @# @#
31 13  6 Not After# @#
32 13 18 Year:# @#
33 13 24 @# @#
34 13 27 Month:# @#
35 13 34 @# @#
36 13 37 Day:# @#
37 13 42 @# @#
38 13 45 Hours:# @#
39 13 52 @# @#
40 13 55 Minutes:# @#
41 13 64 @# @#
42 13 67 Seconds:# @#
43 13 76 @# @#
44 14  5 Subject:# @#
45 14 25 @# @#
46 15 25 @# @#
47 16  5 Subject Public Key Info# @#
48 17  6 Algorithm Identifier# @#
49 18  7 Algorithm:# @#
50 18 18 @# %.0=rsa           .1=sqMod-n       .2=sqMod-nWithRSA.a#
51 18 50 Parameter:# @#
52 18 61 @# @#
53 21 10 Modification:# @#
54 21 24 @# %.3=Delete Value.a#
55  0  0 @# @#
56  0  0 @# @#
57  0  0 @# @#

$ /* ****************************************************************** */
$ /* ----------------------  M A S K  37 ------------------------------ */
$ /* ----------------------- Certificate Pair ------------------------- */
$ /* ****************************************************************** */

$set 67 preload 	$ /* mask 37 (Certificate Pair) 

1   2  5 (Mask 37)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   4 10 @# @#
4   6 15 Forward Certificate:# @#
5   6 40 @# %.0=NO .1=YES.a#
6   8 15 Reverse Certificate:# @#
7   8 40 @# %.0=NO .1=YES.a#
8  21 10 Modification:# @#
9  21 24 @# %.3=Delete Value.a#
10  0  0 @# @#
11  0  0 @# @#
12  0  0 @# @#

$ /* ****************************************************************** */
$ /* ----------------------  M A S K  38a ----------------------------- */
$ /* ----------------------- Certificate List ------------------------- */
$ /* ****************************************************************** */

$set 68 preload 	$ /* mask 38a (Certificate List) 

1    2  5 (Mask 38a)           DIRECTORY SYSTEM# @#
2    2 56 @# @#
3    4 10 @# @#
4    6  5 Signature# @#
5    6 30 Algorithm:# @#
6    6 41 @# %.0=rsa           .1=sqMod-n       .2=sqMod-nWithRSA.a#
7    7 30 Parameter:# @#
8    7 41 @# @#
9    9  5 Issuer:# @#
10   9 30 @# @#
11  10 30 @# @#
12  12  5 Last Update# @#
13  12 18 Year:# @#
14  12 24 @# @#
15  12 27 Month:# @#
16  12 34 @# @#
17  12 37 Day:# @#
18  12 42 @# @#
19  12 45 Hours:# @#
20  12 52 @# @#
21  12 55 Minutes:# @#
22  12 64 @# @#
23  12 67 Seconds:# @#
24  12 76 @# @#
25  14  5 Revoked Certificates:# @#
26  14 30 @# %.0=NO .1=YES.a#
27  20 10 Modification:# @#
28  20 24 @# %.3=Delete Value.a#
29  0  0 @# @#
30  0  0 @# @#
31  0  0 @# @#
32  0  0 @# @#

$ /* ****************************************************************** */
$ /* ----------------------  M A S K  38b ----------------------------- */
$ /* ----------------------- Revoked Certificate ---------------------- */
$ /* ****************************************************************** */

$set 69 preload 	$ /* mask 38 (Revoked Certificate) 

1    2  5 (Mask 38b)           DIRECTORY SYSTEM# @#
2    2 56 @# @#
3    4 10 @# @#
4    4 43 Revoked Certificate# @#
5    6  5 Signature# @#
6    6 30 Algorithm:# @#
7    6 41 @# %.0=rsa           .1=sqMod-n       .2=sqMod-nWithRSA.a#
8    7 30 Parameter:# @#
9    7 41 @# @#
10   9  5 Issuer:# @#
11   9 30 @# @#
12  10 30 @# @#
13  12  5 User Certificate:# @#
14  12 30 @# @#
15  14  5 Revocation Date# @#
16  15 18 Year:# @#
17  15 24 @# @#
18  15 27 Month:# @#
19  15 34 @# @#
20  15 37 Day:# @#
21  15 42 @# @#
22  15 45 Hours:# @#
23  15 52 @# @#
24  15 55 Minutes:# @#
25  15 64 @# @#
26  15 67 Seconds:# @#
27  15 76 @# @#
28  17  5 Other revoked Certificates:# @#
29  17 33 @# %.0=NO .1=YES.a#
30  20 10 Modification:# @#
31  20 24 @# %.3=Delete Value.a#
32  0  0 @# @#
33  0  0 @# @#
34  0  0 @# @#
35  0  0 @# @#

$ /* ****************************************************************** */
$ /* ----------  M A S K  18b ----------------------------------------- */
$ /* ----------- To read attribute names from batch file -------------- */
$ /* ****************************************************************** */

$set 70 preload 	$ /* mask 18b 

1   2  5 (Mask 6d)            DIRECTORY SYSTEM# @#
2   2 46 @# @#
3   5  4 @# @#
4   6  4 @# @#
5   7  4 @# @#
6   8  4 @# @#
7   9  4 @# @#
8  20 60 More:# @#
9  20 65 @# @#

$set 71 preload 	$ mask 39 (DME)

1   2  5 (Mask 39)            DIRECTORY SYSTEM# @#
2   2 56 @# @#
3   5  5 DME NMO Alternate Address:# @#
4   6  5 Address:# @#
5   6 15 @# @#
6   7  5 @# @#
7   8  5 @# @#
8   9  5 @# @#
9  10  5 @# @#
10 11  5 Protocol:# @#
11 12  9 Object Id. 1:# @#
12 12 30 @# @#
13 13  9 Object Id. 2:# @#
14 13 30 @# @#
15 14  9 Object Id. 3:# @#
16 14 30 @# @#
17 15  9 Object Id. 4:# @#
18 15 30 @# @#
19 16  9 Object Id. 5:# @#
20 16 30 @# @#
21 17  9 Object Id. 6:# @#
22 17 30 @# @#
23 18  9 Object Id. 7:# @#
24 18 30 @# @#
25 19  9 Object Id. 8:# @#
26 19 30 @# @#
27 20  9 Object Id. 9:# @#
28 20 30 @# @#
29 21  9 Object Id.10:# @#
30 21 30 @# @#
31  0  0 @# @#
32  0  0 @# @#
33  0  0 @# @#
34  0  0 @# @#
35  0  0 @# @#
36  0  0 @# @#

$ /* ************************************************************************ */
$ /* -----------------------------  M A S K  1 ------------------------------ */
$ /* ************************************************************************ */

$set 72 preload                 $ /* mask 1 (security login mask)             */

1   2  5 (Mask 1)             DIRECTORY SYSTEM                           Logon# @#
2   4 59 Directory ID:# @#
3   4 73 @# @#
4   5  7 Authentication mechanism to be used :# @#
5   5 45 @# %.0=Anonymous             .2=Simple Unprotected    .5=DCE Authentication    .a#
6  21 10 Options: Logon to the DUA Cache# @#

$             /* error messages for error lists                  */
$set 73 preload 	 

1 Non overwritten objects
2 Overwritten objects
3 Unchanged objects
4 Objects not removed


$             /* attribute syntaxes for Schema administration */

$set 74 preload 	 

1  %.4=Case Ignore String                  .\
 10008=Case Ignore IA5 String              .\
     3=Case Exact String                   .\
    10=Octet String                        .\
     1=Distinguished Name                  .\
  1010=Country Name Syntax                 .\
  1011=Password Syntax                     .&
2 10000=ACL Syntax                          .\
     2=Object Identifier Syntax            .\
     5=Printable String                    .\
     6=Numeric String                      .\
     7=Case Ignore List                    .\
     8=Boolean Syntax                      .&
3    9=Integer Syntax                      .\
    11=UTC Time                            .\
    12=Telephone Number Syntax             .\
  1000=Search Guide Syntax                 .\
  1001=Postal Address Syntax               .&
4 1002=Telex Number Syntax                 .\
  1003=TTX-ID Syntax                       .\
  1004=Fax Number Syntax                   .\
  1005=Preferred Delivery Method Syntax    .\
  1006=Presentation Address                .&
5  100=MHS DL Submit Permission Syntax     .\
   101=MHS O/R Address Syntax              .\
   102=MHS O/R Name Syntax                 .\
   103=MHS Preferred Delivery Method Syntax.\
 10003=ASN1 Syntax                         .&
6 1007=Certificate Syntax                  .\
  1008=Certificate Pair Syntax             .\
  1009=Certificate List Syntax             .\
     0=Any Syntax                          .a

$set 75 preload 	 
1 TRUE
2 FALSE

$set 76 preload 	 
1 %.0=OBJECT .1=SUBTREE.3=_______.a
