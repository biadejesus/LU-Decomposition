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
  if (MYTHREAD == 0)
  {
    for (int i = 0; i < dim * dim; i++)
    {
      matriz[i] = (rand() % 20);
    }
  }
}

void printar_matriz(int dim)
{
  if (MYTHREAD == 0)
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
}

void LUDecomposition(int dim)
{

  upper = upc_all_alloc(THREADS, dim * dim * sizeof(double));
  lower = upc_all_alloc(THREADS, dim * dim * sizeof(double));

  int ini = MYTHREAD * ((dim * dim) / THREADS);
  int fim = ini + (((dim * dim) / THREADS) - 1);

  for (int i = 0; i < dim; i++) //percorre a matriz M
  {
    upc_forall(int j = i; j < dim; j++; j)
    {
      double sum = 0;
      for (int k = 0; k < i; k++)
      {
        sum += (lower[(i * dim) + k] * upper[(k * dim) + j]);
      }
      upper[(dim * i) + j] = matriz[(dim * i) + j] - sum;
    }

    upc_barrier;

    upc_forall(int j = i; j < dim; j++; j)
    {
      if (i == j)
      {
        lower[(dim * i) + i] = 1; //diagonal
      }
      else
      {
        double sum = 0;
        for (int k = 0; k < i; k++)
        {
          sum += (lower[(j * dim) + k] * upper[(k * dim) + i]);
        }
        if (upper[(dim * i) + i] == 0)
        {
          printf("\nDivisao por zero!");
          exit(EXIT_FAILURE);
        }
        lower[(dim * j) + i] = ((matriz[(dim * j) + i] - sum) / upper[(dim * i) + i]);
      }
    }

    upc_barrier;
  }
  if (MYTHREAD == 0)
  {
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
  }
}

int main()
{
  gerar_matriz(3);
  upc_barrier;
  printar_matriz(3);
  LUDecomposition(3);
}