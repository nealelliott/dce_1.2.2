#!/bin/sh
base=`dirname $0`
$base/delegate /.:/delegation/servers/server0 quit &
$base/delegate /.:/delegation/servers/server1 quit &
$base/delegate /.:/delegation/servers/server2 quit &
$base/delegate /.:/delegation/servers/server3 quit &
$base/delegate /.:/delegation/servers/server4 quit &
$base/delegate /.:/delegation/servers/server5 quit &
$base/delegate /.:/delegation/servers/server6 quit &
$base/delegate /.:/delegation/servers/server7 quit &
wait
