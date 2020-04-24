//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_asn.h,v $
// Revision 1.1.2.1  1996/10/04  14:22:35  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:45:07  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:29 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:51 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// Note the following conventions used in this code:
//
// All class names end with a "C".
// All pointer names end with a "P".
// All reference names end with an "R".
// All handle names end with an "H".
//
// Thus:
//
// CryptoC	- a cryptographic class
// asnBufP	- a pointer to an ASN.1 buffer
// newObjR	- a reference to a new object
// pkssContextH - a PKSS context handle
//
// Module:  sec_pkss_asn.h
//
// Purpose: class definitions for PKSS ASN.1 messages
//

#ifndef SEC_PKSS_ASN_H
#define SEC_PKSS_ASN_H

//
// Obtain ASN.1 base classes
//
#include <dce/x509.h>

//
// PKSS Application Tags
//

#define PKSS_LOGIN_P1_MSG_TAG		(1)
#define PKSS_LOGIN_P2_MSG_TAG		(2)
#define PKSS_LOGIN_P3_MSG_TAG		(3)
#define PKSS_LOGIN_P4_MSG_TAG		(4)

#define PKSS_CLT_CHG_P1_MSG_TAG		(11)
#define PKSS_CLT_CHG_P2_MSG_TAG		(12)
#define PKSS_CLT_CHG_P3_MSG_TAG		(13)
#define PKSS_CLT_CHG_P4_MSG_TAG		(14)
#define PKSS_CLT_CHG_P5_MSG_TAG		(15)
#define PKSS_CLT_CHG_P6_MSG_TAG		(16)

#define PKSS_SVR_CHG_P1_MSG_TAG		(21)
#define PKSS_SVR_CHG_P2_MSG_TAG		(22)
#define PKSS_SVR_CHG_P3_MSG_TAG		(23)
#define PKSS_SVR_CHG_P4_MSG_TAG		(24)
#define PKSS_SVR_CHG_P5_MSG_TAG		(25)
#define PKSS_SVR_CHG_P6_MSG_TAG		(26)

#define PKSS_MGMT_P1_MSG_TAG		(31)
#define PKSS_MGMT_P2_MSG_TAG		(32)

#define PKSS_MGMT_CLT_ADD_P3_MSG_TAG	(43)
#define PKSS_MGMT_CLT_ADD_P4_MSG_TAG	(44)

#define PKSS_MGMT_SVR_ADD_P3_MSG_TAG	(53)
#define PKSS_MGMT_SVR_ADD_P4_MSG_TAG	(54)

#define PKSS_MGMT_DEL_P3_MSG_TAG	(63)
#define PKSS_MGMT_DEL_P4_MSG_TAG	(64)

#define PKSS_MGMT_CLT_MOD_P3_MSG_TAG	(73)
#define PKSS_MGMT_CLT_MOD_P4_MSG_TAG	(74)

#define PKSS_MGMT_SVR_MOD_P3_MSG_TAG	(83)
#define PKSS_MGMT_SVR_MOD_P4_MSG_TAG	(84)


//-----------------------------------------------------------------+
//								   |
// class definition for PKSS database key (that is, db search key) |
//								   |
//-----------------------------------------------------------------+

class PkssUserKeyC : public asn_sequence
{
public:
  asn_IA5string uname;
  asn_IA5string domain;  // must convert uuid_t to/from string
  asn_integer	usages;
  asn_integer	version;

  PkssUserKeyC (void)
  {
    register_child (&uname);
    register_child (&domain);
    register_child (&usages);
    register_child (&version);
  }

}; // End class definition for PkssUserKeyC


//-------------------------------------------+
//					     |
// class definition for PKSS database record |
//					     |
//-------------------------------------------+

class PkssDBrecC : public asn_sequence // [Kxu]H1:H2:Kou:version
{
public:
  asn_octetstring KxuH1;	// [Kxu]H1; Kxu encrypted w/ H1
  asn_octetstring H2;		// password hash H2
  SubjectPublicKeyInfo Kou;	// public key of user; see X.509
  asn_integer version;	// version of Kxu/Kou; must match version in key above

