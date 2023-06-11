#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ncurses.h>
#include <cstring> 
#include <cstdlib> 
#include <wiringPi.h>


unsigned long int speed = 150000000;
int leds[] = {16,15,0,1,2,4,3,5};


bool login();
void menu();
void subMenu();
void delay(unsigned int a);
void displayTerminal(unsigned char data, unsigned long int speed, const std::string& sequenceName);
void displayLeds(unsigned char data);
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

void menu() {
    int option = -1;
    int speed = 0;

    initscr();  
    cbreak();   
    keypad(stdscr, TRUE); 
    noecho();   

    while (option != 0) {
        clear();  
        printw("=====================\n");
        printw("      BIENVENIDO      \n");
        printw("=====================\n");
        printw("1. El auto fantástico\n");
        printw("2. El choque\n");
        printw("3. Opción 3\n");
        printw("4. Opción 4\n");
        printw("5. Opción 5\n");
        printw("0. Salir\n");
        printw("=====================\n");
        printw("Ingrese una opción: ");
        refresh();  

        scanw("%d", &option);  

        switch (option) {
            case 1:
                printw("\n*** Ejecutando el auto fantástico ***\n\n");
                refresh();
                knightRider(speed);
                break;
                return;
            case 2:
                printw("\n*** Ejecutando el choque ***\n\n");
                refresh();
                // crash(speed);
                break;
                return;
            case 3:
                printw("\n*** Opción 3 seleccionada ***\n\n");
                refresh();
                // opcion3();
                break;
                return;
            case 4:
                printw("\n*** Opción 4 seleccionada ***\n\n");
                refresh();
                // opcion4();
                break;
                return;
            case 5:
                printw("\n*** Opción 5 seleccionada ***\n\n");
                refresh();
                // opcion5();
                break;
                return;                
            case 0:
                printw("\nSaliendo del programa...\n");
                refresh();
                break;
                return;
            default:
                printw("\nOpción inválida. Intente nuevamente.\n\n");
                refresh();
                break;
                return;
        }

        printw("\nPresione cualquier tecla para continuar...");
        refresh();
        getch();  // Espera a que se presione una tecla
    }

    endwin();  
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

void displayLeds(unsigned char data)
{
    for (int i = 0; i < 8; i++) {
        if (data & (1 << i)) {
            digitalWrite(leds[i], HIGH);
        } else {
            digitalWrite(leds[i], LOW);
        }
    }

    std::cout << std::endl;
}

void setupLeds()
{
    wiringPiSetup();
    for (int i = 0; i < 8; i++) {
        pinMode(leds[i], OUTPUT);
    }
}

void turnOffLeds()
{
    for (int i = 0; i < 8; i++) {
        digitalWrite(leds[i], low);
    }
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
            displayLeds(data);

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
            displayLeds(data);

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
            displayLeds(table[i]);
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
            displayLeds(table[i]);
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
        setupLeds();
        menu();
    }
    return 0;
}
