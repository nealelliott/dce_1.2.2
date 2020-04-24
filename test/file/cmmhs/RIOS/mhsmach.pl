# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For Multi-homed DFS Server Support - Platform-dependent Functions
#
#   This file implements functions that are likely to require porting
#   to each platform.

# ----------------------------------------------------------------------------


# ConfigIfDown([IN] host, [IN] interface) - configure interface down on
#     specified host.
#
#     Returns: 1 - successful; 0 - failed
#
sub ConfigIfDown {
    local($host, $iface) = @_;
    local($goterror);

    print "$RSH $host $IFCONFIG $iface down\n";
    $goterror = system("$RSH $host $IFCONFIG $iface down");
    !($goterror); # return result
}


# ConfigIfUp([IN] host, [IN] interface) - configure interface up on
#     specified host.
#
#     Returns: 1 - successful; 0 - failed
#
sub ConfigIfUp {
    local($host, $iface) = @_;
    local($goterror);

    print "$RSH $host $IFCONFIG $iface up\n";
    $goterror = system("$RSH $host $IFCONFIG $iface up");
    !($goterror); # return result
}


# AddRoute([IN] host, [IN] destination, [IN] gateway) - for specified host,
#     add destination/gateway pair to routing table; dest is presumed to
#     be a host (rather than a network).
#
#     Returns: 1 - successful; 0 - failed
#
sub AddRoute {
    local($host, $dest, $gway) = @_;
    local($goterror);

    print "$RSH $host $ROUTE add -host $dest $gway 1\n";
    $goterror = system("$RSH $host $ROUTE add -host $dest $gway 1");
    !($goterror); # return result
}


# DelRoute([IN] host, [IN] destination, [IN] gateway) - for specified host,
#     delete destination/gateway pair from routing table; dest is presumed to
#     be a host (rather than a network).
#
#     Returns: 1 - successful; 0 - failed
#
sub DelRoute {
    local($host, $dest, $gway) = @_;
    local($goterror);

    print "$RSH $host $ROUTE delete -host $dest $gway\n";
    $goterror = system("$RSH $host $ROUTE delete -host $dest $gway");
    !($goterror); # return result
}


1; # needed for perl require()
