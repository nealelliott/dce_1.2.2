$
$ @OSF_COPYRIGHT@
$ COPYRIGHT NOTICE
$ Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
$ ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
$ src directory for the full copyright text.
$
$ HISTORY
$ $Log: d20admhelp.msg,v $
$ Revision 1.1.575.2  1996/03/09  23:14:04  marty
$ 	Update OSF copyright year
$ 	[1996/03/09  22:31:25  marty]
$
$ Revision 1.1.575.1  1995/12/08  16:04:23  root
$ 	Submit OSF/DCE 1.2.1
$ 	[1995/12/08  15:15:41  root]
$ 
$ Revision 1.1.573.5  1994/07/06  15:07:43  marrek
$ 	July 1994 code submission.
$ 	[1994/07/04  13:17:28  marrek]
$ 
$ Revision 1.1.573.4  1994/05/10  15:56:52  marrek
$ 	April 1994 submission.
$ 	[1994/04/29  10:52:43  marrek]
$ 
$ Revision 1.1.573.3  1994/03/23  15:19:06  keutel
$ 	March 1994 code drop
$ 	[1994/03/21  16:32:48  keutel]
$ 
$ Revision 1.1.573.2  1994/02/22  17:41:17  marrek
$ 	Code drop for dce1.1.
$ 	[1994/02/08  10:56:45  marrek]
$ 
$ Revision 1.1.573.1  1994/01/13  09:18:10  marrek
$ 	Help masks rewritten and redesigned.
$ 	[1994/01/12  11:14:13  marrek]
$ 
$ Revision 1.1.571.2  1993/10/14  17:35:59  keutel
$ 	October 1993 code drop
$ 	[1993/10/14  17:34:35  keutel]
$ 
$ Revision 1.1.571.1  1993/10/13  17:33:21  keutel
$ 	October 1993 code drop
$ 	[1993/10/11  17:00:33  keutel]
$ 
$ 	October 1993 code drop
$ 	[1993/10/11  09:01:31  keutel]
$ 
$ Revision 1.1.4.2  1993/08/11  15:05:23  marrek
$ 	July 1993 code drop.
$ 	[1993/08/02  13:01:33  marrek]
$ 
$ Revision 1.1.2.4  1993/01/25  10:13:46  marrek
$ 	Correct comments
$ 	[1993/01/15  12:36:23  marrek]
$ 
$ 	Add comment leader
$ 	[1993/01/14  15:19:35  marrek]
$ 
$ $EndLog$
$
$ $RCSfile: d20admhelp.msg,v $ $Revision: 1.1.575.2 $ $Date: 1996/03/09 23:14:04 $
$
$ 
$ /* *********************************************************************** */
$ /*     Help mask1: Logon mask (d20_1omask)                                 */
$ /* *********************************************************************** */

$set 1 preload

1  (Help mask)                       DIRECTORY SYSTEM
2 
3 Enter information required to bind to the directory. When you run the
4 gdscacheadm process, only the directory ID is required (the intention to
5 bind to the cache is implied).
6 
7 User Credentials (Password and the DN listed below it):
8 
9 User Credentials appear when the Simple authentication mechanism is used.
10 
11   Must correspond to an object (which has password attribute) stored
12   in the directory. If 'A' is this object, the only data that can be
13   managed in any object is that for which 'A' is authorized based on
14   the ACLs of the object on which operation is being performed.
15 
16 
17 
18 
19 
20 To continue press <CR> !



$ /* *********************************************************************** */
$ /*     Help mask2: Logon mask (d20_1omask)                                 */
$ /* *********************************************************************** */

$set 2 preload

1 (Help mask)
2 
3 If either Password or DN or both are not specified, then an anonymous
4 bind (bind with no credentials) is made to directory.
5 
6 Public attributes can always be read independent of whether it is
7 bind with credentials or an anonymous bind.
8 
9 Select one of following options by toggling the space bar:
10 
11  Logon to the Default DSA    Binds to the default DSAs defined in the
12                              cache in the order DSAs have been defined.
13                              If the bind to the first default DSA fails,
14                              an bind attempt is made to bind to the
15                              second default DSA, and so on.
16 
17  Logon to a Specific DSA     Binds to a Specific DSA (DSA name is
18                              entered in the next mask).
19 
20 To continue press <CR> !


$ /* *********************************************************************** */
$ /*     Help mask3: Logon mask (d20_1omask)                                 */
$ /* *********************************************************************** */

$set 3 preload

1 (Help mask)                       DIRECTORY SYSTEM
2 
3  Logon to the DUA Cache      Binds to the DUA Cache. All subsequent
4                              directory operations are performed on the
5                              cache database. User-Credentials are
6                              ignored.
7 
8  Changing Name Structure     Changes the logon name structure in the
9                              current mask (the default logon name
10                              structure has 4 name parts).
11 
12  Default values for the gdsditadm process are:
13 
14     directory ID 1        anonymus bind      bind to default DSA
15 
16  Default values for the gdscacheadm process are:
17 
18     directory ID 1        bind to cache is implied
19 
20 To exit help press <CR> !



$ /* *********************************************************************** */
$ /*     Help mask1: DSA name mask (d20_2omask)                              */
$ /* *********************************************************************** */

$set 4 preload

1 (Help mask)                     DIRECTORY SYSTEM
2 
3 Specifies the DSA name
4 
5 Select one of the following options by toggling the space bar:
6 
7   None (default)              Keep the current DSA name structure
8 
9   Changing Name Structure     Change the DSA name structure (the default
10                             DSA name structure has 5 name parts)
11 
12 DSA name is one of the following depending on the operation performed:
13 
14   Specific DSA to which a bind is made (Logon to a Specific DSA)
15 
16   Specific DSA where shadows are to be created or updated
17 
18   Specific DSA where subtree is to be saved (Save Subtree)
19 
20 To continue press <CR> !



$ /* *********************************************************************** */
$ /*    Help mask2: DSA name mask (d20_2omask)                               */
$ /* *********************************************************************** */

$set 5 preload

1 (Help mask)                     DIRECTORY SYSTEM
2 
3 Specific DSA where subtree is to be appended (Append Subtree)
4 
5 Specific DSA where subtree is to be deleted (Delete Subtree)
6 
7 Source/target DSA from/to which subtree should be copied (Copy Subtree).
8 When the mask appears for the first time, it is the source DSA; when
9 the mask appears for the second time, it is the target DSA.
10 
11 
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask: Admin function mask (d20_3omask)                          */
$ /* *********************************************************************** */

$set 6 preload

1 (Help mask)                       DIRECTORY SYSTEM
2 
3 The Administration functions which are available vary depending on the
4 logon options you selected in the login mask.
5 
6 The Cache Update option is available only when you have selected bind to
7 cache in the login mask.
8 
9 Schema administration, shadow administration, and tree administration
10 options are available only when you have selected bind to DSA
11 (Default/Specific) in the login mask. The Current DSA field contains
12 the name of the bound DSA.
13 
14 More detailed help about object administration (Function 1), Cache Update
15 (Function 2), schema administration (Function 2), shadow administration
16 (Function 3) and tree administration (Function 4) can be found in the
17 help masks for subsequent masks.
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask: Object admin mask (d20_4omask)                           */
$ /* *********************************************************************** */

$set 7 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Select one of the following entry types when binding to a default or
4 specific DSA by toggling the space bar (entry types are not valid for
5 Display Objects operations -- Options 3 and 4):
6 
7     MASTER           Maintains the master entries of the objects
8                      (default).
9 
10     SHADOW           Maintains the shadow entries of the objects. It is
11                      recommended that you use Shadow Administration to
12                      administer shadows when working with other GDS
13                      DSAs. When working with non-GDS DSAs, this is the
14                      only way shadows can be administered.
15 
16 Move the cursor between options and entry type using <Cur-Up> or
17 <Cur-Down> keys.
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask: Special DSA mask (d20_4aomask)                           */
$ /* *********************************************************************** */

$set 8 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Displays the names of up to four local and default DSAs.
4 
5 Page down by pressing <CR> or <Menu> keys to display additional default
6 DSAs names
7 
8 
9 
10 
11 
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask1: Structure rule mask (d20_5omask)                        */
$ /* *********************************************************************** */

$set 9 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask displays the name structure of every structure rule in the
4 administration structure rule table (SRT).
5 
6 The left column displays the naming attribute(s) of a specific structure
7 rule number. The right column shows the name structure (tree structure)
8 of the structure rule.
9 
10 For example if one of the lines in this mask looks like:
11       06 Common-Name                  02-03-04-06
12            Org.-Unit-Name
13 
14 The parent of rule 06 is rule number 04. The parent of rule number 04
15 is rule number 03. The parent of rule number 03 is rule number 02.
16 Naming attributes corresponding to parent rule numbers 04, 03, and 02
17 are displayed on other lines of the mask.
18 
19 Enter the number of the structure rule to be processed.
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask2: Structure rule mask (d20_5omask)                        */
$ /* *********************************************************************** */

$set 10 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use <SCROLL DOWN> to display additional structure rules
4 
5 When selecting logon name structure:
6 
7      Select a structure rule for the object class which has the
8      User-Password attribute
9 
10 When selecting the name structure for the DSA name:
11 
12      Select a structure rule for the object class which has the
13      Presentation-Address attributes
14 
15 If this mask is accepted that means the selected structure rule
16 satisfies the condition.
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask1: Object name mask (d20_6omask)                           */
$ /* *********************************************************************** */

$set 11 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to enter an object name.
4 
5 The names of naming attributes (stored in the AT) are displayed in the
6 left column. Enter the DN of the object in the right column.
7 
8 The Structural Object Class and Auxiliary Object Class fields are
9 displayed when the operation is Add Object, Display Objects, Add
10 Attributes, Delete Attributes, or Modify Attributes.
11 
12 To enter the Structural Object Class, select the Auxiliary Object Class
13 by pressing the space bar:
14 
15      YES     Selects auxiliary object classes from the list of
16              auxiliary object classes belonging to selected structural
17              object class.
18      NO      No auxiliary object class can be selected
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask2: Object name mask (d20_6omask)                           */
$ /* *********************************************************************** */

$set 12 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 If the operation is Display Objects, a wildcard (*) can be used for each
4 name part. Wildcards in name parts, the structural object class, and
5 auxiliary object class are used to generate a search request with a
6 filter.
7 
8 Wildcards can be used in name parts as follows:
9   -- As a complete naming attribute value
10   -- At the beginning of a name part, for example *ith
11   -- At the end of a name part, for example, Smi*
12   -- At the beginning and end of a name part, for example, *mi*
13   -- In the middle of a name part, asterisks are treated as normal
14      characters, for example B**th
15   -- A continuous sequence of only '*'s is same as a single '*'.
16 
17 Enter a wildcard in the structural object class field to find objects
18 which can be of any object class or a specific object class with any
19 auxiliary object class value.
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask3: Object name mask (d20_6omask)                           */
$ /* *********************************************************************** */

