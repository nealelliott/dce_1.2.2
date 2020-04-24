#
# @OSF_FREE_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1993, 1992, 1991, 1990, 1996 Open Software Foundation, Inc.
# Permission is hereby granted to use, copy, modify and freely distribute
# the software in this file and its documentation for any purpose without
# fee, provided that the above copyright notice appears in all copies and
# that both the copyright notice and this permission notice appear in
# supporting documentation.  Further, provided that the name of Open
# Software Foundation, Inc. ("OSF") not be used in advertising or
# publicity pertaining to distribution of the software without prior
# written permission from OSF.  OSF makes no representations about the
# suitability of this software for any purpose.  It is provided "as is"
# without express or implied warranty.
# 
# HISTORY
# $Log: osf.parse_ext.mk,v $
# Revision 1.1.13.2  1996/03/09  23:25:24  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:01  marty]
#
# Revision 1.1.13.1  1995/12/07  22:10:31  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:08:50  root]
# 
# Revision 1.1.11.1  1994/05/20  22:03:29  sommerfeld
# 	Deal with absolute object dir.
# 	[1994/05/17  18:36:57  sommerfeld]
# 
# Revision 1.1.9.1  1993/11/12  16:37:17  zeliff
# 	fixing syntax error
# 	[1993/11/12  16:37:00  zeliff]
# 
# 	OT7996: cleanup uniquelist files
# 	[1993/11/12  16:13:10  zeliff]
# 
# Revision 1.1.7.2  1993/09/23  20:19:55  zeliff
# 	Defect 8662 - Refix value of CACHEFILE
# 	[1993/09/22  21:27:15  zeliff]
# 
# Revision 1.1.5.3  1992/12/31  16:47:58  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:52:00  zeliff]
# 
# Revision 1.1.5.2  1992/12/23  17:35:10  john
# 	Fixed the copyright marker to be OSF_FREE_COPYRIGHT.
# 	[1992/12/23  17:14:37  john]
# 
# Revision 1.1.2.4  1992/05/20  17:17:15  mhickey
# 	Added code to allow including Makefiles to specify that the
# 	cache should be reloaded the first time this makefile is
# 	included on a particular pass.
# 
# 	Defect 3682, 3643
# 	[1992/05/20  17:14:05  mhickey]
# 
# Revision 1.1.2.3  1992/05/18  21:10:33  mhickey
# 	Added ability to cache results of parsing generated makefiles as
# 	an optimization to reduce the performance hit of use of
# 	parse_ext in frequently parsed makefiles that do the same
# 	parsing all the time.  Added lots of helpful comments.
# 
# 	This is necessary because of defect 2403.
# 	[1992/05/18  21:05:48  mhickey]
# 
# Revision 1.1.2.2  1992/05/11  20:44:18  mhickey
# 	Initial submit of extended parsing makefile.
# 
# 	Defect 2404
# 	[1992/05/11  20:43:43  mhickey]
# 
# $EndLog$
# 
#
# This makefile generates a makefile that performs extended parsing of makefile.
# The makfile is invoked in a "procedure-call" manner, as follows:
#   parse_ext_args= action_<ACTION> list_<LIST>...
#   .include <osf.parse_ext.mk>
#
# The value of parse_ext_args will vary depending on the extended parsing desired.
# The current list of parsing extensions and their usage follows.  Detailed 
# descriptions of the arguements are given in the comments at the top of the 
# stanza for each ACTION.
#
#
#  UNIQUELIST
#   Function:
#     removes duplicates from and sorts the list ${_CLOBBER_EXP_SUBDIRS_}
#     and returns the new list in _CLOBBER_EXP_SUBDIRS_
#
#   Usage: 
#          parse_ext_args = action_UNIQUELIST list__CLOBBER_EXP_SUBDIRS_
#          .include <osf.parse_ext.mk>
#
#
#  ADDEXISTS
#   Function:
#     Adds existing directories from one list to another list.
#     
#   Usage: 
#     parse_ext_args = action_ADDEXISTS list_<LISTNAME> dirs_<DIRLIST> \
#                      prefix_-<PREFIX> [addhow_<PREPEND|APPEND> ] [cachetag_<TAG>]
#          .include <osf.parse_ext.mk>
#
#
#  GENLIBLNS
#   Function:
#     Creates and parses a XX_LIBS = ... line for each member of the PROGRAMS list
#     in the including Makefile. 
#
#   Usage:      
#     parse_ext_args = action_UNIQUELIST list_<LISTNAME> [cachtag_<TAG>]
#          .include <osf.parse_ext.mk>
#
#
# NOTE: The temporary mzkefiles created by all of the extended parsing functions
#       can be preserved after the run of make by specifying -DDEBUG_<ACTION> on
#       the make command line.

