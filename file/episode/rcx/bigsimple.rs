#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# /* Copyright (C) 1993 Transarc Corporation - All rights reserved. */
#
# HISTORY
# $Log: bigsimple.rs,v $
# Revision 1.1.27.1  1996/10/02  17:26:37  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:24  damon]
#
# Revision 1.1.20.3  1994/10/06  20:49:13  agd
# 	expand copyright
# 	[1994/10/06  18:18:22  agd]
# 
# 	expand copyright
# 
# Revision 1.1.20.2 1994/06/09 14:02:19 annie 
# 	bigsimple.rs -- combines most of the simple tests in one big script.
# 
# $EndLog$

create a b c d e f g h i j 
chmod 0777 a b c d e f g h i j 

create moe curly larry
createfiles 10 barx

mkdir foob
mkdir 0775 foodirb
mkdir bara1 bara2 bara3
mkdir 0364 tar1 tar2 tar3
mkdirs 5 how
mkdirs 5 0654 now
chmod 111 foob
chmod 222 bara1 bara2 bara3
chmodpaths 5 333 how

create fooo
create barr1 barr2 barr3
createfiles 5 foobar
chmod 0111 fooo
chmod 0222 barr1 barr2 barr3
chmodpaths 5 0333 foobar

create fooc barc
createfiles 10 foobarc
rm barc fooc
rmfiles 5 foobarc

mkdir food
mkdir 111 foood
mkdir bard1 bard2 bard3
mkdirs 5 howd
rmdir food
rmdir bard3 foood bard1
rmdirs 3 howd

create bare
mkdir fooe
mv bare rab
mv fooe eoof
createfiles 10 gigo
mvpaths 5 gigo fifo

create foof1
create barf1 barf2 barf3
createfiles 5 foobarf
chown 3200 foof1
chown 3290 barf1 barf2 barf3
chownpaths 3 3291 foobarf

create foog1
create barg1 barg2 barg3
createfiles 5 foobarg
chgrp 3200 foog1
chgrp 3290 barg1 barg2 barg3
chgrppaths 3 3291 foobarg

create c 
#write b 10 0 0 c

createfiles 5 cc
#writefiles 5 b 2 0 0 1 cc

create ccc
truncate 20000 ccc
