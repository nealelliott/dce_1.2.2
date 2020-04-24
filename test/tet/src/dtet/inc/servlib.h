/*
 *      SCCS:  @(#)servlib.h	1.2 (92/09/30) 
 *
 *	UniSoft Ltd., London, England
 *
 * (C) Copyright 1992 X/Open Company Limited
 *
 * All rights reserved.  No part of this source code may be reproduced,
 * stored in a retrieval system, or transmitted, in any form or by any
 * means, electronic, mechanical, photocopying, recording or otherwise,
 * except as stated in the end-user licence agreement, without the prior
 * permission of the copyright owners.
 *
 * X/Open and the 'X' symbol are trademarks of X/Open Company Limited in
 * the UK and other countries.
 */

/************************************************************************

SCCS:   	@(#)servlib.h	1.2 92/09/30 DTET release 1.0b
NAME:		servlib.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	declarations for extern servlib functions and data objects not
	declared in other header files

MODIFICATIONS:

************************************************************************/


extern int sd_errno;
extern int tc_errno;
extern int xd_errno;

#if __STDC__

extern int sd_logoff(int);
extern int sd_logon();
extern char *sd_msgbuf(int);
extern long sd_snget();
extern int sd_snsys(long, int *, int);
extern char *sd_talk(int, int);
extern void si_forkdaemon();
extern int si_main(int, char **, int);
extern void si_serverloop();
extern int tc_access(int, char *, int);
extern int tc_cfname(int, char *);
extern int tc_chdir(int, char *);
extern int tc_configv(int, char **, int);
extern long tc_exec(int, char *, char **, char *, long, long, int);
extern int tc_fclose(int, int);
extern int tc_fopen(int, char *);
extern int tc_kill(int, long, int);
extern int tc_lockfile(int, char *, int);
extern int tc_logoff(int);
extern int tc_logon(int);
extern int tc_mkdir(int, char *);
extern long tc_mexec(int, char *, char **, char *);
extern char *tc_mksdir(int, char *, char *);
extern char *tc_mktmpdir(int, char *);
extern char *tc_msgbuf(int, int);
extern int tc_putenv(int, char *);
extern int tc_putenvv(int, char **, int);
extern int tc_puts(int, int, char *);
extern int tc_putsv(int, int, char **, int);
extern char **tc_rcvconfv(int, int *, int *);
extern int tc_rmdir(int, char *);
extern int tc_rsys(int, int);
extern int tc_rxfile(int, char *, char *);
extern char *tc_sharelock(int, char *, int);
extern int tc_sndconfv(int, char **, int);
extern int tc_sysname(int, int *, int);
extern char *tc_talk(int, int, int);
extern long tc_texec(int, char *, char **, char *, long, long);
extern int tc_tslfiles(int, char **, int, char *);
extern int tc_tsmfiles(int, char **, int, char *);
extern long tc_uexec(int, char *, char **, long, long);
extern int tc_unlink(int, char *);
extern int tc_wait(int, long, int, int *);
extern int ti_tcmputenv(int, long, long, int *, int);
extern int xd_cfname(char *, char *, char *);
extern int xd_codesfile(char *);
extern int xd_fclose(int);
extern int xd_fopen(char *);
extern char *xd_gets(int);
extern char **xd_getsv(int, int *, int *);
extern int xd_icend(long);
extern int xd_icstart(long, int, long, int);
extern int xd_logoff();
extern int xd_logon();
extern char *xd_msgbuf(int);
extern char **xd_rcfname();
extern int xd_result(long, int);
extern char *xd_talk(int, int);
extern int xd_tpend(long);
extern int xd_tpstart(long, int);
extern int xd_xfile(char *, char *);
extern int xd_xres(long, char *);
extern int xd_xresv(long, char **, int);
extern long xd_xropen(char *);
extern int xd_xrsys(long, int *, int);

#else /* __STDC__ */

extern int sd_logoff();
extern int sd_logon();
extern char *sd_msgbuf();
extern long sd_snget();
extern int sd_snsys();
extern char *sd_talk();
extern void si_forkdaemon();
extern int si_main();
extern void si_serverloop();
extern int tc_access();
extern int tc_cfname();
extern int tc_chdir();
extern int tc_configv();
extern long tc_exec();
extern int tc_fclose();
extern int tc_fopen();
extern int tc_kill();
extern int tc_lockfile();
extern int tc_logoff();
extern int tc_logon();
extern int tc_mkdir();
extern long tc_mexec();
extern char *tc_mksdir();
extern char *tc_mktmpdir();
extern char *tc_msgbuf();
extern int tc_putenv();
extern int tc_putenvv();
extern int tc_puts();
extern int tc_putsv();
extern char **tc_rcvconfv();
extern int tc_rmdir();
extern int tc_rsys();
extern int tc_rxfile();
extern char *tc_sharelock();
extern int tc_sndconfv();
extern int tc_sysname();
extern char *tc_talk();
extern long tc_texec();
extern int tc_tslfiles();
extern int tc_tsmfiles();
extern long tc_uexec();
extern int tc_unlink();
extern int tc_wait();
extern int ti_tcmputenv();
extern int xd_cfname();
extern int xd_codesfile();
extern int xd_fclose();
extern int xd_fopen();
extern char *xd_gets();
extern char **xd_getsv();
extern int xd_icend();
extern int xd_icstart();
extern int xd_logoff();
extern int xd_logon();
extern char *xd_msgbuf();
extern char **xd_rcfname();
extern int xd_result();
extern char *xd_talk();
extern int xd_tpend();
extern int xd_tpstart();
extern int xd_xfile();
extern int xd_xres();
extern int xd_xresv();
extern long xd_xropen();
extern int xd_xrsys();

#endif /* __STDC__ */

