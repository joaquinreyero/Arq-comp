#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

/*
    Dejar de mostrar lo que ingresemos por teclado
*/
void desactivarEcho() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

/*
    Volver a mostrar lo que ingresemos por teclado
*/
void activarEcho() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

/*
    Menu de seleccion de secuencias
*/
void menu(){

    int opcion;

    printf("Menu:\n");
    printf("1. El auto fantástico\n");
    printf("2. El choque\n");
    printf("3. Opcion 3\n");
    printf("4. Opcion 3\n");
    printf("Selecciona una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1:
            printf("El auto fantástico\n");
            break;
        case 2:
            printf("El choque\n");
            break;
        case 3:
            printf("La carrera\n");
            break;
        case 4:
            printf("op4\n");
            break;
        default:
            printf("Opcion invalida\n");
        }
}

/*
    Setea velocidad
*/
void velocidad();
/*  
    Funcion del auto fantastico
*/
void autoFantastico();

/*
    Funcion del choque
*/
void choque();

int main() {
    
    char clave[6]; // +1 para el carácter nulo
    char claveGuardada[6] = "12345"; 
    char c;
    int i = 0;
    int intentos = 0;
    bool claveCorrecta = true;

    for (intentos = 1; intentos <= 3; intentos++) {

        printf("Ingrese su clave de 5 dígitos:\n");

        desactivarEcho(); 

        while ((c = getchar()) != '\n' && c != EOF) {
            if (i < 5) {
                clave[i++] = c;
                printf("*");
            }
        }
        clave[i] = '\0'; // Añadir el carácter nulo al final de la cadena


        if (strlen(clave) == 5) {
            if (strcmp(clave, claveGuardada) == 0) {
                printf("\nBienvenido al Sistema\n");
                break;
            } else {
                printf("\nClave no válida\n");
                 claveCorrecta = false;
            }
        } else {
            printf("\nClave no válida\n");
            claveCorrecta = false;
        }
    }
    
    if (claveCorrecta) {
        activarEcho(); 
        menu();
    }

    return 0;
}