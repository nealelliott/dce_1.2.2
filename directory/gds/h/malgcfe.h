/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: malgcfe.h,v $
 * Revision 1.1.10.2  1996/02/18  23:36:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:51  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:02:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:33  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:54:59  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:49:11  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  13:02:44  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:44:59  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:42:57  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:31:01  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  04:18:45  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:44:02  zeliff]
 * 
 * $EndLog$
 */
#ifndef _MALGCFE_H
#define _MALGCFE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char malgcfe_rcsid[] = "@(#)$RCSfile: malgcfe.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:36:21 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYP		: INCLUDE					      */
/*		  =========					      */
/* NAME		: malgcfe.h					      */
/*		  =========					      */
/* AUTOR	: U. Richter   D AP 143				      */
/* DATUM	: 5.03.86					      */
/*								      */
/* KOMPONENTE	: MA						      */
/*								      */
/* DOK.-NR.	: 3120.50.01					      */
/* PRD#/VERS.	:						      */
/*								      */
/* BESCHREIBUNG	: Externe Schnittstelle	fuer das Log-system.	      */
/*		  Enthaelt alle	Typen (parameterlisten), Konstanten,  */
/*		  Dateistrukturen und Fehlertexte (err_values).	      */
/*								      */
/* SYSTEMABHAENIGKEIT:						      */
/*		  COMMON  (cond. compilation)			      */
/*								      */
/*								      */
/* Vers.Nr. |  Datum  |	 Aenderungen		       | KZ | CR# FM# */
/*  1.0	    |  5.03.86|	Original		       | Ri | CR43    */
/*	    |	      |	CR43: MA_ERROR-Param-liste     |    |	      */
/*	    |	      |	hat zeiger auf Fehlerwerte     |    |	      */
/*	    |	      |	statt Werte selbst.	       |    |	      */
/*  1.1	    | 30.04.84|	em_rootdir:		       | Ri |	      */
/*	    |	      |	Aenderung der Dateinamen fuer  |    |	      */
/*	    |	      |	das Log-system.		       |    |	      */
/*  1.2	    | 28.07.86|	CR65: log-comment-prefix und   | Ri | CR65    */
/*	    |	      |	-name fuer \0 verlaengert.     |    |	      */
/*	    |	      |	CR105: Teilsystem SDE	       | Ri | CR105   */
/*	    |	      |	gestrichen (MANRSD).	       |    |	      */
/*	    |	      |	Einfuehrung von	file-versionen | Ri |	      */
/*	    |	      |	und Erhoehung der Versionen der|    |	      */
/*	    |	      |	betroffenen files.	       |    |	      */
/*  1.3	    | 17.10.86|	Eintragen der SCCS-id	       | Ri | --      */
/*  1.4	    | 31.10.86|	CR230: Erweiterung der Log-    | Ri | CR230   */
/*	    |	      |	datentypen um IA5 und T61      |    |	      */
/*	    |	      |	CR231: Neuer Log-Konfig.-      | Ri | CR231   */
/*	    |	      |	Parameter "MA_LOG-calls        |    |         */
/*	    |	      |	between	update".               |    |         */
/*	    |	      |	CR55: Betriebsdaten abschaltbar| Ri | CR55    */
/*  1.5	    | 23.01.87|	Realisierung der BS2000-       | Ri | --      */
/*	    |	      |	Variante.		       |    |	      */
/*  1.6	    |  2.03.87|	Selektieren der	Return-Codes   | Ri | CR666   */
/*  1.7	    | 10.08.87|	zusaetzlicher Return-Code fuer | Ri | CR1300  */
/*	    |	      |	OPCOM infolge SEVNT-Fehler.    |    |	      */
/*	    |	      |	Verhindern von logbase-Starts  | Ri |	      */
/*	    |	      |	bei laufendem logbase (Log-Pr.)|    |	      */
/*  1.8	    | 24.08.87|	A) Geaenderte Zuordnung	von    | Ri | CR1681  */
/*	    |	      |	TSIDs im Logsystem (X4	       |    | CR1539  */
/*	    |	      |	entfaellt, ED statt EM)	und    |    |	      */
/*	    |	      |	Einfuehrung der	neuen TS:      |    |	      */
/*	    |	      |	ED, FS,	PR, DB.		       |    |	      */
/*	    |  7.09.87|	B) Log-System-Anpassung	fuer   | Ri | CR1691  */
/*	    |	      |	UTM mit	KCBENID	aus em_owner.  |    |	      */
/*  1.9	    | 26.10.87|	A) Teilsystem FA in Logsystem  | Ri | CR1820  */
/*	    |	      |	aufnehmen.		       |    |	      */
/*  1.10    | 10.12.87|	A) Teilweise Ausgabe der Log-  | Ri | CR1887  */
/*	    |	      |	information bei	zu grosser     |    |	      */
/*	    |	      |	La#nge.			       |    |	      */
/*  1.11    | 24.03.88|	Ergaenzung BS2000-Variante:    | Ri | CR1300  */
/*	    |	      |	A) Fehlermeldungen des prepare |    |	      */
/*	    |	      |	ueber AD-utility mehrsprachig. |    |	      */
/*	    |	      |	B) Dabei SINIX mehrsprachig    | Ri |	      */
/*	    |	      |	ueber eigene Datei je Sprache +|    |	      */
/*	    |	      |	Sprachauswahl ueber environ.-  |    |	      */
/*	    |	      |	Variable und Anlagensprache.   |    |	      */
/*	    |	      |				       |    |	      */
/*    2.0   | 05.08.88|	Umstellung auf V2.0	       | Gu | EM2000  */
/*    2.1   | 30.01.89|	Logkonfiguration: Neuaufnahme  | Gu |P66050647*/
/*	    |	      |	von OT anstelle	von FS	       |    |	      */
/*	    |	      |				       |    |	      */
/*								      */
/*datoff **************************************************************/

