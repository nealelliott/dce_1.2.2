#! /bin/sh

###############################################################################
# 
# sml2hlp.sh Script
# Jeff Kaminski 7/11/94
# 
# The sml2hlp.sh script converts option descriptions found on DCE manpages from
# OSF SML format to a format suitable for use with a C program.  The script
# accepts one of the following arguments:
#
# o The name of a command suite (for example, dceback).  The command creates
#   a file that includes the option descriptions from the reference pages for
#   all of the commands in the specified suite.
# 
# o The name of a single command.  The command creates a file that includes
#   the option descriptions from the reference page for only the specified
#   command.
# 
# The script must be issued from the directory that contains the files to be
# processed.  The script creates a single file whose name consists of the
# argument to the script with the addition of a ".hlp.c" extension.
# 
# The script has no effect on the source files from which it extracts text. 
# It can be safely terminated, and it can be executed multiple times on the
# same source files.  It thoroughly initializes its environment with each
# execution.
# 
###############################################################################

#
# Verify that an argument was specified and, if so, begin to initialize the
# environment; otherwise, exit.
#

echo ""
if [ $# -eq 0 ]
   then
   echo " sml2hlp.sh: Converts DCE reference pages to online help format"
   echo " Usage:   sml2hlp.sh {<command_name> | <suite_name>}"
   echo ""
   echo " Note:    Execute the script in the directory that contains the source files"
   echo " for the reference pages."
   echo ""
   exit
   elif [ -f $1.hlp.c ]
      then
      echo "   Removing current $1.hlp.c file."
      echo ""
      rm $1.hlp.c
fi

#
# Finish thoroughly initializing the environment.
#

if [ -f .hlp ]
   then rm .hlp
fi
if [ -f .hlp1 ]
   then rm .hlp1
fi
if [ -f .hlp2 ]
   then rm .hlp2
fi
if [ -f .hlp3 ]
   then rm .hlp3
fi
if [ -f .hlp1A ]
   then rm .hlp1A
fi
if [ -f .hlp2A ]
   then rm .hlp2A
fi
if [ -f .hlp3A ]
   then rm .hlp3A
fi
if [ -f .hlp1B ]
   then rm .hlp1B
fi
if [ -f .hlp2B ]
   then rm .hlp2B
fi
if [ -f .hlp3B ]
   then rm .hlp3B
fi

#
# Begin the real sed processing.
#

for i in `ls | grep $1 $i`
do
echo "   Grabbing option descriptions from $i."
touch .hlp3
temp=`echo $i | sed "
   s/^.*[_]//
   s/\..*//
   "`

cat $i | sed "
   /\.SH \"OPTIONS\"/,/\.SH \"DESCRIPTION\"/!d
   /\.SH \"OPTIONS\"/,/\.SH \"DESCRIPTION\"/{
   s/^$/No help available for this option\./
   /\.SH \"OPTIONS\"/,/\.VL/d
   s/[\\]\*V[^\\]*/<&>/g
   /\.\.\.[\\]\".*/d
   /\.iX.*/d
   s/[\\]\*[CELOV]//g
   s/[\\]-/-/g
   s/[\\]e/\\\\\\\\/g
   s/[\\]|/ /g
   s/[\\][\\]/\\\\\\\\/g
   s/[\\][(]em/--/g
   s/\.PP//
   /\.LI \"-/{
   s/\.LI \"-/$temp+ -/
   s/\"//
   h
   s/$temp+ //
   H
   x
   H
   }
   /^$temp+.*/{
   s/ /@/2
# Example of removing newline:
#   s/@[^\n]*[\n]/, /
# Example of leaving newline:
   s/@[^\n]*//
   }
   /$temp.*/{
   s/[\n]/+ /
   }
   s/\.[VMA]L.*/\\
BEGIN_LIST/
   s/\.LE.*/END_LIST\\
/
#   s/\.LE.*/END_LIST\\
#\\
#/
   /\.LI.*/{
   /.LI$/{
	s/.LI$/+ /
        N
        s/\n//
	}
   /.LI \".*/{
	s/.LI \"/+ /
        s/\".*//
	}
   }
   /\.oS/,/\.oE/{
   s/\.oS//
   /.oE.*/d
   s/^/   /
   }
   /\.iS/,/\.iE/{
   s/\.iS//
   /.iE.*/d
   s/^/   /
   }
   /\.nS/{
   s/\"/:/2
   s/\.nS \"/\\
/
   s/note/Note/
   s/caution/Caution/
   }
   }
   " > .hlp1