  PkssDBrecC (void):asn_sequence(SECURE)
  {
    register_child (&KxuH1);
    register_child (&H2);
    register_child (&Kou);
    register_child (&version);
  }

}; // End class definition for PkssDBrecC


//--------------------------------------------------------+
//							  |
// class definition for PKSS client algorithm choice list |
//							  |
// asn_sequenceof<asn_integer> should be changed to       |
// asn_sequenceof<AlgorithmIdentifier>			  |
//--------------------------------------------------------+

class PkssAlgListC : public asn_sequenceof<asn_integer>
{

}; // End class definition for PkssAlgListC


//--------------------------------------------------+
//						    |
// class definition for PKSS user login phase 1 msg |
//						    |
//--------------------------------------------------+

class PkssUsrGetKeyP1MsgBaseC : public asn_sequence
  // C-->P: userKey:E(Dc):algList
{
public:
  PkssUserKeyC userKey;		// Username (PKSS database key four-tuple)
  asn_integer eDc;		// E(Dc); exponentiated D-H client value
  PkssAlgListC algList;		// list of crypto algorithms PKSS clt supports

  PkssUsrGetKeyP1MsgBaseC (void)
  {
    register_child (&userKey);
    register_child (&eDc);
    register_child (&algList);
  }

}; // End class definition for PkssUsrGetKeyP1MsgBaseC


class PkssLoginP1MsgC : public PkssUsrGetKeyP1MsgBaseC
{
public:
  PkssLoginP1MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_LOGIN_P1_MSG_TAG);
  }

}; // End class definition for PkssLoginP1MsgC


//---------------------------------------------------+
//						     |
// class definition for PKSS user login phase 2 data |
//						     |
//---------------------------------------------------+

class PkssUsrGetKeyP2DataF2C : public asn_sequence  // [[S]H2:H(algList)
{
public:
  asn_octetstring sH2;		// S; sess key encrypted w/H2 before encoding
  asn_octetstring hAlgs;	// H(algs); a hash of client's algList

  PkssUsrGetKeyP2DataF2C (void):asn_sequence(SECURE)
  {
    register_child (&sH2);
    register_child (&hAlgs);
  }

}; // End class definition for PkssUsrGetKeyP2DataF2C


//--------------------------------------------------+
//						    |
// class definition for PKSS user login phase 2 msg |
//						    |
//--------------------------------------------------+

class PkssUsrGetKeyP2MsgBaseC : public asn_sequence
			// P-->C: E(Dp):[[S]H2:H(algs)]D:[Np]S:alg
{
public:
  asn_integer eDp;		// E(Dp); exponentiated D-H PKSS value
  asn_octetstring sH2HalgsD;	// [[S]H2:H(algList)]D; PkssLoginP2DataF2C
  asn_octetstring nps;		// [Np]S; PKSS nonce encrypted w/S b4 encoding
  asn_integer algIdx;	// index into client's supported algorithm list

  PkssUsrGetKeyP2MsgBaseC (void)
  {
    register_child (&eDp);
    register_child (&sH2HalgsD);
    register_child (&nps);
    register_child (&algIdx);
  }

}; // End class definition for PkssUsrGetKeyP2MsgBaseC


class PkssLoginP2MsgC : public PkssUsrGetKeyP2MsgBaseC
{
public:
  PkssLoginP2MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_LOGIN_P2_MSG_TAG);
  }

}; // End class definition for PkssLoginP2MsgC


//---------------------------------------------------+
//						     |
// class definition for PKSS user login phase 3 data |
//						     |
//---------------------------------------------------+
class PkssUsrGetKeyP3DataC : public asn_sequence
{
public:
  asn_integer protoID;		// I; a protocol ID for login or key change
  asn_integer np;		// Np; a nonce generated by PKSS
  asn_integer nc;		// Nc; a nonce generated by client

  PkssUsrGetKeyP3DataC (void):asn_sequence(SECURE)
  {
    register_child (&protoID);
    register_child (&np);
    register_child (&nc);
  }

}; // End class definition for PkssUsrGetKeyP3DataC


//--------------------------------------------------+
//						    |
// class definition for PKSS user login phase 3 msg |
//						    |
//--------------------------------------------------+

