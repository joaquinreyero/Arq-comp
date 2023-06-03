#include <iostream>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ncurses.h>
#include <cstring> 
#include <cstdlib> 

unsigned long int speed = 150000000;
int GPIOpins[]= {8,10,11,12,13,15,16,18};
int bits[8];

extern "C" void knightRiderA();
extern "C" void crashA();

bool login();
void menu();
void subMenu();
void delay(unsigned int a);
void displayTerminal(unsigned char data, unsigned long int speed, const std::string& sequenceName);
void intToBinario(int n);
void setupGPIO();
void ledOn(int pin, int estado);
void knightRider(unsigned long int initialSpeed);
void crash(unsigned long int initialSpeed);


bool login() 
{
    const std::string psw = "12345";    
    const int maxAttempts = 3;          
    int attempts = 0;                   

    initscr();  // Inicializar la pantalla de ncurses
    noecho();   // Desactivar la eco de caracteres

    while (attempts < maxAttempts) {
        std::string enteredPsw;
        printw("Ingresa la clave de 5 dígitos: ");
        refresh();

        char c;
        while ((c = getch()) != '\n') {
            if (c == '\b') {
                if (!enteredPsw.empty()) {
                    enteredPsw.pop_back();
                    printw("\b \b");
                    refresh();
                }
            } else {
                enteredPsw += c;
                printw("*");
                refresh();
            }
        }
        printw("\n");

        if (enteredPsw == psw) {
            printw("Clave correcta. Acceso concedido.\n");
            refresh();
            endwin();  
            return true;
        } else {
            printw("Clave incorrecta. Intento fallido.\n");
            refresh();
            attempts++;
        }
    }

    printw("Número máximo de intentos alcanzado. Acceso denegado.\n");
    refresh();
    endwin();  
    return false;
}

void menu() 
{
    int option = -1;

    while (option != 0) {
        std::cout << "=====================" << std::endl;
        std::cout << "      BIENVENIDO      " << std::endl;
        std::cout << "=====================" << std::endl;
        std::cout << "1. El auto fantástico" << std::endl;
        std::cout << "2. El choque" << std::endl;
        std::cout << "3. Opción 3" << std::endl;
        std::cout << "4. Opción 4" << std::endl;
        std::cout << "5. Opción 5" << std::endl;
        std::cout << "0. Salir" << std::endl;
        std::cout << "=====================" << std::endl;
        std::cout << "Ingrese una opción: ";
        std::cin >> option;

        switch (option) {
            case 1:
                std::cout << "\n*** Ejecutando el auto fantástico ***\n" << std::endl;
                knightRider(speed);
                break;
            case 2:
                std::cout << "\n*** Ejecutando el choque ***\n" << std::endl;
                crash(speed);
                break;
            case 3:
                std::cout << "\n*** Opción 3 seleccionada ***\n" << std::endl;
                //opcion3();
                break;
            case 4:
                std::cout << "\n*** Opción 4 seleccionada ***\n" << std::endl;
                //opcion4();
                break;
            case 5:
                std::cout << "\n*** Opción 5 seleccionada ***\n" << std::endl;
                //opcion5();
                break;                
            case 0:
                std::cout << "\nSaliendo del programa..." << std::endl;
                break;
            default:
                std::cout << "\nOpción inválida. Intente nuevamente.\n" << std::endl;
                break;
        }

        std::cout << std::endl;
    }
}

