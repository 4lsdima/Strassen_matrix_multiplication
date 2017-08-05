#ifndef __MATRIX_HPP_INCLUDED__
#define __MATRIX_HPP_INCLUDED__

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <typeinfo> //for printout type of variable
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip> //for setw()
#include <cassert>
#include <functional> //std::cref()
#include <algorithm> //std::sort()

typedef std::size_t ty;

template<class T>
class matrix
{
private:
		std::vector<T> arr;
public:
	ty r;
	ty c;

	explicit matrix(ty M=0, ty N=0, const char ch='z'):r(M),c(N) // Zero constructor
	{
		// std::cout<<"Zero constructor\n";
		if(r>0 && c>0)
		{
			if(ch=='o') //populate with 1 (ones)
	        {
	        	this->arr=std::vector<T>(M*N,T(1)); //populate with ones
	        }
			else if(ch== 'e')  // make E -matrix
			{
				this->arr=std::vector<T>(M*N,T(0));
	        	int i=0;
	        	while (i<N && M*N>=i*(1+N)) // <N is good idea!!!
	        	{
	        		this->arr[i*(1+N)]=T(1);
	        		i++;
	        	}
			}
	        else  //populate with zeroes
	        {
	        	this->arr=std::vector<T>(M*N,T(0));
	        }
	    }
	}
	//
	explicit matrix(ty M,ty N,const std::vector<T>& v):r(M),c(N) // Constructor from Vector
	{
		arr=v;
		ty S(v.size());
		ty MN(M*N);
		if(MN!=S)
		{
			for(ty i=0;i<(M*N-v.size());++i)
			{
				arr.push_back(T(0));
			}
			arr.resize(MN>S? (MN-S):(S-MN));
		}
	}
	//
	std::vector<T>& getVec() {return arr;}
	const std::vector<T>& getVec()const  {return arr;}
	//
	matrix(const matrix<T>& another):r(another.r),c(another.c) // Copy constructor
	{
		// std::cout<<"Copy Constructor\n";
		this->arr=std::vector<T>(another.getVec());
	}
	//
	std::vector<T>  moveVec()// was &&
	{
		return std::move(arr);
	}
	//
	explicit matrix (matrix&& another):r(another.r),c(another.c),arr(another.moveVec()) //Move constructor
	{
		//std::cout<<"Move Constructor\n";
		another.r=0;
		another.c=0;
	}
	//
	~matrix()
	{
		// std::cout<<"~Destructor()\n";
	}
	//
	void show(int numWidth=10) const
	{
		if(c>0 && r>0)
		{
		    std::cout<<"\n";
		    for( ty i=0;i<this->r;++i)
		    {
		        std::cout <<i<<":   ";
		        for (ty j=0; j < this->c; ++j)
		        {
		            std::cout<< std::setw(numWidth) << this->arr[i*(this->c)+j]<<"  ";
		            // std::cout<<  this->arr[i*(this->c)+j] <<"   ";
		        }
		        std::cout<<"\n";
		    }
		    std::cout<<"\n";
		}
	}
	//
	std::pair<ty,ty> size() const
	{
    	return std::pair<ty,ty>(this->r,this->c);
	}
	//
private:
	inline int chckIndxs(ty i, ty j) const
	{   
		// std::cout<<"chckIndxs:  i = "<<i<<"   j = "<<j<<std::endl;  
		// this->show();
	    if(i<r && j<c && i>=0 && j>=0 ) { return 0; }
	    else
	    {
	        if(i<0)
	        {
	            throw std::invalid_argument("error in matrix::chckIndxs(i,j):   incoming i < 0");
				return -1;            
	        }
	        if(j<0)
	        {
	            throw std::invalid_argument("error in matrix2D::chckIndxs(i,j):   incoming j < 0");
	            return -1;
	        }
	        if(i>=r)
	        {
	        	throw std::invalid_argument("error in matrix::chckIndxs(i,j):   incoming i >= num_of_rows");
				return 1;
	        }
	        if(j>=c)
	        	throw std::invalid_argument("error in matrix::chckIndxs(i,j):   incoming j >= num_of_columns");
				return 1;
	    }
	}
public:
	//
	inline T get(ty i, ty j)const
	{
		try
		{
			if(!chckIndxs(i,j)){return arr[i*(c)+j];}
		}
	    catch (std::invalid_argument &ia){std::cerr << ia.what() << "\n";}
	}
	//
	inline T& set(ty i, ty j)
	{
		try
		{
			if(!chckIndxs(i,j))	{return arr[i*(c)+j];}
		}
	    catch (std::invalid_argument &ia){std::cerr << ia.what() << "\n";}
	}
	//
	inline T& operator()(ty i,ty j) // operator(i,j) (mutator)
	{
    	try
	    {
			if(!chckIndxs(i,j))
			{
				return arr[i*(c)+j];
			}
		}
		catch (std::invalid_argument &ia)
		{
			std::cerr << ia.what() << "\n";
		}
	}
private:
	bool intType()const
	{
		bool res=false;
		if(
		typeid(T)==typeid(short) ||
		typeid(T)==typeid(short int) ||
		typeid(T)==typeid(signed short) ||
		typeid(T)==typeid(signed short int) ||
		typeid(T)==typeid(unsigned short) ||
		typeid(T)==typeid(unsigned short int) ||
		typeid(T)==typeid(int) ||
		typeid(T)==typeid(signed) ||
		typeid(T)==typeid(signed int) ||
		typeid(T)==typeid(unsigned) ||
		typeid(T)==typeid(unsigned int) ||
		typeid(T)==typeid(long) ||
		typeid(T)==typeid(long int) ||
		typeid(T)==typeid(signed long) ||
		typeid(T)==typeid(signed long int) ||
		typeid(T)==typeid(unsigned long) ||
		typeid(T)==typeid(unsigned long int) ||
		typeid(T)==typeid(long long) ||
		typeid(T)==typeid(long long int) || 
		typeid(T)==typeid(signed long long) ||
		typeid(T)==typeid(signed long long int) ||
		typeid(T)==typeid(unsigned long long) ||
		typeid(T)==typeid(unsigned long long int) 
		){
			res=true;
		}
		return res;
	}
	bool floatType()const
	{
		bool res=false;
		if(
			typeid(T)==typeid(float) ||
			typeid(T)==typeid(double) ||
			typeid(T)==typeid(long double)
		){
			res=true;
		}
		return res;
	}
public:
	//
	bool operator==(const matrix<T> &another)const//      '=='
	{
		if((r!=another.r)||(c!=another.c)){return false;}
		if(this->intType())
		{
			for( ty i=0;i < r;++i)
	        {
	            for (ty j = 0; j < c; ++j)
	            {
	                if( this->get(i,j)!=another.get(i,j)) {return false;}
	            }
	        }
	    }
	    else if(this->floatType())
	    {
		    const double eps(1.0E-20);
	        for( ty i=0;i<r;++i)
	        {
	            for (ty j = 0; j < c; ++j)
	            {
	                // if( (std::abs(( (double)this->arr[i][j]-(double)another(i,j)) ) >eps) ){return false;}
	                if( (std::abs(( (double)(this->get(i,j))-(double)(another.get(i,j))) ) >eps) ){return false;}
	            }
	        }
	    }
	    return true;
	}

