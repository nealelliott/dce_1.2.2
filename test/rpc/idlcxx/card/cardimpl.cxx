// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: cardimpl.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:48:28  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:07  marty]
//
// Revision 1.1.2.1  1995/12/11  19:33:45  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:17:48  root]
// 
// $EndLog$
#include "card.h"

#ifdef _WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif /* _WIN32 */

typedef struct {
	char *lines[20];
} Card;

static Card cardTemplate = {
			"+------------------------+\n",
			"|                        |\n",
			"|                        |\n",
			"| Name:                  |\n",
			"|                        |\n",
			"|                        |\n",
			"|                        |\n",
			"| Statistics:            |\n",
			"|                        |\n",
			"|                        |\n",
			"|                        |\n",
			"|                        |\n",
			"|                        |\n",
			"|                        |\n",
			"|                        |\n",
			"|                        |\n",
			"|                        |\n",
			"|                        |\n",
			"|                        |\n",
			"+------------------------+\n"
};

void ICard::newCard(
	IPlayer *v,
	String *s
)
{
	Card *c;

	char *tmp1;
	char *tmp2;
	int x;
	c = new Card();

	for (int i = 0; i < 20; i++)
		(*c).lines[i] = cardTemplate.lines[i];

	tmp1 = new char[28];
	memset(tmp1, ' ', 27);
	ostrstream ost(tmp1, 25);
	ost << "| " << v->getFirstName() << ' ' << v->getLastName();
	//tmp1[strlen(tmp1)] = ' ';
	tmp1[25] = '|';
	tmp1[26] = '\n';
	tmp1[27] = '\0';


	tmp2 = new char[28];
	memset(tmp2, ' ', 27);
	ostrstream ost2(tmp2, 25);
	ost2 << "| " << v->getStatTitle() << ' ' << v->getStat();
	//tmp2[strlen(tmp2)] = ' ';
	tmp2[25] = '|';
	tmp2[26] = '\n';
	tmp2[27] = '\0';


	(*c).lines[4] = tmp1;
	(*c).lines[8] = tmp2;

	for (i = 0; i < 20; i++) {
		(*s) += (*c).lines[i];
	}
}
