#include "stdafx.h"
#include <time.h>
#include "RarefiedMatrix.h"

int main()
{
	srand( time( nullptr ) );

	RarefiedMatrix MyMatrix( 10, 5, -10, 10 );

	MyMatrix.PrintMatrix();

	MyMatrix.Solve( false );
	
	system( "pause" );
	return 0;
}

