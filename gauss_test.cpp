// g++ "gauss_test.cpp" -o G_test -std=c++11 -lboost_system -lboost_thread -DNDEBUG -O3 -Ofast
//usage: ./G_test A.dat B.dat

#include <iostream>
#include <fstream>

#include <cstdlib>
#include <vector>
#include <string>

#include "matrix/matrix.hpp"
#include "matrix/mfunctions.hpp"
#include "matrix/gauss.hpp"


using namespace std;


int main(int argc,char**argv)
{
//////////////////////////////////////////////////////
	// make a random matrice of doubles:

    ifstream infile_M;
    ifstream infile_V;
    if (argc>2)
    {

      infile_M.open(argv[1]);
      infile_V.open(argv[2]);
      cout<< "A.dat = "<<argv[1]<<endl;
      cout<< "B.dat = "<<argv[2]<<endl;

      if (!infile_M.is_open())
      {
          cout<< "\nfile '"<<argv[1]<<"' NOT FOUND!\nexiting\n";
          return 1;
      }
      if (!infile_V.is_open())
      {
          cout<< "\nfile '"<<argv[2]<<"' NOT FOUND!\nexiting\n";
          return 1;
      }



      cout<< "string A.dat = "<<string(argv[1])<<endl;
      cout<< "string B.dat = "<<string(argv[2])<<endl;

      matrix<double>A=file2Matrix(string(argv[1]));
      A.show();
      cout<<"A.r= "<< A.r<< " A.c= "<<A.c<<endl;


      matrix<double>B=file2Matrix(string(argv[2]));
      B.show();
      // // B.resize(A.r,1);
      // cout<< "A:"<<endl;
      
      std::vector<double> b=B.column(0);
      // std::vector<double> b({0,2,2});

      // cout<< "b:"<<endl;
      showVec(b);

      std::cout<< "//solution of Ax=b:\n";

      gSystem<double> G(A,b);

    }

    else
    {

  		matrix<double>A(makeRandMatrix(5, 5, -200, 200)); //size 5x5, values are in semiinterval [-200,200)
  		vector<double> b({5,4,3,1,2});
      cout<< "A:"<<endl;
      A.show();

      // leftside vector:
      showVec(b);

      //solution of Ax=b:
      gSystem<double> G(A,b);
    }

	return 0;
}
