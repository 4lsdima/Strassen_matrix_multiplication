


Class matrix<T> is a sort of "velosiped".
For practical usage boost::numeric::ublas:matrix would be more preferrable.

Nevertheless, Strassen algorithm for matrix multiplication,  that is little more fast than a straightforward method at large matrixes
 and very nice for the  parallelization,  was  been implemented under the class matrix<T>, (along with its multithreaded version).
 (see "strassenMul.hpp", to be discussed below ).

Speed tests:
	A,B- two test matrixes (1500 x 1500) of different ints, generated once.

	Strassen parallel:              13542  ms
	Strassen serial:                69914  ms
	straightforward (ikj):          80119  ms
	dumb (ijk) product:             75366  ms (that's strange, but it's faster than (ikj) )

	So, that was win!


///////////////////////////////////////////////////////////////////////////////////////////////////////
	"matrix.hpp":

Numerical data of the class matrix<T> instance is stored inside a single std::vector<T> arr (private member).
  That is quite good approach because of the simplicity for memory allocation.

There are public members r and c that are stand for numbers of rows and colums respectively.
public methods get(i,j) and set(i,j) to inspect and to get access to the elements of the matrix.
 and an operator(i,j) that doing the same as set(i,j).


class matrix<T> contain necessary methods for the most basic matrix manipulations:

zero constructor:
 example:
	matrix<int>A(3,4) - creates 3x4 matrix of zeroes.
	matrix<int>B(3,4,'o') - creates 3x4 matrix of ones.
	matrix<int>C(3,3,'e') - creates 3x3 identity matrix;

copy constructor:
	matrix<int>D(C);

move constructor:
	matrix<int>Q(std::move(C));

constructor from std::vector<T> v:
	matrix<int>A(4,5,v);

.getVec() and
.getVec()const  -return ref. or cref the data vector ( this->arr ) ;

.moveVec() --returns a movable copy of this->arr ;

operator= :
	A=B;

move operator= :
	A=std::move(B);

operator==(const matrix<T>&) -unary, returns bool;

operator!=(const matrix<T>&)  -unary, returns bool;

.swap(matrix<T>&) performs swap operation *this <==> matrix<T>& another;

.show() -prints out  *this matrix;

operator+= (unary) :
	C+=A;

operator-= (unary) :
	C-=B;

operator+ (binary) :
	C=A+B;

operator*= (unary), to multiply *this  on a scalar value T
	C*=4;

operator/= (unary), to divide *this by a scalar value T


methods for the conversion of elemnt types (return  a new matrix of corresponding T ):

.toInt()
.toFloat()
.toDouble()
.toLongDouble()
.toLong()
.toShort()
.toLongLong()


.row(ty i)  - returns std::vector<T> containing values of a row i;

.column(ty j)  - returns std::vector<T> containing values of a column j;

.get(i,j) const -inspector.Returns an element at position (i,j);

.set(i,j) -mutator. Returns a refference to the  element at position (i,j);

operator()(size_t,size_t) - mutator. Returns a refference to the  element at position (i,j);

crop(size_t i,size_t j,size_t m, size_t n)  - returns a submatrix m x n, to the right-downwards from the element (i,j);

.appendRow(const std::vector<T>& v)  -appends  a row (with corresponding values ) to the bottom edge of a matrix;

.appendColumn(const std::vector<T>& v)  -appends  a column (with corresponding values ) to the righmost edge of a matrix;

.M11() returs a new (movable) matrix, standing for the upper-left quadrant of *this ;

.M12() --//-- upper-right quadrant;

.M21() --//-- lower-left quadrant ;

.M22() --//-- lower-right quadrant;

.transpose() returs a new (movable) matrix , transposed to *this; 

***************************************************

Other useful functions:

matrix<double> makeRandMatrix(size_t nr, size_t nc, double min, double max) --creates a matrix<double>, 
 (nr x nc), with random generated elements, confined 
	inside a semi-interval [min,max)

matrix<T> mul(const matrix <T>& A, const matrix <T>& B) --straightforward product dot(A,B),
 (ikj-method was used to reduce a time of computation);

matrix<int> permutMatrix (const std::vector<int>& P) -- makes a permuntation matrix corresponding to
 permutations inside  vector<int>
	(vector P must contain only unique elements. Otherwise assert() activates;)

void matrix2File(const matrix<T>& Matrix, const std::string& fName) --saves the content of the matrix into
the file (in the form of column of numbers). 
	With first two numbers are the numbers of rows and columns of a matrix respectively.


matrix<double> file2Matrix(const std::string& fName) // reads a text file of column data to make
 a new  matrix<double> ;

void showVec(const std::vector<T>& ) --prints out a vector;


***************************************************


///////////////////////////////////////////////////////////////////////////////////////////////////////
 "strassenMul.hpp":
Strassen method: (the most interesting part of this opus)

template<class T>
inline matrix<T> strassenMul(matrix<T> A, matrix<T> B, char ch='s', ty smax=4); 
 -is a recursive function that implements "Divide && Conquer" algorithm and the Strassen's method.
	Returns the result of dot(A,B);

  Parameter smax (=4 by default) defines minimal dimension of the multipliers,
   when  strassenMul(A,B) just starts multiply by the straightforward method
    (when sizes of matrixes are small enough).

 if Parameter ch is set to be 'p' - strassenMul() starts in 'parallel' mode:
 	7 threads are going to be created at the first level of strassenMul(), to calculate P1,P2,..., P7;



void makeConsistent(matrix<T>& A, matrix<T>& B ) --an utility function to fix sizes of
  the multipliers, to make them consistent for matrix product


void align(matrix<T>& A,matrix<T>& B) --an utility function, to make multiplier's sizes to be even.






