# ______________________________________________________________________
# Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
# All rights reserved.
# ______________________________________________________________________

IDLDIR	= ../../idl
APIDIR	= ../../api

# Something like this with gcc
# CFLAGS = CC=gcc CFLAGS="-g -O -fno-builtin -Wall -I. -I../idl"
CC	= c89
CFLAGS	= -g -D_HPUX_SOURCE -I$(IDLDIR) -I$(APIDIR)
LDFLAGS	= -g -z -Wl,-Bimmediate,-Bnonfatal,-a,default,+b,/lib:/usr/lib
LIB	= -L../../api -lmos -ldce -lc_r

SRC	= main.c
OBJ	= $(SRC:.c=.o)
PROGS	= qif

# ----------------------------------------------------------------

all: $(PROGS)

qif: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LIB)

clean:
	rm -f *.o $(PROGS) core

