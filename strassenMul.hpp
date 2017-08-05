#ifndef __STRASSENMUL_HPP_INCLUDED__
#define __STRASSENMUL_HPP_INCLUDED__


#include "matrix.hpp"
// #include <boost/thread/scoped_thread.hpp> 
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <functional>

template<class T>
inline matrix<T> strassenMul(matrix<T> A, matrix<T> B, char ch='s', ty smax=30);

template<class T>
void align(matrix<T>& A,matrix<T>& B)
{
	// assert(A.c=B.r);
	try
	{
		if(A.c!=B.r)
		{
			throw std::invalid_argument("Sizes of matrices are inconsistent: first.c!=second.r ");
		}	
		if(A.r%2)
		{
			A.resize(A.r+1,A.c);
		}
		if(B.c%2)
		{
			B.resize(B.r,B.c+1);
		}
		if(A.c%2)
		{
			A.resize(A.r,A.c+1);
			B.resize(B.r+1,B.c);
		}
	}
	catch(std::invalid_argument &ia){std::cerr << ia.what() << "\n";}
}
//
template<class T>
void makeConsistent(matrix<T>& A, matrix<T>& B )
{
	if(A.c!=B.r)
	{
		if(A.c<B.r)
		{
			A.resize(A.r,B.r);
		}
		else if(A.c>B.r)
		{
			B.resize(A.c,B.r);
		}
	}

}
//
template<class T>
matrix<T> inner(const matrix<T>& P,const matrix<T>& Q) // wrapper
{
	return strassenMul(P,Q);
}
//
template<class T>//use align(A,B) previously!
inline matrix<T> strassenMul(matrix<T> A, matrix<T> B, char ch, ty smax) //A.r,A.c,B.r,B.c have to be even; if matrixes sizes < smax==> perform usual mul(A,B);
{

	makeConsistent(A,B); //change A.c or B.r to make A and B to be consistent for the multiplication

	assert(A.r && B.r && A.c && B.c);

	ty AR=A.r; //memorize initial sizes!
	ty BC=B.c;

	if(A.r<smax || A.c<smax || B.c<smax)
	{
		// return mul_ijk(A,B); //dumb
		return mul_kji(A,B); //best
	}

	align(A,B); //make A,B  sizes to be even for Strassen method

	if(ch!='p')//serial computation
	{
		matrix<T>A11(A.M11());
		matrix<T>A12(A.M12());
		matrix<T>A21(A.M21());
		matrix<T>A22(A.M22());

		matrix<T>B11(B.M11());
		matrix<T>B12(B.M12());
		matrix<T>B21(B.M21());
		matrix<T>B22(B.M22());


		matrix<T> S1(B12-B22);
		matrix<T> S2(A11+A12);
		matrix<T> S3(A21+A22);
		matrix<T> S4(B21-B11);
		matrix<T> S5(A11+A22);
		matrix<T> S6(B11+B22);
		matrix<T> S7(A12-A22);
		matrix<T> S8(B21+B22);
		matrix<T> S9(A11-A21);
		matrix<T> S10(B11+B12);

		matrix<T>P1=strassenMul(A11,S1);
		matrix<T>P2=strassenMul(S2,B22);
		matrix<T>P3=strassenMul(S3,B11);
		matrix<T>P4=strassenMul(A22,S4);
		matrix<T>P5=strassenMul(S5,S6);
		matrix<T>P6=strassenMul(S7,S8);
		matrix<T>P7=strassenMul(S9,S10);

		matrix<T>C11(P5+P4-P2+P6);
		matrix<T>C12(P1+P2);
		matrix<T>C21(P3+P4);
		matrix<T>C22(P5+P1-P3-P7);

		ty c11r=C11.r;
		ty c11c=C11.c;
		ty c12r=C12.r;
		ty c12c=C12.c;
		ty c21r=C21.r;
		ty c21c=C21.c;
		ty c22r=C22.r;
		ty c22c=C22.c;

		ty Ro1(c11r+c21r);
		ty Ro2(c12r+c22r);

		ty Co1(c11c+c12c);
		ty Co2(c21c+c22c);


		matrix<T>C( ( Ro1>=Ro2 ? Ro1:Ro2 ), (Co1>=Co2 ? Co1:Co2 ));

		for(ty i=0;i<c11r;++i)
		{
			for (ty j = 0; j < c11c; ++j)
			{
				C(i,j)=C11.get(i,j);
			}
		}

		for(ty i=0;i<c12r;++i)
		{
			for (ty j = 0; j < c12c; ++j)
			{
				C(i,c11c+j)=C12.get(i,j);
			}
		}

		for(ty i=0;i<c21r;++i)
		{
			for (ty j = 0; j < c21c; ++j)
			{
				C(i+c11r,j)=C21.get(i,j);
			}
		}

		for(ty i=0;i<c22r;++i)
		{
			for (ty j = 0; j < c22c; ++j)
			{
				C(c12r+i,c12c+j)=C22.get(i,j);
			}
		}

		C.resize(AR,BC);
		return C;

	}

	else //parallel computation
	{
		static int status=0;

		matrix<T>A11(A.M11());
		matrix<T>A12(A.M12());
		matrix<T>A21(A.M21());
		matrix<T>A22(A.M22());

		matrix<T>B11(B.M11());
		matrix<T>B12(B.M12());
		matrix<T>B21(B.M21());
		matrix<T>B22(B.M22());

		matrix<T> S1(B12-B22);
		matrix<T> S2(A11+A12);
		matrix<T> S3(A21+A22);
		matrix<T> S4(B21-B11);
		matrix<T> S5(A11+A22);
		matrix<T> S6(B11+B22);
		matrix<T> S7(A12-A22);
		matrix<T> S8(B21+B22);
		matrix<T> S9(A11-A21);
		matrix<T> S10(B11+B12);

		if(!status)
		{
			++status; //switch off parallel -mode on the next level

			auto f1=boost::bind(&inner<T>,boost::cref(A11),boost::cref(S1));
			auto f2=boost::bind(&inner<T>,boost::cref(S2),boost::cref(B22));
			auto f3=boost::bind(&inner<T>,boost::cref(S3),boost::cref(B11));
			auto f4=boost::bind(&inner<T>,boost::cref(A22),boost::cref(S4));
			auto f5=boost::bind(&inner<T>,boost::cref(S5),boost::cref(S6));
			auto f6=boost::bind(&inner<T>,boost::cref(S7),boost::cref(S8));
			auto f7=boost::bind(&inner<T>,boost::cref(S9),boost::cref(S10));

			auto p1=boost::async(f1);
			auto p2=boost::async(f2);
			auto p3=boost::async(f3);
			auto p4=boost::async(f4);
			auto p5=boost::async(f5);
			auto p6=boost::async(f6);
			auto p7=boost::async(f7);

			matrix<T>P1(p1.get());
			matrix<T>P2(p2.get());
			matrix<T>P3(p3.get());
			matrix<T>P4(p4.get());
			matrix<T>P5(p5.get());
			matrix<T>P6(p6.get());
			matrix<T>P7(p7.get());

			matrix<T>C11=P5+P4-P2+P6;
			matrix<T>C12=P1+P2;
			matrix<T>C21=P3+P4;
			matrix<T>C22=P5+P1-P3-P7;

			ty c11r=C11.r;
			ty c11c=C11.c;
			ty c12r=C12.r;
			ty c12c=C12.c;
			ty c21r=C21.r;
			ty c21c=C21.c;
			ty c22r=C22.r;
			ty c22c=C22.c;

			ty Ro1(c11r+c21r);
			ty Ro2(c12r+c22r);

			ty Co1(c11c+c12c);
			ty Co2(c21c+c22c);

			matrix<T>C( ( Ro1>=Ro2 ? Ro1:Ro2 ), (Co1>=Co2 ? Co1:Co2 ));

			for(ty i=0;i<c11r;++i)
			{
				for (ty j = 0; j < c11c; ++j)
				{
					C(i,j)=C11.get(i,j);
				}
			}

			for(ty i=0;i<c12r;++i)
			{
				for (ty j = 0; j < c12c; ++j)
				{
					C(i,c11c+j)=C12.get(i,j);
				}
			}

			for(ty i=0;i<c21r;++i)
			{
				for (ty j = 0; j < c21c; ++j)
				{
					C(i+c11r,j)=C21.get(i,j);
				}
			}

			for(ty i=0;i<c22r;++i)
			{
				for (ty j = 0; j < c22c; ++j)
				{
					C(c12r+i,c12c+j)=C22.get(i,j);
				}
			}
			C.resize(AR,BC);
			return C;
		}
		else
		{
			matrix<T>P1=strassenMul(A11,S1);
			matrix<T>P2=strassenMul(S2,B22);
			matrix<T>P3=strassenMul(S3,B11);
			matrix<T>P4=strassenMul(A22,S4);
			matrix<T>P5=strassenMul(S5,S6);
			matrix<T>P6=strassenMul(S7,S8);
			matrix<T>P7=strassenMul(S9,S10);

			matrix<T>C11=P5+P4-P2+P6;
			matrix<T>C12=P1+P2;
			matrix<T>C21=P3+P4;
			matrix<T>C22=P5+P1-P3-P7;

			ty c11r=C11.r;
			ty c11c=C11.c;
			ty c12r=C12.r;
			ty c12c=C12.c;
			ty c21r=C21.r;
			ty c21c=C21.c;
			ty c22r=C22.r;
			ty c22c=C22.c;

			ty Ro1(c11r+c21r);
			ty Ro2(c12r+c22r);

			ty Co1(c11c+c12c);
			ty Co2(c21c+c22c);

			matrix<T>C( ( Ro1>=Ro2 ? Ro1:Ro2 ), (Co1>=Co2 ? Co1:Co2 ));

			for(ty i=0;i<c11r;++i)
			{
				for (ty j = 0; j < c11c; ++j)
				{
					C(i,j)=C11.get(i,j);
				}
			}

			for(ty i=0;i<c12r;++i)
			{
				for (ty j = 0; j < c12c; ++j)
				{
					C(i,c11c+j)=C12.get(i,j);
				}
			}

			for(ty i=0;i<c21r;++i)
			{
				for (ty j = 0; j < c21c; ++j)
				{
					C(i+c11r,j)=C21.get(i,j);
				}
			}

			for(ty i=0;i<c22r;++i)
			{
				for (ty j = 0; j < c22c; ++j)
				{
					C(c12r+i,c12c+j)=C22.get(i,j);
				}
			}

			C.resize(AR,BC);
			return C;
		}
	}
}


#endif