class PkssUsrGetKeyP3MsgBaseC : public asn_octetstring	// C-->P: [I:Np:Nc]S
{

}; // End class definition for PkssUsrGetKeyP3MsgBaseC


class PkssLoginP3MsgC : public PkssUsrGetKeyP3MsgBaseC
{
public:
  PkssLoginP3MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_LOGIN_P3_MSG_TAG);
  }

}; // End class definition for PkssLoginP3MsgC


//---------------------------------------------------+
//						     |
// class definition for PKSS user login phase 4 data |
//						     |
//---------------------------------------------------+

class PkssUsrGetKeyP4DataC : public asn_sequence
{
public:
  asn_octetstring kxuH1;	// [Kxu]H1; private key of user encrypted w/H1
  SubjectPublicKeyInfo kou;	// public key of user; see X.509
  asn_integer version;		// V; version of Kxu/Kou pair
  asn_integer nc;		// Nc; a nonce generated by client

  PkssUsrGetKeyP4DataC (void):asn_sequence(SECURE)
  {
    register_child (&kxuH1);
    register_child (&kou);
    register_child (&version);
    register_child (&nc);
  }

}; // End class definition for PkssUsrGetKeyP4DataC


//--------------------------------------------------+
//						    |
// class definition for PKSS user login phase 4 msg |
//						    |
//--------------------------------------------------+

class PkssUsrGetKeyP4MsgBaseC : public asn_octetstring
  // P-->C: [[Kxu]H1:Kou:V:Nc]S
{

}; // End class definition for PkssUsrGetKeyP4MsgBaseC


class PkssLoginP4MsgC : public PkssUsrGetKeyP4MsgBaseC
{
public:
  PkssLoginP4MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_LOGIN_P4_MSG_TAG);
  }

}; // End class definition for PkssLoginP4MsgC


//--------------------------------------------------------------------------+
//									    |
// class definitions for user change key/password msgs (clt generates keys) |
//									    |
// The next four classes are derived from the login phase 1-4 msgs.         |
//									    |
//--------------------------------------------------------------------------+

class PkssCltChgP1MsgC : public PkssUsrGetKeyP1MsgBaseC
{
public:
  PkssCltChgP1MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_CLT_CHG_P1_MSG_TAG);
  }

}; // End class definition for PkssCltChgP1MsgC


class PkssCltChgP2MsgC : public PkssUsrGetKeyP2MsgBaseC
{
public:
  PkssCltChgP2MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_CLT_CHG_P2_MSG_TAG);
  }

}; // End class definition for PkssCltChgP2MsgC


class PkssCltChgP3MsgC : public PkssUsrGetKeyP3MsgBaseC
{
public:
  PkssCltChgP3MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_CLT_CHG_P3_MSG_TAG);
  }

}; // End class definition for PkssCltChgP3MsgC


class PkssCltChgP4MsgC : public PkssUsrGetKeyP4MsgBaseC
{
public:
  PkssCltChgP4MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_CLT_CHG_P4_MSG_TAG);
  }

}; // End class definition for PkssCltChgP4MsgC


//-----------------------------------------------------------------------+
//									 |
// class def for PKSS user chg key/pwd phase 5 data (clt generates keys) |
//									 |
//-----------------------------------------------------------------------+

class PkssCltChgP5DataC : public asn_sequence // [Kxu~]H1~:H2~:Kou~
{
public:
  asn_octetstring newKxuH1;	// [Kxu~]H1~; new Kxu encrypted w/new H1
  asn_octetstring newH2;	// new password hash H2
  SubjectPublicKeyInfo newKou;	// new public key of user; see X.509

  PkssCltChgP5DataC (void):asn_sequence(SECURE)
  {
    register_child (&newKxuH1);
    register_child (&newH2);
    register_child (&newKou);
  }

}; // End class definition for PkssCltChgP5DataC


class PkssCltChgP5SignedC : public asn_sequence  // {[Kxu~]H1~:H2~:Kou~}Kxu
{
public:
  PkssCltChgP5DataC dataToBeSigned;
  asn_integer sigAlg;	// sec_bsafe_alg_type_t == unsigned32; change to
			// AlgorithmIdentifier algId;
  asn_octetstring signature;

