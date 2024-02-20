#include "Metadata.c"
#include "ABM_Archivo_Datos.c"


int menuGral() {

    int op;

    printf("\n\tMENU PRINCIPAL\n");
    printf("\n1- Menu de Metadatos");
    printf("\n2- Menu de datos");
    printf("\n\n0- Salir\n");
    scanf("%i", &op);

    return op;
}


int main() {

    int op , finMeta = 1;


    leerArchivoMeta();

    do {

        op = menuGral();

        switch (op) {
        case 1:

            if(finMeta != 0) {

                finMeta = menuMeta();
            }
            else {
                printf("No se puede modificar la Metadata\n");
            }
            break;
        case 2:
            menuDatos();
            break;
        case 0:
            printf("\nFinalizando programa...\n\n");
            break;   
        default:

            printf("\nOpcion invalida\n");
            break;
        }


    }while (op != 0);



    return 0;
}