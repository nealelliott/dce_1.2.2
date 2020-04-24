/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: magenex.h,v $
 * Revision 1.1.10.2  1996/02/18  23:36:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:49  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:02:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:31  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:54:47  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:48:55  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  13:01:24  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:44:42  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:42:48  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:30:56  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  04:18:34  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:43:56  zeliff]
 * 
 * $EndLog$
 */
#ifndef _MAGENEX_H
#define _MAGENEX_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char magenex_rcsid[] = "@(#)$RCSfile: magenex.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:36:19 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYP		: INCLUDE					      */
/*		  =========					      */
/* NAME		: magenex.h					      */
/*		  =========					      */
/* AUTOR	: U. Richter   D AP 143				      */
/* DATUM	: 5.03.86					      */
/*								      */
/* KOMPONENTE	: MA						      */
/*								      */
/* DOK.-NR.	: 3120.50.01					      */
/* PRD#/VERS.	:						      */
/*								      */
/* BESCHREIBUNG	: Allgemeine Konstante,	die in allen MA-Komponenten   */
/*		  und von allen	MA-Nutzern benoetigt werden.	      */
/*								      */
/* SYSTEMABHAENIGKEIT:						      */
/*		  COMMON  (cond. compilation)			      */
/*								      */
/*								      */
/* Vers.Nr. |  Datum  |	 Aenderungen		       | KZ | CR# FM# */
/*  1.0	    |  5.03.86|	Original		       | Ri |	      */
/*  1.1	    | 30.04.84|	em_rootdir:		       | Ri |	      */
/*	    |	      |	Aenderung der Dateinamen fuer  |    |	      */
/*	    |	      |	das Log-system.		       |    |	      */
/*  1.2	    | 28.07.86|	Einfuehrung von	file-versionen | Ri | --      */
/*  1.3	    | 17.10.86|	Eintragen der SCCS-id	       | Ri |	      */
/*  1.4	    | 31.10.86|	CR230: Erweiterung der Log-    | Ri | CR230   */
/*	    |	      |	datentypen um IA5 und T61      |    |	      */
/*	    |	      |	CR231: Neuer Log-Konfig.-      | Ri | CR231   */
/*	    |	      |	Parameter "MA_LOG-calls        |    |         */
/*	    |	      |	between	update".               |    |         */
/*	    |	      |	CR55: Betriebsdaten abschaltbar| Ri | CR55    */
/*  1.5	    | 23.01.87|	Realisierung der BS2000-       | Ri | --      */
/*	    |	      |	Variante.		       |    |	      */
/*  1.6	    |  2.03.87|	Selektieren der	Return-Codes   | Ri | CR666   */
/*  1.7	    | 23.03.87|	Mehrsprachigkeit bei Fehler-   | Ri | CR1005  */
/*	    |	      |	meldungen (prepare)	       |    |	      */
/*  1.8	    | 15.05.87|	Fehlertexte fuer BS2000-       | Ri | CR1300  */
/*	    |	      |	Variante + neue	Ereignis-kenn. |    |	      */
/*	    |	      |	MAELRDY			       |    |	      */
/*  1.9	    | 27.07.87|	Ergaenzung BS2000-Variante:    | Ri | CR1300  */
/*	    |	      |	keine eigenen BS2000-DHS-      |    |	      */
/*	    |	      |	Dateinamen.		       |    |	      */
/*  1.10    |  5.08.87|	Heruntersetzen der Signal-     | Ri | CR1300  */
/*	    |	      |	lifetime f. fehlerfall (15 min)|    |	      */
/*	    |	      |	Neues Ereignis MAEBLOC zum     | Ri |	      */
/*	    |	      |	Verhindern von Mehrfach-Starts |    |	      */
/*	    |	      |	des Log-Prozesses (logbase).   |    |	      */
/*  1.11    | 24.08.87|	A) Geaenderte Zuordnung	von    | Ri | CR1681  */
/*	    |	      |	TSIDs im Logsystem (X4	       |    | CR1539  */
/*	    |	      |	entfaellt, ED statt EM)	und    |    |	      */
/*	    |	      |	Einfuehrung der	neuen TS:      |    |	      */
/*	    |	      |	ED, FS,	PR, DB.		       |    |	      */
/*	    |  7.09.87|	B) Log-System-Anpassung	fuer   | Ri | CR1691  */
/*	    |	      |	UTM mit	KCBENID	aus em_owner.  |    |	      */
/*  1.12    | 26.10.87|	A) Teilsystem FA in Logsystem  | Ri | CR1820  */
/*	    |	      |	aufnehmen.		       |    |	      */
/*  1.12    |  4.03.88|	Ergaenzung BS2000-Variante:    | Ri | CR1300  */
/*	    |	      |	A) Prefix fuer Dateinamen ueber|    |	      */
/*	    |	      |	Linknamen.		       |    |	      */
/*  1.13    | 24.03.88|	Ergaenzung BS2000-Variante:    | Ri | CR1300  */
/*	    |	      |	A) Fehlermeldungen des prepare |    |	      */
/*	    |	      |	ueber AD-utility mehrsprachig. |    |	      */
/*	    |	      |	B) Dabei SINIX mehrsprachig    | Ri |	      */
/*	    |	      |	ueber eigene Datei je Sprache +|    |	      */
/*	    |	      |	Sprachauswahl ueber environ.-  |    |	      */
/*	    |	      |	Variable und Anlagensprache.   |    |	      */
/*	    |	      |				       |    |	      */
/*    2.0   | 05.08.88|	Umstellung auf V2.0	       | Gu | EM2000  */
/*	    |	      |				       |    |	      */
/*								      */
/*datoff **************************************************************/

