#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <wiringPi.h>
#include <string.h>

unsigned long int speed = 15000000;
int leds[] = {16, 15, 0, 1, 2, 4, 3, 5};
int bits[8];
bool quit;

extern void knightRiderASM();
//extern void crashASM();
extern void crashA();
//extern void Carrera();

bool login();
void menu();
void delay(unsigned int a);
void displayTerminal(unsigned char data, unsigned long int speed, const char* sequenceName);
void displayLeds(int data);
void setupLeds();
void turnOffLeds();
void knightRider(unsigned long int initialSpeed);
void crash(unsigned long int initialSpeed);
void intToBinary(int data);

bool login() 
{
    const char psw[] = "12345";
    const int maxAttempts = 3;
    int attempts = 0;

    initscr(); 
    noecho(); 

    while (attempts < maxAttempts) {
        char enteredPsw[6] = ""; 
        printw("Ingresa la clave de 5 dígitos: ");
        refresh();

        char c;
        while ((c = getch()) != '\n') {
            if (c == '\b') {
                if (strlen(enteredPsw) > 0) {
                    enteredPsw[strlen(enteredPsw) - 1] = '\0';
                    printw("\b \b");
                    refresh();
                }
            } else {
                strncat(enteredPsw, &c, 1); 
                printw("*");
                refresh();
            }
        }
        printw("\n");

        if (strcmp(enteredPsw, psw) == 0) {
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
    
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    echo();

    while (option != 0) {
        echo();
        clear();
        turnOffLeds();
        printw("=====================\n");
        printw("      BIENVENIDO      \n");
        printw("=====================\n");
        printw("1. El auto fantástico\n");
        printw("2. El choque\n");
        printw("3. Opción 3\n");
        printw("4. Opción 4\n");
        printw("5. Opción 5\n");
        printw("0. Regresar al menu principal\n");
        printw("=====================\n");
        printw("Ingrese una opción: ");
        refresh();

        scanw("%d", &option);

        switch (option) {
            case 1:
                printw("\n*** Ejecutando el auto fantástico ***\n\n");
                refresh();
                noecho();
                knightRider(speed);
                break;

            case 2:
                printw("\n*** Ejecutando el choque ***\n\n");
                refresh();
                noecho();
                crash(speed);
                break;

            case 3:
                printw("\n*** Opción 3 seleccionada ***\n\n");
                refresh();
                noecho();
                // opcion3();
                break;

            case 4:
                printw("\n*** Opción 4 seleccionada ***\n\n");
                refresh();
                noecho();
                // opcion4();
                break;

            case 5:
                printw("\n*** Opción 5 seleccionada ***\n\n");
                refresh();
                noecho();
                // opcion5();
                break;

            case 0:
                printw("\nRegresando al menu principal...\n");
                refresh();
                break;

            default:
                printw("\nOpción inválida. Intente nuevamente.\n\n");
                refresh();
                break;
        }
        
        printw("\nPresione cualquier tecla para continuar...");
        refresh();
        getch();
    }

    endwin();
}

void menuASM() 
{
    
    int option = -1;
    
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    echo();

    while (option != 0) {
        echo();
        clear();
        turnOffLeds();
        printw("=====================\n");
        printw("      BIENVENIDO      \n");
        printw("=====================\n");
        printw("1. El auto fantástico\n");
        printw("2. El choque\n");
        printw("3. Opción 3\n");
        printw("4. Opción 4\n");
        printw("5. Opción 5\n");
        printw("0. Regresar al menu principal\n");
        printw("=====================\n");
        printw("Ingrese una opción: ");
        refresh();

        scanw("%d", &option);

        switch (option) {
            case 1:
                printw("\n*** Ejecutando el auto fantástico ***\n\n");
                refresh();
                noecho();
                knightRiderASM();
                break;

            case 2:
                printw("\n*** Ejecutando el choque ***\n\n");
                refresh();
                noecho();
                break;

            case 3:
                printw("\n*** Opción 3 seleccionada ***\n\n");
                refresh();
                noecho();
                // opcion3();
                break;

            case 4:
                printw("\n*** Opción 4 seleccionada ***\n\n");
                refresh();
                noecho();
                // opcion4();
                break;

            case 5:
                printw("\n*** Opción 5 seleccionada ***\n\n");
                refresh();
                noecho();
                // opcion5();
                break;

            case 0:
                printw("\nRegresando al menu principal...\n");
                refresh();
                break;

            default:
                printw("\nOpción inválida. Intente nuevamente.\n\n");
                refresh();
                break;
        }
        
        printw("\nPresione cualquier tecla para continuar...");
        refresh();
        getch();
    }

    endwin();
}

void delayT(unsigned long int a)
{
    while (a)
        a--;
}

void displayTerminal(unsigned char data, unsigned long int speed, const char* sequenceName) 
{
    clear();
    printw("======= %s =======\n", sequenceName);
    printw("Velocidad: %lu\n", speed);
    for (int i = 0; i < 8; i++) {
        if (data & (1 << i))
            printw("*");
        else
            printw("_");
    }
    refresh();
}

void displayLeds(int data)
{   
    
    for (int i = 0; i < 8; i++) {
        if (data & (1 << i)) {
            digitalWrite(leds[i], HIGH);
        } else {
            digitalWrite(leds[i], LOW);
        }
    }
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
        digitalWrite(leds[i], LOW);
    }
}

void knightRider(unsigned long int initialSpeed)
{
    unsigned char data = 0x01;

    initscr();
    curs_set(0); 
    nodelay(stdscr, TRUE); 
    keypad(stdscr, TRUE); 

    while (1) {

        for (int i = 0; i < 7; i++) {

            displayTerminal(data, initialSpeed, "KnightRider");
            displayLeds(data);
            data = (data << 1) | (data >> 7);
            delayT(initialSpeed);

            int key = getch();

            switch (key) {
                case 27:
                    speed = initialSpeed;
                    curs_set(1);
                    nodelay(stdscr, FALSE);
                    keypad(stdscr, FALSE);
                    endwin();
                    return;

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
            displayTerminal(data, initialSpeed, "KnightRider");
            displayLeds(data);
            data = (data >> 1) | (data << 7);
            delayT(initialSpeed);

            int key = getch();
            switch (key) {
                case 27:
                    speed = initialSpeed;
                    curs_set(1);
                    nodelay(stdscr, FALSE);
                    keypad(stdscr, FALSE);
                    endwin();
                    return;

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
}

void crash(unsigned long int initialSpeed)
{

    unsigned char table[] = {0x81, 0x42, 0x24, 0x18};
    initscr();
    curs_set(0); 
    nodelay(stdscr, TRUE); 
    keypad(stdscr, TRUE); 

    while (1) {

        for (int i = 0; i < 4; i++) {
            displayTerminal(table[i], initialSpeed, "Crash");
            displayLeds(table[i]);
            delayT(initialSpeed);
            int key = getch();

            switch (key) {
                case 27:
                    speed = initialSpeed;
                    curs_set(1);
                    nodelay(stdscr, FALSE);
                    keypad(stdscr, FALSE);
                    endwin();
                    return;
                case KEY_UP:
                    if (initialSpeed >= 10000000)
                        initialSpeed -= 10000000;
                    break;
                case KEY_DOWN:
                    initialSpeed += 10000000;
                    break;
            }
            if (i == 4)
                delayT(initialSpeed);
        }

        for (int i = 3; i > 0; i--) {
            displayTerminal(table[i], initialSpeed, "Crash");
            displayLeds(table[i]);
            delayT(initialSpeed);
            int key = getch();
            switch (key) {
                case 27:
                    speed = initialSpeed;
                    curs_set(1);
                    nodelay(stdscr, FALSE);
                    keypad(stdscr, FALSE);
                    endwin();
                    return;
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

void disp_binary(unsigned int data)
{   
    initscr();
    curs_set(0); 
    nodelay(stdscr, TRUE); 
    keypad(stdscr, TRUE);
    
    int i =0;
    clear();
    
    printw("Velocidad: %lu\n", speed);
    
    int key = getch();
    
    switch (key) {
                case 27:
                    curs_set(1);
                    nodelay(stdscr, FALSE);
                    keypad(stdscr, FALSE);
                    endwin();
                    quit = true;
                    return;
                    
                case KEY_UP:
                    if (speed >= 10000000)
                        speed -= 10000000;
                    break;
                case KEY_DOWN:
                    speed += 10000000;
                    break;
            }
    if (quit == false)
    {
        for(int t=128; t>0; t=t/2){
            
            delayT(speed);
            
            if(data & t)
            {
                digitalWrite(leds[i], HIGH);
                printw("*");
            }
            else 
            {
                digitalWrite(leds[i], LOW);
                printw("_");
            }
            i++;
                        
            refresh();
        }
    }
    else
    {
        curs_set(1);
        nodelay(stdscr, FALSE);
        keypad(stdscr, FALSE);
        endwin();
        quit = true;
        return;
    }
            
}

void knightRiderASMinC()
{
    while(quit == false){
        knightRiderASM();
    }
    menu();
    return;
}

void crashASMinC()
{
    while(quit == false){
        crashA();
    }
    menu();
    return;
}

int main()
{   
    bool loguedIn = login();
    
    if (loguedIn) {
        
        setupLeds();
        
        int option = -1;
        initscr();
        cbreak();
        keypad(stdscr, TRUE);
        echo();

        while (option != 0) {
            echo();
            clear();
            turnOffLeds();
            printw("==========================================\n");
            printw("      Desea ejecutar atravez de C o ASM      \n");
            printw("==========================================\n");
            printw("1. C \n");
            printw("2. ASM \n");
            printw("0. Ingrese 0 para salir.      \n");
            refresh();

            scanw("%d", &option);

            switch (option) {
                case 1:
                    printw("\n*** Ejecutando en C ***\n\n");
                    refresh();
                    menu();
                    break;

                case 2:
                    printw("\n*** Ejecutando en ASM ***\n\n");
                    refresh();
                    menuASM();
                    break;
                case 0:
                    printw("\nSaliendo del programa...\n");
                    refresh();
                    echo();
                    endwin();
                    break;
                default:
                    printw("\nOpción inválida. Intente nuevamente.\n\n");
                    refresh();
                    break;
            
            }
        }
    }
    return 0;
}

