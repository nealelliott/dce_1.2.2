#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: passwd_import.sh,v $
# Revision 1.1.10.2  1996/03/11  02:34:43  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:37  marty]
#
# Revision 1.1.10.1  1995/12/11  21:49:01  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:32  root]
# 
# Revision 1.1.8.1  1994/08/04  16:16:07  mdf
# 	Hewlett Packard Security Code Drop
# 	[1994/07/26  19:14:20  mdf]
# 
# Revision 1.1.5.3  1993/03/24  21:34:14  mhickey
# 	Submitting for Sue Cuti (she had some break in the bsubmit)
# 	[1993/03/24  21:32:36  mhickey]
# 
# Revision 1.1.6.2  1993/03/22  15:36:49  cuti
# 	Add "-up" to rgy_edit, so update will go to master only.
# 
# Revision 1.1.5.2  1992/12/31  17:29:58  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:55:31  htf]
# 
# Revision 1.1.2.5  1992/06/25  18:30:27  cuti
# 	Remove comment from restore_db line in test 6.
# 	[1992/06/25  18:27:13  cuti]
# 
# Revision 1.1.2.4  1992/06/24  19:07:26  cuti
# 	Change cuti to wang and burati to buratis to avoid crashing with passwd_override test.
# 	[1992/06/24  17:58:53  cuti]
# 
# Revision 1.1.2.3  1992/06/08  20:34:02  cuti
# 	Converted to testsh format.
# 	[1992/06/08  20:09:51  cuti]
# 
# Revision 1.1.2.2  1992/05/14  19:20:57  cuti
# 	 Removed PASSWD_DIR
# 	[1992/05/14  19:19:14  cuti]
# 
# 	Modified sed script to trim down node specific output.
# 	[1992/05/13  14:32:47  cuti]
# 
# 	 Improve sed script to remove all the comments.
# 	[1992/05/12  21:13:27  cuti]
# 
# 	Created for passwd_import test.
# 
# 	Initial file creation
# 	[1992/05/12  17:48:43  cuti]
# 
# $EndLog$
 
#  Please NOTE:  Before running this script, you should have
#  	1.  define CELLADMIN.
#   2.  run "dce_login $CELLADMIN -dce-".
#	3.  binloign (local rgy) exists.

 
#set -x
         


check_status() 
{ 
#set -x

	if [ "$1" -ne "$2" ] 
	then
		STATUS=2 
		EXIT=1
		if [ $TRACE = "1" ]
		then
			echo "Trace Message:     \"$3\" failed, returnd : $1, expected: $2"
		fi
     fi 

} 

restore_db()
{  
       # Remove the added account, group, and principal for next test.

		if [ "$1" = 6 ] 
		then
			echo "del -p unix_bin -g unix_bin -o org" > /tmp/passwd_import.script
        fi
        echo "del -p rose_l -g osdev -o org" > /tmp/passwd_import.script
        echo "del -p rlr -g osdev -o org" >> /tmp/passwd_import.script
        echo "del -p buratis -g dds -o org" >> /tmp/passwd_import.script
        echo "del -p nana -g coco -o org" >> /tmp/passwd_import.script 
        echo "del -p wang -g dds -o org" >> /tmp/passwd_import.script 
        echo "del -p bpc -g system -o org" >> /tmp/passwd_import.script
        echo "del -p backer -g bin -o org" >> /tmp/passwd_import.script 
        if [ "$1" = "9" -o "$1" = "8" ]
		then
            echo "del -p lp -g lp -o org" >> /tmp/passwd_import.script
            echo "del -p adm -g adm -o org" >> /tmp/passwd_import.script
        fi            
 
        echo "do p" >> /tmp/passwd_import.script 
		if [ "$1" = 6 ] 
		then
			echo "del unix_bin " >> /tmp/passwd_import.script
        fi
        echo "del rose_l " >> /tmp/passwd_import.script
        echo "del rlr " >> /tmp/passwd_import.script
        echo "del buratis " >> /tmp/passwd_import.script
        echo "del  nana " >> /tmp/passwd_import.script 
        echo "del  wang " >> /tmp/passwd_import.script 
        echo "del  bpc " >> /tmp/passwd_import.script
        echo "del  backer " >> /tmp/passwd_import.script 
        if [ "$1" = "9" -o "$1" = "8" ]
		then
            echo "del lp " >> /tmp/passwd_import.script
            echo "del  adm " >> /tmp/passwd_import.script
        fi    

        echo "do g" >> /tmp/passwd_import.script
        if [ "$1" = "9" -o "$1" = "8" ]
		then
            echo "del root" >> /tmp/passwd_import.script
            echo "del other" >> /tmp/passwd_import.script
            echo "del sys" >> /tmp/passwd_import.script
            echo "del adm" >> /tmp/passwd_import.script
            echo "del lp" >> /tmp/passwd_import.script
		fi
        echo "del users" >> /tmp/passwd_import.script 
        echo "del dds" >> /tmp/passwd_import.script
        echo "del coco " >> /tmp/passwd_import.script
        echo "del osdev" >> /tmp/passwd_import.script 
		if [ "$1" = 6 ] 
		then
            echo "del unix_bin " >> /tmp/passwd_import.script
            echo "del sys" >> /tmp/passwd_import.script
        fi

        echo "do acc" >> /tmp/passwd_import.script
        echo "v" >> /tmp/passwd_import.script
        echo "q" >> /tmp/passwd_import.script
        
        rgy_edit -up < /tmp/passwd_import.script >> $2
   
}       