cat .hlp1 | sed "
   /\.nE/d
   /$temp+ .*/{
   s/^/\\
\{\"/
   s/+/\", \"/g
   s/, \" /, \"/g
   s/$/\",/
   }
   " > .hlp2

cat .hlp2 | sed "
   /END_LIST/{
   N
   /END_LIST[\n]$/d
   }
   /\.SH \"DESCRIPTION\"/d
# Old method of stripping the final END_LIST and the final DESCRIPTION tag:
#   /END_LIST/{
#   N
#   N
#   /END_LIST[\n]*\.SH \"DESCRIPTION\"/d
#   }
   " >> .hlp

cat .hlp | sed "
   /^$/{
   N
   s/[\n][\{]/\},\\
\{/
   }
   " >> .hlp3

if [ -f .hlp ]
   then
   rm .hlp
fi
if [ -f .hlp1 ]
   then
   rm .hlp1
fi
if [ -f .hlp2 ]
   then
   rm .hlp2
fi
done

if [ -f .hlp3 ]
   then
   cat .hlp3 | sed "
   1d
   s/[\\]\*[CELOV]//g
   /^$/{
   N
   /^\n$/D
   }
   /^\{\".*/{
   s/$/\\
++/
   }
   /^\},$/{
   s/^/++\\
/
   }
   " > .hlp1
rm .hlp3
fi

if [ -f .hlp1 ]
   then
   cat .hlp1 | sed "
   /^\{\".*/!{
   s/[\"]/\\\\\"/g
   }
   /^\{\".*\", \"-/{
   h
   s/\{\"//
   s/\", \"-/_/
   s/\",.*/\},/
   H
   x
   H
   }
   /^\{\".*/{
   s/[\n]/ /
   }
   s/+ /\\
+ /
   " > .hlp2
   rm .hlp1
fi

if [ -f .hlp2 ]
   then
   cp .hlp2 .hlp1A
   cp .hlp2 .hlp2A
   mv .hlp2 .hlp3A

   cat .hlp1A | sed "
   /^\{\".*/!d
   s/^.* /void /
   s/\},/ _TAKES\(\(FILE\* stream\)\)\;/
   " > .hlp1B
   rm .hlp1A

   cat .hlp2A | sed "
   /^\{\".*/!d
   1{
   s/^/\\
cmd_help_entry_t help_array\[\]=\{\\
/
   }
   " > .hlp1
   rm .hlp2A
   count=`wc -l .hlp1 | sed "
   s/[ ]*//
   s/[ ].*$//
   "`
   cat .hlp1 | sed "
   ${count}a\\
\{\(char \*\)NULL, \(char \*\)NULL, \(char \*\)NULL, \(cmd_print_help_f_t\)NULL\}\\
\}\;
# 
# Old lines to edit end of file:
#   $count{
#   s/[,]$/\}\;/
#   }
#
   " > .hlp2B
   rm .hlp1

   cat .hlp3A | sed "
   /^\{\".*/!{
   /++/d
   /BEGIN_LIST/d
   /END_LIST/d
   /\},/!{
	s/.*/fprintf\(stream, \"&\\\\n\"\)\;/
	}
   }
   /^\{\".*/{
   s/^.* /\\
void /
   s/\},/\(stream\) FILE\* stream\;\\
\{/
   }
   s/^\},$/\}/
   " > .hlp3
#
# Recycle .hlp3 to remove consecutive lines that print blank lines.
#
   rm .hlp3A
   cat .hlp3 | sed "
   /^fprintf[\(]stream, \"[\\]n\"[\)]\;$/{
   N
   /fprintf[\(]stream, \"[\\]n\"[\)]\;$/D
   }" > .hlp3B
   rm .hlp3
fi

if [ -s .hlp1B ]
   then
   cat .hlp3B >> .hlp2B
   cat .hlp2B >> .hlp1B
   cat .hlp1B | sed "
   1i\\
#include \<stdio\.h\>\\
#include \<dcedfs\/param\.h\>\\
#include \<dcedfs\/stds\.h\>\\
#include \<dcedfs\/cmd_help\.h\>\\

   " > $1.hlp.c
   echo "}" >> $1.hlp.c
   echo "" >> $1.hlp.c
   rm .hlp1B
   rm .hlp2B
   rm .hlp3B
   echo ""
   echo "   Generated file $1.hlp.c."
   elif [ -f .hlp1B ]
      then
      rm .hlp1B
      rm .hlp2B
      rm .hlp3B
      echo ""
      echo "   No output file generated."
         else
         echo "   Cannot find a source file named $1."
fi
echo ""

exit
