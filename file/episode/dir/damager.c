/*  Copyright 1994 Transarc Corp., all rights reserved */
/*
 *	Damager -- for use with dtest
 *
 *	Example:
 *	    dtest -f foo baz 30		create directory with 30 entries
 *	    damager foo 5 0x20		set high water mark (at offset 5) to 32
 *	    dtest -c foo		see if salvager catches it
 */
#include <unistd.h>
#include <fcntl.h>
main(argc,argv)
  char **argv; int argc;
{
  char c;
  int f = open (argv[1], O_RDWR);
  lseek (f, strtol(argv[2], NULL, 0), SEEK_SET);
  c = (char) strtol(argv[3], NULL, 0);
  write (f, &c, 1);
}
