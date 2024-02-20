#include "Metadata.h"


struct Metadata {

    int NroCampo;
    char NombreCampo[20];
    char tipo;
    int longitud;

};


void crearArchivoMeta() {

    FILE * fd = fopen("Metadata.dat", "wb");

    if (fd != NULL) {

        printf("\nArchivo Metadata.dat creado con exito!\n");

    }
    fclose(fd);

}


void altaCampo() {

    FILE * fd = fopen("Metadata.dat", "rb+");
    struct Metadata reg;
    int pos = 1;

    if (fd != NULL) {

        fseek(fd,0,0);
        int lecturaExitosa = fread(&reg,sizeof(struct Metadata),1,fd);
        while(lecturaExitosa == 1) {

            pos++;
            lecturaExitosa = fread(&reg,sizeof(struct Metadata),1,fd);            
        }
        fflush(stdin);
        reg.NroCampo = pos;

        printf("\n\tTABLA DE TIPOS DE DATOS Y LONGITUDES\n");
        printf("\n_ char (c) = %i byte\n_ int (i) = %i bytes\n_ float (f) = %i bytes\n_ double (d) = %i bytes\n_ string (s) = cantidad de caracteres (como maximo)\n\n", sizeof(char), sizeof(int), sizeof(float), sizeof(double));

        printf("\nNombre de campo: ");
        fgets(reg.NombreCampo,20,stdin);
        strtok(reg.NombreCampo,"\n");
        printf("\nTipo: ");
        scanf("%c", &reg.tipo);
        printf("\nLongitud: ");
        scanf("%i", &reg.longitud);

        fwrite(&reg, sizeof(struct Metadata), 1, fd);
    }
    fclose(fd);

}

void bajaCampo() {

    FILE *fd = fopen("Metadata.dat", "rb+");

    if(fd != NULL) {
        
        int pos, i = 1, nb;
        printf("\nPosicion del campo que desea borrar?: ");
        scanf("%i",&pos);
        fflush(stdin);

        crearArchivoTemporal("MetaTemporal.dat");

        FILE *temp = fopen("MetaTemporal.dat","rb+");
        
        struct Metadata reg;
        if(temp != NULL) {

            nb = fread(&reg, sizeof(struct Metadata), 1, fd);
            while (nb > 0) {

                if (i != pos) { //copio los registros del archivo menos el registro a borrar en un archivo temporal

                    fwrite(&reg, sizeof(struct Metadata), 1, temp);
                }
                nb = fread(&reg, sizeof(struct Metadata), 1, fd);
                i++;
            }
            
        }
        fclose(fd);
        fd = fopen("Metadata.dat", "wb"); //vuelvo a generar el archivo Metadata.dat vacio
        
        if(fd != NULL) {

            fseek(temp, 0, SEEK_SET);
            i = 1;  // indice para acomodar el nro de campo.
            nb = fread(&reg, sizeof(struct Metadata), 1, temp);
            while (nb > 0) {    //copio los registros del arhivo temporal al archivo de metadata.
                reg.NroCampo = i;
                fwrite(&reg, sizeof(struct Metadata), 1, fd);  
                nb = fread(&reg, sizeof(struct Metadata), 1, temp);
                i++;
                    
            }
            fclose(temp);
            remove("MetaTemporal.dat");   //cierro y elimino el archivo temporal
        }
        
        
    }
    fclose(fd);

}

