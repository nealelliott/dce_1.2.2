/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.13.2  1996/02/17  23:27:57  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:08  marty]
 *
 * Revision 1.1.13.1  1995/12/11  22:43:28  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/01  14:03 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1995/12/11  22:20:21  root]
 * 
 * Revision 1.1.9.1  1994/02/02  22:06:04  ohara
 * 	fix loop counters (they were off by one)
 * 	[1994/02/02  21:07:10  ohara]
 * 
 * 	HP revision /main/HPDCE01/2  1994/02/01  18:14  giza
 * 	merge kk & hpdce01 take 2
 * 
 * Revision 1.1.4.2  1993/08/24  18:57:54  giza
 * 	Added remote test report functions reportTestStatus(), dumpDB().
 * 	[1993/08/24  18:57:26  giza]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/04  22:10:36  cjd]
 * 
 * Revision 1.1.7.1  1993/09/09  19:02:52  cmckeen
 * 	HP's TET'ized version
 * 	[1993/09/09  18:58:41  cmckeen]
 * 
 * Revision 1.1.5.2  1993/08/24  13:06:30  giza
 * 	added reportTestStatus() and dumpDB() functions to manage the test
 * 	result database.
 * 	[1993/08/23  20:28:31  giza]
 * 
 * Revision 1.1.1.5  1993/05/04  22:21:53  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.2.3  1993/02/05  15:21:49  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:42:16  cjd]
 * 
 * Revision 1.1.2.2  1992/09/25  19:24:20  eperkins
 * 	Initial version
 * 	[1992/09/25  19:23:59  eperkins]
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <time.h>
#include <pipetest.h>     
#include <test_common.h>
#include <pthread.h>
#include "tet_code.h"

typedef unsigned char byte;

#define MANAGER_BUFFER_SIZE 2048
#define MANAGER_ELEMENT_COUNT 999
extern FILE *fp;
extern char *filename;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*
 * This section defines the database that we'll use to store information
 * about clients.
 */
#define MAXHOSTS        100     /*
                                 * max number of hosts to run
                                 * this test simultaneously
                                 */
#define MAXCONCURRENT   100     /*
                                 * max number of concurrent
                                 * client processes to run on a
                                 * single client host
                                 */
#define MAX_DB_ENTRIES  (MAXHOSTS*MAXCONCURRENT)
#define HASHSIZE        (MAXHOSTS+1)

struct hostlist {
    struct hostlist *next;
    char *hostname;
    signed32 pass;
    signed32 fail;
};

static struct hostlist *hashtab[HASHSIZE];

/*
 * Here's the elements in the database...
 */
typedef struct {
    uuid_t       handle;         /* unique identifier, created for each client */
    char         host_name[64];  /* the client's network name. */
    unsigned32   net_addr;       /* the client's IP address.   */
    unsigned32   pass;           /* Pass information field the client updates. */
    unsigned32   fail;           /* Fail information field the client updates. */
    idl_byte     logfile[20];    /* Client log file */
    signed32     file_len;
} db_elt_t;

/*
 * Here's the actual database...
 */
db_elt_t database[MAX_DB_ENTRIES];

/*
 * Declare a mutex which the server threads can lock to gain
 * exclusive access to the database.
 */
pthread_mutex_t database_mutex;


void create_record(h, host_name, name_len, net_addr, db_handle, st)
handle_t h;
idl_byte host_name[];
signed32 name_len;
unsigned32 net_addr;
uuid_t *db_handle;
unsigned32 *st;
{                        
    unsigned32 i, tst;
    unsigned_char_p_t uuid_string;
    time_t *tp = 0, *tloc = 0;
    
    tp = (time_t *)time(tloc);
    
    /*
     * lock everyone else out of the database.
     */
    pthread_mutex_lock(&database_mutex);

    /*
     * Find an empty slot...
     */
    for (i = 0; i < MAX_DB_ENTRIES; i++)
    {
        if (uuid_is_nil(&database[i].handle, &tst))
            break;
    }

    if (i == MAX_DB_ENTRIES)
    {
        fprintf(fp, "Error: rejecting create_record request, database is full\n");
        *st = -1;
    }
    else
    {      
        /*
         * Create a universal unique identifier as the key to this entry.
         * Future requests by the client will use this UUID so that we
         * can quickly find the record being modified.
         */
        uuid_create(&database[i].handle, &tst);

        /*
         * Store the supplied info in the database.
         */
        strncpy(database[i].host_name, host_name, name_len);
        database[i].net_addr = net_addr;

        /*
         * Return the created UUID, and the status, back to the client
         */
        *db_handle = database[i].handle;
        *st = 0;
           
        /*
         * Print out a message so that we can see that things are really
         * working.
         */
        uuid_to_string(&database[i].handle, &uuid_string, &tst);
        fprintf(fp, "created record for client@(%s %s) - %s\n",
               database[i].host_name,
                (struct in_addr *)inet_ntoa(database[i].net_addr),
                ctime(&tp));
        rpc_string_free(&uuid_string, &tst);
     } 
    
    /*
     * Unlock the database.
     */
    pthread_mutex_unlock(&database_mutex);
    fflush(fp);
}                                                         

