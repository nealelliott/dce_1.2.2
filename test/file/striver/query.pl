# include in package main itself	
package main;

##########################################################################
#
#  Query routines 
#  
#  These can be used by loads to get answers to various questions
#  they might have.
# 
#  NOTE: 
#    -  The routines here are not a comprehensive set of routines
#       but have been added as their functionality has been needed.
#    -  These rely on other modules like setup.pl
#
##########################################################################

sub FindFsetMntPt {
    local($fset) = @_;
    local($fsetMntDir);

    local($fsetMntDirs) = $fsetMntPts{$fset};
    local(@fsetMntDirs) = split(' ', $fsetMntDirs);

    $fsetMntDir = $fsetMntDirs[0];
    return $fsetMntDir;
}

1;
