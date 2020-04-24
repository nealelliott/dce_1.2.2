/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: dhparamgen.c,v $
 * Revision 1.1.2.1  1996/10/04  14:25:44  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  13:57:29  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:32 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:12 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 * Module:
 *
 * Purpose:
 *
 */

#include <dce/dce.h>
#include <bsafe/global.h>
#include <bsafe/bsafe2.h>
#include <bsafe/atypes.h>
#include  <time.h>
#include  <string.h>
#include  <stdio.h>
#include  <unistd.h>

void T_CALL T_memset (POINTER p, int i, unsigned int ui) {
    memset(p, i, ui);
}

void T_CALL T_memcpy (POINTER p1, POINTER p2, unsigned int ui) {
    memcpy(p1, p2, ui);
}

void T_CALL T_memmove (POINTER p1, POINTER p2, unsigned int ui) {
    memmove(p1, p2, ui);
}

int T_CALL T_memcmp (POINTER p1, POINTER p2, unsigned int ui) {
    return memcmp(p1, p2, ui);
}

POINTER T_CALL T_malloc (unsigned int ui) {
    return malloc(ui);
}

POINTER T_CALL T_realloc (POINTER p, unsigned int ui) {
    return realloc(p, ui);
}

void T_CALL T_free (POINTER p) {
    free(p);
}



typedef struct {
    unsigned char * data;
    size_t len;
} data_t;

#define CREATE_STORAGE(desc, size) \
	if (((desc)->data = (unsigned char *)malloc(size)) == NULL) { \
	    status = BE_ALLOC; \
	    break; \
	}; \
        memset((desc)->data, 0, size); \


B_ALGORITHM_METHOD *RSA_ALGORITHM_CHOOSER[] = {
    &AM_DES_CBC_DECRYPT,/* DES DBD decryption for passwd-based enc pvtkey */
    &AM_DES_CBC_ENCRYPT,/* DES DBD decryption for passwd-based enc pvtkey */
    &AM_DES_EDE3_CBC_ENCRYPT,/* 3xDES encryption */
    &AM_DES_EDE3_CBC_DECRYPT,/* 3xDES decryption pvtkey */
    &AM_MD2,            /* RSA MD2 digest */
    &AM_MD2_RANDOM,     /* RSA MD2 random number generator (RNG) */
    &AM_MD5,            /* RSA MD5 digest */
    &AM_MD5_RANDOM,     /* RSA MD5 random number generator (RNG) */
    &AM_RSA_CRT_DECRYPT,/* RSA pvtkey decryption, AI_PKCS_RSAPrivate */
    &AM_RSA_CRT_ENCRYPT,/* RSA pvtkey encryption, AI_PKCS_RSAPrivate, sign */
    &AM_RSA_DECRYPT,    /* RSA pubkey decryption, AI_PKCS_RSAPublic, sign */
    &AM_RSA_ENCRYPT,    /* RSA pubkey encryption, AI_PKCS_RSAPublic, */
    &AM_RSA_KEY_GEN,    /* RSA key gen */
    &AM_DH_PARAM_GEN,    /* DH parameter generation */
    &AM_DH_KEY_AGREE,    /* DH key gen */
    (B_ALGORITHM_METHOD *)NULL_PTR
};
  
static B_ALGORITHM_OBJ randomAlgorithm = NULL;


