#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int
copynFile(FILE * origin, FILE * destination, int nBytes)
{

	int contador = 0;	
	int ch;
	// ch = getc(origin)// coge el siguiente caracter del fichero origin y lo guarda en ch
	while (contador < nBytes && (ch = getc(origin)) != EOF){//EOF --> final del fichero
	putc((unsigned char) ch,destination); // Escribe el carácter ch en el fichero destination
	contador++;
	}
	return contador;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char*
loadstr(FILE * file)
{
	int n;
	int size = 0;
	char *buf;
	do{
		n = getc(file);
		size++;
	}while((n!= (int)'\0') && (n != EOF));

	if (n == EOF){
		return NULL;
	}
	fseek (file,-size,SEEK_CUR);
	size--;
	buf = (char*) malloc (size);
	fread(buf,1,size,file);
	getc(file);
	return buf;
	
}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry*
readHeader(FILE * tarFile, int *nFiles)
{
	stHeaderEntry* header; //Puntero header a devolver

	//Lee el numero de archivos dentro de mytar
	fread(nFiles,sizeof (int), 1,tarFile);

	//try to alocate memory for header, NULL if fail
	header =(stHeaderEntry *) malloc(sizeof(stHeaderEntry)* (*nFiles));
	if(header == NULL){ // Si es nulo cierra el archivo tarFile
		perror("mtar demasiado grande para asignarlo a memoria");
		fclose(tarFile);
	}
	//alocate header information
	else{
		for(int i = 0; i < *nFiles; i++){
			//Error de control
			if((header[i].name = loadstr(tarFile)) == NULL){
				for(int j = 0; j < i; j++)
					free(header[j].name);
				fclose(tarFile);
				return NULL;
			}
			fread(&header[i].size, sizeof(unsigned int), 1, tarFile);
		}
	}
	return header;
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int
createTar(int nFiles, char *fileNames[], char tarName[])
{
	FILE *tarFile;
	stHeaderEntry *header;
	FILE *inputFile;
	int headerSize;
	if(nFiles <= 0){
		fprintf(stderr,"%s",use);
		return EXIT_FAILURE;
	}
	tarFile= fopen(tarName,"wx");
	headerSize = sizeof(int);
	header = malloc(sizeof(stHeaderEntry)*nFiles);
	for(int i = 0; i < nFiles ; ++i){
		int namesize = strlen(fileNames[i])+1;//+1
		header[i].name = (char*)malloc(namesize);
		stpcpy(header[i].name,fileNames[i]);
		headerSize += namesize + sizeof(header->size);
	}
	// Aqui hace el headerSize asignandole el tam de la cabecera(4 del int mas 4 del header-> int)

	fseek(tarFile,headerSize,SEEK_SET);
	for(int i = 0;i < nFiles;++i){
		inputFile = fopen(fileNames[i],"r");
		header[i].size = copynFile(inputFile,tarFile,INT_MAX);
		fclose(inputFile);
	}

	rewind(tarFile);
	fwrite(&nFiles,sizeof(int),1,tarFile);

	for(int i = 0;i < nFiles; ++i){
		fwrite(header[i].name,sizeof(char),strlen(header[i].name)+1,tarFile);//+1
		//fwrite(&header[i].size,sizeof(header[i].size),1,tarFile);
		fwrite(&header[i].size,sizeof(unsigned int),1,tarFile);
	}
	fprintf(stdout,"mtar file created successfully \n");

	for(int i = 0; i < nFiles; ++i){
		free(header[i].name);
	}

	free(header);
	fclose(tarFile);

	return EXIT_SUCCESS;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int
extractTar(char tarName[])
{

	stHeaderEntry *header; // Puntero de tipo archivo
	FILE *tarFile, *output; // Punteros de tipo archivo
	int nFiles;

	//open tar file
	if((tarFile = fopen(tarName, "r")) == NULL){ // Si el archivo no se abre excepcion
		perror("No se puede abrir el archivo");
		return EXIT_FAILURE;
	}

	// Lee el encabezado y se lo asignan al puntero
	header = readHeader(tarFile, &nFiles); // De aqui cogemos el valor de nFiles
	//creamos y escribimos el archivo con la información
	for(int i = 0; i < nFiles; i++){

		output = fopen(header[i].name, "w");
		copynFile(tarFile,output, header[i].size); // (Antes sin -1) Copiamos lo que hay en el origen (tarFile) y
		//asignamos al fichero destino(output) el numero de bytes que hay en header.size
		fclose(output);
	}

	//fclose(output);// Cerramos el archivo output
	fprintf(stdout, "El archivo ha sido extraido\n");
	//free memory adn close FILE
	free(header);
	fclose(tarFile);

	return EXIT_SUCCESS;

}

int listTar(char tarName[]) {

	stHeaderEntry *header;
	FILE *tarFile;
	int nFiles;



	//abrimos el archivo y si no se puede abrir salta una excepción
	if((tarFile = fopen(tarName, "r")) == NULL){
		perror("No se puede abrir el archivo");
		return 1;
	}

	//Creamos la cabecera
	header = readHeader(tarFile, &nFiles);


	for(int i = nFiles; i >= 0; --i){
		fprintf(stdout, "[%d] Nombre del archivo: %s, ", i, header[i].name);
		fprintf(stdout, "tam: %u bytes \n", header[i].size);
	}
	//Liberamos memoria y cerramos el fichero
	free(header);
	fclose(tarFile);
	return 0;
}


