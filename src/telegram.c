#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <curl/curl.h>
#include <wiringPi.h>
#include <jansson.h>

const char* CHAT_ID = "1013618467";
const char* BOT_TOKEN = "6346084597:AAHbSRZ5xBFKGQWpZ-CpbHUOMwVKQUlv9VI";

int leds[] = {16, 15, 0, 1, 2, 4, 3, 5};
int bits[8];
long int speed = 30;
char welcome_message[1000];

void displayLeds(int data);
void setupLeds();
void turnOffLeds();
void intToBinary(int data);
void turnOffLeds();

extern void knightRiderASM();
extern void policeLightASM();
extern void crashASM();
extern void raceASM();
extern void wavesASM();

struct ResponseData 
{
    char* data;
    size_t size;
};

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

void turnOffLeds()
{
    for (int i = 0; i < 8; i++) {
        digitalWrite(leds[i], LOW);
    }
}

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) 
{
    struct ResponseData* response = (struct ResponseData*)userp;

    size_t realsize = size * nmemb;
    response->data = realloc(response->data, response->size + realsize + 1);
    
    if (response->data == NULL) {
        fprintf(stderr, "Error de memoria\n");
        return 0;
    }

    memcpy(response->data + response->size, contents, realsize);
    response->size += realsize;
    response->data[response->size] = '\0';

    return realsize;
}

char* send_telegram_request(const char* method, const char* parameters) 
{
    struct ResponseData response_data = { NULL, 0 };

    CURL* curl = curl_easy_init();


    if (curl) {
        char url[1000];
        sprintf(url, "https://api.telegram.org/bot%s/%s", BOT_TOKEN, method);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        if (parameters != NULL && strlen(parameters) > 0) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, parameters);
        }

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Error al realizar la solicitud: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    return response_data.data;
}

void send_telegram_message(const char* text) 
{
    char parameters[1000];
    sprintf(parameters, "chat_id=%s&text=%s", CHAT_ID, text);
    send_telegram_request("sendMessage", parameters);
}

void handle_message(const char* text) 
{
    if (strcmp(text, "/1") == 0) {
        printf("Ejecutando el auto fantastico\n");
        send_telegram_message("Ejecutando el Auto fantastico 🚘");
        send_telegram_message(welcome_message);
        knightRiderASM();
        turnOffLeds();
    } else if (strcmp(text, "/2") == 0) {
        printf("Ejecutando el choque\n");
        send_telegram_message("Ejecutando el choque 💥");
        send_telegram_message(welcome_message);
        crashASM();
        turnOffLeds();
    } else if (strcmp(text, "/3") == 0) {
        printf("Ejecutando la carrera\n");
        send_telegram_message("Ejecutando la carrera 🏁");
        send_telegram_message(welcome_message);
        raceASM();
        turnOffLeds();
    } else if (strcmp(text, "/4") == 0) {
        printf("Ejecutando luces policiales\n");
        send_telegram_message("Ejecutando luces policiales 🚨");
        send_telegram_message(welcome_message);
        policeLightASM();
        turnOffLeds();
    } else if (strcmp(text, "/5") == 0) {
        printf("Ejecutando la ola\n");
        send_telegram_message("Ejecutando la ola 🏖️");
        send_telegram_message(welcome_message);
        wavesASM();
        turnOffLeds();
    } else {
        printf("Mensaje no reconocido\n");
        send_telegram_message("Mensaje no reconocido ");
    }
}

void process_telegram_updates() 
{    
    int last_update_id = 0;

    char* response = NULL;

    int clear = 0;

    while (1) {
        char parameters[1000];
        sprintf(parameters, "offset=%d", last_update_id + 1);
        char* response = send_telegram_request("getUpdates", parameters);

        
        if (!response) {
            fprintf(stderr, "Error al obtener las actualizaciones de mensajes\n");
            break;
        }

        json_t* root = json_loads(response, 0, NULL);

        if (!root) {
            fprintf(stderr, "Error al analizar la respuesta JSON\n");
            break;
        }

        json_t* result = json_object_get(root, "result");
        
        size_t index;
        json_t* update;
        json_array_foreach(result, index, update) {
            json_t* update_id = json_object_get(update, "update_id");
            int current_update_id = json_integer_value(update_id);
            
            if (current_update_id > last_update_id) {
                last_update_id = current_update_id;
            }

            json_t* message = json_object_get(update, "message");

            json_t* chat = json_object_get(message, "chat");
            json_t* text = json_object_get(message, "text");
            
            const char* message_text = json_string_value(text);
            handle_message(message_text);
        }

        json_decref(root);

    }
}

void disp_binary(unsigned int data)
{   
    int i = 0;
    
    for(int t=128; t>0; t=t/2)
    {  
        delay(speed);

        if(data & t)
        {
            digitalWrite(leds[i], HIGH);
        }
        else 
        {
        digitalWrite(leds[i], LOW);
        }
        i++;
    }
}           

int main() 
{
    setupLeds();
    turnOffLeds();
    sprintf(welcome_message, "__ 🤖  ¡BIENVENIDOS!  🤖 __ \n\n"
                               "👇  Selecciona una secuencia  👇\n\n"
                               "🚘  /1 El auto fantastico\n"
                               "💥  /2 El choque\n"
                               "🏁  /3 La carrera\n"
                               "🚨  /4 Luces policiales\n"\
                               "🏖️  /5 La ola");
    send_telegram_message(welcome_message);
    process_telegram_updates();
    return 0;
}
