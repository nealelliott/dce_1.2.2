// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
//
// HISTORY
// $Log: String.h,v $
// Revision 1.1.2.1  1996/09/09  16:30:06  bfc
// 	Finish TETizing and cleanup
// 	[1996/09/09  16:26:51  bfc]
//
// $EndLog$
//

// A String class to be used when the machine's native C++ compiler
// doesn't have one.  To use, copy into the appropriate test subdirectory
// (only card and native use String at this point) and build

#ifndef __STRING_HXX
#define __STRING_HXX

#ifdef AIX32
// weird necessity for AIX
#define __strcpy strcpy
#define __strlen strlen
#define __strcat strcat
#endif

#include <iostream.h>

extern "C"
{
#include <string.h>
#include <ctype.h>
}

class String

{
private:
    char 		*cxxl_p;	// Pointer to String representation

public:
			inline String() : cxxl_p(0) {
			}
			inline String(const String &s) {
			    cxxl_p = new char [s.length() + 1];
			    strcpy(cxxl_p, s.cxxl_p);
			}
			inline String(const char *s) {
			    cxxl_p = new char [strlen(s) + 1];
			    strcpy(cxxl_p, s);
			}
			inline String(char *s) {
			    cxxl_p = new char [strlen(s) + 1];
			    strcpy(cxxl_p, s);
			}
			inline ~String() {
				if (cxxl_p) delete cxxl_p;
			}


    // Conversions
    inline operator char *() const;
    inline operator const char *() const;

    // Operators
    friend inline ostream	&operator<<(ostream &, const String &);
    inline String                      &operator=(char * s) {
	cxxl_p = new char [strlen(s) + 1];
	strcpy(cxxl_p,s);
	return *this;
    }
    inline String                      &operator+=(const String &s) {
    	char *t;
	if (cxxl_p) {
		t = new char [strlen(cxxl_p) + s.length() + 1];
		strcpy(t, cxxl_p);
		strcat(t, (char *)s);
	} else {
		t = new char [s.length() + 1];
		strcpy(t, (char *)s);
	}
	cxxl_p = t;
	return *this;
    }

    inline unsigned int length() const;
    inline String              upper();
    inline String              lower();
};

inline String::operator char *(
    ) const

{
    return cxxl_p;
}

inline String::operator const char *(
    ) const
{
    return (const char *)(cxxl_p);
}
inline ostream &operator<<(
    ostream	 &os,  // Reference to the ostream object.
    const String &str  // Reference to the String object.
    )
{
    return os << str.cxxl_p;
}

inline unsigned int String::length(
    ) const
{
    	return strlen(cxxl_p);
}

inline String String::upper(
) {
	String *s;
	int len = this->length();

	s = new String((char *) this->cxxl_p);
	for (int i = 0; i < len; i++)
		if (islower((int)(s->cxxl_p[i])))
			s->cxxl_p[i] = toupper((int) (s->cxxl_p[i]));
	return *s;
}

inline String String::lower(
) {
	String *s;
	s = new String((char *) this->cxxl_p);
	for (int i = 0; i < s->length(); i++)
		if (isupper((int)(s->cxxl_p[i])))
			s->cxxl_p[i] = tolower((int) (s->cxxl_p[i]));
	return *s;
}



#endif
// End of String.h

