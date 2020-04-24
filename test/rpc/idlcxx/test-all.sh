#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: test-all.sh,v $
# Revision 1.1.2.1  1996/09/09  16:30:26  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  16:20:16  bfc]
#
# $EndLog$
#
# Define and run the tests for TET
#
tet_startup=""
tet_cleanup=""
iclist="account account2 accountc bind card handle local_object lookup matrix matrixc native refcnt refmon retry stack static stubexc"
ic1="account"
ic2="account2"
ic3="accountc"
ic4="bind"
ic5="card"
ic6="handle"
ic7="local_object"
ic8="lookup"
ic9="matrix"
ic10="matrixc"
ic11="native"
ic12="refcnt"
ic13="refmon"
ic14="retry"
ic15="stack"
ic16="static"
ic17="stubexc"
#
# Define what to do for each test
#
account()
{
	(cd account ; run)
}
account2()
{
	(cd account2 ; run)
}
accountc()
{
	(cd accountc ; run)
}
bind()
{
	(cd bind ; run)
}
card()
{
	(cd card ; run)
}
handle()
{
	(cd handle ; run)
}
local_object()
{
	(cd local_object ; run)
}
lookup()
{
	(cd lookup ; run)
}
matrix()
{
	(cd matrix ; run)
}
matrixc()
{
	(cd matrixc ; run)
}
native()
{
	(cd native ; run)
}
refcnt()
{
	(cd refcnt ; run)
}
refmon()
{
	(cd refmon ; run)
}
retry()
{
	(cd retry ; run)
}
stack()
{
	(cd stack ; run)
}
static()
{
	(cd static ; run)
}
stubexc()
{
	(cd stubexc ; run)
}
#
# Actually run the tests
#
account
account2
accountc
bind
card
handle
local_object
lookup
matrix
matrixc
native
refcnt
refmon
retry
stack
static
stubexc
#
# Do some postprocessing
#
echo Searching for failures..
grep FAIL */tet_xres
cat */tet_xres > tet_xres
rm */tet_xres
echo Test cases completed.
echo Summarizing results..
summary