#include <dce/dce.h>

/* ---------------------Nummerierung der Teilsysteme -----------------*/

#define	 MIN_NRTS	0	     /*	minimale teilsystem-nr		  */
#define	 MANRAD		0	     /*	Administration			  */
#define	 MANRBF		1	     /*	BS-Benutzerfunktion		  */
#define	 MANRBS		2	     /*	Bediensystem			  */
#define	 MANRDV		3	     /*	Datei-Verwaltungssystem		  */
#define	 MANRDS		4	     /*	Directory Service Agent		  */
#define	 MANRMA		5	     /*	Maintenance			  */
#define	 MANRLF		6	     /*	Lokale Funktionen		  */
#define	 MANRMB		7	     /*	Mailbox	Service	+ MB-MTC	  */
#define	 MANRMC		8	     /*	Mailbox	Client			  */
#define	 MANRMT		9	     /*	MTA				  */
#define	 MANRRO	       10	     /*	ROS				  */
#define	 MANRRT	       11	     /*	RTS				  */
#define	 MANRTX	       12	     /*	TTX-Anschluss			  */
#define	 MANRUT	       13	     /*	Utilities			  */
#define	 MANRX4	       MANRUT	     /*	X.409-Utilities			  */
#define	 MANRED	       14	     /*	Editor				  */
/* #define  MANRFS   15	     V2.1	File-Service			  */
#define	 MANROT	       15 /* V2.1 */ /*	Transfer zu den	Clients		  */
#define	 MANRPR	       16	     /*	Print-Service			  */
#define	 MANRDB	       17	     /*	Database-Service		  */
#define	 MANRFA	       18	     /*	Fremd-Anwendungen		  */
#define	 MAX_NRTS      18	     /*	hoechste teilsystem-nummer	  */
#define	 M_NRTS	       MAX_NRTS+1
				     /*	maximale anzahl	von elementen	  */
				     /*	fuer teilsysteme in vektoren	  */



/* ----------------------Werte fuer log-type--------------------------*/

#define	 MIN_NRTYP	0	     /*	minimaler log-type-wert		   */
#define	 MALTREP	0	     /*	REPORT_DATA			   */
#define	 MALTFUN	1	     /*	FUNCTION TRACE			   */
#define	 MALTERR	2	     /*	ERROR_LOG			   */
#define	 MALTOTH	3	     /*	OTHER				   */
#define	 M_NRTYP	3	     /*	hoechste log-type-nummer	   */
#define	 M_ANZTYP	M_NRTYP+1    /*	maximale anzahl	der log-typen	   */



/*-----------------------Werte fuer log-level------------------------ */

#define	 MIN_LL		1	/* minimaler log-level		      */
#define	 MALLLOW	1	/* log-level = low		      */
#define	 MALLMED	2	/* log-level = medium		      */
#define	 MALLHIG	3	/* log-level = high		      */
#define	 MALLNOL	4	/* log ausgeschaltet = no log	      */
#define	 MALLNAVAIL	5	/* log nicht verfuegbar		      */
#define	 M_LLUSER	3	/* maximaler log-level d. user	      */
#define	 M_LLCONF	5	/* maximaler log-level in d.	      */
				/* konfigurationsdatei		      */



/*-----------------------Werte fuer data-type------------------------ */

#define	 MIN_DTYP	0	/* minimal-wert	f. data-type	      */
#define	 MADTACHAR	0	/* daten sind char im code des lokalen*/
				/* systems. SINIX: ASCII  BS2: EBCDIC */
				/* (abdruckbar)			      */
#define	 MADTBIT	1	/* daten sind ein Bit-Feld	      */
				/* laenge in anzahl bytes	      */