#include <dce/dce.h>

/*-----------------------Versions - Konstante  ---------------------*/

#define	 MA2_V01	1		  /* MA	 version 1.0  IMAI	   */
#define	 MA2_MIN	1		  /* MA	 minimal-version  IMAI	   */
#define	 MA2_MAX	1		  /* MA	 maximal-version  IMAI	   */

#define	 MA3_V01	1		  /* MA	 version 1.0  IMAI--ADM	   */
#define	 MA3_MIN	1		  /* MA	 minimal-version  IMAI--ADM*/
#define	 MA3_MAX	1		  /* MA	 maximal-version  IMAI--ADM*/

#define	 MA_FVL		4		  /* file-version fuer log-datei,  */
					  /* report-datei und letzter	   */
					  /* log-datei			   */
#define	 MA_FVLC	6		  /* file-version fuer log-kon-	   */
					  /* konfigurations-datei	   */
#define	 MA_FVCB	1		  /* file-version fuer hintergr.-  */
					  /* wartungs-datei		   */
#define	 MA_FVEM	1		  /* file-version fuer EM-start-   */
					  /* datei			   */



/*>>>>>> SINIX >>>>>>*/
#if    ( OPSYSTEM  ==  SINIX )

/*---------------------	 Dateinamen fuer Log-System  -----------------*/

#define	 MALFNAM	"/ma10/logf"
					  /* name f. log.file		   */
#define	 MALRNAM	"/ma10/repf"
					  /* name f. report.file	   */
#define	 MALFBNAM	"/ma10/logfback"
					  /* name f. letzter logfile	   */
#define	 MANLNAM	"/ma10/new___log"
					  /* name f. link auf neuen logfile*/
#define	 MACFNAM	"/ma10/logconf"
					  /* name f. Konfiguration	   */
#define	 MASEMFNAM	"/ma10/semlog"
					  /* name f. semaphore-file	   */



/*---------------------	 Verschiedene Konstante	----------------------*/

#define	 MA_LINK_MAX	0	     /*	max-laenge fuer	link-namen: in	    */
				     /*	SINIX =	0.			    */
#define	 MA_USER_LANGUAGE	"LANGUAGE"
				     /*	name der environment-variablen fuer */
				     /*	die gewuenschte	sprachunterstuetzung*/
#define	 MA_N_LANGUAGE	9	     /*	anzahl der dem prepare bekannten    */
				     /*	meldungsdateien.		    */



/*---------------------	 Dateinamen fuer Fehler-     -----------------*/
/*---------------------	 meldungen im prepare.	     -----------------*/

#define	 MAFMDAT_D	"/ma10/messages01"
				     /*	deutsch				    */
#define	 MAFMDAT_GB	"/ma10/messages02"
				     /*	english				    */
#define	 MAFMDAT_F	"/ma10/messages03"
				     /*	francais			    */
#define	 MAFMDAT_I	"/ma10/messages04"
				     /*	italiano			    */