	bool operator!=(const matrix<T> &another) const//     '!='
	{
	    return (!(*this==another));
	}
	//
	matrix<T>& swap(matrix<T>& another)
	{
		ty m=another.r;
		ty n=another.c;
		another.r=this->r;
		another.c=this->c;
		r=m;
		c=n;
		this->arr.swap(another.getVec());
		return *this;
	}

	//
	const matrix<T>& operator=(matrix<T>&& another)//   '=' 
	{
		// std::cout<<"\tMove operator=\n";
		if(this!= &another)
		{
			r=another.r;
			c=another.c;
			arr=another.moveVec();
		}
		return *this;
	}
	const matrix<T>& operator=( const matrix<T>& another)
	{
		// std::cout<<"\tCopy assign. operator=(const Ref)\n";
		if(this==&another){return *this;}
		r=another.r;
		c=another.c;
		arr = another.getVec();
		return *this;
	}
	//
	const matrix<T>& resize(const ty M, const ty N)    //    'resize'
	{
	    if( (M<=0)||(N<=0) ) //wrong arguments (both must be > 0)
	    {
	        std::cerr<<"error in matrix::resize(M,N):  the incoming parameters must be positive! (>0)\n";

	        return *this;
	    }
	    else if((M==this->r)&&(N==this->c)) // no need to resize
	    {
	    	return *this;
	    } 

	    else 
	    {
	    	matrix<T> newM(M,N);
	    	for(ty i=0;i<(this->r < M ? this->r:M );++i)
	    	{
	    		for(ty j=0;j<(this->c < N ? this->c:N );++j)
	    		{
	    			newM.set(i,j)=this->get(i,j);
	    		}
	    	}
	    	r=newM.r;
	    	c=newM.c;
	    	arr=newM.moveVec();
	    	return *this;
	    }
	}
	//

