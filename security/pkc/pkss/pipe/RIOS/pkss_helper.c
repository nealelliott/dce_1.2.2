
/*
 * pkss_helper.c
 */
#include <stdlib.h>

main(int     argc,
     char   *argv[],
     char   *envp[])
{
    /*
     *  This executable will be executed with a setuid of root.
     *
     *  To avoid a security exposure,
     *      1)  This executable should have the execute permissions
     *          of --x--x--x (444) to prevent users from copying the
     *          executable
     *
     *      2)  The platform specific environment variable to specify
     *          the order that the library search path
     *          (On this platform - LIBPATH) will be reset -
     *          to ensure that only the default system libraries will be loaded.
     */

    putenv("LIBPATH=");

    /*
     *  This will call the "original" pkss_helper code.
     */
    execvp("/opt/dcelocal/bin/pkss_helper2", argv);
}


