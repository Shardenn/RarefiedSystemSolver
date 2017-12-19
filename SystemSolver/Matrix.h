#ifndef MATRIX_H
#define MATRIX_H

#define DOUBLE_PRECISION 3

#include <iostream>

using namespace std;

/* Base class for SQUARE matrixes */
template <typename Cell = double>
class Matrix
{
protected:
	int m_Size;
	// /* Number of lines, filled with stuff ( k and k + 2 in this task ) */
	//int m_FirstLineNumber, m_SecondLineNumber;
	Cell **m_Cells;
	void AllocateCells( int );
	void FreeCells();

public:
	Matrix() : 
		m_Size( 0 ), 
		m_Cells( nullptr )
	{}

	Matrix( const Matrix& );
	Matrix( int Size );
	/* Matrix constructor from 5 vectors */
	Matrix( int Size, Cell *, Cell *, Cell *, Cell *, Cell * );
	~Matrix();

	Cell &operator()( int i, int j )
	{
		return m_Cells [ i - 1 ] [ j - 1 ];
	}

	Matrix& operator = ( const Matrix& );
	Matrix  operator + ( const Matrix& );
	Matrix  operator - ( const Matrix& );
	Matrix  operator * ( const Matrix& );

	friend istream& operator >> <> ( istream&, Matrix& );
	friend ostream& operator << <> ( ostream&, const Matrix& );
};


template <typename Cell>
Matrix<Cell>::Matrix( const Matrix<Cell>& M )
{
	AllocateCells( M.size );

	for ( int i = 0; i < m_Size; i++ )
		for ( int j = 0; j < m_Size; j++ )
			m_Cells [ i ] [ j ] = M.cells [ i ] [ j ];
}

template <typename Cell>
Matrix<Cell>::Matrix( int Size )
{
	AllocateCells( Size );

	for ( int i = 0; i < m_Size; i++ )
		for ( int j = 0; j < m_Size; j++ )
			m_Cells [ i ] [ j ] = 0;
}



template <typename Cell>
Matrix<Cell>::~Matrix()
{
	FreeCells();
}

template <typename Cell>
Matrix<Cell>& Matrix<Cell>::operator=( const Matrix& M )
{
	if ( m_Size != M.size )
	{
		FreeCells();
		AllocateCells( M.size );
	}

	for ( int i = 0; i < m_Size; i++ )
		for ( int j = 0; j < m_Size; j++ )
			m_Cells [ i ] [ j ] = M.m_Cells [ i ] [ j ];

	return *this;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator+( const Matrix& M )
{
	Matrix<Cell> res( *this );

	if ( m_Size == M.size )
	{
		for ( int i = 0; i < m_Size; i++ )
			for ( int j = 0; j < m_Size; j++ )
				res.m_Cells [ i ] [ j ] += M.m_Cells [ i ] [ j ];
	}

	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator-( const Matrix& M )
{
	if ( m_Rows != M.m_Rows || m_Columns != M.m_Columns )
		return Matrix();

	Matrix<Cell> Result( *this );

	for ( int i = 0; i < m_Rows; i++ )
		for ( int j = 0; j < m_Columns; j++ )
			Result.m_cells [ i ] [ j ] -= M.m_cells [ i ] [ j ];

	return Result;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator*( const Matrix& M )
{
	if ( m_Columns != M.m_Rows )
		return Matrix();

	Matrix<Cell> Result( m_Rows, M.m_Columns );

	for ( int i = 0; i < m_Rows; i++ )
		for ( int j = 0; j < M.m_Columns; j++ )
		{
			for ( int l = 0; l < m_Columns; l++ )
				Result.m_cells [ i ] [ j ] += m_Cells [ i ] [ l ] * M.m_cells [ l ] [ j ];
		}

	return Result;
}

template <typename Cell>
void Matrix<Cell>::AllocateCells( int Size )
{
	m_Cells = new Cell* [ Size ];

	for ( int i = 0; i < Size; i++ )
		m_Cells [ i ] = new Cell [ Size ];

	m_Size = Size;
}

template <typename Cell>
void Matrix<Cell>::FreeCells()
{
	for ( int i = 0; i < m_Size; i++ )
		delete m_Cells [ i ];

	delete m_Cells;
	m_Size = 0;
}

template <typename Cell>
istream& operator >> ( istream& fi, Matrix<Cell>& M )
{
	for ( int i = 0; i < M.m_Size; i++ )
		for ( int j = 0; j < M.m_Size; j++ )
			fi >> M.m_Cells [ i ] [ j ];

	return fi;
}

template <typename Cell>
ostream& operator << ( ostream& fo, const Matrix<Cell>& M )
{
	fo.precision( DOUBLE_PRECISION );

	for ( int i = 0; i < M.m_Size; i++ )
	{
		fo << "  ";
		for ( int j = 0; j < M.m_Size; j++ )
			fo << M.m_Cells [ i ] [ j ] << " \t";
		fo << endl;
	}
	return fo;
}

#endif MATRIX_H