void reportTestStatus(h, db_handle, pass, fail, logfile, file_len, st)
handle_t h;
uuid_t *db_handle;
unsigned32 pass;
unsigned32 fail;
idl_byte logfile[];
signed32 file_len;
unsigned32 *st;
{   
    unsigned32 i, n;
    unsigned_char_p_t uuid_string;
    
    /*
     * lock everyone else out of the database.
     */
    pthread_mutex_lock(&database_mutex);

    /* 
     * First, find the entry in the database...
     */
    for (i = 0; i < MAX_DB_ENTRIES; i++)
    {
        if (uuid_equal(db_handle, &database[i].handle, st))
            break;
    }

    if (i == MAX_DB_ENTRIES)
    {
        fprintf(fp, "Error: Rejecting create_record request, database is full\n");
        *st = -1;
    }
    else
    {          
        /*
         * Update the field
         */
        database[i].pass = pass;                     
        database[i].fail = fail;                     
        strncpy(database[i].logfile, logfile, file_len);

        /*
         * Print out a message so that we can see that things are really
         * working.
         *
         * uuid_to_string(&database[i].handle, &uuid_string, st);
         * rpc_string_free(&uuid_string, st);
         */

        *st = 0;
    }

    /*
     * Unlock the database.
     */
    pthread_mutex_unlock(&database_mutex);
    fflush(fp);
}

void dumpDB(h, s_only, st)
handle_t h;
unsigned32 s_only;
unsigned32 *st;
{   
    unsigned32 nloops = 0, nhosts = 0, noresult = 0, failed = 0, i, n, tst;
    unsigned_char_p_t uuid_string;
    struct hostlist *uniqify();
    void *empty();
    
    /*
     * lock everyone else out of the database.
     */
    pthread_mutex_lock(&database_mutex);

    /*
     * Empty the hash table just in case dumpDB is called more
     * than once in a row.
     */
    empty();
    
    for (i = 0; i < MAX_DB_ENTRIES; i++) {
        if (uuid_is_nil(&database[i].handle, &tst))
            break;
        else
        {          

            /*
             * Dump the database to the server.log file
             *
             * uuid_to_string(&database[i].handle, &uuid_string, st);
             *
             * Let's store a summary of the per client results to
             * a small hash table to get a summary of the test
             * results at the end of the dump.
             */
            uniqify(database[i].host_name, database[i].pass, database[i].fail);

            if ( ! strlen(database[i].logfile) ) strcpy(database[i].logfile, "rpc_x_comm_failure");
            if ( database[i].fail == 0 && database[i].pass == 0 ) noresult = 1;
            
            if ( !s_only ) {
                for (n=0; (n++) < 80; fprintf(fp, "%s", (n<80) ? "-" : "\n"));
                fprintf(fp, "Test results for client@%s (%s)\n\n\t\
                    Client Local Logfile = %s\n\t\
                    Iterations Passed = %d\n\t\
                    Iterations Failed = %d\n",
                        database[i].host_name,
                        (struct in_addr *)inet_ntoa(database[i].net_addr),
                        database[i].logfile,
                        database[i].pass,
                        database[i].fail);

                fprintf(fp, "\ntet_result = %s\n",
                    noresult ? "NORESULT" : (database[i].fail ? "FAIL" : "PASS") );
            }
            /*
             * rpc_string_free(&uuid_string, st);
             */

        }
    }

    /*
     * Print a summary of the cummulative results for all clients
     * entered in the database.
     */
    if ( !s_only ) {
        fprintf(fp, "\n\nFinal Test Result Summary For Each Client.\n");    
        for (n=0; (n++) < 80; fprintf(fp, "%s", (n<80) ? "-" : "\n"));
    }
    for ( n=0; n < HASHSIZE; n++ ) {
        if ( hashtab[n] != NULL ) {
            nhosts++;
            if ( !s_only ) fprintf(fp, "client host = %s, loops passed = %d, loops failed = %d\n",
                hashtab[n]->hostname,
                hashtab[n]->pass,
                hashtab[n]->fail);
            if ( hashtab[n]->fail || noresult ) failed = 1;
            nloops += ( hashtab[n]->pass + hashtab[n]->fail );
        }
    }
    /*
     * Unlock the database.
     */
    pthread_mutex_unlock(&database_mutex);

    if ( !s_only ) {
        for (n=0; (n++) < 80; fprintf(fp, "%s", (n<80) ? "-" : "\n"));
        fprintf(fp, "\n\n%d Clients reported their status back to the server.", nhosts);
        fflush(fp);
    }
    /*
     * Here's where it gets ugly.  We can't be sure that a client will always connect
     * to the server successfully, so we return this ugly 17 bits of status that give
     * us 1 bit for *known* error status, 8 bits for the number of loops the client
     * ran and successfully communicated to the server DB, and 8 bits for the number
     * of hosts that were in the test.  Therefore the maximum number of hosts or loops
     * that the status bits will report is 255.  These values are compared to the valuse
     * used by the TET driver script dcerpcrun (D_HOSTS, D_LOOPS, D_STATUS) to see if
     * all clients reported, and if they reported on every process.  Since all clients
     * run the same number of processes and loops, this shouldn't break.  All bets are
     * off if concurrency/loops are variable on a per client basis.
     */
                                                                /*  S  NLOOPS   NHOSTS   */
    *st = ( ( nhosts | ((nloops/nhosts)<<8) ) | (failed<<16) ); /* |x|xxxxxxxx|xxxxxxxx| */
}