$set 13 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 In all other operations no wild cards can be entered in the name parts
4 and in the structural object class field.
5 
6 In case of 'Tree Administrattion' and 'Shadow Administration' functions,
7 there is one additional field at the top 'Object Interpretation'.
8 Select one of the following values by pressing the space bar:
9 
10      SINGLE OBJECT           Assigns access to a single object
11 
12      OBJECT AND ITS SUBORDINATES
13                              Assigns access to a subtree (including
14                              subtree root)
15 
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask: Type of ACL mask (d20_6aomask)                           */
$ /* *********************************************************************** */

$set 14 preload

1 (Help mask)           DIRECTORY SYSTEM
2  
3 Select access rights to be changed for object name entered in a
4 previous mask by pressing the space bar:
5 
6      NO      Leaves access right at current value
7 
8      YES     Marks access right to be changed in Mask 6b (for example,
9              select YES for Read Standard to modify access rights to
10              read standard attributes of object name)
11                  
12 Enter <Menu>/<CR> to display Mask 6b (Mask 6b appears for each access
13 right marked with YES).
14 
15 Changing one or more types of access rights for an object changes its
16 ACL. The ACL attribute is public. Enter your DN in the ACL of object
17 whose ACL is being changed with the right to modify public attributes.
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask1: ACL mask (d20_6bomask)                                  */
$ /* *********************************************************************** */

$set 15 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask displays and inputs up to four DNs for the access rights
4 selected in Mask 6a. The access right field will be one of the
5 following:
6 
7     - Modify Public
8     - Read Standard
9     - Modify Standard
10     - Read Sensitive
11     - Modify Sensitive
12 
13 Enter a maximum of four DNs for the access right in the following
14 format:
15 
16   /acronym=value, acronym=value .../acronym=value, acronym=value .../...
17      (with no leading spaces)
18 For example: /C=de/O=Smith Ltd/OU=dep1/CN=Huber,OU=AP11
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask2: ACL mask (d20_6bomask)                                  */
$ /* *********************************************************************** */

$set 16 preload

1 (Help mask)           DIRECTORY SYSTEM
2 Select one of the following Interpretation field values by pressing the
3 space bar:
4 
5       OBJECT      Assigns access to a single object
6 
7       SUBTREE     Assigns access to a subtree (including subtree root)
8 
9       _______     (When no DN is specified)
10 
11 In case of Add Object and Modify Attribute operations, it is a toggle
12 field and one of the values can be selected using space bar.
13 
14 If you are modifying the access rights, overwrite the old access right
15 information with new information.
16 
17 To know about dialog control when this mask is used in the context of
18 'Display Objects' operation, refer to administration manual.
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask1: Mask for simple attributes (d20_7omask)                 */
$ /* *********************************************************************** */

$set 17 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask is used to display and enter attribute values for attributes
4 which do not require special mask.
5 
6 In case of 'Display Objects' operation this is read only mask
7 for more info. on dialog control refer to administration manual.
8 
9 Enter attribute values which do not require a special mask as follows:
10 
11  Character input     must end with an ' (apostrophe) and conform to
12                      the correct attribute syntax. For example, abc'.
13 
14  Hexadecimal input   the character sequence must begin with "x'" and
15                      end with an "'" (apostrophe). For example, x'00FF'.
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask2: Mask for simple attributes (d20_7omask)                 */
$ /* *********************************************************************** */

$set 18 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3  Leading and
4  trailing spaces     are removed.
5 
6  Middle spaces       are reduced to one space for the following syntaxes:
7 
8                        Numeric String        Case Ignore String
9                        Case Exact String     Printable String
10                        Case Ignore List      Telephone Number Syntax
11                        Distinguished Name
12 
13  Ending space         can be used in attribute values where spaces are
14                       allow (for example, with octet string syntax)
15 
16 
17 
18 
19 
20 To continue press <CR> !


$ /* *********************************************************************** */
$ /*     Help mask3: Mask for simple attributes (d20_7omask)                 */
$ /* *********************************************************************** */

$set 19 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Character input values must conform to the following attribute syntaxes:
4 
5  Boolean               can be TRUE or FALSE
6 
7  Preferred Delivery    can be 10 integers or less; separated by
8  Method                spaces; in the range of 0 to 9
9 
10  Distinguished Name   must be entered with no leading
11                       spaces
12 
13 
14 
15 
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask4: Mask for simple attributes (d20_7omask)                 */
$ /* *********************************************************************** */

$set 20 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 
4 Three attributes are displayed per mask. If more than three attributes
5 (which do not require special mask) have been selected from the
6 previous mask, then the additional attribute names are displayed
7 automatically.
8 
9 Enter the attribute name for the DSA Type attribute with one of the
10 following values: default', local', or default/ local'
11 
12 Attribute names entered in the empty name fields and corresponding
13 attribute values are ignored.
14 
15 Enter <CR>/<MENU> after finishing an entry to display subsequent masks.
16 
17 
18 
19 
20 To exit help press <CR> !



$ /* *********************************************************************** */
$ /*    Help mask: PSAP address mask (d20_7aomask)                           */
$ /* *********************************************************************** */

$set 21 preload

1 (Help mask)          DIRECTORY SYSTEM
2 
3 Used to display or enter the PSAP address.
4 
5  Presentation        A presentation selector and a session address
6  Address
7 
8  Session Address     A session selector and a transport address
9 
10  Transport Address   A transport selector and one or more network
11                      addresses
12 
13 The first NSAP address, NSAP-Address1, must be specified; other fields
14 are optional.
15 
16 Modify the PSAP address by overwriting the old value of PSAP address
17 with the new value. You cannot delete or add a PSAP address value since
18 it is always a non-recuring attribute.
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask: Modify simple attribute value mask (d20_8omask)           */
$ /* *********************************************************************** */

$set 22 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask is used to modify an attribute value which does not require
4 special mask. The name of the selected attribute (from previous mask)
5 is displayed in the 'Name' field and the old attribute value is
6 displayed in the 'Old Value' and the 'New Value' fields. If the
7 attribute is recurring and has more than one values, <SCROLL DOWN> can
8 be used to page thru. the other values.
9 
10 The displayed attribute value can be changed or deleted by changing or
11 deleting in the 'New Value' field. A new value can be added (for
12 recurring attributes) by deleting value in the 'Old Value' field and
13 entering a new value in the 'New Value' field. An old value can only be
14 deleted when it is not the last value of a recurring attribute.
15 
16 After pressing <CR>/<MENU> if this mask is left without any error
17 message, that implies modification is successful.
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask: Schema admin. mask (d20_9omask)                           */
$ /* *********************************************************************** */

$set 23 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Schema Administration operations provide functions to:
4 
5       Display the Structure Rule Table (SRT), Object class table (OCT),
6       and Attribute Table (AT)
7 
8       Add, delete, and modify SRT, OCT, and AT table entries
9 
10 After you modify the DSA schema, store it in the bind DSA using the
11 Store Schema operation. If the Store Schema operation fails (for
12 example, the DSA has gone down), the modified schema is stored in a file
13 so that changes are not lost. In the next session (for example, when DSA
14 comes up again), load this schema from the file into local memory using
15 the Load Schema operation. Then try to store it again in the DSA.
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask1: Object class mask (d20_Aomask)                           */
$ /* *********************************************************************** */

$set 24 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask is used to display, add, delete, or modify an object class.
4 
5 Display OCT  <CR>/<MENU> leads to next selected object class (if any)
6              and <F1> displays the object class list mask again with
7              selected object classes.
8 
9 Add OCT      To add an object class:
10 Entry
11              -- Superclasses must exist in the OCT
12              -- Auxiliary Object Classes must exist as auxiliary object
13                 classes in the OCT
14              -- All attributes must exist in the AT
15 
16              Fill in the fields as shown in the following help screens.
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: Object class mask (d20_Aomask)                           */
$ /* *********************************************************************** */

$set 25 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Delete OCT   Only the Object Class Acronym is displayed. Enter the
4 Entry        acronym of object class you want to delete from the OCT in
5              local memory. To delete an object class it must NOT be one
6              of the following:
7 
8              -- one of the standard object classes
9              -- the super class of any object class
10              -- a structural object class, referenced in any SRT entry
11              -- an auxiliary object class for any object class
12 
13 Modify OCT   Only the Object Class Acronym field is displayed. Enter
14 Entry        the acronym of the object class you want to modify. If
15              this  mask is accepted, the object class exists in the
16              OCT. Then Mask 11 reappears and displays all the
17              components of the object class entry.
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask3: Object class mask (d20_Aomask)                           */
$ /* *********************************************************************** */

$set 26 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3              Constraints for the Modify OCT Entry operation are:
4 
5              -- All constraints valid for Add Oct Entry
6              -- If the object class belongs to default schema then the
7                 Object Identifier, File Number, Superclasses and Object
8                 Class Kind fields can not be modified
9              -- Mandatory attributes cannot be removed.
10 
11              Fill in the fields as shown in the following help screens.
12 
13 
14 
15 
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask4: Object class mask (d20_Aomask)                           */
$ /* *********************************************************************** */

$set 27 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 To add and modify and object class in the OCT, enter the appropriate
4 information in the following fields:
5 
6 Object Class Name    Enter the object class name (blanks are not valid)
7 
8 Acronyms of Superclasses
9                      Enter a list of up to 10 acronyms separated by
10                      spaces (3 characters maximum and blanks are
11                      not valid)
12 
13 Object Identifier    Enter the Object Identifier as a sequence of
14                      integers separated by a dots (for example,
15                      85.6.12)
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask5: Object class mask (d20_Aomask)                           */
$ /* *********************************************************************** */

$set 28 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Object Class Kind    Select one of the following object class types by
4                      pressing the space bar:
5 
6                      Abstract     Alias     Auxiliary     Structural
7 
8 File Number          Enter the file number (greater than 0 or equal to
9                      -1) in which the objects of this object class are
10                      stored. O is reserved for the schema object. -1
11                      indicates that there are no concrete objects to
12                      be stored because this object is an abstract,
13                      alias, or auxiliary object class.
14 
15 Auxiliary Object Classes
16                      Enter a list of up to 10 acronyms separated by
17                      spaces (3 characters maximum and blanks are not
18                      valid). Auxiliary classes must exist in the OCT.
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask6: Object class mask (d20_Aomask)                           */
$ /* *********************************************************************** */

$set 29 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Mandatory Attributes
4                      Enter a list of up to 10 acronyms separated by
5                      spaces (3 characters maximum and blanks are
6                      not valid).
7 
8 Optional Attributes  Enter a list of up to 10 acronyms separated by
9                      spaces (3 characters maximum and blanks are
10                      not valid).
11 
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask1: Attribute mask (d20_Bomask)                             */
$ /* *********************************************************************** */

