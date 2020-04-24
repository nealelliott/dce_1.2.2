#!/afs/tr/fs/dev/perl

# /* Copyright (C) 1994 Transarc Corporation - All rights reserved. */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/test/file/episode/vnops/RCS/sched-delete-race.pl,v 1.2 1994/08/04 19:46:58 ota Exp $

# This test verifies the fix for defect 5815.  It exercises races between
# vnvm_schedule and vnvm_delete.

# The test assumes "." is in the directory under test.
# 
# An optional first argument is the number of processes to fork.  The second
# argument specifies the number of files to create/unlink/write.  Both default
# to 10 which was sufficient to trigger this panic on AIX before the fix was
# made.

@children = ();

sub Cleanup
{
    local ($signal) = @_;
    print "Caught $signal signal\n" if ($signal);
    kill 'INT',@children;
    exit ($signal ? 1 : 0);
}

$SIG{'INT'} = 'Cleanup';

$procs = ($ARGV[0] || 10);
$iter = ($ARGV[1] || 10);

$buffer = 'x' x 4096;

for ($i=0; $i<$procs; $i++) {
    last if (!($p = fork));
    push (@children, $p);
}
print "Forked $i processes\n" if ($p);

for ($i=0; $i<$iter; $i++) {
    open (FILE, ">foo.test");
    unlink ("foo.test");
    print FILE $buffer;
    close (FILE);
    `sync`;
}
print "Parent ($$) finished\n" if ($p);
print "Child $pid finished\n" while (($pid = wait) > 0);
  