#define	 MAFMDAT_H	"/ma10/messages05"
				     /*	espaniol			    */
#define	 MAFMDAT_NL	"/ma10/messages06"
				     /*	.....		=>  niederlaendisch */
#define	 MAFMDAT_DK	"/ma10/messages07"
				     /*	dansk		=>  daenisch	    */
#define	 MAFMDAT_N	"/ma10/messages08"
				     /*	norge		=>  norwegisch	    */
#define	 MAFMDAT_S	"/ma10/messages09"
				     /*	swensk		=>  schwedisch	    */

#endif
/*<<<<<< SINIX <<<<<<*/



/*>>>>>> BS2000	>>>>>>*/
#if    ( OPSYSTEM  ==  BS2 )

/*---------------------	 Dateinamen fuer Log-System  -----------------*/
/*---------------------- (BS2000-Variante)	----------------------*/

#define	 MALFNAM	".MA10.LOGF"
					  /* name f. log.file		   */
#define	 MALRNAM	".MA10.REPF"
					  /* name f. report.file	   */
#define	 MALFBNAM	".MA10.LOGFBACK"
					  /* name f. letzter logfile	   */
#define	 MANLNAM	".MA10.NEW###LOG"
					  /* name f. link auf neuen logfile*/
#define	 MACFNAM	".MA10.LOGCONF"
					  /* name f. Konfiguration	   */
#define	 MASEMFNAM	".MA10.SEMLOG"
					  /* name f. semaphore-file	   */



/*---------------------	 Verschiedene Konstante	----------------------*/

#define	 MA_LINK_MAX	55	     /*	max-laenge fuer	link-namen	    */

#endif
/*<<<<<< BS2000	<<<<<<*/



/*---------------------	 Dateinamen fuer die	----------------------*/
/*---------------------	 Hintergrund-Wartung	----------------------*/
/*---------------------- und EM-Start.		----------------------*/
/*---------------------- (SINIX	und BS2000)	----------------------*/

#define	 MACBACKNAM	"/ma10/cbackgrf"
					  /* name f. hintergrundwart.-file */
#define	 MAEMSTFNAM	"/ma10/emstartf"
					  /* name fuer EM-Start-Datei	   */





/*-----------------------Verschiedene Konstante-----------------------*/
/*-----------------------fuer system-aufrufe   -----------------------*/

#define	 MA_CRMOD	0664	/* crmod fuer creat-aufruf (oktal)     */
				/* lesen, schreiben fuer eigentuemer   */
				/* und gruppe, lesen fuer alle	       */



/*>>>>>> BS2000	>>>>>>*/
#if    ( OPSYSTEM  ==  BS2 )
/*---------------------	 Namen fuer Ereignis-	     -----------------*/
/*---------------------	 Contingency-, Common Memory+-----------------*/
/*---------------------- Serialisierungskennungen    -----------------*/

#define	 MAEBLOC	"MAEBLOC"    /*	ereigniskennung	"bloc":		    */
				     /*	Verhindern von logbase-Starts bei   */
				     /*	laufendem logbase (Log-Prozess).    */
#define	 MAEBACK	"MAEBACK"    /*	ereigniskennung	"back":		    */
				     /*	hintergrund-wartung pruefen	    */
#define	 MAEREQ		"MAEREQ"     /*	ereigniskennung	"req":		    */
				     /*	EM-start-/EM-ende-request	    */
#define	 MAELCON	"MAELCON"    /*	ereigniskennung	"lcon":		    */
				     /*	log-konfig. uebernehmen/laden	    */
#define	 MAECRDY	"MAECRDY"    /*	ereigniskennung	"crdy":	 nach	    */
				     /*	CM-oeffnen ist CM mit logconf	    */
				     /*	erstmals geladen worden.	    */
#define	 MAELRDY	"MAELRDY"    /*	ereigniskennung	"lrdy":	 nach	    */
				     /*	aktivierung der	logconf-conti wartet*/
				     /*	der log-basis-prozess auf ende	    */
				     /*	dieser contingency.		    */
#define	 MAELOGS	"MAELOGS"    /*	ereigniskennung	"logs":		    */
				     /*	log-prozess freigeben.		    */
#define	 MAELOG		"MAELOG"     /*	ereigniskennung	"log":		    */
				     /*	log-information	eingetroffen	    */
