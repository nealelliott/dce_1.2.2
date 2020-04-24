# include in package main itself	
package main;

#
# Load specifications
# 

%pctsSpec = (MIN_DIRS,		    1,
			 MIN_DIR_QUOTA,     &SizeInKB("100M"),
			 MIN_DURATION,		1,
			 DEF_PCTS_LIBROOT,  "/cts",
			 DEF_PCTS_CTSROOT,  "/afs/tr/fs/dev/test/cts",
			 DEF_PCTS_DEVICE1,  "/dev/scratch1",
			 DEF_PCTS_DEVICE2,  "/dev/scratch2",
			 );

# Canonalize pcts load specs
#$pctsSpec{MIN_DIR_QUOTA}=$pctsSpec{MIN_DIR_QUOTA});

# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:





