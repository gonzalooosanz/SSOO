
#!/bin/bash

MPOINT="./mount-point" # nuestro directorio donde vamos a realizar nuestro sistema de ficheros

rm -R -f temp
mkdir temp

echo "Copiando fuselib.c en la carpeta temporal..."
cp ./src/fuseLib.c ./temp/fuseLib.c
cp ./temp/fuseLib.c $MPOINT/fuseLib.c
read -p "Press enter..."

echo "Copiando myFS.h en la carpeta temporal..."
cp ./src/myFS.h ./temp/myFS.h
echo "Copiando myFS.h en el FS..."
cp ./temp/myFS.h $MPOINT/myFS.h
read -p "Press enter..."

./my-fsck virtual-disk

diff -q ./src/fuseLib.c $MPOINT/fuseLib.c
diff -q ./src/myFS.h $MPOINT/myFS.h

echo "Truncando fuseLib.c en la carpeta temporal y en el FS de manera que ocupe un bloque de datos menos..."

truncate -s -4096 ./temp/fuseLib.c
truncate -s -4096  $MPOINT/fuseLib.c
echo "Auditando el disco..."
./my-fsck virtual-disk

diff -q ./src/fuseLib.c $MPOINT/fuseLib.c

cp ./src/myFS.c $MPOINT/myFS.c

./my-fsck virtual-disk

diff -q ./src/myFS.c $MPOINT/myFS.c
echo "Truncando myFS.c en la carpeta temporal y en el FS de manera que ocupe un bloque de datos menos..."
truncate -s +8192 ./temp/myFS.h
truncate -s +8192  $MPOINT/myFS.h
echo "Auditando el disco..."
./my-fsck virtual-disk

diff -q ./src/myFS.h $MPOINT/myFS.h