  PkssCltChgP5SignedC (void)
  {
    register_child (&dataToBeSigned);
    register_child (&sigAlg);
    register_child (&signature);
  }
  
}; // End class definition for PkssCltChgP5SignedC


//----------------------------------------------------------------------+
//									|
// class def for PKSS user chg key/pwd phase 5 msg (clt generates keys) |
//									|
//----------------------------------------------------------------------+

class PkssCltChgP5MsgC : public asn_octetstring
{
public:
//  asn_sequence pkssCltChgP5Msg;	// C-->P: [{[Kxu~]H1~:H2~:Kou~}Kxu]S

  PkssCltChgP5MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_CLT_CHG_P5_MSG_TAG);
  }

}; // End class definition for PkssCltChgP5MsgC


//-----------------------------------------------------------------------+
//								  	 |
// class def for PKSS user chg key/pwd phase 6 data (clt generates keys) |
//									 |
//-----------------------------------------------------------------------+

class PkssCltChgP6DataC : public asn_sequence
{
public:
  asn_integer nc;		// Nc; a nonce generated by client
  asn_integer version;		// V; version of Kxu/Kou pair
  asn_integer status;		// a status code

  PkssCltChgP6DataC (void):asn_sequence (SECURE)
  {
    register_child (&nc);
    register_child (&version);
    register_child (&status);
  }

}; // End class definition for PkssCltChgP6DataC


//----------------------------------------------------------------------+
//									|
// class def for PKSS user chg key/pwd phase 6 msg (clt generates keys) |
//									|
//----------------------------------------------------------------------+

class PkssCltChgP6MsgC : public asn_octetstring
{
public:

  PkssCltChgP6MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_CLT_CHG_P6_MSG_TAG);
  }

}; // End class definition for PkssCltChgP6MsgC


//--------------------------------------------------------------------------+
//									    |
// class definitions for user change key/password msgs (svr generates keys) |
//									    |
//--------------------------------------------------------------------------+


class PkssSvrChgP1MsgC : public PkssUsrGetKeyP1MsgBaseC
{
public:
  PkssSvrChgP1MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_SVR_CHG_P1_MSG_TAG);
  }

}; // End class definition for PkssSvrChgP1MsgC


class PkssSvrChgP2MsgC : public PkssUsrGetKeyP2MsgBaseC
{
public:
  PkssSvrChgP2MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_SVR_CHG_P2_MSG_TAG);
  }

}; // End class definition for PkssSvrChgP2MsgC


class PkssSvrChgP3MsgC : public PkssUsrGetKeyP3MsgBaseC
{
public:
  PkssSvrChgP3MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_SVR_CHG_P3_MSG_TAG);
  }

}; // End class definition for PkssSvrChgP3MsgC


class PkssSvrChgP4MsgC : public PkssUsrGetKeyP4MsgBaseC
{
public:
  PkssSvrChgP4MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_SVR_CHG_P4_MSG_TAG);
  }

}; // End class definition for PkssSvrChgP4MsgC


//-----------------------------------------------------------------------+
//									 |
// class def for PKSS user chg key/pwd phase 5 data (svr generates keys) |
//									 |
//-----------------------------------------------------------------------+

class PkssSvrChgP5DataC : public asn_sequence // H1:H2:newpassword:key-chg?:A:L
{
public:
  asn_octetstring oldH1;	// old password hash H1
  asn_octetstring oldH2;	// old password hash H2
  asn_IA5string newPwd;		// new password
  asn_boolean keyChg;		// Kxu/Kou change flag (y/n)
  asn_octetstring pkAlgID;	// pub key algorithm id (sec_pk_algorithm_id_t)
  asn_integer pkKeyLen;		// length of public key

  PkssSvrChgP5DataC (void):asn_sequence (SECURE)
  {
    register_child (&oldH1);
    register_child (&oldH2);
    register_child (&newPwd);
    register_child (&keyChg);
    register_child (&pkAlgID);
    register_child (&pkKeyLen);
  }

}; // End class definition for PkssSvrChgP5DataC