int dh_paramGen (data_t seed_data,
                 unsigned int primeBits,
                 unsigned int exponentBits,
                 data_t * result
) {
    B_ALGORITHM_OBJ dhGen = NULL;
    B_ALGORITHM_OBJ resultAlgorithm = NULL;
    int status = 0;
    unsigned int res_length;
    ITEM * parameters;
    unsigned char * seed;        
    size_t seedLen;
    clock_t process_clock_ticks;
    time_t absolute_clock_ticks;
    A_DH_PARAM_GEN_PARAMS params;

    do {

/*	fprintf(stderr, "Creating random algorithm\n"); fflush(NULL); */

	if (status = B_CreateAlgorithmObject (&randomAlgorithm)) break;;

/*	fprintf(stderr, "Setting random algorithm\n"); fflush(NULL); */

	if (status = B_SetAlgorithmInfo (randomAlgorithm,
	                                 AI_MD5Random,
                                         NULL_PTR)) break;

/*	fprintf(stderr, "Initializing random algorithm\n"); fflush(NULL); */

	if (status = B_RandomInit(randomAlgorithm,
                                  RSA_ALGORITHM_CHOOSER,
                                  (A_SURRENDER_CTX *)NULL_PTR)) break;

/* 	fprintf(stderr, "Updating random algorithm\n"); fflush(NULL); */

	if (status = B_RandomUpdate(randomAlgorithm,
				    seed_data.data,
				    seed_data.len,
				    (A_SURRENDER_CTX *)NULL_PTR)) break;

	absolute_clock_ticks = time(NULL);
    
	seed = (unsigned char *)&absolute_clock_ticks;
	seedLen = sizeof(time_t);

/*	fprintf(stderr, "Updating random algorithm\n"); fflush(NULL); */

	if (status = B_RandomUpdate(randomAlgorithm,
                                    seed,
                                    seedLen,
                                    (A_SURRENDER_CTX *)NULL_PTR)) break;

	process_clock_ticks = clock();
    
	seed = (unsigned char *)&process_clock_ticks;
	seedLen = sizeof(clock_t);


/*	fprintf(stderr, "Updating random algorithm\n"); fflush(NULL); */

	if (status = B_RandomUpdate(randomAlgorithm,
                                    seed,
                                    seedLen,
                                    (A_SURRENDER_CTX *)NULL_PTR)) break;


/*	fprintf(stderr, "Creating DHgen algorithm\n"); fflush(NULL); */

	if (status = B_CreateAlgorithmObject(&dhGen)) break;

	params.primeBits = primeBits;
	params.exponentBits = exponentBits;

/*	fprintf(stderr, "Setting DHgen algorithm\n");  */
/*	fprintf(stderr, 
                "   primeBits = %u, exponentBits = %u\n", 
                primeBits, exponentBits);
        fflush(NULL); */

	if (status = B_SetAlgorithmInfo(dhGen,
                                        AI_DHParamGen,
                                        (POINTER)&params)) break;

/*	fprintf(stderr, "GenerateInit\n"); fflush(NULL); */

	if (status = B_GenerateInit(dhGen,
				    RSA_ALGORITHM_CHOOSER,
				    NULL)) break;

/*	fprintf(stderr, "Creating DH parameter object\n"); fflush(NULL); */

	if (status = B_CreateAlgorithmObject (&resultAlgorithm)) break;;

/*	fprintf(stderr, "GenerateParams\n"); fflush(NULL); */

	if (status = B_GenerateParameters(dhGen,
                                          resultAlgorithm,
                                          randomAlgorithm,
                                          NULL)) break;

/*	fprintf(stderr, "Getting BER\n"); fflush(NULL); */

	if (status = B_GetAlgorithmInfo ((POINTER *)&parameters, 
					 resultAlgorithm,
					 AI_DHKeyAgreeBER)) break;

/*	fprintf(stderr, 
		"Creating %d byte buffer to hold BER\n", 
		parameters->len); fflush(NULL); */

	CREATE_STORAGE(result, parameters->len);
	result->len = parameters->len;

/*	fprintf(stderr, "Copying BER\n"); fflush(NULL); */

	memcpy(result->data, 
               parameters->data, 
               parameters->len);

    } while (0);

    if (status) {
	fprintf(stderr, "Error - A BSAFE routine returned %d\n", status);
    };

    return status;
}

