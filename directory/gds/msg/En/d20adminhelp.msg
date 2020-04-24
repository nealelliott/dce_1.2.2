$
$ @OSF_COPYRIGHT@
$ COPYRIGHT NOTICE
$ Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
$ ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
$ src directory for the full copyright text.
$
$ HISTORY
$ $Log: d20adminhelp.msg,v $
$ Revision 1.1.576.2  1996/03/09  23:14:10  marty
$ 	Update OSF copyright year
$ 	[1996/03/09  22:31:29  marty]
$
$ Revision 1.1.576.1  1995/12/08  16:04:35  root
$ 	Submit OSF/DCE 1.2.1
$ 	[1995/12/08  15:15:48  root]
$ 
$ Revision 1.1.574.3  1994/07/06  15:07:48  marrek
$ 	July 1994 code submission.
$ 	[1994/07/04  13:17:37  marrek]
$ 
$ Revision 1.1.574.2  1994/02/22  17:41:21  marrek
$ 	Code drop for dce1.1.
$ 	[1994/02/08  10:57:08  marrek]
$ 
$ Revision 1.1.574.1  1994/01/13  09:18:22  marrek
$ 	Help masks rewritten and redesigned.
$ 	[1994/01/12  11:14:45  marrek]
$ 
$ Revision 1.1.4.2  1993/08/11  15:08:11  marrek
$ 	July 1993 code drop.
$ 	[1993/08/02  13:02:09  marrek]
$ 
$ Revision 1.1.2.5  1993/02/02  10:19:49  marrek
$ 	Change help masks.
$ 	[1993/02/02  09:43:09  marrek]
$ 
$ Revision 1.1.2.4  1993/01/25  10:14:18  marrek
$ 	Add comment leader
$ 	[1993/01/14  15:21:19  marrek]
$ 
$ $EndLog$
$
$ $RCSfile: d20adminhelp.msg,v $ $Revision: 1.1.576.2 $ $Date: 1996/03/09 23:14:10 $
$
$
$ /* *********************************************************************** */ $ /*     Help mask1: Logon mask (d20_1omask)                                 */
$ /* *********************************************************************** */
$set 1 preload

1  (Help mask)                       DIRECTORY SYSTEM
2 
3 Use the Cursor Up/Down and characters displayed on the left side to 
4 position the cursor bar. Press <CR> to select the function. 
5 
6 Enter one of the following options: 
7 
8 a - Administration of a directory system
9     Starts directory information tree administration process
10 
11 c - Configuration of a directory system
12     Adds, deletes, displays, and modifies directory configuration data
13 
14 b - Activation of a directory system installation
15     Starts the processes (DSAs, C-Stub, S-Stubs, DUA-Cache etc.)
16     of the directory system 
17 
18 d - Deactivation of a directory system installation
19     Kills the processes of the directory system
20 To continue press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask2: (diradm_mask1)                                          */
$ /* *********************************************************************** */
$set 2 preload

1  (Help mask)                       DIRECTORY SYSTEM
2 
3 s - Saving of local data to diskette/tape/file
4     Saves the local data of the chosen directory ID on a
5     diskette/tape/file
6 
7 r - Restoring of saved data from diskette/tape/file
8     Restores the local data of the chosen directory ID from
9     diskette/tape/file
10 
11 f - Further functions
12     Displays mask of additional administration functions
13 
14 
15 
16 
17 
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask1: (diradm_mask2)                                          */
$ /* *********************************************************************** */
$set 3 preload

1  (Help mask)                       DIRECTORY SYSTEM
2 
3 Cursor Up/Down and characters displayed on the left hand side can be
4 used to place the cursor bar. Press <CR> to select the function. 
5 
6 Selection of option: 
7 
8 i - Display directory system status information
9     Displays information about directory processes and the state
10     (active/inactive) of the 'trace' system
11 
12 l - Activate the 'trace' system
13     Activates logging for all directory process and removes old
14     log files
16 
17 t - Deactivate the 'trace' system
18     Deactivates logging for all directory processes
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask1: (confpar_mask)                                          */
$ /* *********************************************************************** */
$set 4 preload