class PkssSvrChgP5SignedC : public asn_sequence
  // {H1:H2:newpassword:key-chg?:A:L}Kxu
{
public:
  PkssSvrChgP5DataC dataToBeSigned;
  asn_integer sigAlg;	// sec_bsafe_alg_type_t == unsigned32; change to
			// AlgorithmIdentifier algId;
  asn_octetstring signature;

  PkssSvrChgP5SignedC (void):asn_sequence (SECURE)
  {
    register_child (&dataToBeSigned);
    register_child (&sigAlg);
    register_child (&signature);
  }
  
}; // End class definition for PkssSvrChgP5SignedC


//----------------------------------------------------------------------+
//									|
// class def for PKSS user chg key/pwd phase 5 msg (svr generates keys) |
//									|
//----------------------------------------------------------------------+

class PkssSvrChgP5MsgC : public asn_octetstring
{
public:
//  asn_octetstring pkssSvrChgP5Msg;// C-->P: [{H1:H2:newpwd:key-chg?:A:L}Kxu]S

  PkssSvrChgP5MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_SVR_CHG_P5_MSG_TAG);
  }

}; // End class definition for PkssSvrChgP5MsgC


//-----------------------------------------------------------------------+
//								  	 |
// class def for PKSS user chg key/pwd phase 6 data (svr generates keys) |
//									 |
//-----------------------------------------------------------------------+

class PkssSvrChgP6DataC : public asn_sequence
{
public:
  asn_integer status;		// a status code
  asn_integer nc;		// Nc; a nonce generated by client
  asn_integer version;		// V; version of Kxu/Kou pair
  SubjectPublicKeyInfo newKou;	// new public key of user; see X.509

  PkssSvrChgP6DataC (void):asn_sequence (SECURE)
  {
    register_child (&nc);
    register_child (&version);
    register_child (&status);
    register_child (&newKou);
    version.set_optional();
    newKou.set_optional();
  }

}; // End class definition for PkssSvrChgP6DataC


//----------------------------------------------------------------------+
//									|
// class def for PKSS user chg key/pwd phase 6 msg (svr generates keys) |
//									|
//----------------------------------------------------------------------+

class PkssSvrChgP6MsgC : public asn_octetstring
{
public:

  PkssSvrChgP6MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_SVR_CHG_P6_MSG_TAG);
  }

}; // End class definition for PkssSvrChgP6MsgC


//-------------------------------------------------+
//						   |
// PKSS mgmt protocols; PKSS mgmt D-H key exchange |
//						   |
//-------------------------------------------------+

class PkssMgmtP1MsgC : public asn_sequence  // C-->P:  I:E(Dc):algList
{
public:
  asn_integer protoID;		// Protocol ID for mgmt add, del, mod user
  asn_integer eDc;		// exponentiated D-H mgmt i/f client value
  PkssAlgListC algList;		// list of crypto algorithms PKSS clt supports

  PkssMgmtP1MsgC (void)
  {
    register_child (&protoID);
    register_child (&eDc);
    register_child (&algList);
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_MGMT_P1_MSG_TAG);
  }

}; // End class definition for PkssMgmtP1MsgC


class PkssMgmtP2MsgC : public asn_sequence  // P-->C:  E(Dp):H(algList):algIdx
{
public:
  asn_integer eDp;		// exponentiated D-H mgmt i/f PKSS value
  asn_octetstring hAlgs;	// a hash of client's algList
  asn_integer algIdx;	// index into client's supported algorithm list

  PkssMgmtP2MsgC (void)
  {
    register_child (&eDp);
    register_child (&hAlgs);
    register_child (&algIdx);
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_MGMT_P2_MSG_TAG);
  }

}; // End class definition for PkssMgmtP2MsgC


//-----------------------------------------------------------+
//							     |
// class def for PKSS mgmt phase 3 data (clt generates keys) |
//							     |
//-----------------------------------------------------------+

class PkssMgmtCltP3DataC : public asn_sequence
{
public:
  PkssUserKeyC userKey;		// Username (PKSS database key four-tuple)
  asn_octetstring kxuH1;	// [Kxu]H1; private key of user encrypted w/H1
  asn_octetstring H2;		// H2 of new user's password
  SubjectPublicKeyInfo kou;	// public key of user; see X.509

