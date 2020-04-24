/*
 * (C) COPYRIGHT Transarc Corp. 1992
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */

#include <stdio.h>
#include <dce/dce_error.h> /* required to call dce_error_inq_text */
#include <pthread.h>   /* needed if application uses threads */
#include <dce/rpcexc.h>    /* need for exception handlers */

#define WARN    1
#define RETURN  2
#define FATAL   3
#define SIL_RET 4

void exit();
