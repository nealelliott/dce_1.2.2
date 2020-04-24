
/*
 * (C) Copyright 1996 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */


/*
Create a file of specified length.

	first argument		name of file
	second argument		size in 8 kb blocks

So, for a 32 mbyte file, the second argument would be 4096.
*/

int descriptor;

#include <stdio.h>
#include <sys/param.h>
#include <sys/file.h>
#include <fcntl.h> /* CREAT, RDWR */
#include <unistd.h>

main(argc,argv)
int argc;
char **argv;
{
	char buffer[8192];	/* 8 kbyte buffer */
	int j;
	int max = 8192;		/* 8 kbytes */

	char file[MAXPATHLEN];	/* filename */
	int blocks;		/* 8 kb blocks */

	int error=0;

	if(argc != 3)
	{
		fprintf(stderr,"Usage: %s file blocks(8 kb)\n",*argv);
		exit(1);
	}

	sscanf(argv[1],"%s",file);
	sscanf(argv[2],"%d",&blocks);

	if(blocks<0)
	{
		fprintf(stderr,"Use blocks >=0\n");
		exit(1);
	}

	fprintf(stdout,"Creating %s to be %d bytes\n",file,blocks*max);
	fflush(stdout);

	if((descriptor=open(file,O_CREAT|O_RDWR,0666))<0)
	{
		perror("can't open");
		exit(1);
	}

	for(j=0;j<blocks;++j)
	{
		if(write(descriptor,buffer,max) != max)
		{
			perror("can't write");
			error=1;
			break;
		}
	}
	
	close(descriptor);
	exit(error);
}