$set 30 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask is used to display, add, delete, or modify an attribute entry.
4 
5 Display AT   <CR>/<MENU> displays next selected attribute entry
6              (if any) and <F1> displays the attribute list mask again
7              with selected attribute entries
8 
9 Delete AT    Only the Attribute Acronym field is displayed. Enter
10 Entry        the acronym of attribute entry you want to delete from
11              the AT in local memory.
12 
13              The attribute entry CANNOT be:
14 
15              -- One of the standard attributes
16              -- Referenced as naming attribute in an SRT entry
17              -- Referenced as naming attribute in an SRT entry
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask2: Attribute mask (d20_Bomask)                             */
$ /* *********************************************************************** */

$set 31 preload

1 (Help mask)           DIRECTORY SYSTEM
2  
3 Modify AT    Only the Attribute Acronym field is displayed. Enter
4 Entry        the acronym of the attribute entry you want to modify.
5              If the entry is accepted, the attribute acronym exists
6              in the AT and Mask 12 is displayed again with all the
7              the components of the attribute entry. Modify the values
8              according to the rules described in the following help
9              screens.
10 
11              The Modify AT Entry operation imposes the following
12              constraints:
13 
14              -- If the attribute is used as naming attribute in some
15                 structure, the attribute cannot be changed from
16                 Case Ignore String syntax
17              -- If the attribute belings to the default schema, the
18                 Object Identifier and Attribute Syntax fields cannot
19                 be modified
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask3: Attribute mask (d20_Bomask)                             */
$ /* *********************************************************************** */

$set 32 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 To add and modify an object class in the OCT, enter the appriopriate
4 information in the following fields:
5 
6 Attribute Acronym    Enter the attribute acronym name (blanks are
7                      not valid)
8 
9 Attribute Name       Enter the attribute name (blanks are not valid)
10 
11 Object Identifier    Enter the object identifier as a list of integers
12                      separated by dots (for example, 85.4.6)
13 
14 Lower Bound          Enter the minimum length of the attribute value
15                      (1 to 9999)
16 
17 Upper Bound          Enter the maximum length of the attribute value
18                      (1 to 9999)
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask4: Attribute mask (d20_Bomask)                             */
$ /* *********************************************************************** */

$set 33 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Number of Recurring Values
4                      Enter the maximum number of values as follows:
5 
6                      -- 0     Unlimited number of values
7                      -- 1     Only 1 value (no recurring values)
8                      -- >1    Restricted number of values
9 
10                      It is recommended that 0 be specified if there is
11                      a large number of possible values as 1 fixes the
12                      amount of space is reserved.
13 
14 Attribute Syntax     Select one of the following syntaxes by pressing
15                      the space bar:
16 
17                      Case Ignore String     Case Exact String
18                      Octet String           ACL Syntax
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask5: Attribute mask (d20_Bomask)                             */
$ /* *********************************************************************** */

$set 34 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      Search Guide Syntax       Password Syntax
4                      Country Name Syntax       Presentation Address
5                      Telephone Number          Fax Number Syntax
6                       Syntax                   TTX-ID Syntax
7                      Telex Number Syntax       Postal Address Syntax
8                      Case Ignore List          Boolean Syntax
9                      Integer Syntax            UTC Time
10                      Numeric String            Printable String
11                      ASN.1 Syntax              MHS O/R Syntax
12                      MHS O/R Name Syntax       MHS DL Submit
13                      MHS Preferred Delivery    Permission Syntax
14                       Method Syntax            Any Syntax
15                      Object Identifier Syntax  DN
16 
17 
18 
19 
20  To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask6: Attribute mask (d20_Bomask)                             */
$ /* *********************************************************************** */

$set 35 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Access Class         Select one of the following access classes 
4                      by pressing the space bar:
5 
6                      -- Public
7                      -- Standard (default)
8                      -- Sensitive
9 
10 Index Level          Select the index level for the attribute. The
11                      index level defines the priority of an attribute
12                      for a search inquiry in the range between
13                      0 (low priority) to 99 (high priority)
14 
15 
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask7: Attribute mask (d20_Bomask)                             */
$ /* *********************************************************************** */

$set 36 preload

1 (Help mask)           DIRECTORY SYSTEM
2  
3 Phonetic Matching   Select one of the following by pressing the
4                     the space bar:
5 
6                      YES     Phonetic matching is allowed. The
7                              attribute value could vary slightly
8                              than what is given in filter of search
9                              query (for example,
10                              Munchen - Muenchen) and still
11                              satisfy the filter
12 
13                      NO      No phonetic matching is allowed
14                              and match should be exact.
15 
16 
17 
18 
19  
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask1: Structure rule mask (d20_Comask)                        */
$ /* *********************************************************************** */

$set 37 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask is used to display, add, delete, or modify a structure rule.
4 
5 Display SRT  <CR>/<MENU> leads to the next selected structure rule
6              (if any) and <F1> displays the structural rule list mask
7              again with the selected structure rules.
8 
9 Delete SRT   Only the first field (Rule 4 Number) is displayed. Enter
10 Entry        the structure rule number you want to delete from SRT
11              local memory. To delete a structure rule it must NOT:
12 
13              -- one of the standard structure rules
14              -- used as a superior rule in any other structure rule
15 
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: Structure rule mask (d20_Comask)                         */
$ /* *********************************************************************** */

$set 38 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Add SRT      Fill in the fields in Mask 10 (individual fields are
4 Entry        described in later help screens)
5 
6              To add a structure rule the following constratints apply:
7 
8              -- The Superior rule must exist in the SRT
9              -- The naming attributes must exist in the AT
10              -- The naming attributes must have case ignore syntax
11              -- The Object Class must exist in the OCT as structural
12                 object class
13              -- The naming attributes must belong to structural object
14                 class
15 
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask3: Structure rule mask (d20_Comask)                         */
$ /* *********************************************************************** */

$set 39 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Modify SRT   Only the Rule Number field is displayed. 
4 Entry
5              Enter the structure rule number you want to modify. If the
6              rule number you specified exists in the SRT, the mask is
7              accepted, and the mask is redisplayed with the
8              components of that structure rule.  The Rule Number field
9              becomes a read-only field.
10 
11              Modify the structure rule by changing the appropriate
12              fields. If the new values are accepted, the operation
13              is successful.
14 
15              Note that the structure rule contained in the default
16              schema can not be modified.
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask4: Structure rule mask (d20_Comask)                         */
$ /* *********************************************************************** */

$set 40 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Modify SRT   To modify a structure rule the following constraints apply:
4 Entry
5              -- The Superior rule must exist in the SRT
6              -- The naming attributes must exist in the AT
7              -- The naming attributes must have case ignore syntax
8              -- The Object Class must exist in the OCT as structural
9                 object class
10              -- The naming attributes must belong to structural object
11                 class
12 
13 To Add and Modify a structure rule in the SRT, enter the appriopriate
14 information in the following fields:
15 
16 Rule Number           Enter the number of the structure rule.
17 
18 Superior Rule Number
19                       Enter the number of the superior structure rule.
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask5: Structure rule mask (d20_Comask)                         */
$ /* *********************************************************************** */

$set 41 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Acronyms of Naming Attributes
4                       Enter the list of up to three acronyms of naming
5                       attributes. These acronyms are separated by
6                       spaces. The naming attributes must exist in
7                       the AT.
8 
9 Structural Object Class
10                      Enter an object class that is assigned to this rule.
11                      It must exist an a structural object class.
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask1: Shadow administration mask (d20_Domask)                  */
$ /* *********************************************************************** */

$set 42 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Shadow administration operations provide functions to:
4 
5      -- Create shadows of objects on non-local DSAs (the unit of
6         shadow creation can be a single object or a subtree).
7      -- Create a shadow update job which describes how often
8         shadows should be updated (including jobs that update
9         shadows which were not created by shadow administration).
10      -- Remove shadows from non-local DSAs.
11      -- Remove a shadow update job.
12      -- Modify the shadow update frequency (using the Update
13      -- Shadowing Job operation).
14      -- Display existing shadowing jobs.
15      -- Display update errors that occurred during shadow update.
16      -- Remove update errors so that an update is not repeated
17         by the delta daemon process to prevent the update from
18         being repeated.
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*  Help mask1: job mask (d20_E1omask)                                     */
$ /* *********************************************************************** */

$set 43 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask defines the job state of the shadow update job.
4 
5 The following fields are displayed:
6 
7     Object Name      Enter the object name on which the operation be
8                      performed.
9 
10     Object Interpretation
11                      Select one of the following object interpretations
12                      by pressing the space bar:
13 
14                      SINGLE OBJECT  Update only the shadow of the object
15 
16                      OBJECT AND ITS SUBORDINATES
17                                     Update the shadow of the object
18                                     and its subordinates
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*  Help mask2: job mask (d20_E1omask)                                     */
$ /* *********************************************************************** */

$set 44 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3     Target DSA       Enter the DSA on which the shadow job
4                      operation will be performed.
5 
6     Job              If the operation is Create Shadows and
7                      Shadowing Job or Create Shadowing Job,
8                      select one of the following values by pressing
9                      the space bar:
10 
11                      -- ACTIVATE UPDATES (active job whose
12                         frequency you can specify in subsequent
13                         masks)
14 
15                      -- NO UPDATES (an inactive job in which
16                         shadows or cache data will not be updated)
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*  Help mask3: job mask (d20_E1omask)                                     */
$ /* *********************************************************************** */

$set 45 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3     Job               If the operation is Update Shadowing and an
4                       active job is displayed, select one of the
5                       following by pressing the space bar:
6 
7                       -- ACTIVATE UPDATES IMMEDIATELY
8                         (daemon process updates shadows or cache
9                         immediately (only once) and the status of the
10                         job remains NOT ACTIVE)
11 
12                      -- DEACTIVATE UPDATES (change status of
13                         job to NOT ACTIVE)
14 
15                      -- MODIFY UPDATE FREQUENCY
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*  Help mask4: job mask (d20_E1omask)                                     */
$ /* *********************************************************************** */

$set 46 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3     Job               If the operation is Update Shadowing and an
4                       inactive job is displayed, select the following
5                       values by pressing the space bar:
6 
7                       -- ACTIVATE UPDATES IMMEDIATELY
8                       -- ACTIVATE UPDATES
9 
10                      If the operation is Cache Update, the Object
11                      Name, Object Interpretation, and Target DSA
12                      fields are not displayed.
13 
14 
15 
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*  Help mask5: job mask (d20_E1omask)                                     */
$ /* *********************************************************************** */

