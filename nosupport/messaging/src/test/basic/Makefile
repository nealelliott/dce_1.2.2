# ______________________________________________________________________
# Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
# All rights reserved.
# ______________________________________________________________________
#
# $Log: Makefile,v $
# Revision 1.1  1996/11/20  16:55:53  salamone
# Initial version
#
# $EndLog$
# ________________________________________________________________________
CC	= c89
CFLAGS	= -g -D_HPUX_SOURCE -I ../../idl 
LDFLAGS	= -g -z -Wl,-Bimmediate,-Bnonfatal,-a,default,+b,/lib:/usr/lib
LIB	= -L../../api -lmos -ldce -lc_r

SRC1 = basic_mq.c
OBJ1 = $(SRC1:.c=.o) 
SRC2 = basic_all_mq.c
OBJ2 = $(SRC2:.c=.o) 
PROGS	= bmq bmq_all

# ----------------------------------------------------------------

all: $(PROGS)

bmq: $(OBJ1)
	$(CC) $(LDFLAGS) -o $@ $(OBJ1) $(LIB)

bmq_all: $(OBJ2)
	$(CC) $(LDFLAGS) -o $@ $(OBJ2) $(LIB)

basic_mq.c: ../../idl/dcemosmsg.h ../../idl/dcemosmac.h ../../idl/dcemossvc.h
basic_all_mq.c: ../../idl/dcemosmsg.h ../../idl/dcemosmac.h ../../idl/dcemossvc.h

clean:
	rm -f *.o $(PROGS) core

