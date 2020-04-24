/*
 * (C) COPYRIGHT Transarc Corp. 1992
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */

#include <dcedfs/stds.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <dce/daclif.h>
#include <dce/rpc.h>
#include <dce/pgo.h>
#include <dce/sec_login.h>
#include <dcedfs/aclint.h>
#include <dcedfs/syscall.h>
#include "check_status.h"
#include "common.h"

#ifndef R_ACCESS
#define R_ACCESS 1
#endif

#ifndef W_ACCESS
#define W_ACCESS 2
#endif

#ifndef X_ACCESS
#define X_ACCESS 4
#endif

#ifndef C_ACCESS
#define C_ACCESS 8
#endif

#ifndef I_ACCESS
#define I_ACCESS 16
#endif

#ifndef D_ACCESS
#define D_ACCESS 32
#endif

#ifndef T_ACCESS
#define T_ACCESS 64
#endif

#ifndef A_ACCESS
#define A_ACCESS 128
#endif

/*
Convert a set of acl permissions into a number for easy comparison

	RETURN: The value of the acl as a number.
*/

EXPORT int acl_to_num(IN char *acl)
{
	char *tmp;
	int retval = 0;

	for (tmp=acl; *tmp; tmp++)
	{
		switch (*tmp)
		{
	  		case 'r':
				retval |= R_ACCESS;
				break;

	  		case 'w':
				retval |= W_ACCESS;
				break;

	  		case 'x':
				retval |= X_ACCESS;
				break;

	  		case 'c':
				retval |= C_ACCESS;
				break;

	  		case 'i':
				retval |= I_ACCESS;
				break;

	  		case 'd':
				retval |= D_ACCESS;
				break;

	  		case 't':
				retval |= T_ACCESS;
				break;

	  		case 'a':
				retval |= A_ACCESS;
				break;
		}
	}

	return(retval);
}


