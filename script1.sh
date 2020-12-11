#!/bin/bash

echo iniciando...
    
upcc -network=smp LUlarge.upc -o lularge
upcc -network=smp LUmedium.upc -o lumedium
upcc -network=smp LUsmall.upc -o lusmall

echo compilado com sucesso!

upcrun -shared-heap 4GB -n 1 lularge >> matriz-large-1.txt
upcrun -shared-heap 4GB -n 2 lularge >> matriz-large-2.txt 
upcrun -shared-heap 4GB -n 4 lularge >> matriz-large-4.txt
upcrun -shared-heap 4GB -n 6 lularge >> matriz-large-6.txt
upcrun -shared-heap 4GB -n 8 lularge >> matriz-large-8.txt

echo acabou testes large

upcrun -shared-heap 4GB -n 1 lumedium >> matriz-medium-1.txt
upcrun -shared-heap 4GB -n 2 lumedium >> matriz-medium-2.txt
upcrun -shared-heap 4GB -n 4 lumedium >> matriz-medium-4.txt
upcrun -shared-heap 4GB -n 6 lumedium >> matriz-medium-6.txt
upcrun -shared-heap 4GB -n 8 lumedium >> matriz-medium-8.txt

echo acabou testes medium

upcrun -shared-heap 4GB -n 1 lusmall >> matriz-small-1.txt
upcrun -shared-heap 4GB -n 2 lusmall >> matriz-small-2.txt
upcrun -shared-heap 4GB -n 4 lusmall >> matriz-small-4.txt
upcrun -shared-heap 4GB -n 6 lusmall >> matriz-small-6.txt
upcrun -shared-heap 4GB -n 8 lusmall >> matriz-small-8.txt

echo acabou testes small
