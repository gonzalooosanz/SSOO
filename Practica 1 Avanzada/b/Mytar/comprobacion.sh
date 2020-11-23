#! /bin/bash
if [ -x mytar ]; then # miro si es ejecutable
   echo "El archivo es ejecutable"
else
   echo "El fichero no existe o no es ejecutable."
   exit 0 # salgo con un 0 (es decir ha habido un error)
fi
if [ -d tmp ]; then # si existe el directorio tmp
   rm -r tmp # Borro el directorio tmp
fi 
mkdir tmp # Creo un nuevo fichero tmp
cd tmp # me cambio al nuevo fichero tmp
touch fichero1.txt # creo el fichero 1
echo "Hola Mundo!" > fichero1.txt
touch fichero2.txt # creo el fichero 2
head -10 /etc/passwd > fichero2.txt
touch fichero3.dat # creo el fichero 3
head -c 1024 /dev/urandom > fichero3.dat
../mytar -c -f filetar.mtar fichero1.txt fichero2.txt fichero3.dat # llamo al programa mytar con los 3 archivos
mkdir out
cp filetar.mtar out # copio el archivo flitar.mtar en el directorio out
cd out
../../mytar -x -f filetar.mtar
diff fichero1.txt ../fichero1.txt && diff fichero2.txt ../fichero2.txt && diff fichero3.dat ../fichero3.dat
if [ $? -eq 0 ]; then # Si todos los archivos son iguales
   cd ../..
   echo "Correcto."
   exit 1
else 
   cd ../..
   echo "No es correcto."
   exit 0
 fi
 