$set 47 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      If the operation is Display Shadowing Job
4                      operation, use <CR> to select a job and
5                      <SCROLL-DOWN> to display the next job.
6                      Selected jobs can be updated without specifying
7                      the job again.
8                      select the following values by pressing the space
9                      bar:
10 
11                      -- ACTIVE (Update job is active and shadows or
12                         cache data are updated at regular intervals)
13 
14                      -- NOT ACTIVE (Update job exists but it is not
15                         active. Shadows or cache data are not updated)
16 
17 
18 
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /* Help mask: Update frequency mask (d20_E2omask)                          */
$ /* *********************************************************************** */

$set 48 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask overlays the previous mask. Use this mask to define the update
4 frequency.
5 
6 The Object Name, Object Interpretation, Target DSA, and Job field values
7 are displayed with the values you selected in previous masks. Object
8 Name, Object Interpretation and Target DSA fields don't appear when a
9 job for cache update is being handled. Cache update job updates all
10 objects in the cache.
11 
12 Select one of the following values for the Update Frequency field by
13 pressing the space bar:
14 
15      HIGH            Execute update job every few minutes
16      MEDIUM          Execute update job every few hours
17      LOW             Execute update job every few days
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /* Help mask1: Update times mask (d20_Eaomask)                             */
$ /* *********************************************************************** */

$set 49 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask overlays the previous mask. Use this mask to define the update
4 times.
5 
6 The Object Name, Object Interpretation, Target DSA, Job, and Update
7 Frequency values are displayed with the values you selected in
8 previous masks. Object Name, Object Interpretation and Target DSA
9 fields don't appear when a job for cache update is being handled. Cache
10 update job updates all objects in the cache.
11 
12 Updates Times field can have one of the following sets of options
13 depending on the value in Update Frequency field:
14 
15   Update Frequency        Options in Update Times field
16      HIGH                    EVERY 5/10/15/30 MINUTES
17      MEDIUM                  EVERY 1/2/4/6/12 HOURS
18      LOW                     EVERY DAY/ONCE A WEEK/TWICE A WEEK
19 
20 To continue press <CR> !


$ /* *********************************************************************** */
$ /* Help mask2: Update times mask (d20_Eaomask)                             */
$ /* *********************************************************************** */

$set 50 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 In case of Create/Modify a shadowing job, press the space bar to select
4 one of the values.
5 
6 The update time in minutes is relative to 0 minutes past the hour.
7 
8 For example, EVERY 10 MINUTES means that the updates are done at
9 0 minutes past the hours, 10 minutes past the hour, 20 minutes past the
10 hour, 30 minutes past the hour, and so on.
11 
12 The update time in hours is relative to 0 a.m.
13 
14 For example, EVERY 2 HOURS means that the updates are done at
15 0 a.m., 2 a.m., 4 a.m., 6 a.m. up to 10 p.m.
16 
17 
18 
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /*     Help mask: Hours mask (d20_Ebomask)                                 */
$ /* *********************************************************************** */

$set 51 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask overlays the previous mask. Use this mask to define the update
4 times.
5 
6 The Object Name, Object Interpretation, Target DSA, Job, and Update
7 Frequency values are displayed with the values you selected in
8 previous masks. Object Name, Object Interpretation and Target DSA
9 fields don't appear when a job for cache update is being handled. Cache
10 update job updates all objects in the cache.
11 
12 Updates Times field has the option:
13 
14 EVERY DAY            The Day of the Week fields are not displayed and
15                      there is only one Hours fields (directly under the
16                      Update Times field).
17 
18 
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /*     Help mask1: Day/Hours mask (d20_Ecomask)                            */
$ /* *********************************************************************** */

$set 52 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask overlays the previous mask. Use this mask to define the update
4 times.
5 
6 The Object Name, Object Interpretation, Target DSA, Job, and Update
7 Frequency values are displayed with the values you selected in
8 previous masks. Object Name, Object Interpretation and Target DSA
9 fields don't appear when a job for cache update is being handled. Cache
10 update job updates all objects in the cache.
11 
12 Updates Times field has the option:
13 
14 ONCE A WEEK          Only one Day of Week and one Hours
15                      field are displayed
16 
17 
18 
19 
20 To continue press <CR> !


$ /* *********************************************************************** */
$ /*     Help mask2: Day/Hours mask (d20_Ecomask)                            */
$ /* *********************************************************************** */

$set 53 preload

1 Select one of the following values for the Day of Week field by
2 pressing the space bar:
3 
4       MONDAY     TUESDAY   WEDNESDAY
5       THURSDAY   FRIDAY    SATURDAY
6       SUNDAY
7 
8 Enter the value for the Hours field to specify the hour of the update.
9 
10 
11 
12 
13 
14 
15 
17 
18 
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /*     Help mask1: Day/Hours Day/Hours mask (d20_Edomask)                  */
$ /* *********************************************************************** */

$set 54 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask overlays the previous mask. Use this mask to define the update
4 times.
5 
6 The Object Name, Object Interpretation, Target DSA, Job, and Update
7 Frequency values are displayed with the values you selected in
8 previous masks. Object Name, Object Interpretation and Target DSA
9 fields don't appear when a job for cache update is being handled. Cache
10 update job updates all objects in the cache.
11 
12 Updates Times field has the option:
13 
14 TWICE A WEEK         Two Day of Week and two Hours fields are
15                      displayed.
16 
17 
18 
19 
20 To continue press <CR> !


$ /* *********************************************************************** */
$ /*     Help mask2: Day/Hours Day/Hours mask (d20_Edomask)                  */
$ /* *********************************************************************** */

$set 55 preload

1 Select one of the following values for the Day of Week fields by
2 pressing the space bar:
3 
4       MONDAY     TUESDAY   WEDNESDAY
5       THURSDAY   FRIDAY    SATURDAY
6       SUNDAY
7 
8 Enter the values for the Hours fields to specify the hour of the update.
9 
10 
11 
12 
13 
14 
15 
17 
18 
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /* Help mask1: Update error mask (d20_E3omask)                             */
$ /* *********************************************************************** */

$set 56 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask is used to display update errors or to specify which
4 operations failed during shadow update and must be performed again by
5 delta update daemon when it is activated next. This mask displays the
6 following fields:
7 
8 Object Name          The name of the object on which operation failed.
9 
10 Target DSA           The DSA where shadow of the object lies.
11 
12 Operation            The operation which caused the problem.
13 
14 Error Time           The time at which the error occurred (in
15                      year-month-day/hour:minute format).
16 
17 Error Count          The number of erroneous attempts to perform the
18                      shadow update.
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /* Help mask2: Update error mask (d20_E3omask)                             */
$ /* *********************************************************************** */

$set 57 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Error Code           Displays the reason for the error.
4 
5 Remove Update Error  Only displayed if the operation is Remove Update
6                      Error. Select on of the following by pressing the
7                      bar:
8 
9                      YES     The update error is removed, and the
10                              update is, therefore, not repeated
11 
12                      NO      The update error is not removed, and the
13                              update is repeated (default)
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask1: Subtree administration mask (d20_10omask)                */
$ /* *********************************************************************** */

$set 58 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Subtree administration consists of the following operations:
4 
5     Save Subtree     Writes the master or shadow entries of a subtree
6                      in binary format to a file. In order to save all
7                      object information, you must have read access to
8                      all attributes of all objects in the subtree.
9 
10     Append Subtree
11                      Appends a subtree from a file (created beforehand
12                      using the Save Subtree operation) under a parent
13                      node. The new parent node must already exist as a
14                      master or shadow in the target DSA. To append the
15                      objects successfully, you must have modify access
16                      rights to the naming attribute(s) of the new root
17                      of the subtree.
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: Subtree administration mask (d20_10omask)                */
$ /* *********************************************************************** */

$set 59 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3      Copy Subtree    Copies a subtree from a source DSA to a target
4                      DSA. This operation is the same as the combination
5                      of a Save Subtree operation followed by an Append
6                      Subtree operation
7 
8      Change Name/Move Subtree
9                      Changes the name of an entry (not necessarily a
10                      leaf node) or moves a subtree. This operation
11                      operates only on master entries. The new parent
12                      node must already exist. You must have modify
13                      access rights to the naming attribute (s) of the
14                      new root of the subtree to move the subtree
15                      successfully.
16 
17      Delete Subtree  Deletes a subtree from MASTER DSA(s), from the
18                      BIND DSA, or from a specific DSA depending on the
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask3: Subtree administration mask (d20_10omask)                */
$ /* *********************************************************************** */

$set 60 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      option selected in subsequent masks. You must have
4                      modify access rights to the naming attribute(s) of
5                      all objects of the subtree to delete the subtree.
6 
7      Change Master   Changes the Master Knowledge attribute of all
8                      objects of a subtree which have the specified old
9                      value of the Master Knowledge attribute. You must
10                      make sure that shadow entries exist for all such
11                      objects on the new master DSA. Only the master
12                      entries on their old master DSA and shadow entries
13                      on new master DSA are changed. You must have
14                      modify access rights to the Master Knowledge
15                      attribute of all objects where the attribute is
16                      to be changed.
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask4: Subtree administration mask (d20_10omask)                */
$ /* *********************************************************************** */

$set 61 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3      Modify Subtree  Modifies the value of an attribute from specified
4                      old value to the specified new value for all the
5                      objects of a subtree which have the specified old
6                      attribute value. You must have modify access
7                      rights to this attribute for all the objects
8                      involved.
9 
10 
11 
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask1: Additional parms. for Subtree admin. mask (d20_11aomask) */
$ /* *********************************************************************** */

$set 62 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to enter additional parameters required by the Save
4 Subtree,  Copy Subtree and Delete Subtree operations.
5 
6 Select one of the following values for the Source DSA field by pressing
7 the bar:
8 
9      MASTER DSA (default)     The target subtree consists of only
10                              master entries and can span multiple DSAs
11 
12      BIND DSA                Limits the scope of operation to one DSA
13 
14      SPECIFIC DSA            Limits the scope of operation to one DSA
15 
16 The target subtree for the operation is a subtree consisting of master
17 or shadow entries on a particular DSA.
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: Additional parms. for Subtree admin. mask (d20_11aomask) */
$ /* *********************************************************************** */

$set 63 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Enter the name of the file where the subtree is to be saved in the File
4 Name field (appears only for the Save operation).
5 
6 
7 
8 
9 
10 
11 
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /*    Help mask1: Additional parms. for Subtree admin. mask (d20_11bomask) */
$ /* *********************************************************************** */

$set 64 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to enter additional parameters required by Append
4 Subtree, Copy Subtree, and Change Name/Move Subtree operations in the
5 following fields:
6 
7 File Name    Enter the name of the file in which the objects are to be
8              saved. The File Name field does not appear when the
9              operation is Copy Subtree.
10 
11 Overwrite Existing Entries
12 
13              Select one of the following values by pressing the space
14              bar:
15 
16              YES (default)   If entries already exist in new subtree,
17                              they will be overwritten by appended entries
18                              entries
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: Additional parms. for Subtree admin. mask (d20_11bomask) */
$ /* *********************************************************************** */

