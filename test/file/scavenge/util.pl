# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: util.pl,v $
# Revision 1.1.8.1  1996/10/17  18:35:38  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:57:40  damon]
#
# Revision 1.1.3.1  1994/07/13  22:32:44  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:17:07  devsrc]
# 
# 	moving from file/episode to test/file
# 	[1994/03/22  21:02:51  delgado]
# 
# 	Transarc delta: jdp-ot3675-lfs-add-salvager-tests 1.1
# 	  Files modified:
# 	    Makefile
# 	    ravage: Makefile, and.c, bash.c, copy.c, copyall.sh
# 	    ravage: metamucil.c, nebconv.pl, pfaults.pl, ravage.sh
# 	    ravage: reravage.sh, rvg_setup.sh, scp.c, setall.sh, swap.c
# 	    ravage: swapall.sh, which.sh, zeroall.sh
# 	    scavenge: Makefile, aggrinfo.pl, efs.pl, efs_query.pl
# 	    scavenge: epidump.pl, fsdump.pl, simple_test.pl, upquota.pl
# 	    scavenge: util.pl
# 	  Selected comments:
# 	    This delta adds the ravager and scavenger tests to the Episode source tree.
# 	    These packages are for testing the Episode salvager.
# 	[1992/05/22  20:46:57  jdp]
# 
# Revision 1.1.53.1  1994/02/04  20:15:27  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:11:26  devsrc]
# 
# Revision 1.1.51.1  1993/12/07  17:20:59  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  14:59:50  jaffe]
# 
# Revision 1.1.4.3  1993/01/21  18:24:02  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  14:14:05  zeliff]
# 
# Revision 1.1.4.2  1992/09/25  16:28:33  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:51:27  jaffe]
# 
# $EndLog$
#

# Copyright (C) 1992, 1991 Transarc Corporation - All rights reserved

package util;

$unpack_all = 0;

$FORMAT_RECORD_SIZE = 3;

%FORMAT_TYPE_TO_SIZE =
(
 'C', 1,
 'c', 1,
 'L', 4,
 'l', 4,
 'n', 2,
 'N', 4,
 'S', 2,
 's', 2,
);


sub Basename {
    local($pathname, $suffix) = @_;
    local(@pathname) = split('/', $pathname);
    local($basename) = pop(@pathname);

    $basename = $1 if ($basename =~ /^(.*)$suffix$/);

    if ($basename =~ /^(.*)$suffix$/) {
	$basename = $1;
    }

    return $basename;
}

sub Div {
    local($x, $y) = @_;
    return int($x / $y);
}

sub HowMany {
    local($x, $y) = @_;
    return int(($x + $y - 1)/ $y);
}

sub RoundUp {
    local($x, $y) = @_;
    return $y * &HowMany($x, $y);
}

