/*
 * @OSF_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: cpwkey.c,v $
 * Revision 1.1.2.1  1996/12/05  07:08:33  rsalz
 * 	Created
 * 	[1996/12/05  07:07:39  rsalz]
 *
 * $EndLog$
 */
/*
 * cpwkey.c - change a password, but set the salt/pepper to
 * the NULL string. Also allow for a key to be used instead.
 * This is useful when setting a cross-cell key
 * or for adding a user's MIT string to key which was generated
 * by our K4 mods. 
 *
 * This is more of a proof of concept program then a production 
 * program. 
 * 
 * DEE 08/12/96
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/binding.h>
#include <dce/passwd.h>

#ifdef DEBUG
#define DEEDEBUG(A) fprintf(stderr,A) 
#define DEEDEBUG2(A,B) fprintf(stderr,A,B) 
#else
#define DEEDEBUG(A)
#define DEEDEBUG2(A,B)
#endif


main(int argc, char *argv[])
{
    extern int optind;
    extern char *optarg;
    int rv;
    int i,j,k;
    char *cp;
    unsigned_char_t *dp;

    error_status_t st;
    int lst;
    dce_error_string_t  err_string;

    char admin_pw[1025];
    int admin_pw_size = 1024;
    char new_pw[1025];
    int new_pw_size = 1024;

    /* set the defaults */


    unsigned_char_t *site = "/.:";
    sec_rgy_handle_t rcontext = NULL;
    sec_rgy_login_name_t user_name;
    sec_passwd_rec_t admin;
    sec_passwd_rec_t user;
    sec_passwd_version_t new_version;

    /* set the defaults */

    user.version_number = sec_passwd_c_version_none;
    user.pepper = NULL;
    user.key.key_type = sec_passwd_plain;
    user.key.tagged_union.plain = NULL;

    admin.version_number = sec_passwd_c_version_none;
    admin.pepper = NULL;
    admin.key.key_type = sec_passwd_plain;
    admin.key.tagged_union.plain = NULL;

    memset(user_name.pname,0,sizeof(user_name.pname));
    memset(user_name.gname,0,sizeof(user_name.gname));
    memset(user_name.oname,0,sizeof(user_name.oname));

    strncpy(user_name.gname, "none", sec_rgy_name_t_size);
    strncpy(user_name.oname, "none", sec_rgy_name_t_size);


    while((rv = getopt(argc,argv,"c:u:pk:v:g:o:P:")) != -1) {
      switch(rv) {
        case 'c':
          site = optarg;
          break;
        case 'u':
          strncpy(user_name.pname,optarg,sec_rgy_name_t_size);
          break;
        case 'p':
#ifdef _AIX
          fprintf(stdout,"NewPassword:");
          fgets(new_pw,new_pw_size,stdin);
          new_pw_size = strlen(new_pw) - 1;
          new_pw[new_pw_size] = '\0';
#else
          krb5_read_password("New Password:", "New Password:",
                new_pw, &new_pw_size);
#endif
          user.key.tagged_union.plain = new_pw;
          break;
        case 'k':
          cp = optarg;
          dp = &user.key.tagged_union.des_key;
          fprintf(stderr,"key:");
          for (i=0;i<8;i++) {
            if( *cp == '\\') {
               cp++;
               sscanf(cp,"%3o",&j);
               *dp++ = j;
               cp = cp+3;
               fprintf(stderr,"\\%03.3o",j);
            } else {
               j = *cp++;
               fprintf(stderr,"%c",j);
               *dp++ = j;
            }
          }
          fprintf(stderr,"\n");
          user.pepper = "";
          user.key.key_type = sec_passwd_des;
          break;
        case 'P':
          user.pepper = optarg;
          break;
        case 'v':
          user.version_number = atoi(optarg);
          break;
        case 'g':
          strncpy(user_name.gname,optarg,sec_rgy_name_t_size);
          break;
        case 'o':
          strncpy(user_name.oname,optarg,sec_rgy_name_t_size);
          break;
        case '?':
          fprintf(stdout,"Error\n");
          break;
       }
    }

#ifdef _AIX
    fprintf(stdout,"AdminPassword:");
    fgets(admin_pw,admin_pw_size,stdin);
    admin_pw_size = strlen(admin_pw) - 1;
    admin_pw[admin_pw_size] = '\0';
#else
    krb5_read_password("Admin Password:", NULL,
        admin_pw, &admin_pw_size);
#endif
    admin.key.tagged_union.plain = admin_pw;

    /* check for correct options, needs work */

    if (!(user_name.pname[0]) ) {
       printf("Required option, -u missing\n");
       exit(1);
    }

    sec_rgy_site_open_update(site, &rcontext, &st);
    if(st) {
         dce_error_inq_text(st, err_string, &lst);
         fprintf(stderr, "Error while binding to update site: %s : %s\n",
                site,err_string);
         exit(1);
    }

    fprintf (stderr,"calling sec_rgy_acct_passwd\n");
    sec_rgy_acct_passwd(rcontext, &user_name,
        &admin, &user, sec_passwd_des, &new_version, &st);

    if(st) {
        dce_error_inq_text(st, err_string, &lst);
        fprintf(stderr, "Error while changing password for %s : %s\n",
                    &user_name.pname, err_string);
        exit(1);
    }
    fprintf (stderr,"Finsihed\n");
}