#define	 MADTHEX	2	/* daten sind	  transparent	      */
				/* und sind sedezimal auszugeben.     */
				/* laenge in anzahl bytes	      */
#define	 MADTBOOL	3	/* daten sind Bool'sche Werte         */
#define	 MADTISHO	4	/* daten sind Short integer	      */
#define	 MADTINT	5	/* daten sind Int integer	      */
#define	 MADTILONG	6	/* daten sind Long integer	      */
#define	 MADTIA5CHAR	7	/* daten sind char im IA5-code	      */
#define	 MADTT61CHAR	8	/* daten sind char im T61-code	      */
#define  BENID          8
#define	 M_DTYP		8	/* maximal-wert	f. data-type	      */
#define	 M_ANZDTYP	M_DTYP+1
				/* maximale anzahl der daten-typen    */



/*-----------------------Verschiedene Konstante-----------------------*/

#define	 L_PREFIX	5	     /*	Prefix-laenge von log-comment	   */
#define	 L_COMNAM      25	     /*	maximale laenge	des namens in log- */
				     /*	comment	der log-konfig-daten	   */
#define	 M_LLOGCOM	L_PREFIX + L_COMNAM
#define	 MAMXLCOM      M_LLOGCOM     /*	maximale laenge	des log-comments   */
				     /*	in der MA_LOG-parameterliste	   */
#define	 MA_EOLR       "==>>"	     /*	ende-kennzeichen des log-record	   */
#define	 MIN_LLR	1 * 640	* L_CHAR
				     /*	minimale groesse fuer log-record,   */
				     /*	die ma_log benoetigt.		    */
#define	 STE_LLR	MIN_LLR
				     /*	schrittweite zur verringerung der   */
				     /*	speicherforderung an malloc	    */
#define	 MAX_LLR	3 * MIN_LLR
				     /*	max. puffer f. logsatz		    */



/* ----------------------Werte fuer error-action----------------------*/
/* ----------------------      MA_ERROR		----------------------*/

#define	 MAR_RETURN	0	     /*	rueckkehr an die aufrufstelle	    */
#define	 MAR_ABOCALL	1	     /*	abort des calling process nach	    */
				     /*	durchfuehrung des error-logging	    */



/*-------------	fester Teil des	Satzes der als log ----------------*/
/*-------------	geschrieben wird		   ----------------*/

typedef	 struct
    {
    unsigned16   marlen;		     /*	Gesamtlaenge des logs		    */
				     /*	logs INCLUSIVE dieses		    */
				     /*	laengenfeldes			    */
    signed16    marlvers;		     /*	file-version fuer logf,	repf, logfb.*/
    signed16    marlsystem;	     /*	BS-Variante + Compiler + Hardware   */
				     /*	die log	erstellt.		    */
    signed16    marltsid;		     /*	Teilsystem-id			    */
    signed32     marltime;		     /*	Sinix time und date		    */
				     /*	interne	Darstellung		    */
    signed32	     marlpid;		     /*	id des logging process		    */
    signed16    marltyp;		     /*	log-type			    */
    unsigned16   marlcol;		     /*	laenge log-comment		    */
    unsigned16   marlcoa;		     /*	rel. Adr. comment		    */
    signed16    marldtyp;		     /*	daten-typ			    */
    unsigned16   marldlen;		     /*	Daten-Laenge			    */
    unsigned16   marldad;		     /*	rel. Adr. Daten			    */
    char     marlbenid[ BENID+1 ]; /* benutzer-id, in BS2 (UTM) die BENID */
				     /*	aus dem	KBKOPF,	in SINIX der	    */
				     /*	prozess-name mit PID.		    */
    signed32     marlalign;		     /*	alignment fuer daten auf Long	    */

    }	 Marlogf;



/*>>>>>> BS2000	>>>>>>*/
#if    ( OPSYSTEM  ==  BS2 )

/*-------------	Halbwort links ausgerichtet im	   ----------------*/
/*-------------	Ganzwort (ausgerichtet!) fuer	   ----------------*/
/*-------------	IPK-Satzlaengenfeld und		   ----------------*/
/*-------------	SAM-PUT-Laengenfeld im BS2000.	   ----------------*/

typedef	 union
    {
    signed16	mahwlng;	     /*	laenge fuer Satzlaengen-feld bei    */
				     /*	IPK und	SAM-I/O-aufrufen. Lanege    */
				     /*	im linken HW, mit erzwungener	    */
				     /*	GW-ausrichtung (signed32).		    */
    signed32	mahwslf;	     /*	GW-ausrichtung + 4 byte		    */

    }	 Mahwfw;



/*-------------	Event-header des log-record fuer   ----------------*/
/*-------------	IPK im	BS2000.			   ----------------*/