	matrix<T>& operator+=(const matrix<T>& another)  //   '+='
	{
	    if( r < another.r || c < another.c ) //need to resize *this
	    {
	        ty mm(r < another.r ? another.r : r);
	        ty nn(c < another.c ? another.c : c);
	        this->resize(mm,nn);
	    }

	    for( ty i=0;i<another.r;++i) // all in sizes of another
	    {
	        for(ty j = 0; j < another.c; ++j)
	        {   
	            this->set(i,j)+=another.get(i,j);
	        }
	    }        
	    return *this;
	}
	//
	matrix<T> operator+(const matrix<T>& rhs)const// 	' + '
	{
	    matrix<T>tmp(*this);
	    tmp+=rhs;
	    return std::move(tmp); 
	}
	//
	matrix<T>& operator-=(const matrix<T> &another)  //   '-='
	{
	    if( r < another.r || c < another.c ) //need to resize *this
	    {
	        ty mm(r < another.r ? another.r : r);
	        ty nn(c < another.c ? another.c : c);
	        this->resize(mm,nn);
	    }

	    for( ty i=0;i<another.r;++i) // all in sizes of another
	    {
	        for(ty j = 0; j < another.c; ++j)
	        {   
	            this->set(i,j)-=another.get(i,j);
	        }
	    }        
	    return *this;
	}
	// 
	matrix<T> operator-(const matrix<T>& rhs)const
	{
	    matrix<T>tmp(*this);
	    tmp-=rhs;
	    return std::move(tmp); 
	}
	//
	const  matrix<T>& operator*=(T multiplier) //  *= scalar T alpha 
	{
	    for(auto& it : arr)
	    {
	    	it*=multiplier;
	    }
	    return *this;
	}
	//
	const  matrix<T>& operator/=(T multiplier) //  *= scalar T alpha 
	{
	    for(auto& it : arr)
	    {
	    	it/=multiplier;
	    }
	    return *this;
	}

	//
	matrix<float> toFloat()const
	{
		std::vector<float> u((this->arr).begin(), (this->arr).end());
		return std::move(matrix<float>(r,c,u));
	}
	//
	matrix<double> toDouble()const
	{
		std::vector<double> u((this->arr).begin(), (this->arr).end());
		return std::move(matrix<double>(r,c,u));
	}
	// 
	matrix<long double> toLongDouble()const
	{
		std::vector<long double> u((this->arr).begin(), (this->arr).end());
		return std::move(matrix<long double>(r,c,u));
	}
	//

