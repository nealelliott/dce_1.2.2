#   
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#  
# All Rights Reserved
#    
SOURCES= Makefile README \
	random-dpool.c random-pool.c random-pool.h \
	random-test-1.c random-test.c \
	random-stats.c \
	random-hpux-assem.s random-hpux.c random-hpux.h \
	test-hpux-assem-1.c test-hpux-assem.c

CC=cc -Ae 
AS=/usr/ccs/bin/as

CFLAGS=+Xi -O

all:  random-test random-test-1 test-hpux-assem random-hpux.o random-stats

random-test: random-pool.o random-test.o
	cc -o random-test random-pool.o random-test.o


random-test-1: random-pool.o random-test-1.o
	cc -o random-test-1 random-pool.o random-test-1.o

KCFLAGS= -Ae ${IDENT} ${OPTIONS} -DKERNEL -D_KERNEL \
	-D_KERNEL_BUILD -D_UNSUPPORTED +XiUVx +O2 -Wp,-H300000 $(OFLAGS) +Hx0 +R25 +DA1.0 +DS1.0 -I/usr/conf -I/usr/conf/h +ESsfc +ESlit
KCC=${CC}

random-hpux.o: random-hpux.c random-pool.h
	${KCC} -c ${KCFLAGS} -o $*.o $*.c

random-hpux-assem.o: random-hpux-assem.s
	${CC} -E $*.s | ${AS} -o $*.o

test-hpux-assem: random-hpux-assem.o test-hpux-assem.o test-hpux-assem-1.o
	${CC} ${CFLAGS} -Wl,-a,archive -o test-hpux-assem random-hpux-assem.o test-hpux-assem.o test-hpux-assem-1.o

kernel: random-hpux.o random-hpux-assem.o
	ar cruv /usr/conf/lib/libusrdrv.a random-hpux.o random-hpux-assem.o
	/usr/sbin/mk_kernel -v 
	echo "Copy /stand/build/vmunix_test to /stand/vmunix and reboot.."

clean:
	rm -f *.o *~ 
	rm -f random-test random-test-1 test-hpux-assem random-stats

random-stats: random-stats.o
	${CC} -o $@ random-stats.o

distrib: ${SOURCES}
	tar cvf /tmp/random.tar ${SOURCES}
