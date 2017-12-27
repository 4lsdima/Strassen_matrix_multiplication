/////////////////////////////***  Utilites ***//////////////////////////////////////////////////
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>

#include "./matrix/matrix.hpp"
#include "./matrix/mfunctions.hpp"
#include "./matrix/strassenMul.hpp"

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

	matrix<int> S_mul=dot(A,B); //straightforvard product for the comparison with Strassen

	end = std::chrono::steady_clock::now();
	t=clock()-t;
	ty ms_mul= std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
	ty tics_mul=t;

	start = std::chrono::steady_clock::now();

	cout<<"\n\n\n";

	std::cout<< " Speed test for Strassen parallel:             "<< ms_Sp<<" ms,  "<< tics_Sp<<" tics\n";
    std::cout<< " Speed test for straightforward dot:           "<< ms_mul<<" ms,  "<< tics_mul<<" tics\n";

	cout<< "PROVERKA:\n";

	if(Sp==S_mul)
	{
		cout << "results of Parallel Strassen and straightforward dot product are identical\n";
	}
	else
	{
		cout << "results of Parallel Strassen and straightforward dot product differ!\n Something went wrong!\n";	
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
	matrix<double> S_mul=dot(A,B); //straightforvard product for the comparison with Strassen

	end = std::chrono::steady_clock::now();
	t=clock()-t;
	ty ms_mul= std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
	ty tics_mul=t;

	cout<<"\n\n\n";
	cout<< "test on matrix<double>:\n\n";

    cout<< " Speed test for Strassen parallel:             "<< ms_Sp<<" ms,  "<< tics_Sp<<" tics\n";
    cout<< " Speed test for straightforward_dot:           "<< ms_mul<<" ms,  "<< tics_mul<<" tics\n";

	// matrix2File(Sp,"S_p_d.txt");
    // matrix2File(S_mul,"S_mul_d.txt");

	cout<< "PROVERKA:\n";

	if(Sp==S_mul)
	{
		cout << "results of Parallel Strassen and straightforward dot product are identical\n";
	}
	else
	{
		cout << "results of Parallel Strassen and straightforward dot product differ!\n Something went wrong!\n";	
	}
}

int main(int argc, char const *argv[])
{
	speed_test_int();
	speed_test_double();
	
	return 0;
}