	matrix<int> toInt() const
	{
		ty S=r*c;
		std::vector<int>u(S);
		auto jt=u.begin();
		for(auto& it:arr)
		{
			*(jt)=(int)round(it);
			++jt;
		}
		// return std::move(matrix<int>(r,c,u));
		return std::move(matrix<int>(r,c,u));
	}
	//
	matrix<short> toShort() const
	{
		ty S=r*c;
		std::vector<short>u(S);
		auto jt=u.begin();
		for(auto& it:arr)
		{
			*(jt)=(short)round(it);
			++jt;
		}
		return std::move(matrix<short>(r,c,u));
	}
	//
	matrix<long> toLong() const
	{
		ty S=r*c;
		std::vector<long>u(S);
		auto jt=u.begin();
		for(auto& it:arr)
		{
			*(jt)=(long)round(it);
			++jt;
		}
		return std::move(matrix<long>(r,c,u));
	}
	//
	matrix<long long> toLongLong() const
	{
		ty S=r*c;
		std::vector<long long>u(S);
		auto jt=u.begin();
		for(auto& it:arr)
		{
			*(jt)=(long long)round(it);
			++jt;
		}
		return std::move(matrix<long long>(r,c,u));
	}
	//
	std::vector<T> row(ty ind)const //    get row(index)
	{
	    try
	    {
	       if(!chckIndxs(ind,0))
	       {
	       		std::vector<T> v(c);
	       		auto it=v.begin();
	          	for (ty i = 0; i < c; ++i)
	          	{
	          		*it=(get(ind,i));
	          		++it;
	          	}
      		    std::cout<<"row("<<ind<<"):\n";
	          	return std::move(v);
	       }
	    }
	    catch (std::invalid_argument &ia)
	    {
	        std::cerr << ia.what() << "\n";
	    }
	}
	// 
	std::vector<T> column(ty ind)const //    get column(index)
	{
	    try
	    {
	       if(!chckIndxs(0,ind))
	       {
	       		std::vector<T> v(r);
	       		auto it=v.begin();
	          	for (ty i = 0; i < r; ++i)
	          	{
	          		*it=(get(i,ind));
	          		++it;
	          	}
	          	std::cout<<"column("<<ind<<"):\n";
	          	return std::move(v);
	       }
	    }
	    catch (std::invalid_argument &ia)
	    {
	        std::cerr << ia.what() << "\n";
	    }
	}

	matrix<T> crop( ty i, ty j, ty m, ty n)const //returns submatrix(m x n) from the element (i,j) to the right-downwards
	{

		try
		{
			if(!chckIndxs(i,j))
			{
				if(i+m>r){throw std::invalid_argument("error in matrix::crop(i,j,m,n)   parameter m >out of size r-i");}
				if(j+n>c){throw std::invalid_argument("error in matrix::crop(i,j,m,n)   parameter n >out of size c-j");}
				std::vector<T> v(m*n);
				auto it=v.begin();
				for(ty k=i; k<i+m;++k)
				{
					for(ty l=j;l<j+n;++l)
					{
						*it=this->get(k,l);
						++it;
					}
				}
				matrix<T> M(m,n,v);
				return std::move(M);
			}
		}
	    catch (std::invalid_argument &ia){std::cerr << ia.what() << "\n";}
	}

	
	void appendRow( std::vector<T>&& v)
	{
		// std::vector<T> v;
		if(v.size()<c)
		{
			v.resize(c,T(0));
		}
		else if(v.size()>c)
		{
			v.resize(c);
		}
		this->resize(r+1,c);

		for(ty i=0;i<c;++i)
		{
			this->set(r-1,i)=v[i];
		}


	}
	void appendColumn(std::vector<T>&& v)
	{
		if(v.size()<r)
		{
			v.resize(r,T(0));
		}
		else if(v.size()>r)
		{
			v.resize(r);
		}
		this->resize(r,c+1);
		for(ty k=0;k<r;++k)
		{
			this->set(k,c-1)=v[k];
		}
	}
	//

	matrix<T> M11()const //upper-left quadrant
	{
		ty m=r/2;
		ty n=c/2;
		assert(m && n);
		matrix<T>R(m,n);
		for(ty i=0; i< m;++i)
		{
			for(ty j=0;j<n;++j)
			{
				R(i,j)=this->get(i,j);
			}
		}
	return std::move(R);
	}

	matrix<T> M21()const //lower-left quadrant
	{
		ty m= r-r/2;
		ty n=c/2;
		assert(m && n);
		matrix<T>R(m,n);
		for(ty i=0; i< m;++i)
		{
			for(ty j=0;j<n;++j)
			{
				R(i,j)=this->get(r/2+i,j);
			}
		}
	return std::move(R);
	}

	matrix<T> M12()const //upper-right quadrant
	{
		ty m= r/2;
		ty n=c-c/2;
		assert(m && n);
		matrix<T>R(m,n);
		for(ty i=0; i< m;++i)
		{
			for(ty j=0;j<n;++j)
			{
				R(i,j)=this->get(i,c/2+j);
			}
		}
	return std::move(R);
	}

	matrix<T> M22()const //lower-right quadrant
	{
		ty m= r-r/2;
		ty n=c-c/2;
		assert(m && n);
		matrix<T>R(m,n);
		for(ty i=0; i< m;++i)
		{
			for(ty j=0;j<n;++j)
			{
				R(i,j)=this->get(r/2+i,c/2+j);
			}
		}
	return std::move(R);
	}