#
# Set the cache file to be in the OBJECTDIR if defined, or /tmp
#
.if defined(OBJECTDIR) && !empty(OBJECTDIR:M/*)
CACHEFILE := ${OBJECTDIR}/pe_cache
.else
CACHEFILE := ${OBJECTDIR:D${_MAKECONF:S/Makeconf//}${OBJECTDIR}/pe_cache:U/tmp/pe_cache}
.endif

#
# Define cleanup targets if we are run by the top make
#
.INTERRUPT:
	@rm -f ${CACHEFILE}

.ifndef _SUBMAKE_

.ifndef DEBUG_DIDONCE
.EXIT:
	@rm -f ${CACHEFILE} 

.endif # DEBUG_DIDONCE
.endif # if _SUBMAKE_

#
# make sure __CTAG is undefined so that it can act as a flag
#
.undef __CTAG

#
# Check to see if there are args defined, and if an action is specified
#
.if defined(parse_ext_args) && !empty(parse_ext_args:Maction_*)
#
# set ACTION to specified action
#
  ACTION = ${parse_ext_args:Maction_*:S/^action_//}
  REQ_ACTION :=${ACTION}

  ###############
  ### Caching ###
  ###############
  #
  # check to see if we are supposed to cache the results of this call
  #
.if !empty(parse_ext_args:Mcachetag_*)
  #
  # We are supposed to be caching.  Set the __CTAG variable for the cache
  # search.
  #
  __CTAG = ${parse_ext_args:Mcachetag_*:S/^cachetag_//}
  __MPASS_ = ${MAKEFILE_PASS:D${MAKEFILE_PASS}:UNONE}
  __LDPASS_= ${parse_ext_args:Mcacheload_*:S/cacheload_//}

  #
  # see if the cache file exists
  #
.if exists(${CACHEFILE})
  # 
  # set the cache hit flag to false
  #
.undef __DID_ONCE_
.undef _CACHESET_
  # 
  # go look for a match for this cachetag (__CTAG).  The cache file consists of 
  # a bunch of stanzas of the form ".if ${__CTAG} == "..." that set whatever 
  # needs to be set to get the effect of having run the shell commands for the
  # request associated with the cache tag.  If a match is found, __DID_ONCE_ is
  # set to indicate that the match was found.
  #
.include "${CACHEFILE}"
.endif  # if exists
  
  #
  # Did we get a hit?
  #
.ifdef __DID_ONCE_

  #
  # clear the hit flag
  #
.undef __DID_ONCE_

  #
  # set ACTION to indicate that we already handled the request
  #
  ACTION = DONE

  #
  # See if we are explicity told to reload the cache on this pass.  Some
  # Makefiles need to tell us to do this so that directories that are 
  # created in initial passes can be added to the cached assignment 
  # statement for use by later passes.
  #
.if !empty(parse_ext_args:Mcacheload_*)
 
  #
  # If MAKEFILE_PASS is not defined, or is not equal to the current pass,
  # then look at the cache; otherwise, skip it, so that the ACTION will
  # be re-executed, and the cache will be reloaded
  # 

.if ${__MPASS_} == "${__LDPASS_}" && ${__CACHESET_} != "${__LDPASS_}"

  #
  # Reset action to make the cache get reloaded
  #
  ACTION=${REQ_ACTION}

.endif  # if ${__MPASS} ...

  # 
  # close our conditionals
  #
.endif  # if !empty(..cacheload..)

  #
  # the usual cleanup
  #


.undef __CTAG_IS
.undef __CACHESET_

  # 
  # close our conditionals
  #
.endif  # ifdef __DID_ONCE_
.endif  # if !empty(..cachetag..)
  ####
  #### END OF CODE FOR CHECKING THE CACHE
  ####

.if ${ACTION} == "ADDEXISTS"
      #################
      ### ADDEXISTS ###
      #################
      #  
      # code for adding existing directories in dirs to list
      #
      # Format for parse_ext_args is 
      #  parse_ext_args = action_ADDEXISTS list_<LISTNAME> dirs_<DIRLIST> \
      #                   prefix_-<PREFIX> [addhow_<PREPEND|APPEND> ] \
      #                   [cachetag_<TAG> ]
      #
      #  where:
      #    LISTNAME   is the name of the list to which existing directories in the
      #               list DIRLIST are to be pre/appended.
      #               
      #    DIRLIST    is the list of directories whose existence is checked.  These
      #               should not have any prefix; they should be absolute paths.
      #
      #    PREFIX     is the prefix to add to each directory in DIRLIST that is
      #               found to exist, before adding it to LISTNAME.
      #
      #    PREPEND|APPEND [optional]  is one of APPEND or PREPEND that indicates
      #                               which end of the list LISTNAME existing 
      #                               directories in DIRLIST are to be added to.
      #
      #    TAG [optional]  is a unique string that marks this invocation of 
      #                    parse_ext.  The presence of this arguement indicates 
      #                    that the results of this invocation are to be cached, 
      #                    and re-used each parse_ext is invoked with this 
      #                    cachetag value.
      #
      # check args
      #
.if   !empty(parse_ext_args:Mlist_*) && !empty(parse_ext_args:Mdirs_*) && \
      !empty(parse_ext_args:Mprefix_*)
       #
       # parse the args
       #
       __DIRS := ${${parse_ext_args:Mdirs_*:S/^dirs_//}}
       __LISTNM := ${parse_ext_args:Mlist_*:S/^list_//}
       __LIST := ${${__LISTNM}}
       __PREFIX := ${parse_ext_args:Mprefix_*:S/^prefix_//}
       
       #
       # Make sure that the input lists are not empty
       #
.if    !empty(__DIRS) && !empty(__LIST)
        #
        # if the addhow is not specified, then default to APPEND'ing existing
        #  members of __DIRS to __LIST
        #
.if     !empty(parse_ext_args:Maddhow_*)
          __ADDHOW := ${parse_ext_args:Maddhow_*:S/^addhow_//}
.else
          __ADDHOW = APPEND
.endif  # if !empty

.if     ${__ADDHOW} == "APPEND"
          #
          # execute a shell script that creates a Makfile that checks for 
          # the directories in list __DIRS, and appends them, prefixed with the
          # specified prefix, to the list __LISTNM.
          #
     DO_IT != \
      echo "${DEBUG_ADDEXISTS:D__LIST_IS=${__LIST}}" > /tmp/addexists$$$$; \
      echo "${DEBUG_ADDEXISTS:D__DIRS_ARE=${__DIRS}}" >> /tmp/addexists$$$$;\
      echo "${DEBUG_ADDEXISTS:D__LISTNM_IS=${__LISTNM}}" >> /tmp/addexists$$$$;\
      for i in ${__DIRS};\
      do \
        echo ".if exists($$i)" >> /tmp/addexists$$$$;\
        echo "   __LIST += \$${__PREFIX}$$i" >> /tmp/addexists$$$$;\
        echo .endif >> /tmp/addexists$$$$;\
      done;\
      echo "${__LISTNM} := \$${__LIST}" >> /tmp/addexists$$$$; \
      echo "$$$$"
.else
          #
          # execute a shell script that creates a Makfile that checks for 
          # the directories in list __DIRS, and prepends them, prefixed with the
          # specified prefix, to the list __LISTNM.  NOTE: to preserve the 
          # generated makefile, run the build with -DDEBUG_ADDEXISTS. 
          #
      DO_IT != \
      echo "${DEBUG_ADDEXISTS:D__LIST_IS=${__LIST}}" > /tmp/addexists$$$$; \
      echo "${DEBUG_ADDEXISTS:D__DIRS_ARE=${__DIRS}}" >> /tmp/addexists$$$$;\
      echo "${DEBUG_ADDEXISTS:D__LISTNM_IS=${__LISTNM}}" >> /tmp/addexists$$$$;\
      for i in ${__DIRS};\
      do \
         echo ".if exists($$i)" >> /tmp/addexists$$$$;\
         echo "  __TLIST += \$${__PREFIX}$$i" >> /tmp/addexists$$$$;\
         echo .endif >> /tmp/addexists$$$$;\
      done;\
      echo "${__LISTNM} := \$${__TLIST} \$${__LIST}" >> /tmp/addexists$$$$; \
      echo "$$$$"
.endif  # if _ADDHOW

        #
        # include the makefile we just created to do the requested parsing
        #
.include  "/tmp/addexists${DO_IT}"
       
        #
        # if we are caching, then add to the cache
        #
.ifdef __CTAG

__SET_DIDONCE != \
        echo "${DEBUG_DIDONCE:D__CTAG_IS = ${__CTAG}}" >> ${CACHEFILE};\
        echo '.if $${__CTAG} == "${__CTAG}"' >> ${CACHEFILE};\
        echo "    ${__LDPASS_:D__CACHESET_=${__MPASS_}}" >> ${CACHEFILE};\
        echo "    __DID_ONCE_=" >> ${CACHEFILE};\
        echo '    ${__LISTNM} := ${${__LISTNM}}'>> ${CACHEFILE};\
        echo ".endif" >> ${CACHEFILE}

.undef __CTAG
.undef __SET_DIDONCE
.endif # ifdef __CTAG

        #
        # Cleanup the variables
        #
.if     !defined(DEBUG_ADDEXISTS)
          RM_IT != rm -f /tmp/addexists${DO_IT}
.endif  # if defined DEBUG

.undef  DO_IT
.undef  RM_IT
.undef  __PREFIX
.undef  __DIRS
.undef  __ADDHOW
.undef  __LIST_IS
.undef  __DIRS_ARE
.undef  __LISTNM_IS
.undef  __TLIST
.undef  __LIST
.undef  __LISTNM
.undef parse_ext_args

        # 
        # close conditionals
        #
.endif  # !empty(__LIST)...       
.endif  # !empty(parse_ext_args...

####
#### end of code for ADDEXISTS
####

.elif ${ACTION} == "GENLIBLNS"
    ####################
    #### GENLIBLNS #####
    ####################
    #
    # code for generating library lists for each item in the PROGRAMS list  
    #
    # format of parse_ext_args is:
    #
    #    parse_ext_args = action_GENLIBLNS list_<LIBLIST> [cachetag_<TAG>]
    #
    # where
    #   LIBLIST   is the list of '-l' specifications to use for each XX_LIBS line
    #             generated.
    #
    #   TAG [optional]  is a unique string that marks this invocation of 
    #                   parse_ext.  The presence of this arguement indicates 
    #                   that the results of this invocation are to be cached, 
    #                   and re-used each parse_ext is invoked with this 
    #                   cachetag value.
    #
    #
    # check args/environment
    #
.if !empty(parse_ext_args:Mlist_*) && defined(PROGRAMS)

     #
     # parse args 
     #
     __LIBLIST := ${${parse_ext_args:Mlist_*:S/^list_//}}
     
     #
     # make sure input list is not empty
     #
.if  !empty(__LIBLIST)
      #
      # create the temp makefile
      #
      DO_IT != \
       echo "${DEBUG_GENLIBLNS:D__LIST_IS=${__LIBLIST}}" > /tmp/genliblns$$$$;\
       for i in ${PROGRAMS}; \
       do \
          echo $${i}_LIBS = ${__LIBLIST} >> /tmp/genliblns$$$$; \
       done; \
       echo "$$$$"

      #
      # include the makefile we just created to get the required parsing done
      #
.include "/tmp/genliblns${DO_IT}"

      #
      # if we are caching, then add to the cache
      #
.ifdef __CTAG

__SET_DIDONCE != \
        echo "${DEBUG_DIDONCE:D__CTAG_IS = ${__CTAG}}" >> ${CACHEFILE};\
        echo '.if $${__CTAG} == "${__CTAG}"' >> ${CACHEFILE};\
        echo "    __DID_ONCE_=" >> ${CACHEFILE};\
        tail +2 /tmp/genliblns${DO_IT} >> ${CACHEFILE};\
        echo ".endif" >> ${CACHEFILE} 

.undef __CTAG
.undef __SET_DIDONCE
.endif # ifdef __CTAG

      #
      # Clean up
      #
.if   !defined(DEBUG_GENLIBLNS)
        RM_IT != rm -f /tmp/genliblns${DO_IT}
.endif  # if !defined(/tmp...)
.undef DO_IT
.undef  RM_IT
.undef __LIST_IS
.undef __LIBLIST
.undef parse_ext_args

      #
      # Close conditionals
      #
.endif # !empty(__LIBLIST)
.endif # !empty(parse_ext_args...)

####
#### End of code for GENLIBLNS
####

.elif ${ACTION} == "UNIQUELIST"
    ##################
    ### UNIQUELIST ### 
    ##################
    #
    # code for uniquifying a list  
    #
    # format for parse_ext_args is 
    #
    #   parse_ext_args = action_UNIQUELIST list_<LISTNAME> [cachtag_<TAG>]
    #
    # where
    #   LISTNAME  on input is the list of items to remove duplicates from, and
    #             on output is the unique list.
    # 
    #   TAG [optional]  is a unique string that marks this invocation of 
    #                   parse_ext.  The presence of this arguement indicates 
    #                   that the results of this invocation are to be cached, 
    #                   and re-used each parse_ext is invoked with this 
    #                   cachetag value.
    #
    #   
    # check args/environment
    #
.if !empty(parse_ext_args:Mlist_*)
     #
     # parse args 
     #
     __LISTNM := ${parse_ext_args:Mlist_*:S/^list_//}
     __LIST := ${${__LISTNM}}

     #
     # Make sure the input list is not empty
     #
.if  !empty(__LIST)

      #
      # generate the unique list
      #
      DO_IT != \
        echo "${DEBUG_UNIQUELIST:D__LIST_IS=${__LIST}}" > /tmp/uniquelist$$$$; \
        echo "${DEBUG_UNIQUELIST:D__LISTNM_IS=${__LISTNM}}" >> /tmp/uniquelist$$$$;\
        echo '${__LISTNM} != for i in $${__LIST}; do echo $$$$i; done | sort -u' >> /tmp/uniquelist$$$$; \
        echo "$$$$"

      #
      # include the makefile we just created to get the requested parsing done
      #
.include "/tmp/uniquelist${DO_IT}"

      #
      # if we are caching, then add to the cache
      #
.ifdef __CTAG

__SET_DIDONCE != \
        echo "${DEBUG_DIDONCE:D__CTAG_IS = ${__CTAG}}" >> ${CACHEFILE};\
        echo '.if $${__CTAG} == "${__CTAG}"' >> ${CACHEFILE};\
        echo "    __DID_ONCE_=" >> ${CACHEFILE};\
        echo '    ${__LISTNM} := ${${__LISTNM}}'>> ${CACHEFILE};\
        echo ".endif" >> ${CACHEFILE} 
.undef __CTAG
.undef __SET_DIDONCE
.endif # ifdef __CTAG

      #
      # cleanup
      #
.if !defined(DEBUG_UNIQUELIST)
          RM_IT != rm -f /tmp/uniquelist${DO_IT}
.endif # if !defined(DEBUG_UNIQUELIST

.undef  DO_IT
.undef  RM_IT
.undef  __LISTNM
.undef  __LIST
.undef  __LIST_IS
.undef  __LSTNAME_IS
.undef parse_ext_args

.endif  # !empty (__LIST)
.endif  # !empty (parse_ext_args...)

####
#### end of code for UNIQUELIST
####

.endif # ${ACTION} ==
.endif # defined(parse_ext_args) ...
.undef parse_ext_args
.undef ACTION
.undef __LDPASS_
.undef __MPASS_
####
#### END OF PARSE_EXT
####