$set 65 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3              NO              Entries existing in new subtree are not
4                              overwritten
5 
6 The Overwrite Existing Entries field does not appear when the operation
7 is Change Name/Move Subtree.
8 
9 New Entries Protected By
10 
11              Select one of the following values by pressing the space
12              bar:
13 
14              ACL of the new parent (default)
15                              The object entries which are being
16                              appended will have same ACL attribute
17                              value as the new parent.
18 
19 
20 To continue press <CR> !


$ /* *********************************************************************** */
$ /*    Help mask3: Additional parms. for Subtree admin. mask (d20_11bomask) */
$ /* *********************************************************************** */

$set 66 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3            Original ACL    Appended object entries retain their ACL
4                            attribute value.
5 
6 Target DSA Select one of the following values by pressing the space
7 bar:
8 
9            BIND DSA (default)
10                            The subtree is appended/copied to a new
11                            parent object which exists in the bound DSA.
12 
13            SPECIFIC DSA
14                           The subtree is appended/copied to a new
15                           parent object which exists on a DSA,
16                           different from the bind DSA. Enter the
17                           specific DSA in the subsequent mask.
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask: Objects/Attrs./Schema/Aoclass list mask (d20_12omask)     */
$ /* *********************************************************************** */

$set 67 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to select items from an item list. Place the cursor bar
4 on the desired item and press <CR>. The item is removed from the list
5 of selected items. Move the cursor bar by pressing the <CURSOR UP>,
6 <CURSOR DOWN>, <SCROLL UP> and <SCROLL DOWN> keys. If you do not
7 select any items from the item list (except in case of Attribute list),
8 then all items are assumed to be selected. Items could be one of the
9 following types depending on the context:
10 
11    -- Objects in search result
12    -- Auxiliary object classes belonging to selected structural object
13       class
14    -- Attributes belonging to selected structural object class and
15       auxiliary object classes
16    -- Attribute entries of Attribute Table (AT)
17    -- Object Class entries of Object Class Table (OCT)
18    -- Structural Rule entries of Structural Rule Table (SRT)
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask: Object name mask for display (d20_13omask)                */
$ /* *********************************************************************** */

$set 68 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to display the name of selected objects from a search
4 result. If search result contains only one object, then name of that
5 object is displayed. Names of naming attributes belonging to a selected
6 structure rule and of all structure rules which are in the same branch
7 till root are displayed in the left column. Naming attribute values are
8 displayed in the right column. Use the following keys to display the
9 selected objects:
10 
11      <CR>/<MENU>             Displays attributes belonging to the object.
12      <SCROLL UP>             Displays the previous selected object
13                              (if any).
14      <SCROLL DOWN>           Displays the next selected object (if any).
15      <F1>                    Displays the object list again.
16 
17 The <SCROLL UP>, <SCROLL DOWN>, and <F1>  keys are valid only
18 if search result contains more than one object.
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /*    Help mask: Err. Object list mask (d20_14omask)                       */
$ /* *********************************************************************** */

$set 69 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This is the list either of objects on which operation failed or of
4 objects overwritten by the operation.
5 
6 
7 
8 
9 
10 
11 
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask1: DNS Cell mask (d20_dnscellomask)                         */
$ /* *********************************************************************** */

$set 70 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify values of the CDS-Cell attribute in the
4 following fields:
5 
6 Namespace UUID       Enter the Universal Unique Identifier (UUID) of
7                      the CDS namespace. It is required to prevent 
8                      ambiguities in CDS namespaces when a server 
9                      manages more than one clearinghouse, and the
10                      clearinghouses are in different namespaces.
11 
12                      A CDS UUIS consists of 16 hex digit pairs
13                      represented as 8 hexadecimal digits followed by
14                      a hyphen, 3 groups of 4 hexadecimal digits
15                      separated by hyphens, a hyphen and 12
16                      hexadecimal digits (for example,
17                      01234567-89ab=cdef-0123-456789abcdef).
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: DNS Cell mask (d20_dnscellomask)                         */
$ /* *********************************************************************** */

$set 71 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Root Dir UUID        Enter the UUID of the root directory. This
4                      parameter, in addition to the Root Dir Name
5                      parameter, is used to form the resolved and
6                      unresolved names in a CDS progress record.
7 
8 Root Dir Name        Enter the root directory. This parameter, in
9                      addition to the Root Dir UUID parameter, is used
10                      to form the unresolved and resolved names in a
11                      CDS progress record.
12 
13 Modification         Select one of the following option by pressing the
14                      space bar (shown only when you have selected 
15                      the Modify Attribute operation):
16 
17                      Modify Value, Delete Value, Add Value
18 
19 
20 To continue press <CR> !


$ /* *********************************************************************** */
$ /*    Help mask3: DNS Cell mask (d20_dnscellomask)                         */
$ /* *********************************************************************** */

$set 72 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      Use Delete Value and Add Value only for recurring
4                      attributes. Use the Delete Attribute operation to
5                      delete the last value of the attribute (which
6                      deletes the complete attribute from the object).
7 
8                      For recurring attributes, use <SCROLL-DOWN> to
9                      scan through the other values. Enter the old and
10                      the new attribute value when performing the
11                      Modify Subtree operation.
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask1: DNS Replica mask (d20_dnsrepomask)                       */
$ /* *********************************************************************** */