#define	 MAEENDL	"MAEENDL"    /*	ereigniskennung	"endl":		    */
				     /*	warte-zustand d. log-basis-prozesses*/
 
#define	 MACBACK	"MACBACK"    /*	kennung	f. "back"-contingency	    */
#define	 MACREQ		"MACREQ"     /*	kennung	f. "req"-conti		    */
#define	 MACLCON	"MACLCON"    /*	kennung	f. "logconf"-conti	    */
#define	 MACLOG		"MACLOG"     /*	kennung	f. "log"-conti		    */

#define	 MAILOG		"MAILOG"     /*	kennung	f. IPK des log-prozess	    */
#define	 MAI		"MAI"	     /*	vorspann fuer IPK-namen	eines	    */
				     /*	beliebigen prozesses.		    */
#define	 MAMLCON	"MAMLCON"    /*	kennung	f. commom memory f. logconf */
#define	 MASLCON	"MASLCON"    /*	serialisierungskennung f. logconf   */



/*-----------------------  Verschiedene	Konstante    -----------------*/
/*-----------------------  fuer	BS2000		     -----------------*/

#define	 MA_LIFETIME	3600	     /*	lifetime f. warten auf signale in   */
				     /*	contingencies.			    */
#define	 MA_EXITTIME	900	     /*	lifetime, deren	erreichen im	    */
				     /*	normalen betrieb pathologisch ist.  */
#define	 MA_TRENNER	'$'	     /*	trennzeichen in	diesen namen	    */
				     /*	anstelle des punkt "."		    */
#define	 MA_FILPRIM	3	     /*	primaer-zuweisung fuer kreierte	    */
				     /*	logsystem-dateien.		    */
#define	 MA_FILSEC	9	     /*	sekundaer-zuweisung fuer kreierte   */
				     /*	logsystem-dateien.		    */
#define	 MA_COEP	'P'	     /*	conti-fehler: nur Pbhead ausgeben   */
#define	 MA_COES	'S'	     /*	conti-fehler: Pbhead + r15 ausgeben:*/
				     /*	synchroner conti-fehler.	    */
#define	 MA_COEA	'A'	     /*	conti-fehler: Pbhead + postcode	+   */
				     /*	ereignis-informations-code (r3+r2)  */
				     /*	ausgeben.			    */



/*-----------------------Parameterlisten fuer  -----------------------*/
/*-----------------------BS2000-System-calls   -----------------------*/

/*--------------  Parameterliste fuer ma2_020_utenaei-aufruf  -----------*/

typedef	 struct
    {				     /*	parameterliste fuer		    */
				     /*	ma2_020_utenaei-aufruf		    */
    Pbhead     *maeihead;	     /*	-> version der gerufenen funktion   */
				     /*	<- ret_code			    */
				     /*	<- err_class			    */
				     /*	<- err_value			    */
    String     maeiname;	     /*	-> name	der ereigniskennung ohne    */
				     /*	   prefix (em_rootdir)		    */

    }	 Pma2020;



/*--------------  Parameterliste fuer ma2_021_utenaco-aufruf  -----------*/

typedef	 struct
    {				     /*	parameterliste fuer		    */
				     /*	ma2_021_utenaco-aufruf		    */
    Pbhead     *macohead;	     /*	-> version der gerufenen funktion   */
				     /*	<- ret_code			    */
				     /*	<- err_class			    */
				     /*	<- err_value			    */
    String     maconame;	     /*	-> name	der contigency-prozesses    */
				     /*	   ohne	prefix (em_rootdir)	    */

    }	 Pma2021;



/*--------------  Parameterliste fuer ma2_022_utcoerr-aufruf   -------------*/

