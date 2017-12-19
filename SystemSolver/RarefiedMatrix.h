#pragma once

#define MAX_SIZE_TO_PRINT_MATRIX 11
#define MAX_SIZE_TO_PRINT_VECTOR 200
#define IN_DEBUG_MODE false
#define TEST_UNIT_VECTOR false

#include "Matrix.h"
#include <vector>
#include <iostream>

using namespace std;

const int DefaultSize = 10;
const int DefaultFirstLine = 5;

double GetRandomDouble( double dMin = -10, double dMax = 10 )
{
	double d = ( double ) rand() / RAND_MAX;

	double RandomValue = dMin + d * ( dMax - dMin );
	
	return RandomValue ? RandomValue : 1;
}

int GetRandomInt( int iMin = -10, int iMax = 10 )
{
	int d = ( int ) rand() % ( iMax - iMin );
	int RandomValue = iMin + d;
	return RandomValue ? RandomValue : 1;
}

class RarefiedMatrix
{
private:
	int m_MatrixSize;
	/*	First vector : main diag					( b == 0 )
		Second vector : upper diag					( c == 1 )
		Third vector : lower diag					( a == 2 )
		Fourth vector : upper line ( k line )		( p == 3 )
		Fifth vector : lower line ( k + 2  line )	( q == 4 )
	*/
	double ** m_RarefiedVectors;
	double * F;
	double * X_Generated;
	double * X_Found;

	int m_FirstLineNumber;

	void AllocateCells( int );

	double ** GetMatrixFromVectors();

	int GetSize()						{ return m_MatrixSize; }
public:
	/* Random vector fillings */
	RarefiedMatrix( int NewSize, int FirstLineNumer, double dMinRandom = -100, double dMaxRandom = 100 );
	
	/* Returns pointer to 5 vectors in matrix */
	double ** GetVectors();

	void Solve( bool );

	void PrintVector( double * Vector, int RequiredPrecision = 3 );
	void PrintMatrix( int RequiredPrecision = 3 );
};

void RarefiedMatrix::AllocateCells( int NewSize )
{
	m_MatrixSize = NewSize;

	F = new double [ m_MatrixSize ];
	X_Generated = new double [ m_MatrixSize ];
	X_Found = new double [ m_MatrixSize ];

	m_RarefiedVectors = new double * [ m_MatrixSize ];

	for ( int i = 0; i < 5; i++ )
	{
		m_RarefiedVectors [ i ] = new double [ m_MatrixSize  ];
	}
}

double ** RarefiedMatrix::GetMatrixFromVectors()
{
	double ** Result;
	Result = new double * [ m_MatrixSize ];
	
	for ( int i = 0; i < m_MatrixSize; i++ )
	{
		Result [ i ] = new double [ m_MatrixSize ];
	}

	for ( int i = 0; i < m_MatrixSize; i++ )
	{
		for ( int j = 0; j < m_MatrixSize; j++ )
		{
			if ( i == j )
				Result [ i ] [ j ] = m_RarefiedVectors [ 0 ] [ i ];	/* Main diagonal */
			else if ( j - 1 == i )
				Result [ i ] [ j ] = m_RarefiedVectors [ 1 ] [ i ];	/* Upper diagonal */
			else if ( i - 1 == j )
				Result [ i ] [ j ] = m_RarefiedVectors [ 2 ] [ j ];	/* Lower diagonal */
			else if ( i == m_FirstLineNumber )
				Result [ i ] [ j ] = m_RarefiedVectors [ 3 ] [ j ];	/* First line */
			else if ( i == m_FirstLineNumber + 2 )
				Result [ i ] [ j ] =  m_RarefiedVectors [ 4 ] [ j ];	/* Second line */
			else
				Result [ i ] [ j ] =  0;
		}
	}

	return Result;
}

