$ 
$ @OSF_COPYRIGHT@
$ COPYRIGHT NOTICE
$ Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
$ ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
$ src directory for the full copyright text.
$ 
$ HISTORY
$ $Log: d20admin.msg,v $
$ Revision 1.1.10.2  1996/03/09  23:14:08  marty
$ 	Update OSF copyright year
$ 	[1996/03/09  22:31:27  marty]
$
$ Revision 1.1.10.1  1995/12/08  16:04:31  root
$ 	Submit OSF/DCE 1.2.1
$ 	[1995/12/08  15:15:45  root]
$ 
$ Revision 1.1.8.5  1994/07/06  15:07:46  marrek
$ 	July 1994 code submission.
$ 	[1994/07/04  13:17:34  marrek]
$ 
$ Revision 1.1.8.4  1994/06/21  14:47:33  marrek
$ 	June 1994 code submission.
$ 	[1994/06/16  09:42:32  marrek]
$ 
$ Revision 1.1.8.3  1994/05/10  15:56:56  marrek
$ 	Bug fixes.
$ 	[1994/04/29  09:29:53  marrek]
$ 
$ 	April 1994 code submission.
$ 	[1994/04/27  12:41:39  marrek]
$ 
$ Revision 1.1.8.2  1994/03/23  15:19:09  keutel
$ 	March 1994 code drop
$ 	[1994/03/21  16:32:53  keutel]
$ 
$ Revision 1.1.8.1  1994/02/22  17:41:20  marrek
$ 	Code drop for dce1.1.
$ 	[1994/02/08  10:56:58  marrek]
$ 
$ Revision 1.1.6.3  1993/08/11  15:07:09  marrek
$ 	July 1993 code drop.
$ 	[1993/08/02  13:01:59  marrek]
$ 
$ Revision 1.1.6.2  1993/07/02  15:09:46  marrek
$ 	Changes for port to HP800.
$ 	[1993/07/02  14:38:22  marrek]
$ 
$ Revision 1.1.4.5  1993/01/25  10:14:13  marrek
$ 	Add comment leader
$ 	[1993/01/14  15:20:37  marrek]
$ 
$ $EndLog$
$
$ $RCSfile: d20admin.msg,v $ $Revision: 1.1.10.2 $ $Date: 1996/03/09 23:14:08 $
$
$set 1 preload              $ /* approximate length of input values */
$                             /* *sccsid = @(#)d20admin.msg	7.5 91/07/22 (K Sys AP 11) */
1 10000

$set 2 preload              $ /* login mask (part 1) */
1  24 43 NOTE: Delta-update operation failed !# @#
2   2  5 (diradm)# @#
3   2 32 DIRECTORY SYSTEM# @#
4   5 14 a - Administration of the directory information tree# @#
5   7 14 c - Configuration of a directory system#  @#
6   9 14 b - Activation of a directory system installation# @#
7  11 14 d - Deactivation of a directory system installation# @#
8  13 14 s - Saving of local data to diskette/tape/file# @#
9  15 14 r - Restoring of saved data from diskette/tape/file# @#
10 17 14 f - Further functions# @#
11 23  2 Your selection! > # @#
12 23 20 @# @#

$set 3 preload              $ /* login mask (part 2) */
1   2  5 (diradm)# @#
2   2 32 DIRECTORY SYSTEM# @#
3   9 14 i - Display of directory system status information# @#
4  11 14 l - Activation of the 'trace' system# @#
5  13 14 t - Deactivation of the 'trace' system# @#
6  23  2 Your selection! > # @#
7  23 20 @# @#

$set 4 preload              $ /* menue parameters */
1  2  5 menupar# @#
2  2 32 DIRECTORY SYSTEM# @#
3  2 61 Parameters# @#
4 14 12 Which language do you prefer ?# @#
5 14 44 @# %.0=german   .1=english  .2=french   .3=italian  .4=spanish  .5=danish   .6=dutch    .7=norwegian.8=swedish  .a#

$set 5 preload              $ /* state info mask (1) */
1 2  5 (Info)# @#
2 2 32 DIRECTORY SYSTEM# @#
3 2 60 Status Information# @#

$set 6 preload              $ /* state info mask (2) */
1 24 2 To continue please press <CR># @#

