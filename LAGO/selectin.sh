#!/bin/bash
#Este programa corre geant4 con las entradas generadas en corsika de la carpeta "entradas", selecciona la entrada con secundarios y corre esas entradas, las que no generaron secundarios simplemente corre otro programa que guardará los datos del shower para obtener el área efectiva.

count=138063
for i in `seq 11 $count`        #
do                      #inicializa el ciclo for

cd /home/galindoa/GEANT4sim/entradas

touch counter
echo "$i">counter


file=""
file="ya$i.dat"

output=$( wc -l $file)
numero=${output:0:2}
#echo $numero

cp $file ya.dat
mv ya.dat /home/galindoa/GEANT4sim/LAGO-build/ya.dat
cd /home/galindoa/GEANT4sim/LAGO-build


if (( numero <= 1 ))
then
  ./noprocesa
#  echo "Archivo no se procesa"
fi

if (( numero > 1 ))
then
  ./OpNovice -m geometry.mac
#  echo "Se corre GEANT4"
fi

cd /home/galindoa/GEANT4sim/entradas

done