RarefiedMatrix::RarefiedMatrix( int NewSize = DefaultSize, int FirstLineNumber = DefaultFirstLine, double dMin, double dMax )
{
	AllocateCells( NewSize );
	
	m_FirstLineNumber -= 1;

	if ( m_FirstLineNumber < 0 )
		m_FirstLineNumber = 0;

	/* Filling all vectors with Size-1 elements */
	for ( int i = 0; i < 5; i++ )
	{
		for ( int j = 0; j < m_MatrixSize; j++ )
		{
			if( IN_DEBUG_MODE )
				m_RarefiedVectors [ i ] [ j ] = GetRandomInt();
			else
			m_RarefiedVectors [ i ] [ j ] = GetRandomDouble( dMin, dMax );
		}
	}

	m_RarefiedVectors [ 3 ] [ m_FirstLineNumber - 1 ]			= m_RarefiedVectors [ 2 ] [ m_FirstLineNumber - 1 ];
	m_RarefiedVectors [ 3 ] [ m_FirstLineNumber ]				= m_RarefiedVectors [ 0 ] [ m_FirstLineNumber ];
	m_RarefiedVectors [ 3 ] [ m_FirstLineNumber + 1 ]			= m_RarefiedVectors [ 1 ] [ m_FirstLineNumber ];
	m_RarefiedVectors [ 4 ] [ m_FirstLineNumber + 1 ]			= m_RarefiedVectors [ 2 ] [ m_FirstLineNumber + 1 ];
	m_RarefiedVectors [ 4 ] [ m_FirstLineNumber + 2 ]			= m_RarefiedVectors [ 0 ] [ m_FirstLineNumber + 2 ];
	m_RarefiedVectors [ 4 ] [ m_FirstLineNumber + 3 ]			= m_RarefiedVectors [ 1 ] [ m_FirstLineNumber + 2 ];

	/* Filling F vector */
	for ( int i = 0; i < m_MatrixSize; i++ )
	{
		if ( IN_DEBUG_MODE )
			X_Generated [ i ] = GetRandomInt();//GetRandomInt();
		else if ( TEST_UNIT_VECTOR )
			X_Generated [ i ] = 1;
		else
			X_Generated [ i ] = GetRandomDouble( dMin, dMax );
		
		F [ i ] = 0;
	}

	double ** GeneratedMatrix = GetMatrixFromVectors();
	for ( int i = 0; i < m_MatrixSize; i++ )
	{
		for ( int j = 0; j < m_MatrixSize; j++ )
		{
			F [ i ] += GeneratedMatrix [ i ] [ j ] * X_Generated [ j ];
		}
	}
}

double ** RarefiedMatrix::GetVectors()
{
	return m_RarefiedVectors;
}

void RarefiedMatrix::PrintMatrix( int RequiredPrecision )
{
	if ( m_MatrixSize > MAX_SIZE_TO_PRINT_MATRIX )
		return;
	
	for ( int i = 0; i < m_MatrixSize; i++ )
	{
		for ( int j = 0; j < m_MatrixSize; j++ )
		{
			cout.precision( RequiredPrecision );
			cout.width( RequiredPrecision + 6 );
			
			if ( i == j )
				cout << m_RarefiedVectors [ 0 ] [ i ];	/* Main diagonal */
			else if ( j - 1 == i )
				cout << m_RarefiedVectors [ 1 ] [ i ];	/* Upper diagonal */
			else if ( i - 1 == j )
				cout << m_RarefiedVectors [ 2 ] [ j ];	/* Lower diagonal */
			else if ( i == m_FirstLineNumber )
				cout << m_RarefiedVectors [ 3 ] [ j ];	/* First line */
			else if ( i == m_FirstLineNumber + 2 )
				cout << m_RarefiedVectors [ 4 ] [ j ];	/* Second line */
			else
				cout << 0;
		}

		cout << "\t\t" << F [ i ] << "\n\n";
	}

	cout << "\n\n\n";
}

