#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void crearArchivoTemporal(char *nombre) {

    FILE *temp = fopen(nombre,"wb");

    if(temp == NULL) {
        printf("\nError en archivo temporal\n");
    }

    fclose(temp);

}


int crearArchivoDatos() {

    FILE *fd = fopen("Datos.dat", "wb");
    int longitud = 0;
    
    if(fd != NULL) {

        FILE *meta = fopen("Metadata.dat","rb");
        if(meta != NULL) {

            struct Metadata reg;
        
            fread(&reg, sizeof(struct Metadata), 1, meta);

            while(!feof(meta)) {
                longitud+= reg.longitud;
                fread(&reg, sizeof(struct Metadata), 1, meta);
            }
            fclose(meta);

            printf("\nArchivo Datos.dat creado con exito!\n");
            printf("Longitud de registros del archivo de datos = %d Bytes\n", longitud);
        }
        
    }
    
    fclose(fd);

    return longitud;
}

void altaDato(int longReg) {

    crearArchivoTemporal("DatosTemporal.dat");

    FILE *temp = fopen("DatosTemporal.dat","ab");

    if(temp != NULL) {

        FILE *meta = fopen("Metadata.dat", "rb");
        
        if(meta != NULL) {
            
            struct Metadata reg;
            
            fread(&reg, sizeof(struct Metadata), 1, meta);

            while(!feof(meta)) {

                printf("\nIngresar %s\n",reg.NombreCampo);

                char *campo = (char*)malloc(reg.longitud);
                fflush(stdin);
                fgets(campo, reg.longitud, stdin);
                strtok(campo,"\n");
                
                fwrite(campo,reg.longitud, 1, temp);
                if(ferror(temp)){
                    printf("error de escritura");
                }
                fread(&reg, sizeof(struct Metadata), 1, meta);
            }

            fclose(meta);
        }

        fclose(temp);

    }

    //Vuelco los datos del archivo temporal al archivo de datos

    FILE *fd = fopen("Datos.dat","ab");
    if(fd != NULL) {

        FILE *temp = fopen("DatosTemporal.dat","rb");

        if(temp != NULL) {

            char dato[longReg];
            int nb = fread(dato, longReg, 1, temp);
            while(nb > 0) {

                fwrite(dato, longReg, 1, fd);
                nb = fread(dato, longReg, 1, temp);
            }
            fclose(temp);
            remove("DatosTemporal.dat");

        }

        fclose(fd);
    }

}

void bajaDato(int longReg) {

    FILE *fd = fopen("Datos.dat", "rb+");

    if(fd != NULL) {
        
        int pos, i = 1, nb;
        printf("\nPosicion del registro que desea borrar?: ");
        scanf("%i",&pos);
        fflush(stdin);

        crearArchivoTemporal("DatosTemporal.dat");

        FILE *temp = fopen("DatosTemporal.dat","rb+");
        
        char dato[longReg];
        if(temp != NULL) {

            nb = fread(dato, longReg, 1, fd);
            while (nb > 0) {

                if (i != pos) { //copio los registros de datos menos el registro a borrar en un archivo temporal

                    fwrite(dato, longReg, 1, temp);
                }
                nb = fread(dato, longReg, 1, fd);
                i++;
            }
            
        }
        fclose(fd);
        fd = fopen("Datos.dat", "wb"); //vuelvo a generar el archivo Datos.dat vacio
        
        if(fd != NULL) {

            fseek(temp, 0, SEEK_SET);
            
            nb = fread(dato, longReg, 1, temp);
            while (nb > 0) {    //copio los datos del arhivo temporal al archivo de datos.

                fwrite(dato, longReg, 1, fd);  
                nb = fread(dato, longReg, 1, temp);
                    
            }
            fclose(temp);
            remove("DatosTemporal.dat");   //cierro y elimino el archivo temporal
        }
        
        
    }
    fclose(fd);

}