typedef	 struct
    {				     /*	parameterliste fuer		    */
				     /*	ma2_022_utcoerr-aufruf.		    */
    Pbhead     *macehead;	     /*	-> version der gerufenen funktion   */
				     /*	wird in	dieser funktion	ignoriert.  */
				     /*	-> ret_code			    */
				     /*	-> err_class			    */
				     /*	-> err_value			    */
				     /*	   des fehlers der zu drucken ist.  */
    String     macefkt;		     /*	adresse	der fehlermeldenden	    */
				     /*	contingency-routine. name muss	    */
				     /*	genau 8	zeichen	lang sein.	    */
    signed32       maceeico;	     /*	ereignis-informations-code der	    */
				     /*	fehlerhaften contingency  (=R2).    */
    signed32       macepost;	     /*	gemeldeter post-code (=R3)	    */
    signed32       macer15;		     /*	synchron gemeldeter rc des system-  */
				     /*	calls (=R15).			    */
    char       macecntl;	     /*	steuer-character fuer die fehler-   */
				     /*	ausgabe:			    */
				     /*		'p':  nur den Pbhead-inhalt */
				     /*		      ausgeben.		    */
				     /*		's':  synchronen rc (r15)   */
				     /*		      eines system-calls    */
				     /*		      und Pbhead ausgeben.  */
				     /*		'a':  asynchrone return-inf.*/
				     /*		      (r2 und r3) und	    */
				     /*		      Pbhead ausgeben.	    */

    }	 Pma2022;



/*ERRON*/
/*-------------Konstanten fuer ERR_VALUE  --------------------------------*/
/*-------------F E H L E R  von	BS2000-	  --------------------------------*/
/*-------------System-calls.		  --------------------------------*/
 
#define	 MAF_201	201	     /*	MA-version in utenaei-parlist nicht */
				     /*	unterstuetzt.			    */
				     /*					    */
#define	 MAF_202	202	     /*	falscher ereignisname.		    */
#define	 MAF_203	203	     /*	fehler v. ENAEI:		    */
				     /*	kennung	schon im proz.:	      ('0c')*/
				     /*	ungueltige operanden:	      ('10')*/
				     /*	max. anzahl v. kennungen:     ('18')*/
				     /*	unterschiedliches queueing:   ('1c')*/
#define	 MAF_204	204	     /*	MA-version in utenaco-parlist nicht */
				     /*	unterstuetzt.			    */
#define	 MAF_205	205	     /*	falscher ereignisname.		    */
#define	 MAF_206	206	     /*	fehler v. ENACO:		    */
				     /*	conti-prozess schon def.:     ('0c')*/
				     /*	ungueltige operanden:	      ('10')*/
				     /*	max. anzahl v. prozessen:     ('18')*/
#define	 MAF_207	207	     /*	fehler v. ENASI:   fehlergruende    */
				     /*	analog zu ENACO.		    */
#define	 MAF_208	208	     /*	fehler v. POSSIG:		    */
				     /*	ungueltige operanden (x'10'), eiid  */
				     /*	(x'14'), coid (x'24') oder zuviele  */
				     /*	conti-prozesse (x'18') oder ereignis*/
				     /*	nicht angemeldet (x'0C').	    */
#define	 MAF_209	209	     /*	ereignis MAEBACK an "back"-conti    */
				     /*	signalisieren fehlerhaft.	    */
#define	 MAF_210	210	     /*	fehler v. SOLSIG: analog zu POSSIG. */
#define	 MAF_211	211	     /*	ereignis-MAEBACK-signalisieren	    */
				     /*	fehlerhaft.			    */
#define	 MAF_212	212	     /*	fehler v. ENAEI-utility. ENAEI auf  */
				     /*	ereignis MAEBACK misslungen.	    */
#define	 MAF_213	213	     /*	fehler v. ENAEI-utility. ENAEI auf  */
				     /*	ereignis MAEREQ	misslungen.	    */
#define	 MAF_214	214	     /*	ereignis-MAEREQ-signalisieren	    */
				     /*	fehlerhaft.			    */
#define	 MAF_215	215	     /*	eroeffnen der IPK "MAILOG" fehlerh. */
				     /*	MAF_216	... MAF_229 siehe malgcfe.h */
#define	 MAF_230	230	     /*	anmelden des common memory MAMLCON  */
				     /*	fehlerhaft.			    */
				     /*	MAF_231	... MAF_249 siehe malgcfe.h */
#define	 MAF_250	250	     /*	anhaengen der log-conti	an die	    */
				     /*	ereignis-kenn. MAELOG fehlerhaft.   */
#define	 MAF_251	251	     /*	contingency-kennung im selben	    */
				     /*	prozess	ein 2. mal angemeldet.	    */
				     /*	MAF_252, MAF_253 siehe mabackx.h    */
#endif
/*<<<<<< BS2000	<<<<<<*/
/*ERROFF*/

#endif /* _MAGENEX_H */
