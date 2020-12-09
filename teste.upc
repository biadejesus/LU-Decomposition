#include <upc_relaxed.h>
#include <upc.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

FILE *arq;

shared[] double *matriz;
shared[] double *upper;
shared[] double *lower;

struct timespec ts;

void gerar_matriz(int dim)
{
  clock_gettime(CLOCK_MONOTONIC, &ts);

  srand((time_t)ts.tv_nsec);
  matriz = upc_all_alloc(THREADS, dim * dim * sizeof(double));
  int j, x;

  upc_forall(int i = 0; i < dim * dim; i++; i)
  {
    matriz[i] = rand() % (20);
  }
}

void printar_matriz(int dim)
{

  for (int i = 0; i < dim; i++)
  {
    for (int j = i * dim; j < i * dim + dim; j++)
    {
      printf("%.2f \t", matriz[j]);
    }
    printf("\n");
  }
  printf("\n----------------------\n");
}

void LUDecomposition(int dim)
{

  upper = upc_all_alloc(THREADS, dim * dim * sizeof(double));
  lower = upc_all_alloc(THREADS, dim * dim * sizeof(double));

  // ini = MYTHREAD * ((dim * dim) / THREADS);
  // fim = ini + (((dim * dim) / THREADS));

  upc_forall(int i = 0; i < dim; i++; i) //percorre a matriz M
  {
    for (int j = i; j < dim; j++) //n*j + k
    {
      int sum = 0;
      for (int k = 0; k < i; k++)
      {
        sum += (lower[(k*dim)+i] * upper[(k*dim)+j]);
      }
      upper[(dim * i) + j] = matriz[(dim * i) + j] - sum;
      //upper[i + j + (i * 2)] = matriz[i + j + (i * 2)];
      // printf("\n 1 = %d", (dim*i) +j);
      // printf("\n 2 = %d", i+j+(i*2));
    }

    upc_barrier;

    for (int j = i; j < dim; j++)
    {
      if (i == j)
      {
        lower[(dim*i)+i] = 1; //diagonal
      }
      else
      {
        int sum = 0;
        for (int k = 0; k < i; k++)
        {
          sum += (lower[(j*dim)+k] * upper[(k*dim)+i]);
        }
        lower[(dim * j) + i] = (matriz[(dim * j) + i] - sum) / upper[(dim*i)+i];
      }
    }
  }
  // printf("\n----------------------\n");
  // for (int i = 0; i < dim; i++)
  // {
  //   for (int j = 0; j < dim; j++){
  //     printf(" %f ", upper[i]);
  //   }
  //   printf("\n");
  // }
  for (int i = 0; i < dim; i++)
  {
    for (int j = i * dim; j < i * dim + dim; j++)
    {
      printf("%.2f \t", upper[j]);
    }
    printf("\n");
  }
  printf("-----------------------------------------\n");
  for (int i = 0; i < dim; i++)
  {
    for (int j = i * dim; j < i * dim + dim; j++)
    {
      printf("%.2f \t", lower[j]);
    }
    printf("\n");
  }
  printf("-----------------------------------------\n");
}

int main()
{
  gerar_matriz(3);
  printar_matriz(3);
  LUDecomposition(3);
}