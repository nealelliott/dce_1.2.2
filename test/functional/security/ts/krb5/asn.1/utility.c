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
 * $Log: utility.c,v $
 * Revision 1.1.2.1  1996/08/28  20:59:25  arvind
 * 	DCE 1.2.2 drop 3.5 - (test) files left out
 * 	[1996/08/28  20:56:48  arvind]
 *
 * $EndLog$
 */

#include "krb5.h"
#include "utility.h"
#include <stdlib.h>
#include <stdio.h>

char hexchar PROTOTYPE((const unsigned int digit));

asn1_error_code asn1_krb5_data_unparse(code, s)
     const krb5_data * code;
     char ** s;
{
  if(*s != NULL) free(*s);
  
  if(code==NULL){
    *s = (char*)calloc(strlen("<NULL>")+1, sizeof(char));
    if(*s == NULL) return ENOMEM;
    strcpy(*s,"<NULL>");
  }else if(code->data == NULL || code->length <= 0){
    *s = (char*)calloc(strlen("<EMPTY>")+1, sizeof(char));
    if(*s==NULL) return ENOMEM;
    strcpy(*s,"<EMPTY>");
  }else{
    int i;

    *s = (char*)calloc(3*(code->length), sizeof(char));
    if(*s == NULL) return ENOMEM;
    for(i = 0; i < code->length; i++){
      (*s)[3*i] = hexchar(((code->data)[i]&0xF0)>>4);
      (*s)[3*i+1] = hexchar((code->data)[i]&0x0F);
      (*s)[3*i+2] = ' ';
    }
    (*s)[3*(code->length)-1] = '\0';
  }
  return 0;
}

char hexchar(digit)
     const unsigned int digit;
{
  if(digit<=9)
    return '0'+digit;
  else if(digit<=15)
    return 'A'+digit-10;
  else
    return 'X';
}

krb5_error_code krb5_data_parse(d, s)
     krb5_data * d;
     const char * s;
{
  /*if(d->data != NULL){
    free(d->data);
    d->length = 0;
  }*/
  d->data = (char*)calloc(strlen(s),sizeof(char));
  if(d->data == NULL) return ENOMEM;
  d->length = strlen(s);
  memcpy(d->data,s,strlen(s));
  return 0;
}

krb5_error_code krb5_data_hex_parse(d, s)
     krb5_data * d;
     const char * s;
{
  int i, digit;
  char *pos;

  d->data = (char*)calloc((strlen(s)+1)/3,sizeof(char));
  if(d->data == NULL) return ENOMEM;
  d->length = (strlen(s)+1)/3;
  for(i=0,pos=(char*)s; i<d->length; i++,pos+=3){
    if(!sscanf(pos,"%x",&digit)) {
	    return EINVAL;
    }
    d->data[i] = (char)digit;
  }
  return 0;
}

#if 0
void asn1buf_print(buf)
     const asn1buf * buf;
{
  asn1buf bufcopy;
  char *s=NULL;
  int length;
  int i;
  
  bufcopy.base = bufcopy.next = buf->next;
  bufcopy.bound = buf->bound;
  length = asn1buf_len(&bufcopy);

  s = calloc(3*length, sizeof(char));
  if(s == NULL) return;
  for(i=0; i<length; i++){
    s[3*i] = hexchar(((bufcopy.base)[i]&0xF0)>>4);
    s[3*i+1] = hexchar((bufcopy.base)[i]&0x0F);
    s[3*i+2] = ' ';
  }
  s[3*length-1] = '\0';

  printf("%s\n",s);
  free(s);
}
#endif