sub FormatFields {
    local(@format) = @_;
    local(@fields);
    local($i, $name);

    for ($i = $[; $i < $#format; $i += $FORMAT_RECORD_SIZE) {
	$name = &GetName($format[$i]);
	push(@fields, $name) unless ($name =~ /^!.*/);
    }

    return @fields;
}

sub FormatPrint {
    local($data, @format) = @_;
    printf &FormatString(@format), &Unpack($data, @format);
}

sub FormatSize {
    local(@format) = @_;
    local($i);
    local($size) = 0;

    for ($i = $[ + 1; $i < $#format; $i += $FORMAT_RECORD_SIZE) {
	$size += &FormatTypeToSize($format[$i]);
    }

    return $size;
}

sub FormatString {
    local(@format) = @_;
    local($format_string) = "";
    local($i, $name);
    local($max_length) = 0;

    for ($i = $[; $i < $#format; $i += $FORMAT_RECORD_SIZE) {
	$name = &GetName($format[$i]);
	if ($name !~ /^!.*/) {
	    $max_length = &Max($max_length, length($name));
	}
    }

    $max_length++;		# Account for the ':'

    for ($i = $[; $i < $#format; $i += $FORMAT_RECORD_SIZE) {
	$name = &GetName($format[$i]);
	if ($name !~ /^!.*/) {
	    local($padded_name) = sprintf("%-${max_length}s", "$name:");
	    $format_string .= "\t${padded_name} ${format[$i + 2]}\n";
	}
    }

    return $format_string;
}

sub FormatToMap {
    local(@format) = @_;
    local($i);
    local(%map);
    local($offset) = 0;

    for ($i = $[; $i < $#format; $i += $FORMAT_RECORD_SIZE) {
	local($name) = &GetName($format[$i]);
	local($format_string) = $format[$i + 1];
	local($print_format) = $format[$i + 2];
	local($size) = &FormatTypeToSize($format_string);

	$map{$name} = "${offset}:${format_string}:${size}:${print_format}";
	$offset += $size;
    }

    return %map;
}

sub FormatTypeToSize {
    local($type) = @_;

    return ($type =~ /^(A|a).+/) ?
	substr($type, 1) : $FORMAT_TYPE_TO_SIZE{$type};
}

sub MapGet {
    local($data, $field, %map) = @_;
    local($offset, $format_string, $size) = split(/:/, $map{$field});

    return unpack($format_string, substr($data, $offset, $size));
}

sub MapGetRaw {
    local($data, $field, %map) = @_;
    local($offset, $format_string, $size) = split(/:/, $map{$field});

    return substr($data, $offset, $size);
}

sub MapOffset {
    local($field, %map) = @_;
    local($offset, $x, $y, $z) = split(/:/, $map{$field});
    return $offset;
}

sub Max {
    local($x, $y) = @_;
    return $x < $y ? $y : $x;
}

sub Min {
    local($x, $y) = @_;
    return $x < $y ? $x : $y;
}

sub RcsCmp {
    local($x, $y) = @_;
    local($cmp);
    local(@r1) = split(/\./, $x);
    local(@r2) = split(/\./, $y);
    local($r1, $r2);

    while ($r1 = shift(@r1)) {
	return 1 if !($r2 = shift(@r2));
	return $cmp if $cmp = ($r1 <=> $r2);
    }
    return -1 if shift(@r2);
    return 0;
}

#
# Controls whether or not to skip items of the form '!name', which are
# not interesting for most uses, but which the salvager tests want to get
# at.
#
sub UnpackAllFields {
    $unpack_all = @_;
}

sub GetName {
    local($field) = @_;
    return ($unpack_all && $field =~ /!.+/) ? substr($field, 1) : $field;
}

sub Unpack {
    local($packed_data, @format) = @_;
    local($format_string) = "";
    local($offset) = 0;
    local($length) = 0;
    local(@unpacked_data) = ();
    local($i, $type, $size);

    for ($i = 0; $i < $#format; $i += $FORMAT_RECORD_SIZE) {
	$type = $format[$i + 1];
	$size = &FormatTypeToSize($type);
	if (&GetName($format[$i]) =~ /^!.*/) {
	    if ($format_string) {
		push(@unpacked_data,
		    unpack($format_string,
			substr($packed_data, $offset, $length)));
		$format_string = "";
		$offset += $length;
		$length = 0;
	    }
	    $offset += $size;
	} else {
	    $format_string .= $type;
	    $length += $size;
	}
    }
    if ($format_string) {
	push(@unpacked_data,
	    unpack($format_string,
		substr($packed_data, $offset, $length)));
    }

    return @unpacked_data;
}

sub UnpackAssoc {
    local($packed_data, @format) = @_;
    local(%field_vals);
    local($i, $j, $name);
    local(@unpacked_data) = &Unpack($packed_data, @format);

    $j = $[;
    for ($i = $[; $i < $#format; $i += $FORMAT_RECORD_SIZE) {
	$name = &GetName($format[$i]);
	if ($name !~ /^!.*/) {
	    $field_vals{$name} = $unpacked_data[$j];
	    $j++;
	}
    }

    return %field_vals;
}
