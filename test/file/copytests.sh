#!/bin/sh

# copy a list of directories
# run as "copytest.sh target src"
# used to copy DFS tests from AFS into DFS
# in this case run as "copytest.sh `pwd` src" in the target directory
# "src" is usually the test/file directory in the install tree

# for the smoke tests
#list="fts rep cache_mgr low util"

# for the OSF test plan
#list="fts rep cache_mgr low util cm acl fset bak bos ravage \
#	recovery tsr ubik zlc tkm test_anode test_vnodeops \
#	serverprefs"

# for the secondary smoke tests
list="cm fts rep acl fset util runall copytests.sh README"

if test $# -ne 2
then
	echo "run as copytest.sh target src"
	exit 1
fi

target=$1
src=$2

if test -d $target
then
	echo "installing in $target"
else
	echo "cannot find target $target"
	exit 1
fi
if test -d $src
then
	echo "installing from $src"
else
	echo "cannot find src $src"
	exit 1
fi

for i in $list
do
	if test -d $src/$i
	then
		echo "copying $i from $src to $target"
	elif test -f $src/$i
	then
		echo "copying $i from $src to $target"
	else
		echo "cannot find $i in $src"
		continue
	fi

	cp -r $src/$i $target
	if test $? -ne 0
	then
		echo "error whilst copying $i from $src to $target"
	else
		chmod -R 777 $target/$i
	fi
done

echo "copy complete"
exit 0
