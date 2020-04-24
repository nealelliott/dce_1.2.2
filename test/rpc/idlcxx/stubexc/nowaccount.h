/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: nowaccount.h,v $
 * Revision 1.1.2.2  1996/03/09  20:50:16  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:25  marty]
 *
 * Revision 1.1.2.1  1995/12/11  19:36:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:19:01  root]
 * 
 * $EndLog$
 */
#ifndef nowaccount_i_h
#define nowaccount_i_h
#include <iostream.h>
#include <dce/trace.hxx>
#include "savings.h"
#include "checking.h"

extern idl_long_int rpc_ss_get_num_ot_entries();

class nowAccount : public Savings , public Checking {
    public: 

	nowAccount(double amt)
	{
	    Trace("nowAccount constructor");
	    balance = amt;
	}

	~nowAccount(void)
	{
	    	Trace("nowAccount destructor");
		return;
	}

	double IDL_STD_STDCALL getBalance()
	{
	    Trace("nowAccount getBalance");
	    return balance;
	}

	void IDL_STD_STDCALL credit(
		double amt
	) { 
	        Trace("nowAccount credit");
		balance += amt;
	}

	void IDL_STD_STDCALL setRate(double r)
	{
		Trace("nowAccount setRate");
		rate = r;
	}

	void IDL_STD_STDCALL update()
	{
	    	Trace("nowAccount update");
		balance += (balance * rate);
	}

	double IDL_STD_STDCALL withDraw(double amt)
	{
	    	Trace("nowAccount withDraw");
		balance -= amt;
		return amt;
	}

	double IDL_STD_STDCALL getAccountBalance()
	{
	    	Trace("nowAccount getAccountBalance");
		return getBalance();
	}
      
	double IDL_STD_STDCALL getCheckingBalance()
	{
	    	Trace("nowAccount getCheckingBalance");
		return getBalance();
	}
      
	double IDL_STD_STDCALL getSavingsBalance()
	{
	    	Trace("nowAccount getSavingsBalance");
		return getBalance();
	}
      
    private:
	double balance;
	double rate;
};

class oldAccount : public Savings {
    public: 

	oldAccount(double amt)
	{
	    Trace("oldAccount constructor");
	    balance = amt;
	}

	~oldAccount(void)
	{
	    	Trace("oldAccount destructor");
		return;
	}

	double IDL_STD_STDCALL getBalance()
	{
	    Trace("oldAccount getBalance");
	    return balance;
	}

	void IDL_STD_STDCALL credit(
		double amt
	) { 
	        Trace("oldAccount credit");
		balance += amt;
	}

	void IDL_STD_STDCALL setRate(double r)
	{
		Trace("oldAccount setRate");
		rate = r;
	}

	void IDL_STD_STDCALL update()
	{
	    	Trace("oldAccount update");
		balance += (balance * rate);
	}

	double IDL_STD_STDCALL getAccountBalance()
	{
	    	Trace("oldAccount getAccountBalance");
		return getBalance();
	}
      
	double IDL_STD_STDCALL getSavingsBalance()
	{
	    	Trace("oldAccount getSavingsBalance");
		return getBalance();
	}
      
    private:
	double balance;
	double rate;
};

#endif