SFLAG="0"
FAIL="1"
SUCCESS="0"
TRACE="0"

for LCV in "$@"
do
if [ "$LCV" = "-d" ]
then
  DFLAG="-d"
  continue
fi
  if [ "$DFLAG" = "-d" ]
  then
    case $LCV in
      "1") FAIL="1" ;;
      "2") SUCCESS="1" ;;
      "3") SUCCESS="1"; FAIL="1" ;;
      "32") TRACE="1" ;;
      "33") TRACE="1"; FAIL="1" ;;
      "34") TRACE="1"; SUCCESS="1" ;;
      "63") TRACE="1"; SUCCESS="1"; FAIL="1" ;;
    esac
    DFLAG="0"
    continue
    fi
  case "$LCV" in
    -d*) DMASK=`echo $LCV | sed 's/^-d//'`
	 case $DMASK in
	   "1") FAIL="1" ;;
	   "2") SUCCESS="1" ;;
	   "3") SUCCESS="1"; FAIL="1" ;;
	   "32") TRACE="1" ;;
	   "33") TRACE="1"; FAIL="1" ;;
	   "34") TRACE="1"; SUCCESS="1" ;;
	   "63") TRACE="1"; SUCCESS="1"; FAIL="1" ;;
	 esac  ;;
    -s) SFLAG="1" ;;
    *[1-9]*) CASEVAL=$LCV ;;
    *[" "]*) LBLSTR=$LCV ;;
  esac
done


#
# Initialize program variables.
#

EXIT=0
CLEANUP=0
STATUS=0

#
# Setup acl_edit logfile for this run.
#

echo ""
echo "---------------------------------------------------------------"
echo "Routine Tested:    passwd_import command"
echo "Tested for:        $CASEVAL $LBLSTR" 
echo""



#
# Send a copy of the output to stderr if -s is specified on command line.
#

if [ $SFLAG = "1" ]
then
  echo "" 1>&2
  echo "---------------------------------------------------------------" 1>&2
  echo "Routine Tested:   passwd import command" 1>&2
  echo "Tested for:        $CASEVAL $LBLSTR" 1>&2
  echo"" 1>&2
fi

#
# Select acl_edit test.
#

case "$CASEVAL" in

