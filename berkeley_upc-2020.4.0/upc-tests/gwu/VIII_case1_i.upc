/*
UPC Testing Suite

Copyright (C) 2000 Chen Jianxun, Sebastien Chauvin, Tarek El-Ghazawi

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
/**
   Test: VIII_case1_i   - forall test.
   Purpose:
         To check that only the thread, whose number is specified 
         in this field, will execute the actual iteration of the 
         forall loop.
   Type: Positive.
   How : - In the forall statement, check that thread, which is executing
           the current iteration, is the one specified by the integer 
           expression. If (MYTHREAD!=expression%THREADS), an error message
           should be returned. 
*/


#include <stdio.h>
#include <errno.h>
#include <upc.h>

shared int a[THREADS];

int
main()
{
  unsigned // works-around a PGI compiler bug
  int i,pe=MYTHREAD;
  int sum=0;
  int errflag=0;

  upc_forall(i=0; i<THREADS; i++; i)
    if (pe!=i%THREADS) {
      errflag=1;
    } 
  
  if (errflag) {
      printf("Failure: on Thread %d with errflag %d\n",MYTHREAD,errflag);
  } else if (MYTHREAD == 0) {
      printf("Success: on Thread %d \n",MYTHREAD);
  }

  return(errflag); 
}

