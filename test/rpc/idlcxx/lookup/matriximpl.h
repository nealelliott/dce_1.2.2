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
 * $Log: matriximpl.h,v $
 * Revision 1.1.2.2  1996/03/09  20:48:59  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:27:30  marty]
 *
 * Revision 1.1.2.1  1995/12/11  19:34:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:18:10  root]
 * 
 * $EndLog$
 */
#ifndef matrix_i_h
#define matrix_i_h
#include <iostream.h>
#include <dce/trace.hxx>
#include "matrix.h"

// Local routine that will support display of matrix content
//class Matrix;
inline void print(Matrix *m); 

// Local Implementation of the Matrix
class matrixImpl : public Matrix {
    public: 
	// Constructor for new2x2
	matrixImpl(double v1, double v2, double v3, double v4)
	    {
	    Trace("matrixImpl constructor");
	    d[0][0] = v1;
	    d[0][1] = v2;
	    d[1][0] = v3;
	    d[1][1] = v4;
	    }

	// Constructor for new3x3
	matrixImpl(
		double v11, double v12, double v13,
		double v21, double v22, double v23,
		double v31, double v32, double v33)
	    {
	    Trace("matrixImpl constructor");
	    d[0][0] = v11;
	    d[0][1] = v12;
	    d[0][2] = v13;
	    d[1][0] = v21;
	    d[1][1] = v22;
	    d[1][2] = v23;
	    d[2][0] = v31;
	    d[2][1] = v32;
	    d[2][2] = v33;
	    }

	~matrixImpl(
	  void
	){
	    	Trace("matrixImpl destructor");
		return;
	}

	void IDL_STD_STDCALL set(
	      /* [in] */ idl_long_int row,
	      /* [in] */ idl_long_int col,
	      /* [in] */ idl_long_float value
	  ) {
	    Trace("matrixImpl set");
	    d[row][col] = value;
	    }
	idl_long_float IDL_STD_STDCALL get(
	  /* [in] */ idl_long_int row,
	  /* [in] */ idl_long_int col
	) { 
	        Trace("matrixImpl get");
		return d[row][col];
	}

	Matrix * IDL_STD_STDCALL invert(
	  void
	){
	    Trace("matrixImpl invert");
	return new matrixImpl(d[1][1],d[1][0],d[0][1],d[0][0]);
	}

	Matrix * IDL_STD_STDCALL multiply(
	    Matrix *m
	){
	    	Trace("matrixImpl multiply");

		Matrix *newp = new matrixImpl(0,0,0,0);

		for (int i = 0; i < 2; i++) {
		    for (int j = 0; j < 2; j++) {
			int s = 0;
			for (int k = 0; k < 2; k++) {
			    s += get(i, k) * m->get(k, j);
			}
			newp->set(i, j, s);
		    }
		}
		return newp;
	}
      
	Matrix * IDL_STD_STDCALL add(
	    /* [in] */ /* Type must appear in user header or IDL */ Matrix *m1
	) {
	    Trace("matrixImpl add");
	    return new matrixImpl(
		d[0][0] + m1->get(0, 0),
		d[0][1] + m1->get(0, 1),
		d[1][0] + m1->get(1, 0),
		d[1][1] + m1->get(1, 1));
	}

    void IDL_STD_STDCALL suicide() {
	    Trace("matrixImpl suicide");
	    delete this;
    }
    private:
	double d[2][2];	//  Actual data
};

inline void print(Matrix *m) {
    cout << "    [" << m->get(0,0) << "," << m->get(0,1) << "]" << endl;
    cout << "    [" << m->get(1,0) << "," << m->get(1,1) << "]" << endl;
}

#endif