void leerArchivoDatos() {

    FILE *fd = fopen("Datos.dat","rb");

    if(fd != NULL) {

        FILE *meta = fopen("Metadata.dat", "rb");

        if(meta != NULL) {

            struct Metadata reg;
            char dato[20];
            printf("\n");

            fread(&reg, sizeof(struct Metadata), 1, meta);

            while(!feof(meta)) {

                printf("\t%15s\t",reg.NombreCampo);
                fread(&reg, sizeof(struct Metadata), 1, meta);

            }
            printf("\n\n");

            rewind(meta);
            rewind(fd);
            int i = 0;
            while(!feof(fd)) {
                
                i++;
                printf("Registro #%i",i);

                int nb = fread(&reg, sizeof(struct Metadata), 1, meta);
                while(nb > 0) {

                    fread(dato, reg.longitud, 1, fd);
                    strtok(dato,"\n");
                    printf("%15s\t",dato);
                    nb = fread(&reg, sizeof(struct Metadata), 1, meta);
                }
                printf("\n");

                rewind(meta);
                
                fread(dato, 1, 1, fd);
                if (feof(fd)) {
                    break;
                }
                else {
                    fseek(fd, -1, SEEK_CUR);
                }
            }

        }
        fclose(meta);
    }

    fclose(fd);

}


void modificarDato(int longReg) {

    leerArchivoDatos();

    int numDato;
    int i = 1;
    int resultadoBusqueda = 0;

    FILE *fd = fopen("Datos.dat", "rb+");
    if (fd == NULL) {
        printf("Error al abrir el archivo DATOS...\n");
        return;
    }

    FILE *meta = fopen("Metadata.dat", "rb");
    if (meta == NULL) {
        printf("Error al abrir el archivo METADATA...\n");
        fclose(fd);
        return;
    }

    printf("\nIngrese el # de dato que desea modificar: ");
    scanf("%d", &numDato);

    if (numDato < 1) {
        printf("Posicion invalida.\n");
        fclose(meta);
        fclose(fd);
        return;
    }

    char dato[longReg];

    //Busca el dato en archivo datos
    int nb = fread(dato, longReg, 1, fd);
    while (nb > 0) {
        if (numDato == i) {
            resultadoBusqueda = 1;
            break;
        }
        nb = fread(dato, longReg, 1, fd);
        i++;
    }

    if (resultadoBusqueda == 0) {
        printf("Dato no encontrado...\n");
    }
    else {
        printf("\nIngrese nuevos valores para el dato #%d:\n", numDato);


        struct Metadata reg1;    
        fread(&reg1, sizeof(struct Metadata), 1, meta);

        long posicion = (numDato/4-1) * longReg;
        fseek(fd, posicion, SEEK_CUR);

        while(!feof(meta)) {
            printf("\nIngresar %s\n",reg1.NombreCampo);

            char *campo = (char*)malloc(reg1.longitud);
            fflush(stdin);
            fgets(campo, reg1.longitud, stdin);
            strtok(campo,"\n");
            
            // Escribe el nuevo valor en el archivo Datos
            fwrite(campo, reg1.longitud, 1, fd);
            fread(&reg1, sizeof(struct Metadata), 1, meta);
        }

        printf("\nSe ha modificado el dato en la posicion #%d\n", numDato);
    }

    fclose(meta);
    fclose(fd);
}


void menuDatos() {

    int op, longReg;

    do {
        
        printf("\n\tMENU DE DATOS\n");
        printf("\n1- Crear archivo vacio de datos");
        printf("\n2- Alta de datos");
        printf("\n3- Baja de datos");
        printf("\n4- Modificar datos");
        printf("\n5- Leer archivo");
        printf("\n\n0- Salir\n");
        scanf("%i", &op);

        switch (op) {
            case 1:
                longReg = crearArchivoDatos();
                break;
            case 2:
                altaDato(longReg);
                break;
            case 3:
                bajaDato(longReg);
                break;
            case 4:
                modificarDato(longReg);
                break;
            case 5:
                leerArchivoDatos();
                break;
            case 0:
                printf("\nSaliendo...\n\n");
                break;        
            default:

                printf("\nOpcion invalida\n");
                break;
            }

    }while(op != 0);


}