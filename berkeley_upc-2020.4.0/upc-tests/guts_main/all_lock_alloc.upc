/*
 * The GWU Unified Testing Suite (GUTS) 
 * Latest modifications and integration to GUTS framework
 *  
 * Copyright (C) 2007 ... Abdullah Kayi
 * Copyright (C) 2007 ... Tarek El-Ghazawi 
 * Copyright (C) 2007 ... The George Washington University
 *
 * ---------------------------------------------------------------------------
 *
 * UPC Testing Suite Original Development
 *
 * Copyright (C) 2003 ... Veysel Baydogan, Proshanta Saha, Tarek El-Ghazawi
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <upc.h>
#include <gula.h>
#include <stdio.h>

upc_lock_t *lock1;
upc_lock_t *lock2;
upc_lock_t * shared all_locks[THREADS];
shared int a;

int
main()
{
    /*
     * To check that the upc_all_lock_alloc function dynamically 
     * allocates a lock and returns the same pointer to all threads
     */

    int i;

    lock1 = upc_all_lock_alloc();

    if(lock1 == NULL)
        GULA_FAIL("failed to allocate a lock");

    all_locks[MYTHREAD] = lock1;
    upc_barrier;

    if(MYTHREAD == 0) {
        for(i=0;i<THREADS-1;i++) {
            if(all_locks[i] != all_locks[i+1])
                GULA_FAIL("failed to return same lock pointer");
        }
    }

    upc_barrier;

    /*
     * To check that the upc_all_lock_alloc function automatically initializes the lock it allocates
     */

    int attempt;
    int local_a=MYTHREAD + 55;

    lock2 = upc_all_lock_alloc();

    if(lock2 == NULL)
        GULA_FAIL("failed to allocate a lock");

    if(MYTHREAD == 0) {
        upc_lock(lock2);
        a = local_a;
    }
    upc_barrier;

    if(MYTHREAD != 0) {
        attempt = upc_lock_attempt(lock2);
        if(attempt == 1)
            GULA_FAIL("lock2 failed to lock");
    }

    upc_barrier;

    if(MYTHREAD == 0)
        upc_unlock(lock2);

#ifdef BUPC_TEST_HARNESS
    upc_barrier;

    if (!MYTHREAD)
        printf ("Passed.\n");

#endif /* BUPC_TEST_HARNESS */
    return 0;
}