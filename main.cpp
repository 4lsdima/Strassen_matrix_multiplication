#include <chrono>
#include <thread>
#include <ctime>
#include "strassenMul.hpp"

using namespace std;

void speed_test_int()
{
	using namespace std;

	matrix<double>Ad(file2Matrix("A.dat"));
	matrix<double>Bd(file2Matrix("B.dat"));

	matrix<int> A(Ad.toInt()); 
	matrix<int> B(Bd.toInt());


	makeConsistent(A,B);

	cout<<"\n\n\n";

	cout<< "A.r= "<<A.r<<endl;
	cout<< "A.c= "<<A.c<<endl;
	cout<< "B.r= "<<B.r<<endl;
	cout<< "B.c= "<<B.c<<endl;

	
	clock_t t;
	

	std::chrono::time_point<std::chrono::steady_clock> start, end;
	
	start = std::chrono::steady_clock::now();
	t=clock();

	matrix<int> Sp=strassenMul(A,B,'p'); // parallel Strassen (7 threads)

	end = std::chrono::steady_clock::now();
	t=clock()-t;
    ty ms_Sp= std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    ty tics_Sp = t;

	std::this_thread::sleep_for(std::chrono::seconds(5));

	start = std::chrono::steady_clock::now();
	t=clock();

	matrix<int> S_mul=mul_ijk(A,B); //straightforvard product for the comparison with Strassen

	end = std::chrono::steady_clock::now();
	t=clock()-t;
	ty ms_mul= std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
	ty tics_mul=t;

	start = std::chrono::steady_clock::now();

	cout<<"\n\n\n";

	std::cout<< " Speed test for Strassen parallel:             "<< ms_Sp<<" ms,  "<< tics_Sp<<" tics\n";
    std::cout<< " Speed test for straightforward_ijk:           "<< ms_mul<<" ms,  "<< tics_mul<<" tics\n";

	cout<< "PROVERKA:\n";

	if(Sp==S_mul)
	{
		cout << "results of Parallel Strassen and mul product are identical\n";
	}
	else
	{
		cout << "results of Parallel Strassen and mul product differ!\n Something went wrong!\n";	
	}
}

void speed_test_double()
{
	using namespace std;

	matrix<double>A(std::move(file2Matrix("A.dat"))); //read the file and make matrix<double>
	matrix<double>B(std::move(file2Matrix("B.dat")));


	makeConsistent(A,B);

	cout<<"\n\n\n";


	cout<< "A.r= "<<A.r<<endl;
	cout<< "A.c= "<<A.c<<endl;
	cout<< "B.r= "<<B.r<<endl;
	cout<< "B.c= "<<B.c<<endl;

	clock_t t;

	std::chrono::time_point<std::chrono::steady_clock> start, end;
	
	start = std::chrono::steady_clock::now();
	t=clock();

	matrix<double> Sp=strassenMul(A,B,'p'); // parallel Strassen (7 threads)

	end = std::chrono::steady_clock::now();
	t=clock()-t;
    ty ms_Sp= std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    ty tics_Sp=t;

	std::this_thread::sleep_for(std::chrono::seconds(5));

	start = std::chrono::steady_clock::now();
	t=clock();
	matrix<double> S_mul=mul_ijk(A,B); //straightforvard product for the comparison with Strassen

	end = std::chrono::steady_clock::now();
	t=clock()-t;
	ty ms_mul= std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
	ty tics_mul=t;

	cout<<"\n\n\n";
	cout<< "test on matrix<double>:\n\n";

    cout<< " Speed test for Strassen parallel:             "<< ms_Sp<<" ms,  "<< tics_Sp<<" tics\n";
    cout<< " Speed test for straightforward_ijk:           "<< ms_mul<<" ms,  "<< tics_mul<<" tics\n";

	cout<< "PROVERKA:\n";

	if(Sp==S_mul)
	{
		cout << "results of Parallel Strassen and mul product are identical\n";
	}
	else
	{
		cout << "results of Parallel Strassen and mul product differ!\n Something went wrong!\n";	
	}
}
//

int main(int argc,char**argv)
{
//////////////////////////////////////////////////////
	// make 2 random matrices of doubles:
/*
	matrix<double>A(makeRandMatrix(1500, 1500, -100, 100)); //size 150x150, values are in semiinterval [-100,100)
	// cout<< "A:"<<endl;
	// A.show();
	matrix<double>B(makeRandMatrix(1500,1500,-100,100));
	// cout<< "B:"<<endl;
	// B.show();

	matrix2File(A,"A.dat"); //write matrix data to file
	matrix2File(B,"B.dat");

*/
//////////////////////////////////////////////////////

	speed_test_int(); // run speed-test!
	// speed_test_double(); 

//////////////////////////////////////////////////////
/*
	matrix<int>Am(4,4,'e'); 	// makes  (4 x 4) identity matrix
	matrix<int>Bm(4,4,'o'); 	//makes (4 x 4) matrix of ones
	matrix<int>Dm(4,4); 	//makes (4 x 4) matrix of zeroes 

	Bm*=2;
	Bm(1,2)=25;
	Bm.set(0,3)=58; //does the same

	cout<<Am.get(0,0)<<endl;

	Am.show();
	Bm.show();
	matrix<int>Bm_t=Bm.transpose();
	Bm_t.show();

	// Am=Bm; //copy-assignement operator

	Am=std::move(Bm); //move assignement operator

	Am.appendRow(std::vector<int>(Am.r,1));
	Am.appendColumn(std::vector<int>(Am.c,0));
	// Am.show();

*/

//////////////////////////////////////////////////////
/*	
	matrix<double>Ad(std::move(file2Matrix("A.dat"))); //read the file and make matrix<double>
	matrix<double>Bd(std::move(file2Matrix("B.dat")));

	matrix<int> A=Ad.toInt(); //convert matrix<double>Ad to matrix<int>
	matrix<int> B=Bd.toInt();

	A.resize(3,2);
	B.resize(2,4);
	vector<int>v=A.column(1);
	showVec(v);

	A.show();
	B.show();
*/


/*
	cout<<"\n\n\n";
	matrix<int>C=A-B;
	C.show();
	C=mul(A,B);
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
*/
//////////////////////////////////////////////////////
//////////////*** Permutation matrix: ***/////////////
//////////////////////////////////////////////////////
/*

	std::vector<int> Pv({3,2,4,5}); //vector of the permutation: {2,3,4,5} <==> {3,2,4,5}
	matrix<short>PM=permutMatrix(Pv);
	PM.show();
*/
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
	return 0;
}
