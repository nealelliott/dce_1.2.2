# include in package main itself	
package main;

# 
# ModesumObject
#
# Parameter $ignoreOutput optional
#
sub ModesumObject {
    local($modesumBinary, $object, $ignoreOutput) = @_;
    local($code) = 0;
    local($cmd) = "$modesumBinary $object";
    $code = &Modesum($cmd, $ignoreOutput);
    return $code;
}

# 
# ModesumRecursive
#
# Parameter $ignoreOutput optional
#
sub ModesumRecursive {
    local($modesumBinary, $object, $ignoreOutput) = @_;
    local($code) = 0;
    local($cmd) = "$modesumBinary -r $object";
    $code = &Modesum($cmd, $ignoreOutput);
    return $code;
}

#
# Modesum
#
# Parameter $ignoreOutput optional
#
sub Modesum {
    local($cmd, $ignoreOutput) = @_;
    local($code) = 0;

    if ($ignoreOutput) {
	$cmd .= " > /dev/null";
    }
    $code = &Sys("$cmd");
    return $code;
}



1;