void RarefiedMatrix::PrintVector( double * Vector, int Precision )
{
	for ( int i = 0; i < m_MatrixSize; i++ )
	{
		cout.precision( Precision );
		cout.width( Precision + 6 );

		cout << Vector [ i ];
	}

	cout << "\n\n\n";
}

void RarefiedMatrix::Solve( bool bWithPrint = false )
{
	if ( m_MatrixSize > MAX_SIZE_TO_PRINT_MATRIX )
		bWithPrint = false;
	
	/* Careful, shitCode here */
	double Coeff;
	
	/* Step 1 */
	for ( int i = 0; i < m_FirstLineNumber; i++ )
	{
		Coeff = 1 / m_RarefiedVectors [ 0 ] [ i ];
		m_RarefiedVectors [ 0 ] [ i ] = 1;

		m_RarefiedVectors [ 1 ] [ i ] *= Coeff;
		F [ i ] *= Coeff;

		Coeff = m_RarefiedVectors [ 2 ] [ i ];
		m_RarefiedVectors [ 2 ] [ i ] = 0;
		/* Overlapping this element */
		if ( i + 1 == m_FirstLineNumber )
			m_RarefiedVectors [ 3 ] [ i ] = 0;

		m_RarefiedVectors [ 0 ] [ i + 1 ] -= Coeff * m_RarefiedVectors [ 1 ] [ i ];
		/* Overlapping this */
		if ( i + 1 == m_FirstLineNumber )
			m_RarefiedVectors [ 3 ] [ i + 1 ] = m_RarefiedVectors [ 0 ] [ i + 1 ];
		F [ i + 1 ] -= Coeff * F [ i ];

		Coeff = m_RarefiedVectors [ 3 ] [ i ];
		m_RarefiedVectors [ 3 ] [ i ] = 0; 

		m_RarefiedVectors [ 3 ] [ i + 1 ] -= Coeff * m_RarefiedVectors [ 1 ] [ i ]; 
		/* Overlapping this elements */
		if( i + 1 == m_FirstLineNumber )
			m_RarefiedVectors [ 0 ] [ i + 1 ] = m_RarefiedVectors [ 3 ] [ i + 1 ];		
		if ( i + 1 == m_FirstLineNumber - 1 )
			m_RarefiedVectors [ 2 ] [ i + 1 ] = m_RarefiedVectors [ 3 ] [ i + 1 ];
		F [ m_FirstLineNumber ] -= Coeff * F [ i ];

		Coeff = m_RarefiedVectors [ 4 ] [ i ];
		m_RarefiedVectors [ 4 ] [ i ] = 0;

		m_RarefiedVectors [ 4 ] [ i + 1 ] -= Coeff * m_RarefiedVectors [ 1 ] [ i ];
		F [ m_FirstLineNumber + 2 ] -= Coeff * F [ i ];	
	}
	
	if ( bWithPrint )
	{
		cout << "Step 1: \n\n";

		PrintMatrix();
	}

	/* Step 2 */
	for ( int i = m_MatrixSize - 1; i >= m_FirstLineNumber + 3; i-- )
	{
		Coeff = 1 / m_RarefiedVectors [ 0 ] [ i ];
		m_RarefiedVectors [ 0 ] [ i ] = 1;

		m_RarefiedVectors [ 2 ] [ i - 1 ] *= Coeff;
		F [ i ] *= Coeff;

		if ( i > m_FirstLineNumber + 3 )
		{
			Coeff = m_RarefiedVectors [ 1 ] [ i - 1 ];
			m_RarefiedVectors [ 1 ] [ i - 1 ] = 0;
			/* Overlapping this element */
			if ( i - 1 == m_FirstLineNumber + 2 )
				m_RarefiedVectors [ 4 ] [ m_FirstLineNumber + 3 ] = 0;

			m_RarefiedVectors [ 0 ] [ i - 1 ] -= Coeff *  m_RarefiedVectors [ 2 ] [ i - 1 ];
			/* Overlapping this element */
			if ( i - 1 == m_FirstLineNumber + 2 )
				m_RarefiedVectors [ 4 ] [ i - 1 ] = m_RarefiedVectors [ 0 ] [ i - 1 ];
			F [ i - 1 ] -= Coeff * F [ i ];
		}

		Coeff = m_RarefiedVectors [ 4 ] [ i ];
		m_RarefiedVectors [ 4 ] [ i ] = 0;
		/* Overlapping this element */
		if ( i == m_FirstLineNumber + 3 )
			m_RarefiedVectors [ 1 ] [ i - 1 ] = 0;

		m_RarefiedVectors [ 4 ] [ i - 1 ] -= Coeff * m_RarefiedVectors [ 2 ] [ i - 1 ];
		/* Overlapping main diagonal */
		if ( i - 1 == m_FirstLineNumber + 2 )
			m_RarefiedVectors [ 0 ] [ i - 1 ] = m_RarefiedVectors [ 4 ] [ i - 1 ];
		F [ m_FirstLineNumber + 2 ] -= Coeff * F [ i ];

		Coeff = m_RarefiedVectors [ 3 ] [ i ];
		m_RarefiedVectors [ 3 ] [ i ] = 0;

		m_RarefiedVectors [ 3 ] [ i - 1 ] -= Coeff * m_RarefiedVectors [ 2 ] [ i - 1 ];
		F [ m_FirstLineNumber ] -= Coeff * F [ i ];

	}

	if ( bWithPrint )
	{
		cout << "Step 2: \n\n";

		PrintMatrix();
	}

	/* Step 3 */
	int i = m_FirstLineNumber + 1;
	
	Coeff = 1 / m_RarefiedVectors [ 0 ] [ i ];
	m_RarefiedVectors [ 0 ] [ i ] = 1;

	/* Handling with i + 1 line */
	m_RarefiedVectors [ 1 ] [ i ] *= Coeff;
	m_RarefiedVectors [ 2 ] [ i - 1 ] *= Coeff;
	F [ i ] *= Coeff;

	/* Handling m_FirstLineNumber line */
	Coeff = m_RarefiedVectors [ 3 ] [ i ];
	m_RarefiedVectors [ 3 ] [ i - 1 ] -= Coeff * m_RarefiedVectors [ 2 ] [ i - 1 ];
	m_RarefiedVectors [ 0 ] [ i - 1 ] = m_RarefiedVectors [ 3 ] [ i - 1 ]; // Handling overlap
	m_RarefiedVectors [ 3 ] [ i + 1 ] -= Coeff * m_RarefiedVectors [ 1 ] [ i ];
	m_RarefiedVectors [ 3 ] [ i ] = 0;
	m_RarefiedVectors [ 1 ] [ i - 1 ] = 0; // Handling overlap
	F [ m_FirstLineNumber ] -= Coeff * F [ m_FirstLineNumber + 1 ];

	/* Handling m_FirstLineNumber + 2 line */
	Coeff = m_RarefiedVectors [ 2 ] [ i ];
	m_RarefiedVectors [ 4 ] [ i ] = 0;
	m_RarefiedVectors [ 2 ] [ i ] = 0; // Handling overlap
	m_RarefiedVectors [ 4 ] [ i - 1 ] -= Coeff * m_RarefiedVectors [ 2 ] [ i - 1 ];
	m_RarefiedVectors [ 4 ] [ i + 1 ] -= Coeff * m_RarefiedVectors [ 1 ] [ i ];
	m_RarefiedVectors [ 0 ] [ i + 1 ] = m_RarefiedVectors [ 4 ] [ i + 1 ]; // Handling overlap
	F [ m_FirstLineNumber + 2 ] -= Coeff * F [ m_FirstLineNumber + 1 ];

	if ( bWithPrint )
	{
		cout << "Step 3: \n\n";

		PrintMatrix();
	}

	/* Step 4 */
	Coeff = 1 / m_RarefiedVectors [ 3 ] [ m_FirstLineNumber ];
	m_RarefiedVectors [ 3 ] [ m_FirstLineNumber ] = 1;
	m_RarefiedVectors [ 0 ] [ m_FirstLineNumber ] = 1; // Overlap
	m_RarefiedVectors [ 3 ] [ m_FirstLineNumber + 2 ] *= Coeff;
	F [ m_FirstLineNumber ] *= Coeff;

	Coeff = m_RarefiedVectors [ 4 ] [ m_FirstLineNumber ];
	m_RarefiedVectors [ 4 ] [ m_FirstLineNumber ] = 0;
	m_RarefiedVectors [ 4 ] [ m_FirstLineNumber + 2 ] -= Coeff * m_RarefiedVectors [ 3 ] [ m_FirstLineNumber + 2 ];
	m_RarefiedVectors [ 0 ] [ m_FirstLineNumber + 2 ] = m_RarefiedVectors [ 4 ] [ m_FirstLineNumber + 2 ];
	F [ m_FirstLineNumber + 2 ] -= Coeff * F [ m_FirstLineNumber ];

	Coeff = 1 / m_RarefiedVectors [ 4 ] [ m_FirstLineNumber + 2 ];
	m_RarefiedVectors [ 4 ] [ m_FirstLineNumber + 2 ] = 1;
	m_RarefiedVectors [ 0 ] [ m_FirstLineNumber + 2 ] = 1; // Overlap
	F [ m_FirstLineNumber + 2 ] *= Coeff;


	if ( bWithPrint )
	{
		cout << "Step 4: \n\n";

		PrintMatrix();
	}

	/* Step 5 */
	Coeff = m_RarefiedVectors [ 2 ] [ m_FirstLineNumber ];
	m_RarefiedVectors [ 2 ] [ m_FirstLineNumber ] = 0;
	m_RarefiedVectors [ 1 ] [ m_FirstLineNumber + 1 ] -= Coeff * m_RarefiedVectors [ 3 ] [ m_FirstLineNumber + 2 ];
	F [ m_FirstLineNumber + 1 ] -= Coeff * F [ m_FirstLineNumber ];

	Coeff = m_RarefiedVectors [ 3 ] [ m_FirstLineNumber + 2 ];
	m_RarefiedVectors [ 3 ] [ m_FirstLineNumber + 2 ] = 0;
	F [ m_FirstLineNumber ] -= F [ m_FirstLineNumber + 2 ] * Coeff;

	if ( bWithPrint )
	{
		cout << "Step 5: \n\n";

		PrintMatrix();
	}

	/* Step 6 */
	X_Found [ m_FirstLineNumber + 2 ] = F [ m_FirstLineNumber + 2 ];
	for ( int i = m_FirstLineNumber + 1; i >= 0; i-- )
	{
		X_Found [ i ] = F [ i ] - m_RarefiedVectors [ 1 ] [ i ] * X_Found [ i + 1 ];
	}

	/* Step 7 */
	for ( int i = m_FirstLineNumber + 3; i < m_MatrixSize; i++ )
	{
		X_Found [ i ] = F [ i ] - m_RarefiedVectors [ 2 ] [ i -1 ] * X_Found [ i - 1 ];
	}

	/* Print only if matrices are quite small or we are testing unit vector */
	if( m_MatrixSize <= MAX_SIZE_TO_PRINT_VECTOR || TEST_UNIT_VECTOR )
	{
		cout << "Generated X:\n";
		PrintVector( X_Generated );

		cout << "Found X:\n";
		PrintVector( X_Found );
	}

	double MaxTolerance = 0;

	for ( int i = 0; i < m_MatrixSize; i++ )
		if ( abs( X_Generated [ i ] - X_Found [ i ] ) > MaxTolerance )
			MaxTolerance = abs( X_Generated [ i ] - X_Found [ i ] );

	cout.precision( 6 );
	cout << "Maximum tolerance between X generated and found : \n" << MaxTolerance << "\n\n";
}