#!/bin/bash

PARALELO=codigos/sap.out
SEQUENCIAL=codigos/sa.out

run(){
	for input in nrw1379.tsp pla7397.tsp brd14051.tsp
		do
			echo "Executando o algoritmo sequencial... Arquivo: = $input"
			START=$(date +%s.%N)
			for ((i=1;i<=n;i++)); do
				$SEQUENCIAL -t $temp -a $input
			done
            		END=$(date +%s.%N)
			TIME_SEQ=$(python3 -c "print('{:.2f}'.format(${END} - ${START}))")
			echo "Levou $TIME_SEQ segundos para rodar o $input sequencial"
			echo ""
			for p in 1 2 3 4
				do
					echo "Executando o algoritmo paralelo com $p processos... Input file = $input"
					START=$(date +%s.%N)
					for ((i=1;i<=n;i++)); do
						mpirun -n $p $PARALELO -t $temp -a $input
					done
					END=$(date +%s.%N)
					TIME_PAR=$(python3 -c "print('{:.2f}'.format(${END} - ${START}))")
					echo "Levou $TIME_PAR segundos para rodar o $input paralelo"
					SPEEDUP=$(python3 -c "print('{:.2f}'.format(${TIME_SEQ} / ${TIME_PAR}))")
					echo "o speedup para $input com $p processos foi $SPEEDUP"
					echo ""
				done
		done
}

helpFunction()
{
    echo ""
    echo "Uso: $0 -n "
    echo -e "\t-n numero de vezes que o programa vai executar "
    echo -e "\t-t temperatura inicial "
	echo -e "\t-h ajuda "
}


while getopts "n:t:h" opt
do
   case "$opt" in
    n ) n="$OPTARG" ;;
    t ) temp="$OPTARG" ;;
    h ) helpFunction ;;
    ? ) helpFunction ;;
  esac
done

if [ -z "$n" ]
then
   echo "Parametros vazios";
   helpFunction
fi

echo ""

if [ -f "$PARALELO" ] && [ -f "$SEQUENCIAL" ] ; then
	run
else 
  echo "$PARALELO ou $SEQUENCIAL nao existem. Por favor execute 'make' antes de executar este script."
fi