#
# Passwd and group file missing.
#

	"1")     
        rm -r  /tmp/tmp_dir

		passwd_import -d /tmp/tmp_dir -v -u $CELLADMIN -p -dce- 2> /tmp/passwd_import_er.out 

		grep "Unable to access passwd file"  /tmp/passwd_import_er.out > /dev/null

		if [ $? -ne 0 ]
		then 
	    	STATUS=2
			EXIT=1 
			echo "\"Unable to access passwd file\" not in error msg"

  		else
    		if [ $TRACE = "1" ]
    		then
      			echo "Trace Message:     Obtained passwd_import error message" 
      			if [ $SFLAG = "1" ]
      			then
        			echo "Trace Message:      Obtained passwd_import error message" 1>&2
      			fi
    		fi
  		fi

  	;; # End case 1		
                         

	"2") 
		mkdir /tmp/tmp_dir    
        touch  /tmp/tmp_dir/passwd

		passwd_import -d /tmp/tmp_dir -v -u $CELLADMIN -p -dce- 2> /tmp/passwd_import_er.out 

		grep "Unable to access group file"  /tmp/passwd_import_er.out > /dev/null

		if [ $? -ne 0 ]
		then 
	    	STATUS=2
			EXIT=1 
			echo "\"Unable to access group file\" not in error msg"

  		else
    		if [ $TRACE = "1" ]
    		then
      			echo "Trace Message:     Obtained passwd_import error message" 
      			if [ $SFLAG = "1" ]
      			then
        			echo "Trace Message:      Obtained passwd_import error message" 1>&2
      			fi
    		fi
  		fi

  	;; # End case 2

		                    


	"3") 
		rm /tmp/tmp_dir/passwd    
        touch  /tmp/tmp_dir/group

		passwd_import -d /tmp/tmp_dir -v -u $CELLADMIN -p -dce- 2> /tmp/passwd_import_er.out 

		grep "Unable to access passwd file"  /tmp/passwd_import_er.out > /dev/null

		if [ $? -ne 0 ]
		then 
	    	STATUS=2
			EXIT=1
			echo "\"Unable to access passwd file\" not in error msg"

  		else
    		if [ $TRACE = "1" ]
    		then
      			echo "Trace Message:     Obtained passwd_import error message" 
      			if [ $SFLAG = "1" ]
      			then
        			echo "Trace Message:      Obtained passwd_import error message" 1>&2
      			fi
    		fi
  		fi

  	;; # End case 3

                    

	"4") 
		sed -e "/[\/ ]\*.*/d" ../moretests/passwd | sed -e "1d" > /tmp/tmp_dir/passwd
		sed -e "/[\/ ]\*.*/d" ../moretests/group  | sed -e "1d"  > /tmp/tmp_dir/group

		passwd_import -d /tmp/tmp_dir -v -o org -u $CELLADMIN -p -dce- 2> /tmp/passwd_import_er.out 

		grep "Registry object not found"  /tmp/passwd_import_er.out > /dev/null

		if [ $? -ne 0 ]
		then 
	    	STATUS=2
			EXIT=1  
			echo "\"Registry object not found\" not in error msg"

  		else
    		if [ $TRACE = "1" ]
    		then
      			echo "Trace Message:     Obtained passwd_import error message" 
      			if [ $SFLAG = "1" ]
      			then
        			echo "Trace Message:      Obtained passwd_import error message" 1>&2
      			fi
    		fi
  		fi

  	;; # End case 4
                                

	"5")   
        # 5.  Without -i (no name conflict resolution), no uid conflict resolution.
        
        # Add org (a new org name). 
		if [ $TRACE = "1" ]
		then
        	echo "Add org name"
        fi
        ../moretests/add_org.sh 
                   
        echo " "
        EXIT=$?
        
        if [ $EXIT -ne 0 ]
        then
        	echo "Add org name to rgy failed, please check the database and /tmp/add_org.out."
        	echo "Retry again."
        	exit $EXIT
        fi    
           
         
        echo "n" > /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        
        # run passwd_import
		if [ $TRACE = "1" ]
		then
        	echo "passwd_import -d /tmp/tmp_dir -v -o org -u $CELLADMIN -p -dce- < /tmp/passwd_import.script 2> /tmp/passwd_import_n.out"
       	fi
        passwd_import -d /tmp/tmp_dir -v -o org -u $CELLADMIN -p -dce- < /tmp/passwd_import.script > /tmp/passwd_import_n.out  2>& 1
       	echo " "
             
        # Change user's passwd and account status, so the user can login.
           
        echo "v" > /tmp/passwd_import.script
        echo "c -p bpc -g system -o org -pw -dce- -mp -dce- -av -pv"  >> /tmp/passwd_import.script
        echo "c -p rlr -g osdev -o org -pw -dce- -mp -dce- -av -pv" >> /tmp/passwd_import.script
        echo "do p" >> /tmp/passwd_import.script 
        echo "v" >> /tmp/passwd_import.script 
        echo "do g" >> /tmp/passwd_import.script 
        echo "v" >> /tmp/passwd_import.script 
        echo "quit" >> /tmp/passwd_import.script
           
        rgy_edit -up < /tmp/passwd_import.script >> /tmp/passwd_import_n.out
                 
        # nana should be the alias of wang, since they have same uid in passwd file.

        echo "do p" > /tmp/passwd_import.script
        echo "v wang -f" >> /tmp/passwd_import.script
        echo "quit" >> /tmp/passwd_import.script

        rgy_edit -up < /tmp/passwd_import.script > /tmp/passwd_import.out
       
        grep " pr " /tmp/passwd_import.out > /dev/null
        exit1=$?

        grep 31997 /tmp/passwd_import.out > /dev/null
        exit2=$?   

        if [ $exit1 = 0 -a $exit2 = 0 ]
        then  
        	EXIT=0
        else
    		STATUS=2 
    		EXIT=1
			if [ $TRACE = "1" ]
			then
				echo "Trace Message:     Imported rgy data is not correct, check /tmp/passwd_import_n.out"
			fi
     	fi  

        echo "do p" > /tmp/passwd_import.script
        echo "v nana -f" >> /tmp/passwd_import.script
        echo "quit" >> /tmp/passwd_import.script

        rgy_edit -up < /tmp/passwd_import.script > /tmp/passwd_import.out
       
        grep " al " /tmp/passwd_import.out > /dev/null
        exit1=$?

        grep 31997 /tmp/passwd_import.out > /dev/null
        exit2=$?   

        if [ $exit1 = 0 -a $exit2 = 0 ]
        then  
        	EXIT=0
        else
    		STATUS=2 
    		EXIT=1
			if [ $TRACE = "1" ]
			then
				echo "Trace Message:     Imported rgy data is not correct, check /tmp/passwd_import_n.out"
			fi
     	fi  


 	    #Try to login users imported from passwd  file
        ../moretests/binlogin bpc -dce- >> /tmp/passwd_import_n.out
        check_status $? 0 "binlogin bpc  -dce-"
         
        ../moretests/binlogin rlr -dce- >> /tmp/passwd_import_n.out
        check_status $? 0 "binlogin rlr  -dce-"
                         
		restore_db 5 /tmp/passwd_import_n.out                    
         
  	;; # End case 5
             
	"6")

	# 6. Without -i.   Existent org name, resolve bin as unix_bin and an alias of the same unix id
    # in group and principal domain.
    #
        
        echo "n" > /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "unix_bin" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "a">> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "a" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "unix_bin" >> /tmp/passwd_import.script 
        echo "y" >> /tmp/passwd_import.script
        echo "a" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        
        
        # run passwd_import
		if [ $TRACE = "1" ]
		then
        	echo "passwd_import -d /tmp/tmp_dir -v -o org -u $CELLADMIN -p -dce- < /tmp/passwd_import.script 2> /tmp/passwd_import_ny.out"
        fi
        passwd_import -d /tmp/tmp_dir -v -o org -u $CELLADMIN -p -dce- < /tmp/passwd_import.script > /tmp/passwd_import_ny.out  2>& 1
         
        echo " "
        echo "v" > /tmp/passwd_import.script 
        echo "c -p unix_bin -g unix_bin -o org -pw -dce- -mp -dce- -av -pv" >> /tmp/passwd_import.script
        echo "v bin -f" >> /tmp/passwd_import.script
        echo "v unix_bin -f" >> /tmp/passwd_import.script
        echo "do p" >> /tmp/passwd_import.script 
        echo "v" >> /tmp/passwd_import.script 
        echo "do g" >> /tmp/passwd_import.script 
        echo "v" >> /tmp/passwd_import.script 
        echo "q" >> /tmp/passwd_import.script
                  
        rgy_edit -up < /tmp/passwd_import.script  >> /tmp/passwd_import_ny.out
        
        ../moretests/binlogin unix_bin -dce- >> /tmp/passwd_import_ny.out
        check_status $? 0 "binlogin unix_bin  -dce-"
           
		restore_db 6 /tmp/passwd_import_ny.out
 
 ;; # case 6
    

	"7") 

        #  7.  Same as above with -i (ignore name conflict) with no resolution for conflict uid.
        
        
        echo "n" > /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        echo "n" >> /tmp/passwd_import.script
        
        
        # run passwd_import
 		if [ $TRACE = "1" ]
		then
       		echo "passwd_import -d /tmp/tmp_dir -v -i -o org -u $CELLADMIN -p -dce- < /tmp/passwd_import.script 2> /tmp/passwd_import_i.out"
        fi
        passwd_import -d /tmp/tmp_dir -v -i -o org -u $CELLADMIN -p -dce- < /tmp/passwd_import.script > /tmp/passwd_import_i.out  2>& 1
                
        
        echo " "
        echo "v" > /tmp/passwd_import.script 
        echo "c -p nana -g coco -o org -pw -dce- -mp -dce- -av -pv" >> /tmp/passwd_import.script
        echo "do p" >> /tmp/passwd_import.script 
        echo "v" >> /tmp/passwd_import.script 
        echo "do g" >> /tmp/passwd_import.script 
        echo "v" >> /tmp/passwd_import.script 
        echo "q" >> /tmp/passwd_import.script
                  
        rgy_edit -up < /tmp/passwd_import.script >> /tmp/passwd_import_i.out
                                     
        ../moretests/binlogin nana -dce- >> /tmp/passwd_import_i.out
        check_status $? 0 "binlogin nana  -dce-"
            
		restore_db 7 /tmp/passwd_import_i.out
        
	;; # case 7


	"8")

        # 8.   Ignore name conflict (with -i).  Resolve uid conflict with generating a new uid.
        #
        
        echo "y" > /tmp/passwd_import.script
        echo "g" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "g" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "g" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "g" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "g" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "g" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "g" >> /tmp/passwd_import.script
        
        
        # run passwd_import
		if [ $TRACE = "1" ]
		then
        	echo "passwd_import -d /tmp/tmp_dir -v -i -o org -u $CELLADMIN -p -dce- < /tmp/passwd_import.script 2> /tmp/passwd_import_ig.out"
        fi
        passwd_import -d /tmp/tmp_dir -v -i -o org -u $CELLADMIN -p -dce- < /tmp/passwd_import.script > /tmp/passwd_import_ig.out  2>& 1
                
        echo " "
        echo "v" > /tmp/passwd_import.script 
        echo "c -p lp -g lp -o org -pw -dce- -mp -dce- -av -pv" >> /tmp/passwd_import.script
        echo "c -p adm -g adm -o org -pw -dce- -mp -dce- -av -pv" >> /tmp/passwd_import.script 
        echo "do p" >> /tmp/passwd_import.script 
        echo "v" >> /tmp/passwd_import.script 
        echo "do g" >> /tmp/passwd_import.script 
        echo "v" >> /tmp/passwd_import.script 
        echo "q" >> /tmp/passwd_import.script
                  
        rgy_edit -up < /tmp/passwd_import.script >> /tmp/passwd_import_ig.out
        
        
        # Running binlogin instead of dce_login, so no shell exe happening.
        ../moretests/binlogin lp -dce- >> /tmp/passwd_import_ig.out
        check_status $? 0 "binlogin lp  -dce-"
              
        ../moretests/binlogin adm -dce-  >> /tmp/passwd_import_ig.out
        check_status $? 0 "binlogin adm  -dce-"
          
		restore_db 8 /tmp/passwd_import_ig.out
	;;    

	"9")
        #  9.  with -i, uid conflict resolution with entering new uid. 
        # 
        
        echo "y" > /tmp/passwd_import.script
        echo "e" >> /tmp/passwd_import.script
        echo "31500" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "e" >> /tmp/passwd_import.script
        echo "31501" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "e" >> /tmp/passwd_import.script
        echo "31502" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "e" >> /tmp/passwd_import.script
        echo "31503" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "e" >> /tmp/passwd_import.script
        echo "31504" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "e" >> /tmp/passwd_import.script
        echo "31505" >> /tmp/passwd_import.script
        echo "y" >> /tmp/passwd_import.script
        echo "e" >> /tmp/passwd_import.script
        echo "31506" >> /tmp/passwd_import.script
        
        
        
        # run passwd_import
		if [ $TRACE = "1" ]
		then
        	echo "/opt/dcelocal/bin/passwd_import -d /tmp/tmp_dir -v -i -o org -u $CELLADMIN -p -dce- < /tmp/passwd_import.script 2> /tmp/passwd_import_ie.out"
        fi
        passwd_import -d /tmp/tmp_dir -v -i -o org -u $CELLADMIN -p -dce- < /tmp/passwd_import.script > /tmp/passwd_import_ie.out  2>& 1
        
        echo " "
        echo "v" > /tmp/passwd_import.script 
        echo "c -p lp -g lp -o org -pw -dce- -mp -dce- -av -pv" >> /tmp/passwd_import.script
        echo "c -p adm -g adm -o org -pw -dce- -mp -dce- -av -pv" >> /tmp/passwd_import.script 
        echo "do p" >> /tmp/passwd_import.script 
        echo "v" >> /tmp/passwd_import.script 
        echo "do g" >> /tmp/passwd_import.script 
        echo "v" >> /tmp/passwd_import.script 
        echo "q" >> /tmp/passwd_import.script
                  
        rgy_edit -up < /tmp/passwd_import.script  >> /tmp/passwd_import_ie.out
                                     
		grep "adm " /tmp/passwd_import_ie.out | grep "31505" >> /tmp/passwd_import_ie.out
		exit1=$?

		grep "lp " /tmp/passwd_import_ie.out | grep "31506" >> /tmp/passwd_import_ie.out
		exit2=$?
           
		grep "root " /tmp/passwd_import_ie.out | grep "31500" >> /tmp/passwd_import_ie.out
		exit3=$?   

		grep "other " /tmp/passwd_import_ie.out | grep "31501" >> /tmp/passwd_import_ie.out
		exit4=$?
         
		grep "sys " /tmp/passwd_import_ie.out | grep "31502" >> /tmp/passwd_import_ie.out
		exit5=$?  

		if [ $exit1 = 0 -a $exit2 = 0 -a $exit3 = 0 -a $exit4 = 0 -a $exit5 = 0 ]
		then
        	EXIT=0
        else
    		STATUS=2 
    		EXIT=1
			if [ $TRACE = "1" ]
			then
				echo "Trace Message:     Imported rgy data is not correct, check /tmp/passwd_import__ie.out"
			fi
     	fi  

  
        # Running binlogin instead of dce_login, so no shell exe happening.
        ../moretests/binlogin lp -dce-  >> /tmp/passwd_import_ie.out
        check_status $? 0 "binlogin lp  -dce-"
        
        ../moretests/binlogin adm -dce- >> /tmp/passwd_import_ie.out
        check_status $? 0 "binlogin adm  -dce-"
        
        
        # Remove the added account, group, and principal for next test.
        restore_db 9 /tmp/passwd_import_ie.out
        
        # Remove org name form rgy DB.
         
        echo " "
        echo "delete org name"
        ../moretests/del_org.sh >> /tmp/passwd_import_ie.out
        
        if [ $? -ne 0 ]
        then
        	STATUS=3
        	EXIT=1
        	echo "Delete org name to rgy failed"
        	exit $EXIT
        fi    
        
	;; # case 9

esac

#
# Report passwd_import test results.
#

if [ $FAIL = "1" ]
then

  case $STATUS in

    "1")

    echo "Failure Message:   Setup failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   Setup failed with error $EXIT" 1>&2
    fi

    ;; # End case 1

    "2")

    echo "Failure Message:   passwd_import test case failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   passwd_import test case failed with error $EXIT" 1>&2
    fi

    ;; # End case 2

    "3")

    echo "Failure Message:   Cleanup failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   Cleanup failed with error $EXIT" 1>&2
    fi

    ;; # End case 3

  esac # End case $STATUS in

fi # End if [ $FAIL = "1" ]

if [ $SUCCESS = "1" -a $EXIT -eq 0 ]
then
  echo "Test Status:       Test SUCCEEDED"
  if [ $SFLAG = "1" ]
  then
    echo "Test Status:       Test SUCCEEDED" 1>&2
  fi
fi

if [ $FAIL = "1" -a $EXIT -gt 0 ]
then
  echo "Test Status:       Test FAILED"
  if [ $SFLAG = "1" ]
  then
    echo "Test Status:       Test FAILED" 1>&2
  fi
fi

#
# Exit this shell and return error code.
#
exit $EXIT

#
# End script passwd_import.sh
#