typedef	 struct
    {
    Mahwfw	maehlng;	     /*	Gesamtlaenge der event-nachricht    */
				     /*	incl. log-record und eigene laenge  */
				     /*	des laengen-feldes.		    */
    signed16	maehswrp;	     /*	schalter "schreiben report-datei"   */
    signed16	maehswlg;	     /*	schalter "schreiben log-datei"	    */
    signed16	maehswsi;	     /*	schalter "ueberpruefen logf-groesse"*/
    signed16	maehsize;	     /*	max. groesse d.	log-datei logf in KB*/
				     /*	schalter sind Short, damit	    */
				     /*	garantierte laenge!!		    */

    }	 Maehlog;



/*-------------	Gesamte	Event-Daten des	Log-record ----------------*/
/*-------------	(incl. SAM-PUT-header) im BS2000.  ----------------*/

typedef	 struct
    {
    Maehlog	maevhd;		     /*	event-header f.	log-record incl.    */
				     /*	satzlaengen-feld der gesamten	    */
				     /*	nachricht mit ausrichtung Long.	    */
    Mahwfw	maevsamh;	     /*	satzlaengen-feld fuer die SAM-I/O   */
				     /*	(PUT).				    */
    Marlogf	maevrec;	     /*	log-record incl. log-record-header. */

    }	 Maevtlog;

#endif
/*<<<<<< BS2000	<<<<<<*/



/*--------------  Satz der Konfigurationsdaten eines -----------------*/
/*--------------  einzelnen Teilsystems		     -----------------*/

typedef	 struct
    {
    signed16    marctsid;		     /*	Teilsystem-id.		  */
    signed16    marcllev;		     /*	log-level		  */
    Bool     marcltyp[M_ANZTYP];     /*	schalter fuer log-types	  */
    unsigned16   marcmdl;		     /*	maximale Datenlaenge f.	  */
				     /*	teilsystem		  */
    unsigned16   marcprel;		     /*	auszuwertende Laenge des  */
				     /*	log_comment_prefix	  */
    unsigned16   marcnaml;		     /*	tatsaechliche Laenge des  */
				     /*	log_comment_namens	  */
    char     marcprefix[L_PREFIX+1]; /* prefix des log-comment    */
				     /*	+ 1 Byte fuer ggf. \0	  */
    char     marclcnam[L_COMNAM+1];  /* log-comment-Name          */
				     /*	+ 1 Byte fuer ggf. \0	  */

    }	 Marcts;



/*--------------  Satz,	der die	Konfigurationsdaten aller    -----------*/
/*--------------  Teilsysteme im Konfigurationsfile enthaelt -----------*/

typedef	 struct
    {
    Marcts    marcarr[M_NRTS];
    signed16     marcvers;		     /*	file-version der log-config-datei   */
				     /*	logconf.			    */
    signed16     marcsystem;	     /*	BS-Variante + Compiler + Hardware,  */
				     /*	die die	Log-Konfig.-daten erstellt  */
				     /*	hat und	die sie	interpretieren darf.*/
    signed16     marclbu;		     /*	logs seit letzten einlesen d. daten */
				     /*	vom Log-Konfigurationsfile	    */
    signed16     marclbs;		     /*	logs seit    size-check		    */
				     /*	des log-files			    */
    unsigned16    marclfsize;	     /*	maximale log-file groesse	    */
				     /*	in K-bytes			    */
    Bool      marcexit[M_ANZTYP];    /*	schalter fuer abbruch (=true) des   */
				     /*	aktuellen prozesses bei	log-fehlern.*/
				     /*	1 schalter / logtype		    */
    signed16     marccbu;		     /*	calls (of MA_LOG) between update    */
    Bool      marcrtyp;		     /*	ein-/ausschalten betriebsdaten	    */
				     /*	TRUE:  betriebsdaten eingeschaltet  */

    }	 Marconf;



/*>>>>>> BS2000	>>>>>>*/
#if    ( OPSYSTEM  ==  BS2 )

/*--------------  SAM-PUT und SAM-GET gerechter	     -----------------*/
/*--------------  Satz der Log-Konfigurationsdaten   -----------------*/
/*--------------  im Common memory des BS2000.	     -----------------*/

typedef	 struct
    {
    Mahwfw	macmlng;	     /*	satz-laengenfeld fuer SAM-PUT	    */
    Marconf	macmlconf;	     /*	gesamte	log-konfigurations-daten.   */

    }	 Macmlcon;

#endif
/*<<<<<< BS2000	<<<<<<*/



/*--------------  Satzformat fuer Fehlermeldungen des	     -----------*/
/*--------------  prepare (Message-dateien).		     -----------*/

#define	 MA_L_MSG	70	     /*	Message-Laenge			    */

