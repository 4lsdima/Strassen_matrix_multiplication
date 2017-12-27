#ifndef __GAUSS_HPP_INCLUDED__
#define __GAUSS_HPP_INCLUDED__

#include <stdexcept>
#include <cmath>
#include <cassert>

#include "matrix.hpp"


typedef std::size_t ty;
// typedef double ld;

#define eps (ld)1.0e-014 //to compare double values with


// Universal exceptional handler:
void handle_except(std::exception_ptr eptr) // passing by value is ok
{
    try
    {
        if(eptr) {std::rethrow_exception(eptr);}
    }
    catch(const std::exception& e)
    {
        std::cout << "Caught exception \"" << e.what() << "\"\n";
    }
}


template<class ld>
class gSystem
{
public:
	matrix<ld> Matrix;
	

	ld Det=ld(0);
	ty Rg=ty(0);
	
	// gSystem(){
	// 	std::cout<<"Default_ctr 'gSystem()'\n";
	// }
	// gSystem()=delete; 
	explicit gSystem( matrix<ld> A , std::vector<ld> b)// constructor
	{
		
		Matrix=std::move(A);
		// std::cout<< "1)\n";

		Matrix.show();

		// std::cout<< "2)\n";
		Matrix.appendColumn(std::move(b));
		
		Matrix.show();
		// std::cout<< "3)\n";
		// try
		// {
		  	printf("\nRow-EchelonForm:\n");
		  	this->rowEchelonForm();
		  	Matrix.show();

		  	printf("Rg= %lu\n",Rg);
	  	    printf("\nApprovedForm:\n");
	  	    this->approvedForm();
	  	     printf("\nCalc Det:\n");
	  	    this->calculateDET();
	        printf("Determinant = %4.4f\n", Det);
	        this->swapBack();
		  	Matrix.show();
		  	int gs=this->generalSolution();
		  	// printf("\n\n General Solution case = %d\n",gs );

	    // }
		// catch(...)  
  //       {  
  //           auto eptr = std::current_exception();  
  //           handle_except(eptr);
  //       } 

	}
	~gSystem(){}//destructor

private:
	std::vector<ld> mults;
	
	std::vector<std::pair<ty,ty>> swapped_cols;// for columns permutation's history

	int sign;// for overall signature


	void swap_rows(ty ind1, ty ind2)
	{
		ld tmp;
		for(ty i=0; i< Matrix.r;++i)
		{
			tmp=Matrix(ind1,i);
			Matrix(ind1,i)=Matrix(ind2,i);
			Matrix(ind2,i)=tmp;
		}
	    printf("Swapped rows: {%d, %d}\n",(int)ind1,(int)ind2);
	    sign*=-1; //each swap_rows affects the signature
	}

	void swap_columns(ty ind1, ty ind2)
	{
		ld tmp;
		for(ty j=0; j<Matrix.c ; ++j)
		{
			tmp=Matrix(j,ind1);
			Matrix(j,ind1)=Matrix(j,ind2);
			Matrix(j,ind2)=tmp;
		}
		swapped_cols.push_back(std::pair<ty,ty>(ind1,ind2));
    	printf("Swapped columns: {%d, %d}\n",(int)ind1,(int)ind2);
	    sign*=-1; // each swap_columns affects the signature
	
	}

	std::pair<bool, ty> nz_row_check(ty ind)//looks for a row (since ind+1 th) 'i' with 
	// non-zero element at column(ind), 
	//if found, returns (1,i), else (0,0); (first value is 'flag')
	{
		
	    for(ty i=ind+1;i<Matrix.r;++i)
	    {
	        if(fabs(Matrix(i,ind)>eps))
	        {
	            return std::pair<bool,ty>(1,i);
	        }
	    }
	    return std::pair<bool,ty>(0,0);//if not found

	}
	std::pair<bool,ty> nz_col_check(ty ind)//looking for a column with non-zero elemnts
	 // in a lower-right quadrant from the element Matrix(ind,ind)
	// if found, returns (1,j), else (0,0); (first value is 'flag')
	{
    
	    for(ty i=ind+1;i<Matrix.r;++i)///? maybe i=ind+1 ?
	    {
	        for(ty j=ind+1;j<Matrix.c-1;++j) 
	        {
	            if(fabs(Matrix(i,j))>eps)
	            {
	                return std::pair<bool,ty>(1,j);
	            }
	        }
	    }
	    return std::pair<bool,ty>(0,0);//if not found
	}


