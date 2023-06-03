#include <wiringPi.h>

int leds[] = {16,15,0,1,2,4,3,5};
int bits[8];

void intToBinario(int n);
void ledsOn(int led, int bit);

void intToBinario(int n) 
{
    int i = 7;
    while (i != -1) {
        bits[i] = n % 2;
        n = n / 2;
        i = i - 1;
    }
    for (int x = 0; x < 8; x++) {
        ledsOn(leds[x], bits[x]);
    }
}

void ledsOn(int led, int bit)
{
	if (bit == 1){
         digitalWrite(led, HIGH);
    }
	else{
        digitalWrite(led, LOW);
		}

}

int main(){
    intToBinario(6);
}
