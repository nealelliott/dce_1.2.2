# ______________________________________________________________________
# Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
# All rights reserved.
# ______________________________________________________________________
#
# $Log: Makefile,v $
# Revision 1.5  1996/12/06  11:26:18  salamone
# Clean-up
#
# Revision 1.4  96/12/06  11:00:24  11:00:24  salamone (Julie Salamone)
# Support ACK datatype
# 
# Revision 1.3  96/12/04  15:24:52  15:24:52  bowe (John Bowe)
# Take out util_signal.c, since the whole thing if #ifdef'ed out.
# 
# Revision 1.2  1996/11/23  12:08:23  salamone
# No longer need mosutil.c
#
# Revision 1.1  96/11/22  18:03:57  18:03:57  salamone (Julie Salamone)
# Initial version
# 
# $EndLog$
# ________________________________________________________________________
CC	= c89
SAMS    = sams

CFLAGS	= -g -D_HPUX_SOURCE -I. -I../api -I../idl -I../tcl_dce
LDFLAGS	= -g -z -Wl,-Bimmediate,-Bnonfatal,-a,default,+b,/lib:/usr/lib
LIB	= -L../api -lmos -L../tcl_dce -ltcl -ldce -lm -lc_r

# MOS file support
MOS_FILES		 = queue.c 

# DCECP core and utility files
DCECP_FILES 		 = main.c commands.c parseargv.c \
                           dcemcpsvc.c dcemcpmsg.c \
			   util_general.c util_help.c util_login.c \
			   util_var.c util_binding.c

SRC = ${DCECP_FILES} ${MOS_FILES}
OBJ = $(SRC:.c=.o) 
PROGS = moscp

# ----------------------------------------------------------------

all: $(PROGS)

moscp: $(OBJ) dcemcp.msg
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LIB)
	gencat dcemcp.cat dcemcp.msg

# ----------------------------------------------------------------
# Dependencies
#
main.c:                 util_login.h \
			dcemcpmsg.h
commands.c:             util_login.h \
                        dcemcpmsg.h

util_general.c:         util_general.h dcemcpmsg.h
util_login.c:           util_login.h dcemcpmsg.h
util_var.c:             util_var.h dcemcpmsg.h
util_binding.c:         util_binding.h dcemcpmsg.h
${MOS_FILES}:           queue.h dcemcpmsg.h \
			../api/mos_ack.h \
		        ../api/mosif.h \
		        ../idl/mostypes.h \
		        ../idl/dcemosmsg.h

# ----------------------------------------------------------------
# sams-generated stuff

dcemcpmsg.h:	mcp.sams
	${SAMS} -oh mcp.sams
dcemcpmsg.c:	mcp.sams
	${SAMS} -ot mcp.sams
dcemcp.msg:	mcp.sams
	${SAMS} -om mcp.sams
dcemcpsvc.c:	mcp.sams
	${SAMS} -oS mcp.sams
dcemcpsvc.h:	mcp.sams
	${SAMS} -os mcp.sams
dcemcpmac.h:	mcp.sams
	${SAMS} -ou mcp.sams

# ----------------------------------------------------------------
clean:
	rm -f *.o $(PROGS) core dcemcpmsg.h dcemcpmsg.c dcemcp.msg dcemcp.cat\
		dcemcpsvc.c dcemcpsvc.h dcemcpmac.h