	matrix<T> transpose()const
	{
		ty m=c;
		ty n=r;
		matrix<T>R(m,n);
		for(ty i=0;i<r;++i)
		{
			for(ty j=0;j<c;++j)
			{
				R(j,i)=this->get(i,j);
			}
		}
		return std::move(R);
	}

};

template<class T>
void showVec(const std::vector<T>& v)
{
	std::cout<<"\n";
	for (auto& it:v)
	{
		std::cout<<it<<"  ";
	}
	std::cout<<"\n";
	std::cout<<"  size = "<<v.size()<<std::endl;
	std::cout<<"\n";
}

template <class T>
inline matrix<T> mul_kji(const matrix <T>& A, const matrix <T>& B) 
{
	assert(A.r && B.r && A.c && B.c);
	try
	{
		if(A.c!=B.r)
		{
			throw std::invalid_argument("Error in mul(): the sizes of matrixes are inconsistent for the multiplication!");
		}
		matrix<T>C(A.r,B.c);

		for(ty k=0; k<A.c;++k)
		{
			for(ty j=0;j<A.r;++j)
			{
				for(ty i=0;i<B.c;++i)
				{
					C(j,i)+=A.get(j,k)*B.get(k,i);
				}
			}
		}
		return std::move(C);
	}
	catch(std::invalid_argument &ia){std::cerr << ia.what() << "\n";}
}
//
template <class T>
inline matrix<T> mul_kij(const matrix <T>& A, const matrix <T>& B) 
{
	assert(A.r && B.r && A.c && B.c);
	try
	{
		if(A.c!=B.r)
		{
			throw std::invalid_argument("Error in mul(): the sizes of matrixes are inconsistent for the multiplication!");
		}
		matrix<T>C(A.r,B.c);
		for(ty k=0; k<A.c;++k)	
		{
			for(ty i=0;i<B.c;++i)
			{
				for(ty j=0;j<A.r;++j)
				{
					C(j,i)+=A.get(j,k)*B.get(k,i);
				}
			}
		}
		return std::move(C);
	}
	catch(std::invalid_argument &ia){std::cerr << ia.what() << "\n";}
}
//
template <class T>
inline matrix<T> mul_jki(const matrix <T>& A, const matrix <T>& B) 
{
	assert(A.r && B.r && A.c && B.c);
	try
	{
		if(A.c!=B.r)
		{
			throw std::invalid_argument("Error in mul(): the sizes of matrixes are inconsistent for the multiplication!");
		}
		matrix<T>C(A.r,B.c);

		for(ty j=0;j<A.r;++j)
		{
			for(ty k=0; k<A.c;++k)	
			{
				for(ty i=0;i<B.c;++i)
				{
					C(j,i)+=A.get(j,k)*B.get(k,i);
				}
			}
		}
		return std::move(C);
	}
	catch(std::invalid_argument &ia){std::cerr << ia.what() << "\n";}
}
template <class T>
inline matrix<T> mul_jik(const matrix <T>& A, const matrix <T>& B) 
{
	assert(A.r && B.r && A.c && B.c);
	try
	{
		if(A.c!=B.r)
		{
			throw std::invalid_argument("Error in mul(): the sizes of matrixes are inconsistent for the multiplication!");
		}
		matrix<T>C(A.r,B.c);

		for(ty j=0;j<A.r;++j)
		{
			for(ty i=0;i<B.c;++i)
			{
				for(ty k=0; k<A.c;++k)		
				{
					C(j,i)+=A.get(j,k)*B.get(k,i);
				}
			}
		}
		return std::move(C);
	}
	catch(std::invalid_argument &ia){std::cerr << ia.what() << "\n";}
}