$set 7 preload              $ /* configuration mask (1) */
1 2  5 (confpar)# @#
2 2 32 DIRECTORY SYSTEM# @#
3 2 64 Configuration# @#
4 5  8 Which configuration mode ?:# @#
5 5 36 @# %.1=Creation of configuration data.2=Deletion of configuration data.3=Display of configuration data .4=Changing of configuration data.a#

$set 8 preload              $ /* configuration mask (2) */
1  7  8 For which directory ID should the operation be# @#
2  8  8 performed ? [1-20]:# @#
3  8 28 @# %.1=1 .2=2 .3=3 .4=4 .5=5 .6=6 .7=7 .8=8 .9=9 .10=10.11=11.12=12.13=13.14=14.15=15.16=16.17=17.18=18.19=19.20=20.a#

$set 9 preload              $ /* configuration mask (2d) */
1  7  8 For which directory ID should the operation be# @#
2  8  8 performed ? [2-20]:# @#
3  8 28 @# %.2=2 .3=3 .4=4 .5=5 .6=6 .7=7 .8=8 .9=9 .10=10.11=11.12=12.13=13.14=14.15=15.16=16.17=17.18=18.19=19.20=20.a#

$set 10 preload              $ /* configuration mask (3) */
1 10  8 Which configuration type ?:# @#
2 10 36 @# %.0=Client system              .a#
3 12  8 How many clients have access to the directory system at the# @#
4 13  8 same time ? [1-256]:# @#
5 13 29 @# @#

$set 11 preload              $ /* configuration mask (4) */
1 15  8 How many server processes should be activated ? [1-256]:# @#
2 15 65 @# @#
3 17  8 Authentication mechanism that the DSA supports ?:# @#
4 17 57 @# %.2=Simple Unprotected   .5=DCE Authentication   .25=Simple + DCE        .a#

$set 12 preload             $ /* configuration mask (5) */
1  2  5 (confpar)# @#
2  2 32 DIRECTORY SYSTEM# @#
3  2 64 Configuration# @#

$set 13 preload             $ /* saving of data (part 0) */
1  2  5 (savepar)# @#
2  2 32 DIRECTORY SYSTEM# @#
3  2 64 Save Data# @#
4  8 10 For which directory ID do you wish to# @#
5  9 10 save the local data ? [1-20]:# @#
6  9 40 @# %.1=1 .2=2 .3=3 .4=4 .5=5 .6=6 .7=7 .8=8 .9=9 .10=10.11=11.12=12.13=13.14=14.15=15.16=16.17=17.18=18.19=19.20=20.a#
7 11 10 On which medium do you wish to save the local data ?# @#
8 11 64 @# %.0=Diskette.1=Tape    .2=File    .a#

$set 14 preload             $ /* saving of data (part 1.1) */
1 13 10 Security password, if required:# @#
2 13 42 @# @#
3 15 10 Do you wish to format the media ?# @#
4 15 44 @# %.0=No .1=Yes.a#

$set 15 preload             $ /* saving of data (part 1.2) */
1 13 10 Name of the file : # @#
2 13 29 @# @#
3 15 10 Security password, if required:# @#
4 15 42 @# @#

$set 16 preload             $ /* saving of data (part 1) */
1 2  5 (savepar)# @#
2 2 32 DIRECTORY SYSTEM# @#
3 2 64 Save Data# @#

$set 17 preload             $ /* saving of data (part 2) */
1 23 2  $                                                                  # @#
2 24 2  $                                                                  # @#
3 19 10 Insert %2d. media volume and then press the# @#
4 20 10 Return-key to start the operation.# @#

$set 18 preload             $ /* saving of data (part 3) */
1 23 2 Creating file list ...# @#

$set 19 preload             $ /* saving of data (part 4) */
1 17 10 The backup requires %2d media volume(s).# @#

$set 20 preload             $ /* saving of data (part 5) */
1 23 2 Writing data to media volume %2d ...# @#

$set 21 preload             $ /* saving of data (on file) (part 5) */
1 23 2 Writing data ...# @#

$set 22 preload             $ /* restoring of data (part 0) */
1 2  5 (respar)# @#
2 2 32 DIRECTORY SYSTEM# @#
3 2 64 Restore Data# @#
4 6 10 For which directory ID do you wish to restore the# @#
5 7 10 local data ? [1-20]:# @#
6 7 31 @# %.1=1 .2=2 .3=3 .4=4 .5=5 .6=6 .7=7 .8=8 .9=9 .10=10.11=11.12=12.13=13.14=14.15=15.16=16.17=17.18=18.19=19.20=20.a#
7 9 10 From which medium should the data be read ?:# @#
8 9 55 @# %.0=Diskette.1=Tape    .2=File    .a#

