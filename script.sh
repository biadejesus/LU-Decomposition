#!/bin/bash

echo iniciando...
    
upcc -network=smp LUlarge.upc -o lularge
upcc -network=smp LUmedium.upc -o lumedium
upcc -network=smp LUsmall.upc -o lusmall

echo compilado com sucesso!

sudo perf stat -o large-1.txt -a -r 10 -e cache-misses,cycles,instructions,branches,mem-loads,branch-misses,mem-stores,power/energy-ram/,power/energy-pkg/ upcrun -n 1 lularge >> saida-large-1.txt
sudo perf stat -o large-2.txt -a -r 10 -e cache-misses,cycles,instructions,branches,mem-loads,branch-misses,mem-stores,power/energy-ram/,power/energy-pkg/ upcrun -n 2 lularge >> saida-large-2.txt 
sudo perf stat -o large-3.txt -a -r 10 -e cache-misses,cycles,instructions,branches,mem-loads,branch-misses,mem-stores,power/energy-ram/,power/energy-pkg/ upcrun -n 3 lularge >> saida-large-3.txt
sudo perf stat -o large-4.txt -a -r 10 -e cache-misses,cycles,instructions,branches,mem-loads,branch-misses,mem-stores,power/energy-ram/,power/energy-pkg/ upcrun -n 4 lularge >> saida-large-4.txt


echo acabou testes large

sudo perf stat -o medium-1.txt -a -r 10 -e cache-misses,cycles,instructions,branches,mem-loads,branch-misses,mem-stores,power/energy-ram/,power/energy-pkg/ upcrun -n 1 lumedium >> saida-medium-1.txt
sudo perf stat -o medium-2.txt -a -r 10 -e cache-misses,cycles,instructions,branches,mem-loads,branch-misses,mem-stores,power/energy-ram/,power/energy-pkg/ upcrun -n 2 lumedium >> saida-medium-2.txt
sudo perf stat -o medium-3.txt -a -r 10 -e cache-misses,cycles,instructions,branches,mem-loads,branch-misses,mem-stores,power/energy-ram/,power/energy-pkg/ upcrun -n 3 lumedium >> saida-medium-3.txt
sudo perf stat -o medium-4.txt -a -r 10 -e cache-misses,cycles,instructions,branches,mem-loads,branch-misses,mem-stores,power/energy-ram/,power/energy-pkg/ upcrun -n 4 lumedium >> saida-medium-4.txt

echo acabou testes medium

sudo perf stat -o small-1.txt -a -r 10 -e cache-misses,cycles,instructions,branches,mem-loads,branch-misses,mem-stores,power/energy-ram/,power/energy-pkg/ upcrun -n 1 lusmall >> saida-small-1.txt
sudo perf stat -o small-2.txt -a -r 10 -e cache-misses,cycles,instructions,branches,mem-loads,branch-misses,mem-stores,power/energy-ram/,power/energy-pkg/ upcrun -n 2 lusmall >> saida-small-2.txt
sudo perf stat -o small-3.txt -a -r 10 -e cache-misses,cycles,instructions,branches,mem-loads,branch-misses,mem-stores,power/energy-ram/,power/energy-pkg/ upcrun -n 3 lusmall >> saida-small-3.txt
sudo perf stat -o small-4.txt -a -r 10 -e cache-misses,cycles,instructions,branches,mem-loads,branch-misses,mem-stores,power/energy-ram/,power/energy-pkg/ upcrun -n 4 lusmall >> saida-small-4.txt

echo acabou testes small
