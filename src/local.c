#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef MAX  
#define MAX 10000
#endif

extern void knightRiderA();
extern void crashA();

char cadenaRemove[MAX];
char cadenaRescri[MAX];
char primera[MAX];
char segunda[MAX];

int pines[]={8,10,11,12,13,15,16,18};
long int GlobalID = 0;

int bits[8];

void intToBinario(int n){
    int i=7;
    while(i !=-1){
        bits[i]=n%2;  
        n=n/2; 
        i=i-1;
    }
    for (int x = 0; x<8; x++){
        EncenderLuces(pines[x], bits[x]);
    }
}
void EncenderLuces(int pin, int estado){
    memset(cadenaRescri,0,MAX);
    sprintf(cadenaRescri, "%u", pin);
    memccpy(memccpy(primera, "gpio -g write ", '\0', MAX) - 1, cadenaRescri, '\0', MAX);
	if (estado==1){
        memccpy(memccpy(segunda, primera, '\0', MAX) - 1, " 1", '\0', MAX);
    }
	else{
        memccpy(memccpy(segunda, primera, '\0', MAX) - 1, " 0", '\0', MAX);
		}
	system(segunda);
	memset(primera,0,MAX);
	memset(segunda,0,MAX);
	memset(cadenaRescri,0,MAX);
	}

int main(){
    system("gpio -g mode 8 output");
	system("gpio -g mode 10 output");
	system("gpio -g mode 11 output");
	system("gpio -g mode 12 output");
	system("gpio -g mode 13 output");
	system("gpio -g mode 15 output");
	system("gpio -g mode 16 output");
	system("gpio -g mode 18 output");

    knightRiderA();
    intToBinario(0);



}