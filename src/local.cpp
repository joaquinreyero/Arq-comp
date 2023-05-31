#include <iostream>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ncurses.h>

bool login () 
{
    struct termios oldSettings, newSettings;
    tcgetattr(STDIN_FILENO, &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= ~ICANON;   // Desactiva el modo de línea canónica
    newSettings.c_lflag &= ~ECHO;     // Desactiva la eco de caracteres
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    const std::string claveGuardada = "12345";  // Clave guardada
    const int intentosMaximos = 3;             // Número máximo de intentos
    int intentos = 0;                          // Contador de intentos

    while (intentos < intentosMaximos) {
        std::string claveIngresada;
        std::cout << "Ingresa la clave de 5 digitos: ";
        std::cin >> claveIngresada;

        if (claveIngresada == claveGuardada) {
            std::cout << "Clave correcta. Acceso concedido." << std::endl;
            tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);  // Restaura la configuración original de la terminal
            return true;
        } else {
            std::cout << "Clave incorrecta. Intento fallido." << std::endl;
            intentos++;
        }
    }

    std::cout << "Número máximo de intentos alcanzado. Acceso denegado." << std::endl;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);  // Restaura la configuración original de la terminal
    return false;
}

void menu () 
{
int opcion = -1;

    while (opcion != 0) {
        std::cout << "Bienvenido" << std::endl;
        std::cout << "1. El auto fantástico\n" << std::endl;
        std::cout << "2. El choque\n" << std::endl;
        std::cout << "3. Opción 3" << std::endl;
        std::cout << "4. Opción 4" << std::endl;
        std::cout << "0. Salir" << std::endl;
        std::cout << "Ingrese una opción: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                //opcion1();
                break;
            case 2:
                //opcion2();
                break;
            case 3:
                //opcion3();
                break;
            case 4:
                //opcion4();
                break;
            case 0:
                std::cout << "Saliendo del programa..." << std::endl;
                break;
            default:
                std::cout << "Opción inválida. Intente nuevamente." << std::endl;
                break;
        }

        std::cout << std::endl;
    }

}

void delay (unsigned long int a) 
{
    while (a)
        a--;
}

void displayTerminal(unsigned char data, unsigned long int speed, const std::string& sequenceName) {
    printw("\n"); 
    printw("======= %s =======\n", sequenceName.c_str()); 
    printw("Velocidad: %lu\n", speed); 
    for (int i = 0; i < 8; i++) {
        if (data & (1 << i))
            printw("*");
        else
            printw("_");
    }
    printw("\n");
    refresh();
}

void knightRider(unsigned long int speed) {
    unsigned char data = 0x01;
    initscr();
    curs_set(0); // Ocultar el cursor
    nodelay(stdscr, true); // Habilitar la entrada no bloqueante
    keypad(stdscr, true); // Habilitar el modo de captura de teclas especiales

    while (true) {

        for (int i = 0; i < 7; i++) {
            clear();
            displayTerminal(data, speed, "KnightRider");

            data = (data << 1) | (data >> 7);

            delay(speed);

            int key = getch();
            switch (key) {
                case 27: // Tecla Escape
                    endwin(); 
                    return;
                case KEY_UP: 
                    if (speed >= 100)
                        speed -= 100;
                    break;
                case KEY_DOWN: 
                    speed += 100;
                    break;
            }
        }

        for (int i = 0; i < 7; i++) {
            clear();
            displayTerminal(data, speed, "KnightRider");

            data = (data >> 1) | (data << 7);

            delay(speed);

            int key = getch();
            switch (key) {
                case 27: // Tecla Escape
                    endwin(); 
                    return;
                case KEY_UP: 
                    if (speed >= 100)
                        speed -= 100;
                    break;
                case KEY_DOWN: 
                    speed += 100;
                    break;
            }
        }
    }

    endwin();
}


int main(){
    knightRider(150000000);
}