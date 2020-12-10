#!/bin/bash

echo iniciando...
    
upcc -network=smp LUlarge.upc -o lularge
upcc -network=smp LUmedium.upc -o lumedium
upcc -network=smp LUsmall.upc -o lusmall

echo compilado com sucesso!

sudo perf stat -o large-1.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 1 lularge >> matriz-large-1.txt
sudo perf stat -o large-2.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 2 lularge >> matriz-large-2.txt 
sudo perf stat -o large-4.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 4 lularge >> matriz-large-4.txt
sudo perf stat -o large-6.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 6 lularge >> matriz-large-6.txt
sudo perf stat -o large-8.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 8 lularge >> matriz-large-8.txt

echo acabou testes large

sudo perf stat -o medium-1.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 1 lumedium >> matriz-medium-1.txt
sudo perf stat -o medium-2.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 2 lumedium >> matriz-medium-2.txt
sudo perf stat -o medium-4.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 4 lumedium >> matriz-medium-4.txt
sudo perf stat -o medium-6.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 6 lumedium >> matriz-medium-6.txt
sudo perf stat -o medium-8.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 8 lumedium >> matriz-medium-8.txt

echo acabou testes medium

sudo perf stat -o small-1.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 1 lusmall >> matriz-small-1.txt
sudo perf stat -o small-2.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 2 lusmall >> matriz-small-2.txt
sudo perf stat -o small-4.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 4 lusmall >> matriz-small-4.txt
sudo perf stat -o small-6.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 6 lusmall >> matriz-small-6.txt
sudo perf stat -o small-8.txt -B -e cache-misses,cycles,instructions,mem-loads,mem-stores,power/energy-pkg/ upcrun -shared-heap 2GB -n 8 lusmall >> matriz-small-8.txt

echo acabou testes small