$set 23 preload             $ /* restoring of data (part 1.1) */
1 11 10 Security password:# @#
2 11 29 @# @#
3 13 10 Attention: Your existing data will be overwritten !!!# @#

$set 24 preload             $ /* restoring of data (part 1.2) */
1 11 10 Name of the file : # @#
2 11 29 @# @#
3 13 10 Security password:# @#
4 13 29 @# @#
5 15 10 Attention: Your existing data will be overwritten !!!# @#

$set 25 preload             $ /* restoring of data (part 1) */
1 2  5 (respar)# @#
2 2 32 DIRECTORY SYSTEM# @#
3 2 64 Restore Data# @#

$set 26 preload             $ /* restoring of data (part 2) */
1 23 2  $                                                                  # @#
2 24 2  $                                                                  # @#
3 17 10 Insert %2d. media volume and then press the# @#
4 18 10 Return-key to start the operation.# @#

$set 27 preload             $ /* restoring of data (part 3) */
1 15 10 The backup consists of %2d media volume(s).# @#

$set 28 preload             $ /* restoring of data (part 4) */
1 23 2 Reading data from media volume %2d ...# @#

$set 29 preload             $ /* restoring of data (from file) (part 4) */
1 23 2 Reading data ...# @#

$set 30 preload             $ /* saving/restoring of data (on/from file) */
1 23 2  $                                                                  # @#
2 24 2  $                                                                  # @#
3 18 10 Press the Return-key to start the operation.# @#

$set 31 preload             $ /* error mask */
1 23 2  $                                                                            # @#
2 23 2 @# @#
3 24 2 To continue please press <CR># @#

$set 32 preload             $ /* configuration type */
1 %.0=Client system             .1=Client-Server system      .a
2 %.0=Client system             .a
3 %.0=No .a
4 %.0=No .1=Yes.a

$set 33 preload             $ /* error messages */
1 Error:
2 Error: The selected function can't be executed !
3 Error: The selected function can't be executed !
4 Error: The directory system is active !
5 Error: The directory system isn't configured !
6 Error: The directory system isn't active !
7 Error: Configuration information doesn't exist !
8 Error: Configuration information already exists !
9 Error: Can't send distributed command to directory system processes !
10 Error: Wrong selection !
11 Error: The directory system is still in use !
12 Error: Can't read file list from media volume !
13 Error: Wrong media volume label !
14 Error: Wrong user key !
15 Error: This directory identifier isn't configured !
16 Error: Invalid configuration information !
17 Error: Can't format media volume !
18 Error: Can't write data to media volume !
19 Error: Can't read data from media volume !
20 Error: Can't read file list !
21 Error: The selected language isn't installed !
22 Error: One file exceeds the size of the media volume !
23 Error: Can't find any files which can be saved !
24 Error: The selected directory system installation doesn't exist !
25 Error: No permission to run DIR-X !
26 Error: Important Directory-process cannot be started !
27 Warning: The restored data base doesn't agree to the DSA !
28 Warning: The restored data base doesn't agree to the DUA-cache !
29 Error: Invalid input !
30 Error: The selected database system is unknown !
31 Error: The process gdsipcinit cannot be started !
32 Error: The process gdsipcchk cannot be started !
33 Error: The process gdscache cannot be started !
34 Error: The process gdscstub cannot be started !
35 Error: The process gdssstub cannot be started !
36 Error: The process gdsdsa cannot be started !
37 Error: Can't write data to file !
38 Error: Can't read data from file !
39 Error: File doesn't exist !
40 Error: Can't read file list from file !
41 Error: Can't execute administration !
42 Error: The selected function is executed by somebody else !
43 Warning: Conversion to the local string format fails !
44 One of the fields could not be converted to the T61 string !
45 Error: Invalid command line parameter
46 Error: Invalid directory ID
47 Error: Cannot change directory
48 Error: Cannot fork
49 Error: Key error
50 Error: Invalid count of maximum IPC associations
51 Error: Invalid count of maximum ROS associations
52 Error: Invalid authentication mechanism
53 Error: Invalid interval time
