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
 * $Log: player.h,v $
 * Revision 1.1.4.2  1996/09/10  18:21:01  bfc
 * 	finish TETizing and cleanup
 * 	[1996/09/10  18:15:24  bfc]
 *
 * Revision 1.1.4.1  1996/09/09  20:58:55  bfc
 * 	finish TETizing and cleanup
 * 	[1996/09/09  20:41:24  bfc]
 * 
 * Revision 1.1.2.3  1996/03/09  20:48:37  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:27:13  marty]
 * 
 * Revision 1.1.2.2  1996/02/23  19:22:38  root
 * 	OT 13357
 * 	[1996/02/23  19:19:12  root]
 * 
 * Revision 1.1.2.1  1995/12/11  19:33:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:17:53  root]
 * 
 * $EndLog$
 */

#ifndef player_included
#define player_included

#ifdef AIX32
// weird necessity for AIX
#define __strcpy strcpy
#define __strlen strlen
#define __strcat strcat
#endif

#include <iostream.h>

////////////////////////////////////////////////////////////////////////////////
//
// A generic sports player class definition
//

class Player {

private:
	char *	firstName;		// a players first name
	char *	lastName;		// a players last name

public:
	// default constructor
	Player() {};

	// another constructor
	Player(char * fn, char *ln) 
	{
		firstName = new char[strlen(fn) + 1];
		strcpy(firstName, fn);

		lastName = new char[strlen(ln) + 1];
		strcpy(lastName, ln);
	};

	// copy constructor
	Player(const Player &p) {
		firstName = new char[strlen(p.getFirstName()) + 1];
		strcpy(firstName, p.getFirstName());

		lastName = new char[strlen(p.getLastName()) + 1];
		strcpy(lastName, p.getLastName());
	}

	// assignment operator
	Player&
	operator=(const Player &p) {
		if (firstName) delete [] firstName;
		firstName = new char[strlen(p.getFirstName()) + 1];
		strcpy(firstName, p.getFirstName());

		if (lastName) delete [] lastName;
		lastName = new char[strlen(p.getLastName()) + 1];
		strcpy(lastName, p.getLastName());

		return *this;
	}

	// destructor
	~Player() {
		if (firstName) delete [] firstName;
		if (lastName)  delete [] lastName;
	}

	virtual char * getFirstName() const { return firstName; }
	virtual char * getLastName() const { return lastName; }
	virtual char * getStatTitle() const = 0;
	virtual double getStat() const = 0;

	friend ostream &
	operator<<(ostream & os, const Player & c) {
		return os << "Player: " << c.getFirstName() << ' ' 
			  << c.getLastName() << ' '
			  << ' ' << c.getStatTitle() << ": " 
			  << c.getStat();
	}
};

////////////////////////////////////////////////////////////////////////////////
//
// A baseball player class definition
//

class BaseballPlayer : public virtual Player {

private:
	static char * statTitle;
	double	average;
public:
	// default constuctor
	BaseballPlayer() {}

	// another constructor
	BaseballPlayer(char * fn, char *ln, double ba) 
	    	: Player(fn, ln), average(ba) {}

	virtual char *getStatTitle() const { return statTitle; }
	virtual double getStat() const { return average; }
};

////////////////////////////////////////////////////////////////////////////////
//
// A basketball player class definition
//

class BasketballPlayer : public Player {

private:
	static char * statTitle;
	double	percentage;
public:
	// default constructor
	BasketballPlayer();

	// another constructor
	BasketballPlayer(char * fn, char *ln, double shoot)
	    	: Player(fn, ln) , percentage(shoot) {}

	virtual char * getStatTitle() const { return statTitle; }
	virtual double getStat() const { return percentage; }
};

#endif