    bool isZeroColumnTail(ty row_ind, ty column_ind)
     //check if the rest(from row(row_ind)) of the column(column_ind), is all of zeroes
	{
	    for(ty j=row_ind+1;j<Matrix.r;++j)
	    {
	        if(fabs(Matrix(j,column_ind))>eps)
	        {
	            return 0;
	        }
	    }
    	return 1;
	}
    
    void row_routine(ty ind)//substracts given row(ind) from rows below,
    // to make Matrix(ind+1,...)=0,...Matrix(Matrix.r-1,...)=0
	{
	    ld mu=Matrix(ind,ind);
	    if(fabs(mu)<eps)
	    {
	    	// throw "row_routine(...): zero division ";
	    	return;
	    }
	    mults.push_back(mu); //append value Matrix(ind,ind) to array mults, for further Det calculations
	    
	    for(ty j=ind;j<Matrix.c;++j)
	    {
	        Matrix(ind,j)/=mu; //divide all elements in row(ind) by mu;
	    }

	    for (ty i=ind+1;i<Matrix.r;++i) //substract mul*row(ind) from all rows below it, mul= Matrix(i,ind)
	    {
	        ld mul=Matrix(i,ind);
	        for(ty j=ind;j<Matrix.c;++j)
	        {
	        	// std::cout<< Matrix(i,j)<<" -= "<< mul <<"*"<<Matrix(ind,j) <<std::endl;
	            Matrix(i,j)-=Matrix(ind,j)*mul;
	        }

	        // Matrix.show();
	    }
	}



	void rowEchelonForm()//converts Matrix to upper-triangleForm, with diagonal elements =1
	{
	    // POEHALE: from the begining (element Matrix(0,0) ) 
	    
	    for(ty i=0;i<Matrix.r;++i)
	    {
	        if(fabs(Matrix(i,i))<eps) //if given diagonal element is zero,then:
	        {
	            // printf("given diagonal element is zero\n" );
	            if(isZeroColumnTail(i,i))//check whether the rest of this column consists of  zeroes
	            {
	                // if TRUE: need to swap columns:
	                // printf(" TRUE: need to swap columns:\n");
	                std::pair<bool,ty> pc=nz_col_check(i);//looking for a column with 
	                //non-zero elemnts in a lower-right quadrant from the element (ind,ind)
	              	// pc.first is flag, pc.second is alternative column index
                    if(pc.first)//non zero column found: there is an opportunity to swap with it:
                    {
                        // printf("there is an opportunity to swap with another column:\n");
                        // printf("apply column swapping\n");
                        swap_columns(i,pc.second);//apply column swapping
                    }
	            }
	            else //no need in swap_columns, there is a posibility to just swap rows:
	            {
	                // printf("no need in swap_columns, there is a posibility to just swap rows:\n");
	                std::pair<bool,ty> pr=nz_row_check(i);
                    if(pr.first)//non-zero row found
                    {
                        // printf("non-zero row found\n");
                        // printf("apply swap_rows:\n");
                        swap_rows(i,pr.second);
                    }
	            }

	        }
	        // printf("row_routine\n");
	        row_routine(i); //execute row_routine:
	    }
	    
	    for (ty i = 0; i < std::min(Matrix.r,Matrix.c-1); ++i) // Rg calculation:
	    {
	        if(fabs(Matrix(i,i))>eps)
	        {
	            this->Rg++;
	        }
	    }
	}
	void approvedForm()//converts Matrix to diagonal form (Approved Stepwise Form)
	{
	    ld mul;
	    for (ty k = 1;k<this->Rg; ++k)
	    {
	        for(ty i=0;i<k;++i)
	        {
	            mul=Matrix(i,k);
	            for(ty j=k;j<Matrix.c;++j)
	            {
	                Matrix(i,j)-=mul*(Matrix(k,j));
	            }
	        }
	    }
	}

	void swapBack() // if swapping the columns took place, we need to 
	//swap back values in rightmost column, in accordance with list m->swapped_cols
	{
		// ty sw_len=swapped_cols.size();
		ty n=Matrix.c-1;
	    if(swapped_cols.size()) // if swap_columns took place 
	    {
	    	ld tmp;
	    	for (auto it = swapped_cols.rbegin(); it != swapped_cols.rend(); ++it)
	    	{
	    		tmp=Matrix(it->first,n);
	    		Matrix(it->first,n)=Matrix(it->second,n);
	    		Matrix(it->second,n)=tmp;
	    	}
		}
	}
	void calculateDET()
	{
	    if(Rg < Matrix.r) //if Rg<num_of_rows => det =0
	    {
	        Det=ld(0);
	        return;
	    }
	    Det=ld(1);
	    for(auto& it:mults)
	    {
	    	Det*=it;
	    }
	    Det*=sign;
	}

