#include <stdio.h>

const int MAX = 100;

void luDecomposition(int **mat, int n)
{
	int lower[n][n], upper[n][n];
	memset(lower, 0, sizeof(lower));
	memset(upper, 0, sizeof(upper));

	// Decomposing matrix into Upper and Lower
	// triangular matrix
	for (int i = 0; i < n; i++) 
	{
		// Upper Triangular
		for (int k = i; k < n; k++)
		{
			// Summation of L(i, j) * U(j, k)
			int sum = 0;
			for (int j = 0; j < i; j++)
				sum += (lower[i][j] * upper[j][k]);

			// Evaluating U(i, k)
			upper[i][k] = mat[i][k] - sum;
		}

		// Lower Triangular
		for (int k = i; k < n; k++) 
		{
			if (i == k)
				lower[i][i] = 1; // Diagonal as 1
			else
			{
				// Summation of L(k, j) * U(j, i)
				int sum = 0;
				for (int j = 0; j < i; j++)
					sum += (lower[k][j] * upper[j][i]);

				// Evaluating L(k, i)
				lower[k][i]
					= (mat[k][i] - sum) / upper[i][i];
			}
		}
	}

	for (int i = 0; i < n; i++) 
	{
		// Lower
		for (int j = 0; j < n; j++)
			printf("\n %d \n", lower[i][j]);

		// Upper
		for (int j = 0; j < n; j++)
			printf("\n %d \n", upper[i][j]);
	}
}

// Driver code
int main()
{
	int **mat;
    mat = (int *)malloc(MAX * sizeof(int));
	mat[1][1]= 2;
    mat[1][2]= -1;
    mat[1][3]= -2;
    mat[2][1]= -4;
    mat[2][2]= 6;
    mat[2][3]= 3;
    mat[3][1]= -4;
    mat[3][2]= -2;
    mat[3][3]= 8;
	luDecomposition(mat, 3);
	return 0;
}
