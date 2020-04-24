# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:49:54  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:31  marty]
#
# Revision 1.1.4.1  1995/12/11  22:55:57  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:25:07  root]
# 
# Revision 1.1.2.2  1993/07/20  16:07:33  cmckeen
# 	Initial DCE version
# 	[1993/07/15  15:36:11  cmckeen]
# 
# $EndLog$

VPATH	= ../apilib/:../dtetlib/:../inetlib/:../servlib/

LIBRARIES       = libdtetapi.a

EXPDIR          = /usr/lib/

libdtetapi.a_OFILES = dconfig.o dresfile.o dcancel.o tet_fork.o tet_exec.o \
	exit.o getlist.o getsys.o remexec.o remkill.o remwait.o rtab.o sync.o \
	amsg.o avmsg.o btmsg.o bufchk.o buftrace.o dtmsg.o dtsize.o \
        eaccess.o equindex.o errname.o fappend.o fgetargs.o fioclex.o \
        fionbio.o fork.o ftoa.o generror.o genfatal.o getargs.o hexdump.o \
        ldst.o llist.o ltoa.o ltoo.o ltox.o madir.o maperr.o mapsig.o \
        mkdir.o notty.o optarg.o prerror.o ptflags.o ptspid.o ptstate.o \
        ptstype.o pttype.o ptype.o putenv.o remvar.o repcode.o reqcode.o \
        rtoa.o sigmap.o strstore.o svote.o sysbyid.o sysent.o systate.o \
        targs.o tdump.o tfname.o trace.o unmaperr.o unmapsig.o valmsg.o \
        wait3.o \
	accept.o connect.o host.o lhost.o listn.o nbio.o poll.o rdwr.o \
        tccdaddr.o tccdport.o tptab.o tsinfo.o tstcmenv.o \
	cloop.o fio.o forkd.o logon.o msgbuf.o ptab.o sdasync.o sdead.o \
        sderrmsg.o sdmasync.o sdsasync.o sdsnget.o sdsnsys.o sdtalk.o \
        sdusync.o server.o sloop.o smain.o sproc.o talk.o tcaccess.o \
        tccfnam.o tcconf.o tcdir.o tcerrmsg.o tcexec.o tcfio.o tckill.o \
        tclfile.o tcputenv.o tcrxfile.o tcsdir.o tcshlock.o tcsname.o \
        tcrsys.o tctalk.o tctdir.o tcmexec.o tctexec.o tctsfile.o tcuexec.o \
        tcunlink.o tcwait.o titcmenv.o xdcfnam.o xdcodesf.o xderrmsg.o \
        xdfio.o xdictp.o xdrcfnam.o xdresult.o xdtalk.o xdxfile.o xdxres.o \
        xdxropen.o xdxrsys.o



INCFLAGS        = -I../inc

.include <${RULES_MK}>