typedef	 struct
    {
    char    ma_msg_key[6];           /* Meldungs-Schluessel                 */
    char    ma_meld_txt[MA_L_MSG];   /* Meldungs-Text                       */
    byte   ma_nl;		     /*	Satzbegrenzer			    */

    }	 Ma_mess;



/* -------------  Parameterliste fuer den MA2_002_ERROR-Aufruf------- */
/* -------------	     und fuer den ma2_007_errlog-Aufruf------ */

typedef	 struct
    {
    Pbhead	*maerhead;	     /*	-> Version der gerufenen Funktion   */
				     /*	<- ret_code			    */
				     /*	<- err_class, Fehler-typ	    */
				     /*	<- err_value, Fehler-Nummer	    */
    Pbhead	*maerrinf;	     /*	-> zeiger auf den fehlerbereich,    */
				     /*	   aus dem err_class und err_value  */
				     /*	   in den log geschrieben werden.   */
				     /*	   version und ret_code	werden	    */
				     /*	   nicht ausgewertet.		    */
    signed16	maeract;	     /*	-> error-action			    */

    }	Pma2002,  Pma2007;
 


/* -------------  Parameterliste fuer den ma2_003_log-Aufruf--------- */

typedef	 struct
    {
    Pbhead   *maplhead;		     /*	-> version der gerufenen funktion   */
				     /*	<- ret_code			    */
				     /*	<- err_class			    */
				     /*	<- err_value			    */
    signed16    mapltsid;		     /*	-> Teilsystemid.		    */
    signed16    mapltyp;		     /*	-> log-type			    */
    signed16    mapllev;		     /*	-> log-level			    */
    String   maplcom;		     /*	-> adresse des log-comment	    */
    unsigned16   maplcol;		     /*	-> log-comment-laenge  (anzahl Char)*/
    signed16    mapldtyp;		     /*	-> data-type			    */
    unsigned16   mapldlng;		     /*	<> data-laenge (anzahl typen !!)    */
				     /*	-> data-laenge (s.o.), wie vom	    */
				     /*	   aufrufer gefordert.		    */
				     /*	<- data-laenge,	tatsaechlich	    */
				     /*	   geschrieben.	rest abgeschnitten. */
    String   mapldad;		     /*	-> adresse log-daten		    */

    }	 Pma2003;



/*--------------  Parameterliste fuer ma2_004_rlconf-aufruf -------------*/
/*--------------		  und ma3_006_wlconf-aufruf -------------*/

typedef	 struct
    {				     /*	parameterliste fuer		    */
				     /*	ma2_004_rlconf-aufruf  und	    */
				     /*	ma3_006_wlconf-aufruf		    */
    Pbhead     *marchead;	     /*	-> version der gerufenen funktion   */
				     /*	<- ret_code			    */
				     /*	<- err_class			    */
				     /*	<- err_value			    */
    Marconf    *marcpconf;	     /*	-> zeiger auf config-satz (ganz)    */

    }	 Pma2004,  Pma3006;






/*ERRON*/
/*-------------Konstanten fuer ERR_VALUE----------------------------------*/
/*-------------F E H L E R  im log-system---------------------------------*/
#define	 MAF_001	   1	     /*	oeffnen	semaphore-datei	!= OK	    */
#define	 MAF_002	   2	     /*	schreiben log-konfigurationsdaten   */
				     /*	nicht OK, datei	nicht da, etc.	    */
#define	 MAF_003	   3	     /*	teilsystem-id ausserhalb bereich    */
#define	 MAF_004	   4	     /*	log-type ausserhalb bereich	    */
#define	 MAF_005	   5	     /*	log-level ausserhalb bereich	    */
#define	 MAF_006	   6	     /*	log-comment-laenge fehlerhaft	    */
#define	 MAF_007	   7	     /*	log-data-laenge	fehlerhaft	    */
#define	 MAF_008	   8	     /*	data-type ausserhalb bereich	    */
#define	 MAF_009	   9	     /*	log- bzw. report-satz schreiben	    */
				     /*	fehlerhaft.			    */
#define	 MAF_010	  10	     /*	bei READ keine daten (EOF)  oder    */
				     /*	nicht alle erhalten.		    */
#define	 MAF_011	  11	     /*	close-Fehler:  falscher	file-	    */
				     /*	descriptor bei log-konfig.datei	    */
#define	 MAF_012	  12	     /*	system-version ( hardware, BS und   */
				     /*	compiler) oder file-version in	    */
				     /*	log-konfiguration falsch	    */
#define	 MAF_013	  13	     /*	MA-version in log-parameterliste    */
				     /*	wird nicht unterstuetzt.	    */
#define	 MAF_014	  14	     /*	MA-version in rlconf-wlconf-Para-   */
				     /*	meterliste wird	nicht unterstuetzt. */
#define	 MAF_015	  15	     /*	oeffnen	Log-Konfigurationsdatei	    */
				     /*	fehlerhaft.			    */