1  (Help mask)                       DIRECTORY SYSTEM
2 
3  Use space bar to toggle between following options:
4 
5      - Display of configuration data
6      - Changing of configuration data
7      - Creation of configuration data
8      - Deletion of configuration data
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
$ /*     Help mask1: (conf1d_mask)                                           */
$ /* *********************************************************************** */
$set 5 preload

1  (Help mask)                       DIRECTORY SYSTEM
2 
3 
4 
5 
6 
7                       NO HELP PROVIDED !
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
$ /*     Help mask1: (conf1_mask)                                            */
$ /* *********************************************************************** */
$set 6 preload                  

1  (Help mask)                       DIRECTORY SYSTEM
2 
3 
4 
5 
6 
7                        NO HELP PROVIDED !
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
$ /*     Help mask1: (conf2_mask)                                            */
$ /* *********************************************************************** */
$set 7 preload

1  (Help mask)                       DIRECTORY SYSTEM
2 
3 Select the configuration type by pressing the space bar:
4 
5      - Client System
6      - Client-Server System
7 
8 Use <CR> to select one of the options.      
9 
10 A client system has a C-Stub process and a DUA-Cache process.
11 
12 A directory installation has one C-Stub and one DUA-Cache process
13 running. A client-server system additionally has server processes
14 (DSAs) and S-Stub process for the directory identifier.
15 
16 The number of client processes that can have access to the directory
17 system cannot exceed the number of binds to the directory system.
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask1: (conf3_mask)                                            */
$ /* *********************************************************************** */
$set 8 preload                  

1  (Help mask)                       DIRECTORY SYSTEM
2 
3 Determine the number of server processes that are activated based on
4 the minimum number of applications that will need to access the
5 directory server at the same time.
6 
7 The number of server processes activated is determined by the system
8 load.
9 
10 If the system load increases, additional server processes are activated
11 automatically. If the system load decreases, server processes are
12 terminated automatically. The number of running processes is never less
13 than the number of server processes you specify.
14 
15 Select the authentication mechanism by pressing the space bar:
16 
17        Simple Unprotected, DCE Authentication, Simple + DCE
18 
19 
20 To exit help press <CR> !

$ /* *********************************************************************** */
$ /*     Help mask1: (dirsi_mask)                                            */
$ /* *********************************************************************** */
$set 9 preload                  

1  (Help mask)                       DIRECTORY SYSTEM
2 
3  Select the type of medium on which you want to save local data by pressing
4  the space bar:
5 
6       - diskette
7       - tape
8       - file  
9 
10 Enter the option by pressing <CR>. 
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
$ /*     Help mask1: (direi_mask)                                            */
$ /* *********************************************************************** */
$set 10 preload                  

1  (Help mask)                       DIRECTORY SYSTEM
2 
3 Select the type of medium from which the date should be read by pressing
4  the space bar:
5 
6       - diskette
7       - tape
8       - file  
9 
10 Enter the option by pressing <CR>. 
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
$ /*     Help mask1: (dirsiA_mask)                                           */
$ /* *********************************************************************** */
$set 11 preload                  

1  (Help mask)                       DIRECTORY SYSTEM
2 
3 Enter a password (10 characters maximum) if you want to protect the data
4 files in your directory.
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
$ /*     Help mask1: (dirsiF_mask)                                           */
$ /* *********************************************************************** */
$set 12 preload                  

1  (Help mask)                       DIRECTORY SYSTEM
2 
3 Enter the password (if any) used when the local data was saved.
4 
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
$ /*     Help mask1: (direiA_mask)                                           */
$ /* *********************************************************************** */
$set 13 preload                  

1  (Help mask)                       DIRECTORY SYSTEM
2 
3 Enter the same password (if any) that was entered when the data was
4 saved
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
$ /*     Help mask1: (direiF_mask)                                           */
$ /* *********************************************************************** */
$set 14 preload                  

1  (Help mask)                       DIRECTORY SYSTEM 2 
3 Enter the same password (if any) that was entered when the data was
4 saved.
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
