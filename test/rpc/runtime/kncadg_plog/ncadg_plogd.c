/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ncadg_plogd.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:25  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:54  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:14:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:02  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:53:30  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:21:03  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  15:51:38  rsalz
 * 	Edited as part of rpc6 drop.
 * 	[1992/05/01  00:58:05  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:43:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * ncadg_plogd <vmunix>
 * read the ncadg pkt log (for subsequent processing by ncadg_plog_dump).
 *      stdin  - /dev/kmem
 *      stdout - binary log file output
 *
 * e.g. ncadg_plogd /vmunix </dev/kmem >plog.dat
 * or perhaps in a script taking "timely" snapshots
 *  #!/bin/sh
 *  while :
 *  do
 *  	echo "ncadg_plogd"
 *  	ncadg_plogd /vmunix </dev/kmem >>plog.dat
 *  	sleep 2
 *  done
 *
 * Note: a single pkt log snapshot (by default) is ~25KB (i.e. the
 * plog.dat file above can grow rather quickly).
 *
 * cache the address of the interesting kernel variables in:
 *    "/tmp/ncadg_plogd.db"
 */

#include <stdio.h>
#include <sys/file.h>
#include <ncadg_plog.h>

#define plog_addr_var   "_rpc_g_dg_pkt_log"
#define plog_bytes_var  "_rpc_g_dg_pkt_log_bytes"

pktlog_elt_p_t pkt_log;

main(argc, argv)
int argc;
char *argv[];
{
    int s, p, rc, wc, kfd, pos;
    FILE *fp;
    char *f;
    char *kernel = argv[1];
    char cmd[80];
    char tmpstr1[80], tmpstr2[80];
    unsigned long plog_addr;
    unsigned long plog_bytes;

    /*
     * Determine the addresses of the interesting kernel variables
     * (a pointer to and size of the DG pkt log)
     */
    f = "/tmp/ncadg_plogd.db";
    if ((fp = fopen(f, "r")) == NULL) {
        sprintf(cmd, "nm %s | grep '%s$' > %s", kernel, plog_addr_var, f);
        s = system(cmd);
        if (s != 0) {
            fprintf(stderr, "couldn't run '%s' - exit status %d\n", cmd, s);
            exit(1);
        }
        sprintf(cmd, "nm %s | grep '%s$' >> %s", kernel, plog_bytes_var, f);
        s = system(cmd);
        if (s != 0) {
            fprintf(stderr, "couldn't run '%s' - exit status %d\n", cmd, s);
            exit(1);
        }
        fp = fopen(f, "r");
    }
    s = fscanf(fp, "%lx %s %s", &plog_addr, &tmpstr1[0], &tmpstr2[0]);
    if (s != 3) {
        fprintf(stderr, "couldn't get plog addr (bad scanf format)\n");
        exit(1);
    }
    if (strcmp(tmpstr2, plog_addr_var) != 0) {
        fprintf(stderr, "couldn't get plog addr (incorrect/no '%s' var)\n", 
            plog_addr_var);
        exit(1);
    }
    s = fscanf(fp, "%lx %s %s", &plog_bytes, &tmpstr1[0], &tmpstr2[0]);
    if (s != 3) {
        fprintf(stderr, "couldn't get plog size (bad scanf format)\n");
        exit(1);
    }
    if (strcmp(tmpstr2, plog_bytes_var) != 0) {
        fprintf(stderr, "couldn't get plog size (incorrect/no '%s' var)\n", 
            plog_bytes_var);
        exit(1);
    }
    fclose(fp);

#ifdef NOTDEF
    /*
     * Open up /dev/kmem.
     */
    if ((kfd = open("/dev/kmem", O_RDONLY, 0)) == -1) {
        perror("couldn't open /dev/kmem");
        exit(1);
    }
#else
    /*
     * just use stdin for /dev/kmem
     */
    kfd = 0;
#endif

    /*
     * determine the size of the log.
     */
    if ((pos = lseek(kfd, plog_bytes, L_SET)) == -1) {
        fprintf(stderr, "couldn't seek to 0x%lx\n", plog_addr);
        perror("");
        exit(1);
    }
    rc = read(kfd, &plog_bytes, sizeof(plog_bytes));
    if (rc != sizeof(plog_bytes)) {
        fprintf(stderr, "couldn't read plog size at 0x%lx\n", pos);
        if (rc == -1)
            perror("");
        exit(1);
    }

    if (plog_bytes == 0) {
        fprintf(stderr, "doesn't look like pkt logging was enabled\n");
        exit(1);
    }

    /*
     * Determine the addr of the real log
     * Note: rpc_g_dg_pkt_log is a pointer to the real log.
     */
    if ((pos = lseek(kfd, plog_addr, L_SET)) == -1) {
        fprintf(stderr, "couldn't seek to 0x%lx\n", plog_addr);
        perror("");
        exit(1);
    }
    rc = read(kfd, &plog_addr, sizeof(plog_addr));
    if (rc != sizeof(plog_addr)) {
        fprintf(stderr, "couldn't read plog address pointer at 0x%lx\n", pos);
        if (rc == -1)
            perror("");
        exit(1);
    }

    if (plog_addr == 0) {
        fprintf(stderr, "doesn't look like pkt logging was enabled\n");
        exit(1);
    }

    /*
     * suck out the pkt log
     */

    pkt_log = (pktlog_elt_p_t) malloc(plog_bytes);

    if ((pos = lseek(kfd, plog_addr, L_SET)) == -1) {
        perror("lseek");
        exit(1);
    }
    rc = read(kfd, pkt_log, plog_bytes);
    if (rc != plog_bytes) {
        fprintf(stderr, "couldn't read %d pkt log bytes at 0x%lx\n", 
            plog_bytes, pos);
        if (rc == -1)
            perror("");
        exit(1);
    }

    /*
     * Dump the BINARY contents out to stdout (prefixed with a date stamp
     * and the number of plog bytes).
     */
    system("echo '=+=+=+=+=+=+=+=+= '");
    system("date");
    wc = write(1, &rc, sizeof(rc));
    wc = write(1, pkt_log, plog_bytes);
    if (wc != plog_bytes) {
        fprintf(stderr, "couldn't write %d bytes\n", plog_bytes);
        if (wc == -1)
            perror("");
        exit(1);
    }

    exit(0);
}