  PkssMgmtCltP3DataC (void):asn_sequence (SECURE)
  {
    register_child (&userKey);
    register_child (&kxuH1);
    register_child (&H2);
    register_child (&kou);
  }

}; // End class definition for PkssMgmtCltP3DataC


//---------------------------------------------------------------+
//								 |
// class def for PKSS mgmt phase 3 msg base (clt generates keys) |
//								 |
//---------------------------------------------------------------+

class PkssMgmtCltP3MsgBaseC : public asn_octetstring
  // C-->P: [Userkey:[Kxu]H1:H2:Kou]D
{

}; // End class definition for PkssMgmtCltP3MsgBaseC


//-------------------------------------------------------------------+
//								     |
// class def for PKSS mgmt add user phase 3 msg (clt generates keys) |
//								     |
//-------------------------------------------------------------------+

class PkssMgmtCltAddP3MsgC : public PkssMgmtCltP3MsgBaseC
{
public:
  PkssMgmtCltAddP3MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_MGMT_CLT_ADD_P3_MSG_TAG);
  }

}; // End class definition for PkssMgmtCltAddP3MsgC


//-----------------------------------------------------------+
//							     |
// class def for PKSS mgmt phase 4 data (clt generates keys) |
//							     |
//-----------------------------------------------------------+

class PkssMgmtCltP4DataC : public asn_sequence
{
public:
  asn_integer version;		// V; version of Kxu/Kou pair
  asn_integer status;		// a status code

  PkssMgmtCltP4DataC (void)
  {
    register_child (&version);
    register_child (&status);
  }

}; // End class definition for PkssMgmtCltP4DataC


//---------------------------------------------------------------+
//								 |
// class def for PKSS mgmt phase 4 msg base (clt generates keys) |
//								 |
//---------------------------------------------------------------+

class PkssMgmtCltP4MsgBaseC : public asn_octetstring // P-->C: [V:Status]D
{

}; // End class definition for PkssMgmtCltP4MsgBaseC


//-------------------------------------------------------------------+
//								     |
// class def for PKSS mgmt add user phase 4 msg (clt generates keys) |
//								     |
//-------------------------------------------------------------------+

class PkssMgmtCltAddP4MsgC : public PkssMgmtCltP4MsgBaseC
{
public:
  PkssMgmtCltAddP4MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_MGMT_CLT_ADD_P4_MSG_TAG);
  }

}; // End class definition for PkssMgmtCltAddP4MsgC


//-----------------------------------------------------------+
//							     |
// class def for PKSS mgmt phase 3 data (svr generates keys) |
//							     |
//-----------------------------------------------------------+

class PkssMgmtSvrP3DataC : public asn_sequence
{
public:
  PkssUserKeyC userKey;		// Username (PKSS database key four-tuple)
  asn_IA5string pwd;		// new user's password
  asn_octetstring pkAlgID;	// pub key algorithm id (sec_pk_algorithm_id_t)
  asn_integer pkKeyLen;		// length of public key

  PkssMgmtSvrP3DataC (void):asn_sequence (SECURE)
  {
    register_child (&userKey);
    register_child (&pwd);
    register_child (&pkAlgID);
    register_child (&pkKeyLen);
  }

}; // End class definition for PkssMgmtSvrP3DataC


//---------------------------------------------------------------+
//								 |
// class def for PKSS mgmt phase 3 msg base (svr generates keys) |
//								 |
//---------------------------------------------------------------+
class PkssMgmtSvrP3MsgBaseC : public asn_octetstring
  // C-->P: [Username:password:A:L]D
{

}; // End class definition for PkssMgmtSvrP3MsgBaseC


//-------------------------------------------------------------------+
//								     |
// class def for PKSS mgmt add user phase 3 msg (svr generates keys) |
//								     |
//-------------------------------------------------------------------+

class PkssMgmtSvrAddP3MsgC : public PkssMgmtSvrP3MsgBaseC
{
public:
  PkssMgmtSvrAddP3MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_MGMT_SVR_ADD_P3_MSG_TAG);
  }

}; // End class definition for PkssMgmtSvrAddP3MsgC


