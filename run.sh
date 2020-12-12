#!/bin/bash

LARGE=lularge
MEDIUM=lumedium
SMALL=lusmall

run(){
	mkdir saidas
	for alg in $LARGE $MEDIUM $SMALL
		do
			echo "Executando o algoritmo $alg com 1 threads..."
			START=$(date +%s.%N)
			for ((i=1;i<=n;i++)); do
				upcrun -n 1 $alg >> $alg-seq.txt
				mv $alg-seq.txt saidas
			done
			END=$(date +%s.%N)
			TIME_SEQ=$(python3 -c "print('{:.2f}'.format(${END} - ${START}))")
			echo ""
			for t in 1 2 3 4
				do
					echo "Executando o algoritmo $alg com $t threads..."
					START=$(date +%s.%N)
					for ((i=1;i<=n;i++)); do
						upcrun -n $t $alg >> $alg-$t.txt
						mv $alg-$t.txt saidas
					done
					END=$(date +%s.%N)
					TIME_PAR=$(python3 -c "print('{:.2f}'.format(${END} - ${START}))")
					echo "Levou $TIME_PAR segundos para rodar o $input paralelo"
					SPEEDUP=$(python3 -c "print('{:.2f}'.format(${TIME_SEQ} / ${TIME_PAR}))")
					echo "o speedup para $alg com $t threads foi $SPEEDUP"
					echo ""
				done
		done
}

helpFunction()
{
    echo ""
    echo "Uso: $0 -n "
    echo -e "\t-n numero de vezes que o programa vai executar "
	echo -e "\t-h ajuda "
}


while getopts "n:t:h" opt
do
   case "$opt" in
    n ) n="$OPTARG" ;;
    h ) helpFunction ;;
    ? ) helpFunction ;;
  esac
done

if [ -z "$n" ]
then
   echo "Parametros vazios";
   helpFunction
   exit
fi

echo ""

if [ -f "$LARGE" ] && [ -f "$MEDIUM" ] && [ -f "$SMALL" ] ; then
	run
else 
  echo "$LARGE $MEDIUM $SMALL nao existem. Por favor execute 'make' antes de executar este script."
fi