#define	 MAF_016	  16	     /*	bei schreiben log-konfig.daten	    */
				     /*	nicht alles geschrieben.	    */
#define	 MAF_017	  17	     /*	read: lesen log-konfig.	fehlerhaft  */
#define	 MAF_018	  18	     /*	semaphore-datei	nicht sperrbar,	    */
				     /*	nicht da.			    */
#define	 MAF_019	  19	     /*	log-, report-datei nicht oeffenbar, */
				     /*	nicht da.			    */
#define	 MAF_020	  20	     /*	nicht alles in log-, report-datei   */
				     /*	geschrieben, nicht da, keine	    */
				     /*	schreiberlaubnis		    */
#define	 MAF_021	  21	     /*	close: falscher	file-descr. bei	    */
				     /*	log- bzw. report-file		    */
#define	 MAF_022	  22	     /*	stat: log-file nicht da, pfad falsch*/
#define	 MAF_023	  23	     /*	link auf hilfsdatei new___log,	    */
				     /*	logfback oder logf fehlerhaft	    */
#define	 MAF_024	  24	     /*	aendern	der laenge logf	auf 0	    */
				     /*	fehlerhaft. kein		    */
				     /*	zugriff	 etc.			    */
#define	 MAF_025	  25	     /*	semaphoredatei nicht entsperrbar,   */
				     /*	nicht da.			    */
#define	 MAF_026	  26	     /*	log-konfig.-datei nicht	sperrbar,   */
				     /*	nicht da.			    */
#define	 MAF_027	  27	     /*	log-konfig.-datei nicht	entsperrbar,*/
				     /*	nicht da.			    */
/*	 MAF_028	  28		siehe hintergrund-wartung	    */
/*	 MAF_029	  29		siehe hintergrund-wartung	    */
#define	 MAF_030	  30	     /*	kein dyn. speicher f. log-record    */
				     /*	erhaeltlich			    */
#define	 MAF_031	  31	     /*	version	in MA_ERROR-param-list	    */
				     /*	wird nicht unterstuetzt		    */
#define	 MAF_032	  32	     /*	error-action (MA_ERROR)	falsch	    */
#define	 MAF_033	  33	     /*	unlink auf logfback, logf oder	    */
				     /*	hilfsdatei new___log fehlerhaft	    */




/*-------------Konstanten fuer ERR_VALUE----------------------------------*/
/*-------------W A R N U N G E N	----------------------------------*/

#define	 MAW_001	  1	     /*	log-daten werden wegen zu geringem  */
				     /*	speicher oder ueberschreitung der   */
				     /*	max. datenlaenge abgeschnitten.	    */
				     /*	tatsaechliche laenge wird	    */
				     /*	zurueckgemeldet.		    */



/*>>>>>> BS2000	>>>>>>*/
#if    ( OPSYSTEM  ==  BS2 )
/*-------------Konstanten fuer ERR_VALUE----------------------------------*/
/*-------------	      im  BS2000.	 ---------------------------------*/
/*-------------F E H L E R  im log-system---------------------------------*/
#define	 MAF_216	216	     /*	installations-directory	fuer log-   */
				     /*	prozess	fehlt.			    */
#define	 MAF_217	217	     /*	initialisierung	d. log-prozesses    */
				     /*	misslungen.			    */
#define	 MAF_218	218	     /*	aktivieren der "logconf"-conti	    */
				     /*	fehlerhaft.			    */
#define	 MAF_219	219	     /*	warten des log-prozesses auf	    */
				     /*	freigabe durch start-prozess wegen  */
				     /*	lifetime-ablauf	abgebrochen.	    */
#define	 MAF_220	220	     /*	anhaengen d. "log"-conti an die	    */
				     /*	IPK (MAILOG) fehlerhaft. (REVNT)    */
#define	 MAF_221	221	     /*	(F)STAT	fuer logf-groesse fehlerhaft*/
#define	 MAF_222	222	     /*	OPEN-fehler bei	logf, repf.	    */
#define	 MAF_223	223	     /*	kreieren od. aktivieren	der "save"- */
				     /*	conti fehlerhaft.		    */
#define	 MAF_224	224	     /*	ad_sync_aufruf fehlerhaft.	    */
#define	 MAF_225	225	     /*	warten von log-base-prozess	    */
				     /*	unkorrekt abgebrochen		    */
#define	 MAF_226	226	     /*	anfordern d. serialisierung des	    */
				     /*	CM fehlerhaft.			    */
#define	 MAF_227	227	     /*	aufheben der serialisierung des	    */
				     /*	CM fehlerhaft.			    */
#define	 MAF_228	228	     /*	anmelden der ereignis-kennung	    */
				     /*	MAELCON	fehlerhaft.		    */
