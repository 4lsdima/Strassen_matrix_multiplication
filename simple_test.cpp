#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

#include "matrix/matrix.hpp"
#include "matrix/mfunctions.hpp"
// #include "strassenMul.hpp"

using namespace std;


int main(int argc,char**argv)
{
//////////////////////////////////////////////////////
	// make 2 random matrices of doubles:

	matrix<double>A(makeRandMatrix(15, 15, -20, 20)); //size 15x15, values are in semiinterval [-20,20)
	cout<< "A1:"<<endl;
	A.show();
	matrix<double>B(makeRandMatrix(9,9,-100,100));
	matrix<short>Bs=B.toShort();
	cout<< "B1:"<<endl;
	B.show();

	matrix2File(A,"A1.dat"); //write matrix data to file
	matrix2File(B,"B1.dat");


//////////////////////////////////////////////////////

	matrix<int>Am(4,4,'e'); 	// makes  (4 x 4) identity matrix
	matrix<int>Bm(4,4,'o'); 	//makes (4 x 4) matrix of ones
	matrix<int>Dm(4,4); 	//makes (4 x 4) matrix of zeroes 

	Bm*=2;
	Bm(1,2)=25;
	Bm.set(0,3)=58; //does the same

	cout<<Am.get(0,0)<<endl;

	
	
	matrix<int>Bm_t=Bm.transpose();

	Am=Bm; //copy-assignement operator

	
	Am=std::move(Bm); //move assignement operator

	cout<< "Am:\n";
	Am.show();
	cout<< "Bm_t:\n";
	Bm_t.show();

	matrix<int> Gm(Am);
	cout<< "Gm:\n";
	Gm.show();
	matrix<int>Gmt=Am.transpose();
	cout<< "Gmt:\n";
	Gmt.show();

	
	matrix<int>Km=dot(Gm,Gmt);
	cout<< "Gm*Gmt:\n";
	Km.show();




	Am.appendRow(std::vector<int>(Am.r,1));
	Am.appendColumn(std::vector<int>(Am.c,0));
	cout<< "new Am:\n";
	Am.show();
	cout<< "Dm:\n";
	Dm.show();

	matrix<long double>Bm_t_ld=B.toLongDouble();
	cout<< "Bm_t_ld:\n";
	Bm_t_ld.show();





//////////////////////////////////////////////////////
	
	matrix<double>Ad(std::move(file2Matrix("A.dat"))); //read the file and make matrix<double>
	matrix<double>Bd(std::move(file2Matrix("B.dat")));

	matrix<int> A1=Ad.toInt(); //convert matrix<double>Ad to matrix<int>
	matrix<int> B1=Bd.toInt();

	A1.resize(3,2);
	B1.resize(2,4);
	vector<int>v=A1.column(1);
	showVec(v);


	cout<<"\nA1:\n";

	A1.show();
	cout<<"\nB1:\n";
	B1.show();




	cout<<"\n\n\n";
	matrix<int>C=A1-B1;
	C.show();
	cout<<"\ndot()\n";
	C=dot(A1,B1);
	C.show();
	matrix<int>S=C.crop(0,0,1,1);
	S.show();
	C.appendColumn(std::vector<int>({10,20,30}));
	C.show();
	matrix<int>C11(C.M11());
	C11.show();
	matrix<int>C12=C.M12();
	C12.show();
	matrix<int>C21=C.M21();
	C21.show();
	matrix<int>C22=C.M22();
	C22.show();

//////////////////////////////////////////////////////
//////////////*** Permutation matrix: ***/////////////
//////////////////////////////////////////////////////

	std::cout<< "permutation {3,2,4,5}\n";
	std::vector<int> Pv({3,2,4,5}); //vector of the permutation: {2,3,4,5} <==> {3,2,4,5}
	matrix<short>PM=permutMatrix(Pv);
	PM.show();

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
	return 0;
}
