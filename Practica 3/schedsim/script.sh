!/bin/bash

nameSched=( "RR" "SJF" "FCFS" "PRIO" )

#Preguntamos por el fichero de ejemplo que se desea simular.
echo '¿Qué fichero quieres simular?'
read fichero
#Si el fichero no existe, se informa del error y se vuelve a preguntar por el fichero de ejemplo.
while [ ! -f $fichero ]; do
   echo -e "\nERROR: El fichero no existe.\n"
   echo '¿Qué fichero quires simular?'
   read fichero   
done

#Preguntamos cuantas CPUs deseamos simular.
echo '¿Cuántas CPUs deseas simular?'
echo 'Número máximo de CPUs: 8'
read maxCPUs

#Si el número de CPUs que se desean simular es mayor que 8, se informa del error y se vuelve a preguntar por el número de CPUs que se desean simular.
while [ $maxCPUs -gt 8 ]; do
   echo -e  "\nError: Supera el maximo de CPUs ejecutables (MAXIMO = 8).\n"
   echo '¿Cuántas CPUs deseas simular?(MAXIMO = 8)'
   read maxCPUs
done

#Si existe la carpeta de resultados, se borra y se crea otra vez.
if [ -d solucion ]; then
rm -r solucion
fi
mkdir solucion

#Para cada planificador, se hace una planificación del fichero de ejemplo, con un número de CPus entre 1..maxCPUs.
for item in "${nameSched[@]}"; do
   for (( cpus = 1 ; $cpus<=$maxCPUs; cpus++ )) do
      ./schedsim -q -p -n "$cpus" -s "$item" -i "$fichero"
      for((i=0 ; $i < $cpus ; i++)) do
         mv CPU_$i.log solucion/"$item"-CPU-$i.log
      done
   done
   #Generamos la gráfica 
   cd ../gantt-gplot
      
   for((j=0; $j <$maxCPUs; j++)) do
      ./generate_gantt_chart ../schedsim/solucion/"$item"-CPU-"$j".log
   done
   cd ../schedsim
done