EXPORT int check_access(IN char *filename, IN int bind_to_entry)
{
	struct stat statbuf;
	char file[256];

	printf("Check Access: %s: ",filename);
	fflush(stdout);

	check_access_by_acl(filename);

	sprintf(file,"%s",filename);

	if (stat(file, &statbuf)<0)
	{
		printf("%s is NOT ACCESSIBLE\n",filename);
		fflush(stdout);
		return 0;
	}

	if (statbuf.st_mode & S_IFREG)
	{
		/* itsa file */

		printf("%s is a FILE\n",filename);
		fflush(stdout);

		return check_file_access(filename);
	}
	else if (statbuf.st_mode & S_IFDIR)
	{
		/* itsa dir */

		printf("%s is a DIRECTORY\n",filename);
		fflush(stdout);

#ifdef SUNOS5
		if ((statbuf.st_atim.tv_nsec == 0) &&
			(statbuf.st_mtim.tv_nsec == 0) &&
			(statbuf.st_ctim.tv_nsec == 0))
		{
#else
		/* ifdef AIX32 */

	    	if ((statbuf.st_atime == 0) &&
	        	(statbuf.st_mtime == 0) &&
	        	(statbuf.st_ctime == 0))
		{
#endif

			printf("%s has ILLEGAL ATTRIBUTES\n",
				filename);
			fflush(stdout);

			return 0;
		}

		return check_dir_access(filename);
	}
}

/* Check for access on a dfs directory */

PRIVATE int check_dir_access(IN char *dirname)
{
  	/* Permissions to check for: rwxidc */

  	char buf[MAXPATHLEN + 64], *tmpfile, *shortname;
  	DIR *dirp;
  	int retval = 0, fd, ret, i = 0;

  	/*
		Check for r
  		The basic check is to see if we can list
		the files in the directory.
  	*/

	if ((dirp = opendir(dirname)) != NULL)
		retval |= R_ACCESS;
	else
	{
		printf("check_dir_access:Could not open %s\n",
			dirname);
		fflush(stdout);
	}


	/*
		Check for x
		The basic check is to see if we can cd to the directory.
	*/

	if (getcwd(buf, MAXPATHLEN) != NULL)
		if (chdir(dirname) != -1)
		{
			retval |= X_ACCESS;
			chdir(buf);
		}
		else
		{
			printf("check_dir_access:Could not chdir %s\n",
				dirname);
			fflush(stdout);
		}
	else
	{
		printf("check_dir_access:Could not getcwd\n");
		fflush(stdout);
	}

	/*
		Check for w
	 	No simple check for w.
	*/

	/* Check for i */

	sprintf(buf, "%s/ck%d.%d", dirname, getpid(), time(NULL));

	tmpfile = buf;
	if ((fd = open(tmpfile, O_RDWR | O_CREAT, 0640)) != -1)
	{
		if (write(fd, "clackety, clack...clackety, \
			 clack..clack,clack,clack,clack, \
			woosh..............", 75) != 75)
		{
			printf("check_dir_access:Cannot write %s\n",
				tmpfile);
			fflush(stdout);
		}
		else
		{
			retval |= I_ACCESS;
			retval |= X_ACCESS;
			retval |= W_ACCESS;
		}
		close(fd);
	}
	else
	{
		printf("check_dir_access:Cannot open %s\n",
			tmpfile);
		fflush(stdout);
	}

	/* Check for d */

	sprintf(buf, "%s/.to_delete.%d", dirname, i);
	do
	{
		i++;
		sprintf(buf, "%s/.to_delete.%d", dirname, i);
	}
	while (access(buf, F_OK) == 0);

	i--;
	sprintf(buf, "%s/.to_delete.%d", dirname, i);

	if (access(buf, F_OK) != -1)
	{
		if (unlink(buf) != -1)
		{
			retval |= D_ACCESS;
			retval |= X_ACCESS;
			retval |= W_ACCESS;
		}
		else
		{
			printf("check_dir_access:Cannot unlink %s\n",
				buf);
			fflush(stdout);
		}
	}
	else
	{
		printf("check_dir_access:Cannot access %s\n",
			buf);
		fflush(stdout);
	}

	/* Check for c */

	sprintf(buf, "/bin/acl_edit %s -m `/bin/acl_edit %s -l \
		 | /bin/grep user_obj`> /dev/null 2>&1", dirname,
		 dirname);
	ret = system(buf);
	if (ret/256 == 0)
		retval |= C_ACCESS;

	if ((shortname = strrchr(dirname, '/')) == NULL)
		shortname = dirname;
	else
		shortname++;

	return retval;
}

/* Check for access on a dfs file */

PRIVATE int check_file_access(IN char *filename)
{
	int fd, i;
	int retval = 0, ret;
	char *shortname, buf[MAXPATHLEN + 64], *ptr, *data;
	struct stat statbuf;

	strcpy(buf, filename);
	if ((ptr = strrchr(buf, '/')) == NULL)
		buf[0] = '\0';
	else
	{
		ptr++;
		*ptr = '\0';
	}
	strcat(buf, ".to_copy");

	if (stat(buf, &statbuf) == -1)
	{
		printf("%s not found", buf);
		fflush(stdout);
	}
	else
	{
		data = (char *) malloc(statbuf.st_size);

		if ((fd = open(buf, O_RDONLY)) != -1)
		{
			if (read(fd, data, statbuf.st_size)
				 != statbuf.st_size)
			{
				printf("Couldnt read %d bytes \
					 in %s\n",
					statbuf.st_size, buf);
				fflush(stdout);
			}
		}
		close(fd);
	}

	/* Check for w */

	if ((fd = open(filename, O_WRONLY | O_TRUNC)) != -1)
	{
		if (write(fd, data, statbuf.st_size) == statbuf.st_size)
			retval |= W_ACCESS;
		else
		{
			printf("check_file_access: cannot write \
				%s\n",filename);
			fflush(stdout);
		}
	}
	else
	{
		printf("check_file_access: cannot open for write %s\n",
			filename);
		fflush(stdout);
	}
	close(fd);

	/* Check for r */

	if ((fd = open(filename, O_RDONLY)) != -1)
	{
		if (read(fd, buf, MAXPATHLEN) == MAXPATHLEN)
		{
			if (memcmp(buf, data, MAXPATHLEN) == 0)
				retval |= R_ACCESS;
			else
			{
				printf("check_file_access: \
					cannot verify %s\n",
					filename);
				fflush(stdout);
			}
		}
		else
		{
			printf("check_file_access: cannot read %s\n",
				filename);
			fflush(stdout);
		}
	}
	else
	{
		printf("check_file_access: cannot open for read %s\n",
			filename);
	        fflush(stdout);
	}
	close(fd);

	free(data);

	sprintf(buf, "%s > /dev/null 2>&1", filename);
	ret = system(buf);
	if (ret/256 == 0)
		retval |= X_ACCESS;

	/* Check for c */

	sprintf(buf, "/bin/acl_edit %s -m `/bin/acl_edit %s -l \
		 | /bin/grep user_obj` > /dev/null 2>&1",
		 filename, filename);
	ret = system(buf);
	if (ret/256 == 0)
		retval |= C_ACCESS;

	return retval;
}

check_access_by_acl(filename)
char *filename;
{
	char buf[256];

	sprintf(buf,"dcecp -c acl check %s",filename);

	system(buf);
}
