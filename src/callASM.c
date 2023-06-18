#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <wiringPi.h>
#include <string.h>

extern void knightRiderASM();
extern void policeLightASM();
extern void crashASM();
extern void raceASM();
extern void wavesASM();

bool login();
void menuASM();
void delay(unsigned int a);
void displayLeds(int data);
void setupLeds();
void turnOffLeds();
void intToBinary(int data);
void knightRiderASMinC();
void policeLightASMinC();
void crashASMinC();
void raceASMinC();
void wavesASMinC();


int leds[] = {16, 15, 0, 1, 2, 4, 3, 5};
int bits[8];
bool quit;

bool login() 
{
    const char psw[] = "12345";
    const int maxAttempts = 3;
    int attempts = 0;

    initscr(); 
    noecho(); 

    while (attempts < maxAttempts) {
        char enteredPsw[6] = ""; 
        printw("Ingresa la clave de 5 digitos: ");
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

    printw("Número maximo de intentos alcanzado. Acceso denegado.\n");
    refresh();
    endwin();
    return false;
}

void menuASM() 
{
    
    int option = -1;
    
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    echo();
    
    quit = false;

    while (option != 0) {
        echo();
        clear();
        turnOffLeds();
        printw("=====================\n");
        printw("      BIENVENIDO      \n");
        printw("=====================\n");
        printw("1. El auto fantastico\n");
        printw("2. El choque\n");
        printw("3. La carrera\n");
        printw("4. Luces policiales\n");
        printw("5. Las olas \n");
        printw("0. Salir\n");
        printw("=====================\n");
        printw("Ingrese una opcion: ");
        refresh();

        scanw("%d", &option);

        switch (option) {
            case 1:
                printw("\n*** Ejecutando el auto fantastico ***\n\n");
                refresh();
                noecho();
                knightRiderASMinC();
                break;

            case 2:
                printw("\n*** Ejecutando el choque ***\n\n");
                refresh();
                noecho();
                crashASMinC();
                break;

            case 3:
                printw("\n*** Ejecutando la carrera ***\n\n");
                refresh();
                noecho();
                raceASMinC();
                break;

            case 4:
                printw("\n*** Ejectuando luces policiales ***\n\n");
                refresh();
                noecho();
                policeLightASMinC();
                break;

            case 5:
                printw("\n*** Ejecutando las olas ***\n\n");
                refresh();
                noecho();
                wavesASMinC();
                break;

            case 0:
                printw("\nCerrando...\n");
                refresh();
                break;
                return;
                
            default:
                printw("\nOpcion inválida. Intente nuevamente.\n\n");
                refresh();
                break;
        }
        return;
    }

    endwin();
}

void delayT(unsigned long int a)
{
    while (a)
        a--;
}

void setupLeds()
{
    wiringPiSetup();
    for (int i = 0; i < 8; i++) {
        pinMode(leds[i], OUTPUT);
    }
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
                    if (speed >= (1500000))
                        speed -= (1000000);
                    break;
                case KEY_DOWN:
                    speed += (1000000);
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
    menuASM();
    return;
    }

void policeLightASMinC()
{
    while(quit == false){
        policeLightASM();
    }
    menuASM();
    return;    
}

void crashASMinC()
{
    while(quit == false){
        crashASM();
    }
    menuASM();
    return;
}

void raceASMinC()
{
    while(quit == false){
        raceASM();
    }
    menuASM();
    return;
}

void wavesASMinC()
{
    while(quit == false){
        wavesASM();
    }
    menuASM();
    return;
}

int main()
{
    turnOffLeds();

    bool loguedIn = login();
    
    if (loguedIn) {
        
        setupLeds();
        
        int option = -1;
        initscr();
        cbreak();
        keypad(stdscr, TRUE);
        echo();

        menuASM();
    }
    return 0;
}