int main(int argc, char * argv[]) {
    unsigned primeBits = 0;
    unsigned exponentBits = 0;
    data_t seed_data = {NULL,0};
    data_t result_ber = {NULL, 0};
    char * filename = NULL;
    char backup_filename[FILENAME_MAX];
    int status;
    int syntax_error = 0;
    int c_source = 0;
    int backup = 0;
    int modify = 0;
    int i;
    int c;
    extern int optind;
    extern char *optarg;
    FILE * output_file;
    FILE * input_file;
    char * line[512];
 
    while ((c = getopt(argc, argv, "bcmf:p:l:s:")) != EOF) {
        switch (c) {
        case 'c':
            c_source = 1;
            break;
        case 'f':
            filename = optarg;
            break;
        case 'b':
            backup = 1;
            break;
        case 'm':
            modify = 1;
            backup = 1;
            break;
        case 'p':
            if (sscanf(optarg, "%u", &primeBits) != 1) {
                fprintf(stderr, 
                        "Error parsing primeBits value \"%s\"\n", optarg);
                syntax_error = 1;
            };
            break;
        case 'l':
            if (sscanf(optarg, "%u", &exponentBits) != 1) {
                fprintf(stderr, "Error parsing expBits value \"%s\"\n", optarg);
                syntax_error = 1;
            };
            break;
        case 's':
            seed_data.data = (unsigned char *)optarg;
            seed_data.len = strlen(optarg);
            break;
        default:
            syntax_error = 1;
        }
    };

    if (modify && c_source) {
        fprintf(stderr, "Incompatible options \"-c\" and \"-m\"\n");
        syntax_error = 1;
    };

    if (syntax_error) {
        fprintf(stderr, 
"Usage: %s [-c] [-f <file> [-m] [-b]] [-p <pBits>] [-l <expBits>] [-s <seed>]\n", 
                argv[0]);
        fprintf(stderr, "\n"
"       -c : Generate D-H parameters as a C-source declaration (default is \n"
"            to generate the parameters as a PKSS configuration-file entry).\n"
               );
        fprintf(stderr, 
"       -f file : Write parameters to the specified file.  If the file exists,\n"
"                 it will be overwritten.\n");
        fprintf(stderr, 
"       -b : Take a backup copy of any existing file before overwriting it.\n");
        fprintf(stderr, 
"       -m : Modify the file if it already exists.  This option implies the -b\n"
"            option, and is incompatible with the -c option.\n");
        fprintf(stderr, 
"       -p pBits : The number of bits in the shared prime value.\n");
        fprintf(stderr, 
"       -l expBits : The number of bits in the exponent (private value).\n",
"                    This should be at least twice the number of effective\n"
"                    bits of key required.\n");
        fprintf(stderr, 
"       -s seed : A random string that DHParamgen will use to seed its\n"
"                 random number generator.\n");
	return EXIT_FAILURE;
    };

    if (filename) {
        if (backup) {
            if (input_file = fopen(filename, "r")) {
                fclose(input_file);
                strncpy(backup_filename, 
                        filename, 
                        sizeof(backup_filename));
                strncat(backup_filename, 
                        "~", 
                        sizeof(backup_filename) - strlen(backup_filename));
		if (rename(filename, backup_filename) != 0) {
                    fprintf(stderr, "Couldn't take a backup of the file\n");
                    return EXIT_FAILURE;
                }
            } else {
                backup = 0; modify = 0;
            };
        };
        output_file = fopen(filename, "w");
        if (modify) {
            if ((input_file = fopen(backup_filename, "r")) == NULL) {
                fprintf(stderr, 
                        "Error: Can't open backup file \"%s\" for reading.\n",
                        backup_filename);
                return EXIT_FAILURE;
            };
            while (fgets(line, sizeof(line), input_file) != NULL) {
                if (strncmp(line, 
                            "*.param.dhparams.", 
                            17) != 0) {
                    fputs(line, output_file);
                };
            };
            fclose(input_file);
        };
    } else {
        output_file = stdout;
    };


    while (primeBits == 0) {
        fprintf(stdout, "primeBits: "); fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == EOF) return EXIT_FAILURE;
        if (sscanf(line, "%u", &primeBits) != 1) {
            fprintf(stderr, 
                    "Error parsing primeBits value \"%s\"\n", optarg);
            primeBits = 0;
        };
    };

    while (exponentBits == 0) {
        fprintf(stdout, "expBits: "); fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == EOF) return EXIT_FAILURE;
        if (sscanf(line, "%u", &exponentBits) != 1) {
            fprintf(stderr, 
                    "Error parsing expBits value \"%s\"\n", optarg);
            exponentBits = 0;
        };
    };

    while (seed_data.len == 0) {
        fprintf(stdout, "Random seed string: "); fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == EOF) return EXIT_FAILURE;
        seed_data.len = strlen(line);
	seed_data.data = (unsigned char)(&line[0]);
    };


    if (output_file == NULL) {
        fprintf(stderr, "Error opening \"%s\" for writing\n", filename);
        return EXIT_FAILURE;
    };


    if (status = dh_paramGen (seed_data,
                              primeBits,
                              exponentBits,
                              &result_ber)) return status;


    if (c_source){


        fprintf(output_file, 
"/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        fprintf(output_file,
" *\n");
        fprintf(output_file, 
" * Diffie-Hellman parameter block generated by DHPARAMGEN.\n");
        fprintf(output_file, 
" * Copyright (C) Digital Equipment Corporation, 1996\n");
        fprintf(output_file, 
" */\n");
        fprintf(output_file,
"\n");
        fprintf(output_file,
"const unsigned char dh_parameters_BER_data_%ux%u[%u] = {", 
primeBits, exponentBits, result_ber.len);
        for (i=0; i<result_ber.len; i++) {
            if (i%10 == 0) fprintf(output_file, "\n   ");
            if (i == result_ber.len-1) 
                fprintf(output_file, "0x%2.2x};\n", result_ber.data[i]);
            else
                fprintf(output_file, "0x%2.2x, ", result_ber.data[i]);
        };

        fprintf(output_file,
"\n");
        fprintf(output_file,
"const sec_pk_data_t dh_parameters_BER_%ux%u = {\n", 
primeBits, exponentBits);
        fprintf(output_file,
"   %u,\n", result_ber.len);
        fprintf(output_file,
"   dh_parameters_BER_data_%ux%u};\n", primeBits, exponentBits);
        fprintf(output_file,
"\n");
        fprintf(output_file, 
"/* ----------------------------------------------------------------- */\n");
        fprintf(output_file,
"\n");
        fprintf(output_file,
"\n");
    } else {
        fprintf(output_file, 
                "*.param.dhparams.bytecount: %d\n", 
                result_ber.len);
        fprintf(output_file, 
                "*.param.dhparams.linecount: %d", 
                (result_ber.len+9)/10);

        for (i=0; i<result_ber.len; i++) {
            if (i%10 == 0) fprintf(output_file, 
                                   "\n*.param.dhparams.line.%d: ", 
                                   (i/10) + 1);
            if (i == result_ber.len-1) 
                fprintf(output_file, "0x%2.2x\n", result_ber.data[i]);
            else
                fprintf(output_file, "0x%2.2x ", result_ber.data[i]);
        };

        fprintf(output_file,"\n");

    };
    return 0;
}
