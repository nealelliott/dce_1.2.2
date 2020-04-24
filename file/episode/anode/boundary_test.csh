#! /bin/csh -f

alias timeit 'time \!:*'

goto PHASE3

echo "$0 - Phase 1 starting ..."
set var = 1
while ($var <= 16) 
	./test_anode boundary1.createAggr.test
	if ( $status != 0 ) then
		echo "test_anode failure"
		exit 1
	endif
	echo ""
	./test_anode boundary1.test -var $var
	if ( $status != 0 ) then
		echo "test_anode failure"
		exit 1
	endif
	./test_anode dev-two.rcvr.test		
	if ( $status != 0 ) then
		echo "test_anode failure"
		exit 1
	endif
	./test_anode boundary1.test -var $var
	if ( $status != 0 ) then
		echo "test_anode failure"
		exit 1
	endif
	echo ""
	@ var ++
end
echo "$0 - successfully completed Phase 1 \n"
echo ""

echo "$0 - Phase 2 starting ..."
./test_anode boundary1.createAggr.test
if ( $status != 0 ) then
	echo "test_anode failure"
	exit 1
endif

set var = 1
echo ""
while ($var <= 16) 
	./test_anode boundary1.test -var $var
	if ( $status != 0 ) then
		echo "test_anode failure"
		exit 1
	endif
	./test_anode dev-two.rcvr.test		
	if ( $status != 0 ) then
		echo "test_anode failure"
		exit 1
	endif
	echo ""
	@ var ++
end
echo "$0 - successfully completed Phase 2"
echo ""

PHASE3:
echo "$0 - Phase 3 starting ..."
./test_anode boundary1.createAggr.test
if ( $status != 0 ) then
	echo "test_anode failure"
	exit 1
endif
echo ""
set var = 31
while ($var <= 45) 
	./test_anode boundary1.test -var $var
	if ( $status != 0 ) then
		echo "test_anode failure"
		exit 1
	endif
	./test_anode dev-two.rcvr.test		
	if ( $status != 0 ) then
		echo "test_anode failure"
		exit 1
	endif
	./test_anode boundary1.verify.test -var $var
	if ( $status != 0 ) then
		echo "test_anode failure"
		exit 1
	endif
	@ var ++
	echo ""
end
echo "$0 - successfully completed Phase 3"
echo ""

echo "$0 - ALL CLEAR"
exit 0


