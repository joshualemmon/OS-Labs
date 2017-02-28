#include <stdio.h>
#include <pthread.h>

#define ARRAYSIZE 9

int sudoku[9][9];

void *checkrow(void* row)
{
	int* r = (int*)row;
	int countarray[9] = { 0 };
	for(int i = 0; i < ARRAYSIZE; i++)
	{
		if(r[i] != 0)
			countarray[r[i]-1]++;
	} 
	for (int i = 0; i < ARRAYSIZE; i++)
	{
		if (countarray[i] > 1)
			return (void*) 0;
	}
	return (void*) 1;
}

void *checkcol(void* col)
{
	int* c = (int*)col;
	int countarray[9] = { 0 };
	for(int i = 0; i < ARRAYSIZE; i++)
	{
		if(c[i] != 0)
			countarray[c[i]-1]++;
	} 
	for (int i = 0; i < ARRAYSIZE; i++)
	{
		if (countarray[i] > 1)
			return (void*) 0;
	}
	return (void*) 1;
}

void *checksquare(void* square)
{
	int (*s)[ARRAYSIZE/3][ARRAYSIZE/3] = square;
	int countarray[9] = { 0 };
	for(int i = 0; i < ARRAYSIZE/3; i++)
	{
		for(int j = 0; j < ARRAYSIZE/3; j++)
		{
			if((*s)[i][j]!=0)
				countarray[(*s)[i][j]-1]++;
		}
	}
	for (int i = 0; i < ARRAYSIZE; i++)
	{
		if (countarray[i] > 1)
			return (void*) 0;
	}
	return (void*) 1;
}

void readpuzzle(int sudoku[ARRAYSIZE][ARRAYSIZE])
{
	FILE* f = fopen("puzzle.txt", "r");
	if(f)
	{
		for(int i = 0; i < 9; i++)
		{
			for(int j = 0; j < 9; j++)
			{
				fscanf(f,"%d", &sudoku[i][j]);
			}
		}
	}
}
int rows()
{
	void* valid;
	int validrow = 1;
	pthread_t rowthreads[9];
	for(int i = 0; i < 9; i++)
	{
		pthread_create(&rowthreads[i], NULL, checkrow, sudoku[i]);
	}
	for(int i = 0; i < 9; i++)
	{
		pthread_join(rowthreads[i],&valid);
		if(!(int)valid)
			validrow = 0;
	}
	if(validrow)
		printf("Rows are valid\n");
	else 
		printf("Rows are invalid\n");
	return validrow;
}

int cols()
{
	void* valid;
	int validcol = 1;
	pthread_t colthreads[9];
	for(int i = 0; i < 9; i++)
	{
		int column[9];
		for(int j = 0; j < 9; j++)
		{
			column[j] = sudoku[i][j];
		}
		pthread_create(&colthreads[i], NULL, checkcol, column);
	}
	for(int i = 0; i < 9; i++)
	{
		pthread_join(colthreads[i],&valid);
		if(!(int)valid)
			validcol = 0;
	}
	if(validcol)
		printf("Columns are valid\n");
	else 
		printf("Columns are invalid\n");
	return validcol;
}

int squares()
{
	void* valid;
	int validsquare = 1;
	pthread_t squarethreads[9];
	for(int i = 0; i < 9; i++)
	{
		int columncounter = 3*i;
		int rowcounter = i;
		int square[3][3];
		for(int j = 0; j < 3; j++)
		{
			for(int k = 0; k < 3; k++)
			{
				square[j][k] = sudoku[rowcounter][columncounter++];
			}
			columncounter-=3;
			rowcounter++;
		}
		for(int j = 0; j < 3; j++)
		{
			for(int k = 0; k < 3; k++)
			{
				printf("%d ",square[j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}	


	return validsquare;
}

int main()
{	
	readpuzzle(sudoku);

	if(rows() && cols() && squares())
		printf("Solution is valid\n");
	else
		printf("Solution is invalid\n");
	return 0;
}