void ModificarArchivoMeta() {

    FILE *fd = fopen("Metadata.dat", "rb+");
    struct Metadata reg;
    if(fd != NULL) {
        
        //Muestro los campos
        printf("\n# de campo\tNombre\t\tTipo\t\tLongitud\n\n");
        fread(&reg, sizeof(struct Metadata), 1, fd);
        while (!feof(fd)) {
            printf("    %i\t",reg.NroCampo);
            printf(" %15s\t", reg.NombreCampo);
            printf("  %c\t\t", reg.tipo);
            printf("   %i\n", reg.longitud);
            fread(&reg, sizeof(struct Metadata), 1, fd);
        }
        rewind(fd);

        int campoEditar;
        
        printf("\nSeleccione campo a editar\n");
        scanf("%d", &campoEditar);
        fseek(fd, (sizeof(struct Metadata) * (campoEditar - 1)), SEEK_SET);
        

        fread(&reg, sizeof(struct Metadata), 1, fd);
        if (campoEditar == reg.NroCampo) {
            
            printf("\nDATOS ACTUALES DEL CAMPO A EDITAR\n\n");
            printf("Numero de Campo: %d\n", reg.NroCampo);
            printf("Nombre de Campo: %s\n", reg.NombreCampo);
            printf("Tipo: %c\n", reg.tipo);
            printf("Longitud: %d\n", reg.longitud);

            printf("\n\tTABLA DE TIPOS DE DATOS Y LONGITUDES\n");
            printf("\n_ char (c) = %i byte\n_ int (i) = %i bytes\n_ float (f) = %i bytes\n_ double (d) = %i bytes\n_ string (s) = cantidad de caracteres (como maximo)\n\n", sizeof(char), sizeof(int), sizeof(float), sizeof(double));

            fflush(stdin);
            printf("\nIngrese el nuevo nombre de campo: ");
            fgets(reg.NombreCampo, sizeof(reg.NombreCampo), stdin);
            strtok(reg.NombreCampo, "\n");
                
            fflush(stdin);
            printf("Ingrese el nuevo tipo: ");
            scanf("%c", &reg.tipo);

            fflush(stdin);
            printf("Ingrese la nueva longitud: ");
            scanf("%d", &reg.longitud);

            //mueve el puntero al inicio del registro
            fseek(fd, (sizeof(struct Metadata) * (-1)), SEEK_CUR);
            fwrite(&reg, sizeof(struct Metadata), 1, fd);

            printf("Campo modificado.\n");
            
            }else{
                printf("No encontrado.\n");
            }
        
    }else {
        printf("No se pudo abrir el archivo de metadatos.\n");
    }

    fclose(fd);
}



void leerArchivoMeta() {

    FILE *fd = fopen("Metadata.dat", "rb");

    if(fd != NULL) {

        struct Metadata reg;
        printf("\n------------------------ METADATA ------------------------\n");
        printf("\n# de campo\tNombre\t\tTipo\t\tLongitud\n\n");
        
        fread(&reg, sizeof(struct Metadata), 1, fd);
        while (!feof(fd)) {

            printf("    %i\t",reg.NroCampo);
            printf(" %15s\t", reg.NombreCampo);
            printf("  %c\t\t", reg.tipo);
            printf("   %i\n", reg.longitud);
            fread(&reg, sizeof(struct Metadata), 1, fd);
        }

        printf("\n----------------------------------------------------------\n\n\n");

    }
    
    fclose(fd);

}


int menuMeta() {

    int op;

    do {

        printf("\n\tMENU DE METADATA\n");
        printf("\n1- Crear archivo de Metadata");
        printf("\n2- Cargar datos de registro");
        printf("\n3- Eliminar datos de registro");
        printf("\n4- Modificar datos de registro");
        printf("\n5- Leer archivo");
        printf("\n\n0- Salir\n");
        scanf("%i", &op);
        fflush(stdin);
        if(op == 0) {
            char sale;
            printf("\nEsta seguro que desea salir? Los datos no podran volver a ser modificados\n S/N : ");
            scanf("%c",&sale);

            if(sale == 'n' || sale == 'N') {
                op = 6;
            }
        }

        switch (op) {
            case 1:
                crearArchivoMeta();
                break;
            case 2:
                altaCampo();
                break;
            case 3:
                bajaCampo();
                break;
            case 4:
                ModificarArchivoMeta();
                break;
            case 5:
                leerArchivoMeta();
                break;
            case 0:
                printf("\nSaliendo...\n\n");
                break;
            case 6:
                break;        
            default:
                printf("\nOpcion invalida\n");
                break;
            }

    }while(op != 0);

    return op;
}