	int generalSolution()
	{
	    printf("\n\n================================ SOLUTION: ================================\n" );
	    ty n=Matrix.c-1;
	    if(Rg==Matrix.r) // case of determined system:
	    {
	        printf("\nSystem is determined\n");
	        printf(" Unique solution is:\n\n");
	        for(ty i=0;i<Rg;++i)
	        {
	            printf("%10.4f\n",Matrix(i,n));
	        }   
	        printf("\n\n");
	        return 0;
	    }
	    else
	    {   
	        //check if the case of non zeroes in rightmost column at positions:
	        // Rg, Rg+1,....M-1 (system has no solutions)
	        for(ty i=Rg;i<Matrix.r;++i) 
	        {
	            if(fabs(Matrix(i,n))>eps)
	            {
	                printf("\nSystem has no solutions!\n\n");
	                return 1;
	            }
	        }
	        //case of general solution for undetermined system:
	        matrix<ld> vv;//for fundamental system:

	        ty dim=n-Rg; // the dimensionality of the fundamental system
	        if (dim>0)
	        {
	            printf("\nThere are %d lineary independent specific solutions for homogenious part, to form the\n\n Fundamental System:\n",(int)dim);
	            vv=matrix<ld> (n,dim); 
	            // now populate the bottom of vv with 0 and 1 in case of i-Rg==j:
	            //
	            //      *   *   *
	            //      *   *   *
	            //      1   0   0
	            //      0   1   0
	            //      0   0   1
	            //
	            for (ty i = Rg; i < n; ++i)
	            {
	                for(ty j=0;j<dim;++j)
	                { 
	                	vv(i,j)= (i-Rg==j ? ld(1): ld(0) );
	                }
	            }

	            ld sum;
	            for(ty j=0;j<dim;++j)//calulate the upper part of the fundamental system in accordance with Matrix
	            {
	                for(ty i=0;i<Rg;++i)
	                {
	                    sum=ld(0);
	                    for(ty k=Rg;k<n;++k)
	                    {
	                        sum+=vv(k,j)*(-1)*Matrix(i,k);
	                    }
	                    vv(i,j)=sum;
	                }
	            }    
	            vv.show(); //prints out our fundamental system

	            const char* cs1=((dim>2) ? "...+" : ""); //just for good-looking printout: if dim>2 --> cs1="...+", else cs1=""
	            const char* cs2=((dim>2) ? "..." : ","); //just for good-looking printoutif dim>2 --> cs2="...", else cs1=","
	            if(isZeroColumnTail(ty(0), n)) // if rightmost column contains only zeroes, just print these messages:
	            {
	                printf("\n\nThus, General Solution would be:\n\n\tC1*V1 +%s C%d*V%d\n\n",cs1,(int)dim,(int)dim);
	                printf("\t..where V1%sV%d - are the columns of the fundamental system\n\t  and C1%sC%d -are some constants\n\n",cs2,(int)dim,cs2,(int)dim);
	                
	            }
	            else //if rightmost column contains non-zero elements, the Specific non-homogenious solution is equal to that rightmost column
	            {
	                printf("\n\nThus, General Solution would be:\n\n\tC1*V1 +%s C%d*V%d + Vno\n\n",cs1,(int)dim,(int)dim);
	                printf("\t..where V1%sV%d - are the columns of the fundamental system\n\t  and C1%sC%d -are some constants\n",cs2,(int)dim,cs2,(int)dim);
	                printf("\t  and Vno -is the specific non-homogenious solution:\n\n");

	                for(ty i=0;i<Matrix.r;++i) //printout Vno -specific non-homogenious solution:
	                {
	                    printf("\t%10.4f\n",Matrix(i,n) );

	                }
	                for(ty i=Matrix.r;i<n;++i)
	                {
	                    printf("\t%10.4f\n",(double)0 );
	                }
	                printf("\n\n");
	            }
	            return 2;
	        }
	        else if(dim==0)
	        {
	        	std::vector<ld>la (Matrix.column(Matrix.c-1));

	        	std::vector<ld> sol;
	        	for(auto it = la.begin();it!=la.begin()+Rg;++it)
	        	{
	        		sol.push_back(*it);
	        	}
	        	printf("\n\n Unique solution would be:\n\n");
	        	for(auto it: sol)
	        	{
	        		printf("\t%10.4f\n",it );
	        	}
	        	printf("\n\n");

	        	return 3;

	        }
	    }
	}
};

#endif