template <class T>
inline matrix<T> mul_ijk(const matrix <T>& A, const matrix <T>& B) // BEST !!!
{
	assert(A.r && B.r && A.c && B.c);
	try
	{
		if(A.c!=B.r)
		{
			throw std::invalid_argument("Error in mul(): the sizes of matrixes are inconsistent for the multiplication!");
		}
		matrix<T>C(A.r,B.c);
		for(ty i=0;i<B.c;++i)
		{
			for(ty j=0;j<A.r;++j)
			{
				for(ty k=0; k<A.c;++k)		
				{
					C(j,i)+=A.get(j,k)*B.get(k,i);
				}
			}
		}
		return std::move(C);
	}
	catch(std::invalid_argument &ia){std::cerr << ia.what() << "\n";}
}
template <class T>
inline matrix<T> mul_ikj(const matrix <T>& A, const matrix <T>& B) 
{
	assert(A.r && B.r && A.c && B.c);
	try
	{
		if(A.c!=B.r)
		{
			throw std::invalid_argument("Error in mul1(): the sizes of matrixes are inconsistent for the multiplication!");
		}
		matrix<T>C(A.r,B.c);

		for(ty i=0;i<B.c;++i)
		{
			for(ty k=0; k<A.c;++k)
			{
				for(ty j=0;j<A.r;++j)	
				{
					C(j,i)+=A.get(j,k)*B.get(k,i);
				}
			}
		}
		return std::move(C);
	}
	catch(std::invalid_argument &ia){std::cerr << ia.what() << "\n";}
}
/////////////////////////////***  Utilites ***//////////////////////////////////////////////////
#include <random>
double randomReal(double min, double max) //generates random double in [min, max)
{
	assert(min < max);
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    	//Each call to dis(gen) generates a new random double
    std::uniform_real_distribution<> dis(min, max);//Use dis to transform the 
    // random unsigned int generated by gen into a double in [1, 2)
    return dis(gen);
} 
//   
matrix<double> makeRandMatrix(ty nr, ty nc, double min, double max)
{
	assert(nr>0 && nc>0);
	std::vector<double> v(nr*nc);
	for(auto& it:v)
	{
		it=randomReal(min,max);
	}
	return matrix<double>(nr,nc,v);
}
//
void remDublicates(std::vector<int>& v) //v must be sorted!
{
	if(v.size()==1){return;}
	std::vector<int>::iterator jt=v.begin();
	for (std::vector<int>::iterator  it= v.begin()+1; it!= v.end(); ++it)
	{
		if(*it!=*jt)
		{
			++jt;
			if(it!=jt)
			{
				*jt=*it;
			}
		}
	}
	v.resize(jt-v.begin()+1);
}

matrix<short> permutMatrix (const std::vector<int>& P) // makes a permuntation matrix corresponding to permutations inside  vector<int>
{
 	std::vector<int> P1(P);
  	std::sort(P1.begin(), P1.end());
  	remDublicates(P1);
  	assert(P1.size()==P.size());
	matrix<short>M(P.size(),P.size());
  	
  	for (ty i = 0; i < P.size(); ++i)
	{
 		if (P[i]!=P1[i])
 		{
 			for(ty j=0;j<P.size();++j)
 			{
 				if(P[j]==P1[i])
 				{
 					M(i,j)=1;
 				}
 			}
 		}
 		else{M(i,i)=1;}	
 	}
 	return std::move(M);
}



template <class T>
void matrix2File(const matrix<T>& Matrix, const std::string& fName) // saves matrix's content to text file
{
	std::ofstream myfile(fName);
	if(myfile.is_open())
	{
		myfile<< Matrix.r << std::endl;
		myfile<< Matrix.c << std::endl;
		std::vector<T> v(Matrix.getVec());
		for(auto& it : v)
		{
			myfile<< it << std::endl;
		}
		myfile.close();
	}
}

matrix<double> file2Matrix(const std::string& fName) // reads a text file of numbers to make matrix<double> 
{
	ty nr(0);
	ty nc(0);
	std::vector<double> v;

	std::ifstream in((const char *)fName.c_str());
  	if(!in) { 	std::cerr << "Cannot open input file.\n"; 	}
  	else 
  	{
		int co(0); 
		ty len(0); 
		ty l(0);		
  		std::stringstream ss;
  		ss << in.rdbuf();    
  		for (std::string line; getline(ss, line); )
  		{
  			if(co==0)
  			{
  				nr=std::stoi(line);
  				++co;
  			}
  			else if(co==1)
  			{
  				nc=std::stoi(line);
  				++co;
  				len=nr*nc;
  			}
  			else
  			{
  				if(l<len)
  				{
  					v.push_back(std::stod(line));
  					++l;
  				}
  			}
        }
  	}
  	in.close();
  	return matrix<double>(nr,nc,v);
}

#endif