$set 73 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify the values of the CDS-Replica attribute in the
4 following fields:
5 
6 Replica Type         Select MASTER for a modifiable replica or read-
7                      only replica.  (MASTER can be changed to READ
8                      ONLY.
9 
10 Clearinghouse UUID   Enter the clearinghouse UUID. The clearinghouse
11                      UUID never changes. Since the clearinghouse name
12                      can change, the clearinghouse UUID is used to
13                      verify the validity of the clearinghouse.
14 
15                      A CDS UUID consists of 16 hex digit pairs
16                      represented as 8 hexadecimal digits, followed by a
17                      hyphen, 3 groups of 4 hexadecimal digits separated
18                      by hyphens, a hyphen and 12 hexadecimal digits
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: DNS Replica mask (d20_dnsrepomask)                       */
$ /* *********************************************************************** */

$set 74 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      (for example, 01234567-89ab-cdef-0123-
4                      456789abcdef).
5 
6 Clearinghouse Name   Enter the full name of the clearinghouse in which
7                      the replica is stored. This name is the name of a
8                      naming attribute that contains information on the
9                      last known address of the clearinghouse. This
10                      information enables the creation of a Remote
11                      Procedure Call (RPC) binding to the server that
12                      maintains the clearinghouse.
13 
14 Tower                Enter the tower set of the server that maintains
15                      the clearinghouse. A CDS tower contains addressing
16                      information and information on protocols supported
17                      by the clearinghouse server. The format of the to-
18                      wer value is the same format as a substring of the
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask3: DNS Replica mask (d20_dnsrepomask)                       */
$ /* *********************************************************************** */

$set 75 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      RPC string binding as follows: protseq:netaddr
4 
5 Modification         Select one of the following option by pressing the
6                      space bar (shown only when you have selected
7                      the Modify Attribute operation):
8 
9                      Modify Value
10                      Delete Value
11                      Add Value
12 
13                      Use Delete Value and Add Value only for recurring
14                      attributes. Use the Delete Attribute operation to
15                      delete the last value of the attribute (which
16                      deletes the complete attribute from the object).
17 
18                      For recurring attributes, use <SCROLL-DOWN> to
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask4: DNS Replica mask (d20_dnsrepomask)                       */
$ /* *********************************************************************** */

$set 76 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      scan through the other values. Enter the old and
4                      the new attribute value when performing the
5                      Modify Subtree operation.
6 
7 
8 CDS cell information is stored in two attributes, the CDS-Cell attribute,
9 and the CDS-replica attribute. The information in one attribute
10 complements the information in the other. Therefore, each object that
11 contains a CDS-Cell attribute also contains a a CDS -Replica attribute.
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /*    Help mask1: TTX ID. mask (d20_ttxidomask)                            */
$ /* *********************************************************************** */

$set 77 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify attributes with TTX-ID Syntax in the
4 following fields:
5 
6 Attribute name       Displays the name of the attribute, for
7                      example, TTX-Terminal-Identifier.
8 
9 Terminal              Enter a unique terminal identifier for a telex
10                      terminal. Enter the teletex nonbasic
11                      parameters in the remaining fields.
12 
13 Control              Enter the control character set for a teletex
14                      terminal (optional).
15 
16 Graphic              Enter the graphic character set for a teletex
17                      terminal (optional).
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: TTX ID. mask (d20_ttxidomask)                            */
$ /* *********************************************************************** */

$set 78 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Miscel               Enter the miscellaneous capabilities for a
4                      teletex terminal (optional).
5 
6 Page Formats         Enter the page format for a teletex terminal
7                      (optional).
8 
9 Private Use           Enter the Private Use parameters for a teletex
10                      terminal (optional).
11 
12 Modification         Select one of the following option by pressing the
13                      space bar (shown only when you have selected
14                      the Modify Attribute operation):
15 
16                      Modify Value
17                      Delete Value
18                      Add Value
19 
20 To continue press <CR> !


$ /* *********************************************************************** */
$ /*    Help mask3: TTX ID. mask (d20_ttxidomask)                            */
$ /* *********************************************************************** */

$set 79 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      Use Delete Value and Add Value only for recurring
4                      attributes. Use the Delete Attribute operation to
5                      delete the last value of the attribute (which
6                      deletes the complete attribute from the object).
7 
8                      For recurring attributes, use <SCROLL-DOWN> to
9                      scan through the other values. Enter the old and
10                      the new attribute value when performing the
11                      Modify Subtree operation.
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask1: Telex mask (d20_telexomask)                              */
$ /* *********************************************************************** */

$set 80 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify attributes with Telex Number Syntax in the
4 the following fields:
5 
6 Attribute name       Displays the name of the atttribute, for
7                      example, Telex-Number.
8 
9 Telex Number         Enter the telex number of the telex terminal.
10 
11 Country Code         Enter the country code of a telex terminal.
12 
13 Answerback           Enter the answerback code of a telex terminal.
14 
15 Modification         Select one of the following option by pressing the
16                      space bar (shown only when you have selected.
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: Telex mask (d20_telexomask)                              */
$ /* *********************************************************************** */

$set 81 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      the Modify Attribute operation):
4 
5                      Modify Value    Delete Value     Add Value
6 
7                      Use Delete Value and Add Value only for recurring
8                      attributes. Use the Delete Attribute operation to
9                      delete the last value of the attribute (which
10                      deletes the complete attribute from the object).
11 
12                      For recurring attributes, use <SCROLL-DOWN> to
13                      scan through the other values. Enter the old and
14                      the new attribute value when performing the
15                      Modify Subtree operation.
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask1: Postal Address mask (d20_t61omask)                       */
$ /* *********************************************************************** */

$set 82 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify attributes with Postal Address Syntax in the
4 following fields:
5 
6 Attribute name       Displays the name of the attribute, for example,
7                      Postal-Address.
8 
9 Part 1, Part 2, ... Part 6
10                      Enter at least one string. Normally, the postal
11                      address indlucdes an addresse's name, street
12                      address, city, state or province, postal code,
13                      and possibly a post office box number.
14 
15 Modification         Select one of the following option by pressing the 
16                      space bar (shown only when you have selected 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: Postal Address mask (d20_t61omask)                       */
$ /* *********************************************************************** */

$set 83 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      the Modify Attribute operation):
4 
5                      Modify Value    Delete Value    Add Value
6 
7                      Use Delete Value and Add Value only for recurring
8                      attributes. Use the Delete Attribute operation to
9                      delete the last value of the attribute (which
10                      deletes the complete attribute from the object).
11 
12                      For recurring attributes, use <SCROLL-DOWN> to
13                      scan through the other values. Enter the old and
14                      the new attribute value when performing the
15                      Modify Subtree operation.
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask1: Fax mask (d20_faxomask)                                  */
$ /* *********************************************************************** */

$set 84 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify attributes with Fax Number Syntax in the
4 following fields:
5 
6 Attribute Name       Displays the name of the attribute, for
7                      example, Fax-Telephone-Number.
8 
9 Telephone Number     Enter the telephone number for the facsimile
10                      telephone number.
11 
12 Fax parameters       Select the G3 facsimile nonbasic parameters
13                      in the remaining fields. Select YES or NO
14                      (default) by pressing the space bar.
15 
16 Modification         Select one of the following option by pressing the
17                      space bar (shown only when you have selected
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: Fax mask (d20_faxomask)                                  */
$ /* *********************************************************************** */

$set 85 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      the Modify Attribute operation):
4 
5                      Modify Value    Delete Value    Add Value
6 
7                      Use Delete Value and Add Value only for recurring
8                      attributes. Use the Delete Attribute operation to
9                      delete the last value of the attribute (which
10                      deletes the complete attribute from the object).
11 
12                      For recurring attributes, use <SCROLL-DOWN> to
13                      scan through the other values. Enter the old and
14                      the new attribute value when performing the
15                      Modify Subtree operation.
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask1: OR Address mask (d20_oraddomask)                         */
$ /* *********************************************************************** */

$set 86 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify the type of O/R Address of an attribute value.
4 Every user or distribution list (DL) is assigned one or originator/
5 recipient (O/R) addresses. An O/R address is an attribute list that
6 distinguishes one user from another and identifies the user's point of
7 access to the MHS or the DL's expansion point.
8 
9 This mask displays the following fields:
10 
11 Attribute name       Displays the name of the attribute, for example,
12                      MHS-O/R-Address.
13 
14 O/R Address Type     Select one of the following values by pressing the
15                      space bar:
16 
17                      Mnemonic        Displays Mask 28 for entering a
18                                      mnemonic O/R address.
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: OR Address mask (d20_oraddomask)                         */
$ /* *********************************************************************** */

$set 87 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      Numeric          Displays Mask 29 for entering a
4                                       numeric O/R address.
5 
6                      Structured Postal
7                                      Displays Mask 30 for entering a
8                                      structured postal O/R address.
9 
10                      Unstructured Postal
11                                      Displays Mask 31 for entering an
12                                      unstructured postal O/R address.
13 
14                      Terminal        Displays Mask 32 for entering a
15                                      terminal O/R address.
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask1: Mnemonic OR Address mask (d20_moraddomask)               */
$ /* *********************************************************************** */

$set 88 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify an attribute with O/R Address Syntax of type
4 Mnemonic.
5 
6 A mnemonic O/R address is one that mnemonically identifies a user or
7 distribution list (DL). It identifies an administration management
8 domain (ADMD) and a user or DL relative to it. This mask displays the
9 following fields:
10 
11 Attribute name       Displays the name of the attribute, for example,
12                      MHS-O/R-Address.
13 
14 Country Name         Enter the name of the country of the ADMD that the
15                      ADMD name attribute field denotes.
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: Mnemonic OR Address mask (d20_moraddomask)               */
$ /* *********************************************************************** */

$set 89 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 ADMD Name            Enter an administration domain name that identifies
4                      an ADMD relative to the country denoted by the
5                      country name.
6 
7 PRMD Name            Enter a private domain name that identifies a
8                      private management domain (PRMD). As a national
9                      matter this identification may be either relative
10                      to the country denoted by a country name (so that
11                      PRMD names are unique within the country), or
12                      relative to the ADMD identified by an adminis-
13                      tration domain name.
14 
15 
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask3: Mnemonic OR Address mask (d20_moraddomask)               */
$ /* *********************************************************************** */

$set 90 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Org. Name            Enter an organization name that identifies an
4                      organization. As a national matter, this identi-
5                      fication may be either relative to the country
6                      denoted by a country name (so that organization
7                      names are unique within the country), or relative
8                      to the management domain identified by a private
9                      domain name, or an administration domain name or
10                      both.
11 
12 OU1,OU2,OU3,OU4      Enter the organizational unit names that identify
13                      one or more units (for example, divisions or
14                      departments) of the organization denoted in the
15                      Org. Name field. Each unit, with the exception of
16                      the first, is a subunit of the units whose name
17                      precede it in the attribute.
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask4: Mnemonic OR Address mask (d20_moraddomask)               */
$ /* *********************************************************************** */

$set 91 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Com. Name            Enter a common name that identifies a user or
4                      a distribution list.
5 
6 Given Name           Enter the user's given name.
7 
8 Initials             Enter the initials of all of the user's names,
9                      with the exception of his or her surname.
10 
11 Surname              Enter the user's surname.
12 
13 Generation           Enter the user's generation, for example Jnr.
14 
15 Domain Type1,Domain Type2, ...
16 
17                      Enter the name of a class of information.
18 
19 
20 To continue press <CR> !


$ /* *********************************************************************** */
$ /*    Help mask5: Mnemonic OR Address mask (d20_moraddomask)               */
$ /* *********************************************************************** */

$set 92 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Domain Value1,Domain Value2, ...
4 
5                      Enter an instance of the class of information
6                      that the preceding Domain Type denotes. Note that
7                      widespread use of standard attributes produces
8                      more uniform and thus more user-friendly O/R
9                      addresses. However, it is anticipated that not
10                      all Management Domains (MDs) will be able to
11                      employ such attributes immediately. The purpose
12                      of domain-defined attributes is to permit an MD
13                      to retain its existing, native addressing con-
14                      ventions for a time. It is intended, however,
15                      that all MDs will migrate toward the use of
16                      standard attributes, and that domain defined
17                      attributes will only be used for an interim
18                      period.
19 
20 To continue press <CR> !


$ /* *********************************************************************** */
$ /*    Help mask6: Mnemonic OR Address mask (d20_moraddomask)               */
$ /* *********************************************************************** */

$set 93 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Modification         Select one of the following options by pressing
4                      the space bar (shown only when you have selected
5                      the Modify Attribute operation):
6 
7                      Modify Value    Delete Value    Add Value
8 
9                      Use Delete Value and Add Value only for recurring
10                      attributes. Use the Delete Attribute operation to
11                      delete the last value of the attribute (which
12                      deletes the complete attribute from the object).
13 
14                      For recurring attributes, use <SCROLL-DOWN> to
15                      scan through the other values. Enter the old and
16                      the new attribute value when performing the
17                      Modify Subtree operation.
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask1: Numeric OR Address mask (d20_noraddomask)               */
$ /* *********************************************************************** */

$set 94 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify an attribute with O/R Address Syntax of type
4 Numeric.
5 
6 A numeric O/R address is one that numerically identifies a user. It
7 identifies an administration management domain (ADMD), and a user
8 relative to it.
9 
10 This mask displays the following fields:
11 
12 Attribute name       Displays the name of the attribute, for example,
13                      MHS-O/R-Address.
14 
15 Country Name         Enter the name of the country of the ADMD that the
16                      ADMD name attribute field denotes.
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask2: Numeric OR Address mask (d20_noraddomask)               */
$ /* *********************************************************************** */

$set 95 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 ADMD Name            Enter an administration domain name that identifies
4                      an ADMD relative to the country denoted by the
5                      country name.
6 
7 PRMD Name            Enter  a private domain name that identifies a
8                      private management domain (PRMD). As a national
9                      matter this identification may be either relative
10                      to the country denoted by a country name (so that
11                      PRMD names are unique within the country), or
12                      relative to the ADMD identified by an
13                     administration domain name.
14 
15 Numeric User Identifier
16                      Enter a numeric user identifier that numerically
17                      identifies a user relative to the ADMD denoted by
18                      an administration domain name.
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask3: Numeric OR Address mask (d20_noraddomask)               */
$ /* *********************************************************************** */

$set 96 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Domain Type1,Domain Type2, ...
4                      Enter the name of a class of information.
5 Domain Value1,Domain Value2, ...
6                      Enter an instance of the class of information that
7                      the preceding Domain Type denotes. Note that
8                      widespread use of standard attributes produces
9                      more uniform and thus more user-friendly O/R
10                      addresses. However, it is aniticipated that not
11                      all LManagement Domains (MDs) will be able to
12                      employ such attributes immediately. The purpose
13                      of domain-defined attributes is to permit an MD
14                      to retain its existing, native addressing
15                      conventions for a time. It is intended, however,
16                      that all MDs will migrate toward the use of
17                      standard attributes, and that domain-defined
18                      attributes will only be used for an interim period.
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask4: Numeric OR Address mask (d20_noraddomask)               */
$ /* *********************************************************************** */

$set 97 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Modification         Select one of the following options by pressing
4                      the space bar (shown only when you have selected
5                      the Modify Attribute operation):
6 
7                      Modify Value   Delete Value   Add Value
8 
9                      Use Delete Value and Add Value only for recurring
10                      attributes. Use the Delete Attribute operation to
11                      delete the last value of the attribute (which
12                      deletes the complete attribute from the object).
13 
14                      For recurring attributes, use <SCROLL-DOWN> to
15                      scan through the other values. Enter the old and
16                      the new attribute value when performing the
17                      Modify Subtree operation.
18 
19 
20 To continue press <CR> !


$ /* *********************************************************************** */
$ /*     Help mask5: Numeric OR Address mask (d20_noraddomask)               */
$ /* *********************************************************************** */

$set 98 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify an attribute with O/R Address Syntax of type 
4 Structured Postal.
5 
6 A postal O/R address is one that identifies a user by means of a postal
7 address. This mask displays the following fields:
8 
9 Attribute name        Displays the name of the attribute, for example,
10                      MHS-O/R-Address.
11 
12 Country Name         Enter the name of the country of the ADMD that the
13                      ADMD name attribute field denotes.
14 
15 ADMD Name            Enter an administration domain name that
16                      identifies an ADMD relative to the country denoted
17                      by the country name.
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask6: Numeric OR Address mask (d20_noraddomask)               */
$ /* *********************************************************************** */

$set 99 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 PRMD Name            Enter a private domain name that identifies a
4                      private management domain (PRMD). As a national
5                      matter this identification may be either relative
6                      to the country denoted by a country name (so that
7                      PRMD names are unique within the country), or
8                      relative to the ADMD identified by an
9                      administration domain name.
10 
11 Postal Country Name
12                      Enter the name of the country in which the user
13                      receives physical messages.
14 
15 
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask7: Numeric OR Address mask (d20_noraddomask)               */
$ /* *********************************************************************** */

$set 100 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Postal code          Enter the postal code for the geographical area in
4                      which the user receives physical messages. It
5                      identifies the area relative the country denoted
6                      by the Postal Country Name attribute. Its values
7                      are defined by the postal administration of that
8                      country.
9 
10 Postal Address Details
11                      Enter additional information that may be useful,
12                      for identifying the exact point at which the user
13                      receives physical messages, for example, room and
14                      floor numbers in a large building.
15 
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask8: Numeric OR Address mask (d20_noraddomask)               */
$ /* *********************************************************************** */

$set 101 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Postal Delivery Point Name
4                      Identify the locus of distribution of the user's
5                      physical messages, other than that denoted by the
6                      Postal Office Name attribute, for example, a
7                      geographical area.
8 
9 Postal Delivery System
10                      Enter the name of the postal delivery system
11                      through which the user is to receive physical
12                      messages.
13 
14 Postal General Delivery Address
15                      Enter the code that users give to the post office
16                      denoted by the Postal Office Name attribute for
17                      collection of the physical messages awaiting
18                      delivery to them.
19 
20 To continue press <CR> !



$ /* *********************************************************************** */
$ /*     Help mask9: Numeric OR Address mask (d20_noraddomask)               */
$ /* *********************************************************************** */

$set 102 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Postal Locale        Identify the point of delivery of the user's
4                      physical messages other than that denoted by the
5                      General Delivery Address, Postal Office Box
6                      Number, and  Postal Street Address attributes, for
7                      example, a building or small village.
8 
9 Postal Office Box Number
10                      Enter the number of the post office box where the
11                      user receives physical mesages. The box is located
12                      at the post office denoted by the Postal Office
13                      Name attribute.
14 
15 Postal Office Name   Enter the name of the municipality; for example,
16                      city or village where the office in which the user
17                      receives physical messages is located.
18 
19 
20 To continue press <CR> !


$ /* *********************************************************************** */
$ /*     Help mask10: Numeric OR Address mask (d20_noraddomask)              */
$ /* *********************************************************************** */

$set 103 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Postal Office Number Enter a number for the means of distinguishing
4                      between serveral post offices denoted by the
5                      Postal Office Name attribute.
6 
7 Postal Organization Number
8                      Enter the name of the postal organization through
9                      which the user receives physical messages.
10 
11 Postal Patron Details
12                      Enter any additional information required to
13                      identify the user for purposes of physical
14                      delivery; for example the name of the
15                      organizational unit through which the user
16                      receives physical messages.
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask11: Numeric OR Address mask (d20_noraddomask)              */
$ /* *********************************************************************** */

$set 104 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Postal Patron Name   Enter the name under which the user receives
4                      physical messages.
5 
6 Postal Street Address
7                      Enter the street address where the user receives
8                      physical messages; for example, 43 Primrose Lane
9 
10 Modification         Select one of the following options by pressing
11                      the space bar (shown only when you have selected
12                      the Modify Attribute operation):
13 
14                      Modify Value    Delete Value    Add Value
15 
17 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask12: Numeric OR Address mask (d20_noraddomask)              */
$ /* *********************************************************************** */

$set 105 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      Use Delete Value and Add Value only for recurring
4                      attributes. Use the Delete Attribute operation to
5                      delete the last value of the attribute (which
6                      deletes the complete attribute from the object).
7 
8                      For recurring attributes, use <SCROLL-DOWN> to 
9                      scan through the other values. Enter the old and
10                      the new attribute value when performing the
11                      Modify Subtree operation.
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /*    Help mask1: Structured Postal OR Address mask (d20_soraddomask)      */
$ /* *********************************************************************** */

$set 106 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify an attribute with O/R Address Syntax of type
4 Unstructured Postal.
5 
6 This mask displays the following fields:
7 
8 Attribute name       Displays the name of the attribute, for example,
9                      MHS-O/R-Address.
10 
11 Country Name         Enter the name of the country of the ADMD that the
12                      ADMD name attribute field denotes.
13 
14 ADMD Name            Enter an administration domain name that identifies
15                      an ADMD relative to the country denoted by the
16                      country name.
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: Structured Postal OR Address mask (d20_soraddomask)      */
$ /* *********************************************************************** */

$set 107 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 PRMD Name            Enter  a private domain name that identifies a
4                      private management domain (PRMD). As a national
5                      matter this identification may be either relative
6                      to the country denoted by a country name (so that
7                      PRMD names are unique within the country), or
8                      relative to the ADMD identified by an
9                      administration domain name.
10 
11 Postal Country Name
12                      Enter the name of the country in which the user
13                      receives physical messages.
14 
15 Postal code          Enter the postal code for the geographical area in
16                      which the user receives physical messages. It
17                      identifies the area relative the country denoted
18                      by the Postal Country Name attribute. Its values
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask3: Structured Postal OR Address mask (d20_soraddomask)      */
$ /* *********************************************************************** */

$set 108 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      are defined by the postal administration of that
4                      country.
5 
6 Postal Address in Full
7                      Enter the free-form and possibly multi-line postal
8                      address of the user.
9 
10 Postal Address in Lines (Part 1), (Part 2), ...
11                      Enter the free-form postal address of the user in
12                      a sequence of printable strings, each representing
13                      a line of text.
14 
15 Postal Delivery System
16                      Enter the name of a postal delivery system through
17                      which the user receives physical messages.
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask4: Structured Postal OR Address mask (d20_soraddomask)      */
$ /* *********************************************************************** */

$set 109 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Modification         Select one of the following options by pressing
4                      the space bar (shown only when you have selected
5                      the Modify Attribute operation):
6 
7                      Modify Value    Delete Value    Add Value
8 
9                      Use Delete Value and Add Value only for recurring
10                      attributes. Use the Delete Attribute operation to
11                      delete the last value of the attribute (which
12                      deletes the complete attribute from the object).
13 
14                      For recurring attributes, use <SCROLL-DOWN> to
15                      scan through the other values. Enter the old and
16                      the new attribute value when performing the
17                      Modify Subtree operation.
18 
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /*    Help mask1: Unstructured Postal OR Address mask (d20_uoraddomask)    */
$ /* *********************************************************************** */

$set 110 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify an attribute with O/R Address Syntax of type
4 Terminal.
5 
6 A terminal O/R address identifies a user by means of the network
7 address and, if required, the type of terminal. It may also identify
8 the ADMD through which that terminal is accessed. In the case of a
9 telematic terminal, it specifies the terminal's network address and,
10 if applicable, its terminal identifier and terminal type. In the case
11 of a telex terminal, it specifies its telex number.
12 
13 This mask displays the following fields:
14 
15 Attribute name       Displays the name of the attribute, for example,
16                      MHS-O/R-Address.
17 
18 ISDN Number          Enter the ISDN numer of the user's terminal.
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: Unstructured Postal OR Address mask (d20_uoraddomask)    */
$ /* *********************************************************************** */

$set 111 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 ISDN Subaddress      Enter the ISDN subaddress of the user's terminal,
4                      if any.
5 
6 Presentation Address
7                      If you select, YES, Mask 7a is displayed for
8                      entering the presentation address of the user's
9                      terminal. If you select, NO, a presentation
10                      address is not included in the O/R address.
11 
12 X121 Address         Enter the network address of the user's terminal.
13 
14 Country Name         Enter the name of the country of the ADMD that the
15                      ADMD name attribute field denotes.
16 
17 
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask3: Unstructured Postal OR Address mask (d20_uoraddomask)    */
$ /* *********************************************************************** */

$set 112 preload

1 (Help mask)         DIRECTORY SYSTEM
2 
3 ADMD Name            Enter an administration domain name that
4                      identifies an ADMD relative to the country denoted
5                      by the country name.
6 
7 PRMD Name            Enter  a private domain name that identifies a
8                      private management domain (PRMD). As a national
9                      matter this identification may be either relative
10                      to the country  denoted by a country name (so that
11                      PRMD names are unique within the country), or
12                      relative to the ADMD identified by an
13                      administration domain name.
14 
15 Terminal Identifier  Enter the Terminal Identifier of a terminal; for
16                      example, a telex reply.
17 
18 
19 
20 To continue press <CR> !



$ /* *********************************************************************** */
$ /*    Help mask4: Unstructured Postal OR Address mask (d20_uoraddomask)    */
$ /* *********************************************************************** */

$set 113 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Terminal Type        Select one of the following terminal types by
4                      pressing the space bar:
5 
6                      Not used (default)              G3 FACSIMILE
7                      TELEX                           G4 FACSIMILE
8                      TELETEX                         IA5 TERMINAL
9 
10 Domain Type1,Domain Type2, ...
11                      Enter the name of a class of information.
12 
13 Domain Value1,Domain Value2, ...
14                      Enter an instance of the class of information that
15                      the preceding Domain Type denotes. Note that
16                      widespread use of standard attributes produces
17                      more uniform and  thus more user-friendly O/R
18                      addresses. However, it is aniticipated that not
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask5: Unstructured Postal OR Address mask (d20_uoraddomask)    */
$ /* *********************************************************************** */

$set 114 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      all Management Domains (MDs) will be able to
4                      employ such attributes immediately. The purpose
5                      of domain-defined attributes is to permit an MD
6                      to retain its existing, native addressing
7                      conventions for a time. It is intended, however,
8                      that all MDs will migrate toward the use of
9                      standard attributes, and that domain-defined
10                      attributes will only be used for an interim period.
11 
12 Modification         Select one of the following options by pressing
13                      the space bar (shown only when you have selected
14                      the Modify Attribute operation):
15 
16                      Modify Value   Delete Value   Add Value
17 
18 
19 
20 To continue press <CR> !


$ /* *********************************************************************** */
$ /*    Help mask6: Unstructured Postal OR Address mask (d20_uoraddomask)    */
$ /* *********************************************************************** */

$set 115 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      Use Delete Value and Add Value only for recurring
4                      attributes. Use the Delete Attribute operation to
5                      delete the last value of the attribute (which
6                      deletes the complete attribute from the object).
7 
8                      For recurring attributes, use <SCROLL-DOWN> to
9                      scan through the other values. Enter the old and
10                      the new attribute value when performing the
11                      Modify Subtree operation.
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /* Help mask1: Terminal OR Address mask (d20_toraddomask)                  */
$ /* *********************************************************************** */

$set 116 preload                  

1 (Help mask )          DIRECTORY SYSTEM
2 
3 This mask handles attributes with O/R Address Syntax of type Terminal.
4 
5 The value shown in O/R Address Type field tells the type of O/R Address
6 being handled. It is a toggle field if you have selected 'Modify
7 Attribute' operation. To specify the type of new O/R Address value,
8 use space bar to select one of the following types:
9 
10      - Mnemonic
11      - Numeric
12      - Structured Postal
13      - Unstructured Postal
14      - Terminal
15 
16 For each type of O/R address their is a special mask which follows this
17 mask.
18 The Modification field is shown only when you have selected 'Modify
19 Attribute' operation. It is a toggle field and one of the following
20 To continue pres <CR> !

$ /* *********************************************************************** */
$ /* Help mask2: Terminal OR Address mask (d20_toraddomask)                  */
$ /* *********************************************************************** */

$set 117 preload

1 (Help mask )          DIRECTORY SYSTEM
2 
3 options can be selected using space bar:
4      - Modify Value (Administrator should overwrite on old value. This
5 option leads to modification of the old value by new value)
6      - Delete Value (Deletes the displayed value of attribute)
7      - Add Value (Administrator should overwrite on old value. This
8 option leads to addition of new value for the attribute)
9 
10 'Delete Value' and 'Add Value' make sense only for recurring attributes.
11 To delete the last value of the attribute, use 'Delete Attribute'
12 operation which deletes the complete attribute from the object. Use
13 <SCROLL-DOWN> to scan thru. the other values (In case of recurring
14 attribute) of the attribute.
15 
16 To understand semantics of other fields, please refer to admin. manual.
17 
18 
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /*    Help mask: DL Submit perm. mask (d20_dlpermomask)                    */
$ /* *********************************************************************** */

$set 118 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify an attribute with MHS DL Submit Permission 
4 Syntax.
5 
6 Mask 33 displays the following fields:
7 
8 Attribute name       Displays the name of the attribute; for example,
9                      MHS-DL-Submit-Permission.
10 
11 DL Submit Permission Type:
12                      Select one of the following by pressing the space bar:
13 
14                      Individiual             Displays Mask 34
15                      Member of DL            Displays Mask 34
16                      Pattern Match           Displays Mask 34
17                      Member of Group         Displays Mask 35
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask1: OR Name mask (d20_ornameomask)                           */
$ /* *********************************************************************** */

$set 119 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify an attribute with MHS O/R Name Syntax or MHS 
4 DL Submit Permission Syntax and Individual, Member Of DL or Member
5 Of Group as one of the types.
6 
7 Mask 34 displays the following fields:
8 
9 Attribute name       Displays the name of the attribute; for example,
10                      MHS-DL-Members.
11 
12 DL Submit Permission Type (if the operation is Modify Attribute)
13                      Select one of the following by pressing the space
14                      bar:
15 
16                      Individual        Member of DL
17                      Pattern Match     Member Group
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: OR Name mask (d20_ornameomask)                           */
$ /* *********************************************************************** */

$set 120 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 When Member of Group is selected Mask 35 is displayed.  Depending on
4 on the current value of the attribute, the default value can be one of
5 the first three options. This field is a display field when the
6 operation is Add Object or Add Attribute. It is a toggle field when the
7 operation in Modify Attribute.
8 
9 Directory Name       Enter the name assigned to the user or DL by the
10                      worldwide (X.500) directory. It has Distinguished
11                      Name syntax.
12 
13 O/R Address          Select YES or NO by pressing the space bar. O/R
14                      address masks are displayed to handle O/R
15                      address. NO is the default value. One of the two
16                      fields (Directory Name, O/R Address) must be
17                      present.
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask3: OR Name mask (d20_ornameomask)                           */
$ /* *********************************************************************** */

$set 121 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Modification         Select one of the following options by pressing
4                      the space bar (shown only when you have selected
5                      the Modify Attribute operation):
6 
7                      Modify Value    Delete Value   Add Value
8 
9                      Use Delete Value and Add Value only for recurring
10                      attributes. Use the Delete Attribute operation to
11                      delete the last value of the attribute (which
12                      deletes the complete attribute from the object).
13 
14                      For recurring attributes, use <SCROLL-DOWN> to
15                      scan through the other values. Enter the old and
16                      the new attribute value when performing the
17                      Modify Subtree operation.
18 
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /*    Help mask1: Member Of Group mask (d20_mogomask)                      */
$ /* *********************************************************************** */

$set 122 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Use this mask to specify an attribute with MHS DL Submit Permission
4 Syntax and Member Of Group as type.
5 
6 Mask 35 displays the following fields:
7 
8 
9 Attribute name       Displays the name of the attribute; for example,
10                      MHS-DL-Submit-Permission.
11 
12 DL Submit Permission Type (if the operation is Modify Attribute)
13                      Select one of the following by pressing the space
14                      bar:
15 
16                      Individual        Member of DL
17                      Pattern Match     Member Group
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask2: Member Of Group mask (d20_mogomask)                      */
$ /* *********************************************************************** */

$set 123 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Member of Group     Enter each member of the group of names whose
4                     name is specified, or of each nested group of
5                     names recursively. It has Distinguished Name
6                     syntax.
7 
8 Modification        Select one of the following options by pressing the
9                     space bar (shown only when you have selected
10                      the Modify Attribute operation):
11 
12                      Modify Value   Delete Value   Add Value
13 
14                      Use Delete Value and Add Value only for recurring
15                      attributes. Use the Delete Attribute operation to
16                      delete the last value of the attribute (which
17                      deletes the complete attribute from the object).
18 
19 
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*    Help mask3: Member Of Group mask (d20_mogomask)                      */
$ /* *********************************************************************** */

$set 124 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3                      For recurring attributes, use <SCROLL-DOWN> to
4                      scan through the other values. Enter the old and
5                      the new attribute value when performing the
6                      Modify Subtree operation.
7 
8 
9 
10 
11 
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !


$ /* *********************************************************************** */
$ /*   Help mask: DME NMO Junction attribute (d20_dmeomask / mask 39)        */
$ /* *********************************************************************** */

$set 125 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask is used  to display and add attribute values  for an alternate
4 address.
5 
6 An alternate address consists of an  address and  object identifiers. An
7 object  identifier  should  be entered with values separated with points.
8 For example: 85.4.3.12
9 
10 In case of a 'Display Objects' operation the values  cannot  be  edited.
11 For  more  information  on  dialog  control  refer to the administration
12 manual.
13 
14 In case of an 'Add Object' operation  a  value - at least  an  address -
15 should be entered. No ' (apostrophe) is  needed. The  values  cannot  be
16 modified later.  Also,  additional attributes cannot be added er deleted
17 later.
18 
19 
20 To exit help press <CR> !



$ /* *********************************************************************** */
$ /*   Help mask1: Certificate attribute (d20_certomask / mask 36)           */
$ /* *********************************************************************** */

$set 126 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask is used to display attribute with Certificate List syntax.
4 
5 Version                Version number of the certificate
6 
7 Serial Number          Serial number of the certificate issued by a
8                        certification authority
9 
10 Signature              Algorithm and Parameter used by the signature
11                        to sign the certificate.
12 
13 Issuer                 Distinguished Name of the certification
14                        authority which issued the certificate
15 
16 Validity               Laps of time where the certificate is valid
17 
18 
19 
20 To continue press <CR> !



$ /* *********************************************************************** */
$ /*   Help mask2: Certificate attribute (d20_certomask / mask 36)           */
$ /* *********************************************************************** */

$set 127 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Subject                Distinguished Name of the user who has been
4                        assigned the certificate by the certification
5                        authority
6 
7 Subject Public Key
8   Algorithm Identifier Algorithm and Parameter used for the public key
9 
10 Modification           Present only in case of 'Modify Attribute'
11                        Only 'Delete Value' is allowed.
12 
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !



$ /* *********************************************************************** */
$ /*   Help mask: Certificate Pair attribute (d20_certpomask / mask 37)      */
$ /* *********************************************************************** */

$set 128 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask is used to display if a forward certificate or a reverse
4 certificate are present in an attribute with Certificate Pair syntax.
5 
6 Forward Certificate    Field with YES or NO depending if there is
7                        some forward certificate(s) or not
8 
9 Reverse Certificate    Field with YES or NO depending if there is
10                        some reverse certificate(s) or not
11 
12 Modification           Present only in case of 'Modify Attribute'
13                        Only 'Delete Value' is allowed.
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !



$ /* *********************************************************************** */
$ /*   Help mask: Certificate List attribute (d20_certlomask / mask 38a)     */
$ /* *********************************************************************** */

$set 129 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask is used to display attributes with Certificate List syntax.
4 
5 Signature              Algorithm and Parameter used by the signature
6                        to sign the certificate.
7 
8 Issuer                 Distinguished Name of the certification
9                        authority which issued the certificate
10 
11 Last Update            Date and time of the last update of the
12                        certificate by the certification authority
13 
14 Revoked Certificates   Field with YES or NO depending if there is
15                        some revoked certificate(s) or not
16 
17 Modification           Present only in case of 'Modify Attribute'
18                        Only 'Delete Value' is allowed.
19 
20 To exit help press <CR> !



$ /* *********************************************************************** */
$ /*   Help mask1: Revoked Certificate attribute (d20_rcertomask / mask 38b) */
$ /* *********************************************************************** */

$set 130 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 This mask is used to display attributes with Revoked Certificate syntax.
4 
5 Signature              Algorithm and Parameter used by the signature
6                        to sign the certificate.
7 
8 Issuer                 Distinguished Name of the certification
9                        authority which issued the certificate
10 
11 Serial Number          Serial number of the certificate
12 
13 Revocation Date        Date and time of the last update of the
14                        certificate by the certification authority
15 
16 Revoked Certificates   Field with YES or NO depending if there is
17                        some other revoked certificate(s) or not
18 
19 
20 To continue press <CR> !



$ /* *********************************************************************** */
$ /*   Help mask: First part of the logon mask (d20_secomask / Logon)        */
$ /* *********************************************************************** */

$set 131 preload

1 (Help mask)           DIRECTORY SYSTEM
2 
3 Enter information required to bind to the directory. When you run the
4 gdscacheadm process, only the directory ID is required (the intention to
5 bind to the cache is implied).
6 
7 Bind data consists of the following information:
8 
9 Directory Identifier (Directory-ID):
10 
11   Select one of the configured directory IDs (an integer 1 through 20)
12 
13 Authentication mechanism to be used
14 
15    Select an authentication mechanism. Following values are valid:
16        Anonymous, Simple Unprotected,
17        DCE Authentication
18    You can toggle with the space bar
19 
20 To exit help press <CR> !