//-----------------------------------------------------------+
//							     |
// class def for PKSS mgmt phase 4 data (svr generates keys) |
//							     |
//-----------------------------------------------------------+

class PkssMgmtSvrP4DataC : public asn_sequence
{
public:
  asn_integer status;		// a status code
  asn_integer version;		// V; version of Kxu/Kou pair
  SubjectPublicKeyInfo kou;	// public key of new user; see X.509

  PkssMgmtSvrP4DataC (void)
  {
    register_child (&status);
    register_child (&version);
    register_child (&kou);
    version.set_optional();
    kou.set_optional();
  }

}; // End class definition for PkssMgmtSvrP4DataC


//---------------------------------------------------------------+
//								 |
// class def for PKSS mgmt phase 4 msg base (svr generates keys) |
//								 |
//---------------------------------------------------------------+

class PkssMgmtSvrP4MsgBaseC : public asn_octetstring
{

}; // End class definition for PkssMgmtSvrP4MsgBaseC


//-------------------------------------------------------------------+
//								     |
// class def for PKSS mgmt add user phase 4 msg (svr generates keys) |
//								     |
//-------------------------------------------------------------------+

class PkssMgmtSvrAddP4MsgC : public PkssMgmtSvrP4MsgBaseC
  // P-->C: [V:Kou:Status]D
{
public:

  PkssMgmtSvrAddP4MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_MGMT_SVR_ADD_P4_MSG_TAG);
  }

}; // End class definition for PkssMgmtSvrAddP4MsgC


//----------------------------------------------+
//					 	|
// class def for PKSS mgmt del user phase 3 msg |
//						|
//----------------------------------------------+

class PkssMgmtDelP3MsgC : public asn_sequence
{
public:

  PkssUserKeyC userKey;

  PkssMgmtDelP3MsgC (void)
  {
    register_child (&userKey);
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_MGMT_DEL_P3_MSG_TAG);
  }

}; // End class definition for PkssMgmtDelP3MsgC

  
//----------------------------------------------+
//						|
// class def for PKSS mgmt del user phase 4 msg |
//						|
//----------------------------------------------+

class PkssMgmtDelP4MsgC : public asn_sequence
{
public:
  asn_integer status;

  PkssMgmtDelP4MsgC (void)
  {
    register_child (&status);
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_MGMT_DEL_P4_MSG_TAG);
  }

}; // End class definition for PkssMgmtDelP4MsgC
  

//-----------------------------------------------+
//						 |
// PKSS mgmt modify user (client generates keys) |
//						 |
//-----------------------------------------------+

class PkssMgmtCltModP3MsgC : public PkssMgmtCltP3MsgBaseC
{
public:
  PkssMgmtCltModP3MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_MGMT_CLT_MOD_P3_MSG_TAG);
  }

}; // End class definition for PkssMgmtCltModP3MsgC


class PkssMgmtCltModP4MsgC : public PkssMgmtCltP4MsgBaseC
{
public:
  PkssMgmtCltModP4MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_MGMT_CLT_MOD_P4_MSG_TAG);
  }

}; // End class definition for PkssMgmtCltModP4MsgC


//-----------------------------------------------+
//						 |
// PKSS mgmt modify user (server generates keys) |
//						 |
//-----------------------------------------------+

class PkssMgmtSvrModP3MsgC : public PkssMgmtSvrP3MsgBaseC
{
public:
  PkssMgmtSvrModP3MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_MGMT_SVR_MOD_P3_MSG_TAG);
  }

}; // End class definition for PkssMgmtSvrModP3MsgC


class PkssMgmtSvrModP4MsgC : public PkssMgmtSvrP4MsgBaseC
{
public:
  PkssMgmtSvrModP4MsgC (void)
  {
    set_tag_class (APPLICATION_CLASS);
    set_tag (PKSS_MGMT_SVR_MOD_P4_MSG_TAG);
  }

}; // End class definition for PkssMgmtSvrModP4MsgC


#endif // SEC_PKSS_ASN_H
