#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	int c,ret;
	int cin = atoi(argv[2]); // Funcion atoi transforma int a char
	char *buff;
	buff = malloc(1);
	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	/* Read file byte by byte */
	// En la funcion fread para establecer el tamaño de bytes de bloque que usar en cada entrada modificamos el segundo valor
	while ((c = fread(buff, cin,1,file)) != EOF) {
		/* Print byte to stdout */
		ret=fwrite(buff,cin, 1 , stdout);

		if (ret==EOF){
			fclose(file);
			err(3,"fwrite() failed!!");
		}
	}

	fclose(file);
	return 0;
}