unsigned32 hash(char *s) {
    unsigned32 hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

struct hostlist *lookup(char *s) {
    struct hostlist *np;

    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->hostname) == 0)
            return np;
    return NULL;
}

char *strdup(char *s) {
    char *p;

    p = (char *) malloc(strlen(s)+1);
    if (p != NULL)
        strcpy(p, s);
    return p;
}

void *empty() {
    unsigned32 n;
        
    for (n = 0; n < HASHSIZE; n++) {
        if (hashtab[n] != NULL) {
            free((void *) hashtab[n]->hostname);
            free((void *) hashtab[n]);
            hashtab[n] = NULL;
        }
    }
}

        
struct hostlist *uniqify(char *hostname, int pass, int fail) {
        struct hostlist *np;
        unsigned hashval;

        if ((np = lookup(hostname)) == NULL) {
            np = (struct hostlist *) malloc(sizeof(*np));
            if (np == NULL || (np->hostname = strdup(hostname)) == NULL)
                return NULL;
            hashval = hash(hostname);
            np->pass = pass;
            np->fail = fail;
            np->next = hashtab[hashval];
            hashtab[hashval] = np;
        } else { /* hostname already in the list */
                np->pass += pass;
                np->fail += fail;
        }
        return np;
}


void his_oper2(iop_struct,correct_to_server)
    /* [in, out] */pipe_of_struct *iop_struct;
    /* [out] */ndr_boolean *correct_to_server;
{
byte manager_buffer[MANAGER_BUFFER_SIZE];
    unsigned long data_count;
    pt_struct iop_checksums;
    pt_struct *struct_array;
    float *float_array;
    int i,j;
    ndr_long_float wk_double;
    ndr_short_float wk_single;

#ifdef DEBUG
    fprintf(fp, "Entered his_oper2\n");
#endif
    *correct_to_server = true;
    iop_checksums.pt_long = 0;
    iop_checksums.pt_double = 0.0;
    pthread_getspecific (thread_gdata_key, (pthread_addr_t) &manager_buffer); 
    struct_array = (pt_struct *)manager_buffer;
    j = 1;
TRY
    while (true)
    {
        (*iop_struct->pull)(iop_struct->state, struct_array,
                            MANAGER_BUFFER_SIZE/sizeof(pt_struct),
                            &data_count);
        if (data_count == 0)
        {
#ifdef DEBUG
            fprintf(fp, "Manager checksums for iop_struct: %d %.1f\n",
               iop_checksums.pt_long, iop_checksums.pt_double);
#endif
              if ( iop_checksums.pt_long != 1498500 ) {
                    *correct_to_server = false;
                    fprintf(fp, "Error in Manager checksums for iop_struct: %d %.1f\n",
                       iop_checksums.pt_long, iop_checksums.pt_double);
                    }
              if ( iop_checksums.pt_double != 1998000.0 ) {
                    *correct_to_server = false;
                    fprintf(fp, "Error in Manager checksums for iop_struct: %d %.1f\n",
                       iop_checksums.pt_long, iop_checksums.pt_double);
                    }
            break;
        }
        for (i=0; i<data_count; i++)
        {
            if (struct_array[i].pt_long != j*3)
            {
                fprintf(fp, "j = %d, pt_long = %d\n", j, struct_array[i].pt_long);
            }
            wk_double = j*4.0;
            if (wk_double != struct_array[i].pt_double)
            {
                fprintf(fp, "server j = %d, wk_double = %.8f, pt_double = %.8f\n", 
                       j, wk_double, struct_array[i].pt_double);
            }
            iop_checksums.pt_long += struct_array[i].pt_long;
            iop_checksums.pt_double += struct_array[i].pt_double;
            j++;
        }
    }

    struct_array = (pt_struct *)manager_buffer;
    data_count = MANAGER_BUFFER_SIZE/sizeof(pt_struct);
    j=1;
    while (true)
    {
        for (i=0; i<data_count; i++)
        {
            struct_array[i].pt_long = 5*j;
            struct_array[i].pt_double = (double)(6*j);
            j++;
            if (j > MANAGER_ELEMENT_COUNT)
            {
                i++;
                break;
            }
        }
        (*iop_struct->push)(iop_struct->state,(pt_struct *)manager_buffer,i);
         if (j > MANAGER_ELEMENT_COUNT) break;
    }
    (*iop_struct->push)(iop_struct->state,(pt_struct *)manager_buffer,0);
    free(manager_buffer);	
    fflush(fp);
CATCH_ALL
	RERAISE;
ENDTRY
}
globaldef pipetest_v0_0_epv_t pipetest_v0_0_m_epv = {his_oper2,create_record,reportTestStatus, dumpDB};