#define	 MAF_229	229	     /*	anmelden der serialisierungs-kennung*/
				     /*	MASLCON	fehlerhaft.		    */
				     /*	MAF_230	 siehe	magenex.h	    */
#define	 MAF_231	231	     /*	anmelden des common memory MAMLCON  */
				     /*	fuer log-konfig. fehlerhaft.	    */
#define	 MAF_232	232	     /*	ereignis MAELCON mit MA_CMTOFILE    */
				     /*	signalisieren fehlerhaft.	    */
#define	 MAF_233	233	     /*	eroeffnen der IPK fuer einen mail-  */
				     /*	prozess	(nicht log-prozess)	    */
				     /*	fehlerhaft.			    */
#define	 MAF_234	234	     /*	SEVNT-Fehler nach OPCOM	infolge	    */
				     /*	fehlender IPK.			    */
#define	 MAF_235	235	     /*	SEVNT fehlerhaft.		    */
#define	 MAF_236	236	     /*	schliessen logf	wegen um-linken	    */
				     /*	fehlerhaft.			    */
#define	 MAF_237	237	     /*	loeschen logfback bei um-linken	    */
				     /*	der logf fehlerhaft.		    */
#define	 MAF_238	238	     /*	kreieren logf nach umbenennen in    */
				     /*	logfback fehlerhaft.		    */
#define	 MAF_239	239	     /*	oeffnen	logf nach umlinken	    */
				     /*	fehlerhaft.			    */
#define	 MAF_240	240	     /*	file-version in	log-record falsch,  */
				     /*	evtl. version log-prozess und	    */
				     /*	loggender proz.	unterschiedlich.    */
#define	 MAF_241	241	     /*	OPEN auf die log-konfigurations-    */
				     /*	datei "logconf"	fehlerhaft.	    */
#define	 MAF_242	242	     /*	einlesen der log-konfigurations-    */
				     /*	datei "logconf"	in das common memory*/
				     /*	fehlerhaft (READ).		    */
#define	 MAF_243	243	     /*	version	der log-konfig.-datei	    */
				     /*	"logconf" falsch, oder fuer andere  */
				     /*	hardware oder software oder falsche */
				     /*	laenge.	(log-prozess).		    */
#define	 MAF_244	244	     /*	signalisieren, dass log-konfigurat. */
				     /*	in das common memory geladen wurde, */
				     /*	fehlerhaft.			    */
#define	 MAF_245	245	     /*	schreiben (WRITE) der log-konfigur. */
				     /*	aus dem	common memory in die datei  */
				     /*	"logconf" fehlerhaft.		    */
#define	 MAF_246	246	     /*	CLOSE auf die log-konfigurations-   */
				     /*	datei "logconf"	fehlerhaft.	    */
#define	 MAF_247	247	     /*	schliessen logf	wegen log-prozess-  */
				     /*	beendigung fehlerhaft.		    */
#define	 MAF_248	248	     /*	schliessen repf	wegen log-prozess-  */
				     /*	beendigung fehlerhaft.		    */
#define	 MAF_249	249	     /*	STXIT-routine fuer log-prozess	    */
				     /*	nicht anmeldbar.		    */
				     /*	MAF_250	... MAF_251 siehe magenex.h */
				     /*	MAF_252	... MAF_253 siehe mabackx.h */
#define	 MAF_254	254	     /*	warten auf freigabe durch logconf-  */
				     /*	conti fehlerhaft.		    */
#define	 MAF_255	255	     /*	logf fehlt bei groessen-bestimmung. */
				     /*	(log-basis-prozess).		    */
#define	 MAF_256	256	     /*	OPCOM nach SEVNT-Fehler	infolge	    */
				     /*	geschlossener IPK fehlerhaft.	    */
#define	 MAF_257	257	     /*	ENAEI (anmelden) von Ereignis	    */
				     /*	MAEBLOC	fehlerhaft.		    */
#define	 MAF_258	258	     /*	logbase-Start bei laufendem	    */
				     /*	logbase	(Log-Prozess).		    */
#endif
/*<<<<<< BS2000	<<<<<<*/
/*ERROFF*/



/* --------------------	Meldungsschluessel der Log-  -----------------*/
/* --------------------	aufbereitungsroutine	      ----------------*/
/* --------------------	(prepare).		      ----------------*/