void subMenu(int sequence)
{
    int option = -1;

    while (option != 0) {
        std::cout << "=====================" << std::endl;
        std::cout << "  Seleccione una plataforma:  " << std::endl;
        std::cout << "=====================" << std::endl;
        std::cout << "1. Terminal" << std::endl;
        std::cout << "2. Leds" << std::endl;
        std::cout << "0. Salir" << std::endl;
        std::cout << "=====================" << std::endl;
        std::cout << "Ingrese una opción: ";
        std::cin >> option;
        std::cout << "\n*** Ejecutando el auto fantástico ***\n" << std::endl;

        switch (option) 
        {
            case 1:
                switch (sequence)
                {
                    case 1:
                        knightRider(speed);
                        break;
                    case 2:
                        crash(speed);
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    default:
                        std::cout << "\nOpción inválida.\n" << std::endl;
                        menu();
                        break;
                }
                break;

            case 2:
                switch (sequence)
                    {
                        case 1:
                            knightRiderA();
                            intToBinario(0);
                            break;
                        case 2:
                            crashA();
                            intToBinario(0);
                            break;
                        case 3:
                            break;
                        case 4:
                            break;
                        case 5:
                            break;
                        default:
                            std::cout << "\nOpción inválida.\n" << std::endl;
                            menu();
                            break;
                    }
                break;
            case 0:
                std::cout << "\nVolviendo al Menu..." << std::endl;
                menu();
                break;
            default:
                std::cout << "\nOpción inválida. Intente nuevamente.\n" << std::endl;
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

void displayTerminal(unsigned char data, unsigned long int speed, const std::string& sequenceName) 
{
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

void intToBinario(int n)
{
    int bits[8];
    int i = 7;
    while (i != -1) {
        bits[i] = n % 2;
        n = n / 2;
        i = i - 1;
    }
    for (int x = 0; x < 8; x++) {
        ledOn(GPIOpins[x], bits[x]);
    }
}

void setupGPIO()
{
    system("gpio -g mode 8 output");
	system("gpio -g mode 10 output");
	system("gpio -g mode 11 output");
	system("gpio -g mode 12 output");
	system("gpio -g mode 13 output");
	system("gpio -g mode 15 output");
	system("gpio -g mode 16 output");
	system("gpio -g mode 18 output");
}

void ledOn(int pin, int estado)
{
    const int MAX = 100;

    char cadenaRescri[MAX];
    memset(cadenaRescri, 0, sizeof(cadenaRescri));
    sprintf(cadenaRescri, "%u", pin);

    char primera[MAX];
    memset(primera, 0, sizeof(primera));
    memccpy(primera, "gpio -g write ", '\0', sizeof(primera)) - 1;
    memccpy(primera, cadenaRescri, '\0', sizeof(primera));

    char segunda[MAX];
    memset(segunda, 0, sizeof(segunda));
    memccpy(segunda, primera, '\0', sizeof(segunda)) - 1;
    if (estado == 1) {
        memccpy(segunda, " 1", '\0', sizeof(segunda));
    } else {
        memccpy(segunda, " 0", '\0', sizeof(segunda));
    }

    system(segunda);

    memset(primera, 0, sizeof(primera));
    memset(segunda, 0, sizeof(segunda));
    memset(cadenaRescri, 0, sizeof(cadenaRescri));
}

void knightRider(unsigned long int initialSpeed) 
{
    unsigned char data = 0x01;
    initscr();
    curs_set(0); // Ocultar el cursor
    nodelay(stdscr, true); // Habilitar la entrada no bloqueante
    keypad(stdscr, true); // Habilitar el modo de captura de teclas especiales

    while (true) {

        for (int i = 0; i < 7; i++) {
            clear();
            displayTerminal(data, initialSpeed, "KnightRider");

            data = (data << 1) | (data >> 7);

            delay(initialSpeed);

            int key = getch();
            switch (key) {
                case 27: // Tecla Escape
                    endwin();
                    speed = initialSpeed;
                    std::cout << "\n*** Cerrando el auto fantástico ***\n" << std::endl; 
                    menu();
                case KEY_UP: 
                    if (initialSpeed >= 10000000)
                        initialSpeed -= 10000000;
                    break;
                case KEY_DOWN: 
                    initialSpeed += 10000000;
                    break;
            }
        }

        for (int i = 0; i < 7; i++) {
            clear();
            displayTerminal(data, initialSpeed, "KnightRider");

            data = (data >> 1) | (data << 7);

            delay(initialSpeed);

            int key = getch();
            switch (key) {
                case 27: // Tecla Escape
                    endwin();
                    speed = initialSpeed; 
                    std::cout << "\n*** Cerrando el auto fantástico ***\n" << std::endl;
                    menu();
                case KEY_UP: 
                    if (initialSpeed >= 10000000)
                        initialSpeed -= 10000000;
                    break;
                case KEY_DOWN: 
                    initialSpeed += 10000000;
                    break;
            }
        }
    }

    endwin();
}

void crash(unsigned long int initialSpeed) 
{

    unsigned char table []={0x81,0x42,0x24, 0x18};
    initscr();
    curs_set(0); // Ocultar el cursor
    nodelay(stdscr, true); // Habilitar la entrada no bloqueante
    keypad(stdscr, true); // Habilitar el modo de captura de teclas especiales
        
    while (true) 
    {
        for (int i = 0; i < 4; i++)
        {
            clear();
            displayTerminal(table[i], initialSpeed, "Crash");
            delay(initialSpeed);
            int key = getch();
            switch (key) {
                case 27: // Tecla Escape
                    endwin();
                    speed = initialSpeed; 
                    std::cout << "\n*** Cerrando el choque ***\n" << std::endl;
                    menu();
                case KEY_UP: 
                    if (initialSpeed >= 10000000)
                        initialSpeed -= 10000000;
                    break;
                case KEY_DOWN: 
                    initialSpeed += 10000000;
                    break;
            } 
            if (i == 4)
                 delay(initialSpeed);
        }
        for (int i = 3; i > 0; i--)
        {
            clear();
            displayTerminal(table[i], initialSpeed, "Crash");
            delay(initialSpeed);
            int key = getch();
            switch (key) {
                case 27: // Tecla Escape
                    endwin();
                    speed = initialSpeed;
                    std::cout << "\n*** Cerrando el choque ***\n" << std::endl; 
                    menu();
                case KEY_UP: 
                    if (initialSpeed >= 10000000)
                        initialSpeed -= 10000000;
                    break;
                case KEY_DOWN: 
                    initialSpeed += 10000000;
                    break;
            }  
        }    
    }
    
    endwin(); 
}

int main()
{

    bool loguedIn = login();
    
    if (loguedIn){
        setupGPIO();
        menu();
    }
    return 0;
}