#define	 MAP_001	"MA0001"     /*	preparation of file .....	    */
#define	 MAP_002	"MA0002"     /*	file processed .....		    */
#define	 MAP_003	"MA0003"     /*	file is	already	in use		    */
#define	 MAP_004	"MA0004"     /*	file is	empty			    */
#define	 MAP_005	"MA0005"     /*	..... warnung .....		    */
#define	 MAP_006	"MA0006"     /*	Falsche	Monatsangabe		    */
#define	 MAP_007	"MA0007"     /*	Falsche	Stundenangabe		    */
#define	 MAP_008	"MA0008"     /*	Falsche	Minutenangabe		    */
#define	 MAP_009	"MA0009"     /*	Falsche	Sekundenangabe		    */
#define	 MAP_010	"MA0010"     /*	Falsche	Zeitangabe		    */
#define	 MAP_011	"MA0011"     /*	unbekannter Parameter		    */
#define	 MAP_012	"MA0012"     /*	Parameterfehler: Zahl erwartet (PID)*/
#define	 MAP_013	"MA0013"     /*	Teilsystemkennung unbekannt	    */
#define	 MAP_014	"MA0014"     /*	Zeitangabe der Form [[[jj]mm]dd].   */
				     /*	[hh[mm[ss]]] erwartet		    */
#define	 MAP_016	"MA0016"     /*	======== Selektions-Parameter falsch*/
#define	 MAP_017	"MA0017"     /*	==== open-fehler ====		    */
#define	 MAP_018	"MA0018"     /*	==== read-fehler ====		    */
#define	 MAP_019	"MA0019"     /*	==== lseek-fehler ====		    */
#define	 MAP_020	"MA0020"     /*	==== read-fehler (laengen-feld)	====*/
#define	 MAP_021	"MA0021"     /*	==== lseek-fehler (laengen-feld) ===*/
#define	 MAP_022	"MA0022"     /*	====== file hat	falsche	file-version*/
#define	 MAP_023	"MA0023"     /*	name, dateimerkmale falsch	    */
#define	 MAP_024	"MA0024"     /*	Zu wenig Parameter (TSID)	    */
#define	 MAP_025	"MA0025"     /*	Zu wenig Parameter (PID)	    */
#define	 MAP_027	"MA0027"     /*	Zu wenig Parameter (FROM)	    */
#define	 MAP_028	"MA0028"     /*	Zu wenig Parameter (TO)		    */
#define	 MAP_029	"MA0029"     /*	Falsche	Tagesangabe		    */
#define	 MAP_030	"MA0030"     /*	fortsetz. von ..23: oder gesperrt.  */








/* ----------------------Importieren der error-logging----------------*/
/* ----------------------	 routine	      ----------------*/

/*
IMPORT   signed16    ma2_007_errlog();   fkt: schreiben error-log (MA_ERROR)
*/

/*inon ****************************************************************/
/*								      */
/* TYP		: C-MAKRO					      */
/*								      */
/* NAME		: MA_ERROR					      */
/*								      */
/* AUTOR	: U. Richter  D	AP 143				      */
/* DATUM	: 5.03.86					      */
/*								      */
/* BESCHREIBUNG	: Aufruf der error-Log-routine ma2_007_errlog().      */
/*		  Das Makro uebergibt ausser der vom Benutzer	      */
/*		  gelieferten Parameterliste den Dateinamen	      */
/*		  und die Zeilennummer der Aufrufstelle.	      */
/*								      */
/* EINGABEPARAMETER:						      */
/*		  pl->maerhead->version				      */
/*		  pl->maerrinf					      */
/*		  pl->maeract					      */
/*		  (__LINE__ und	__FILE__ werden	selbst erzeugt)	      */
/*								      */
/* AUSGABEPARAMETER:						      */
/*		  pl->maerhead->ret_code			      */
/*		  pl->maerhead->err_class			      */
/*		  pl->maerhead->err_value			      */
/*								      */
/* RETURNVALUE	   :						      */
/*		  pl->maerhead->ret_code			      */
/*								      */
/* STANDARDKOPF	   :						      */
/*		  identisch mit	Funktion ma2_007_errlog()	      */
/*		  (Modul errorxx.c)				      */
/*								      */
/* GLOBALE DATEN (ausgewertet):	-				      */
/* GLOBALE DATEN (veraendert) :	-				      */
/*								      */
/* AUFGERUFENE FUNKTIONEN/PROGRAMME/MAKROS:			      */
/*		  ma2_007_errlog()				      */
/*								      */
/* BEMERKUNGEN	: -						      */
/*								      */
/* SYNTAX	: */						    /**/
/*								      */
/* ----------------------M A K R O    MA_ERROR	      ----------------*/
/*								      */
/*inoff	***************************************************************/


#define  MA2_002_ERROR(x) d27_loginfo (D27_MAERR2,  D27_ERRMSG33, \
				     (x)->maerrinf->err_class,\
				      (x)->maerrinf->err_value );


#define  ma2_003_log(x) d27_loginfo (D27_MALOG2,  \
					       (x)->maplcol,(x)->maplcom); \
			d27_loginfo (D27_MALOG2,  \
					       (x)->mapldlng,(x)->mapldad)

#endif /* _MALGCFE_H */
