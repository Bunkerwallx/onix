


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <telegram-bot-authenticate.h>

// Función de callback para recibir los datos de la respuesta HTTP
size_t write_callback(void *content, size_t size, size_t nmemb, void *userp) {
    // No hacemos nada con los datos recibidos en este ejemplo
    return size * nmemb;
}

int main() {
    // Configurar los datos de autenticación del bot de Telegram
    const char *token = "TOKEN_DEL_BOT_DE_TELEGRAM";

    // Crear una instancia del objeto de autenticación
    TelegramBotAuthenticate *auth = telegram_bot_authenticate_new(token);

    // Obtener la dirección y el User-Agent desde la entrada del usuario
    char ip_address[100];
    char user_agent[100];

    printf("Ingrese la dirección IP: ");
    fgets(ip_address, sizeof(ip_address), stdin);
    ip_address[strcspn(ip_address, "\n")] = '\0';

    printf("Ingrese el User-Agent de iOS: ");
    fgets(user_agent, sizeof(user_agent), stdin);
    user_agent[strcspn(user_agent, "\n")] = '\0';

    // Configurar el objeto CURL con la dirección IP y el User-Agent ingresados
    CURL *curl = curl_easy_init();
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "User-Agent: iOS");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_PROXY, ip_address);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    // Validar un usuario de Telegram
    long user_id = 123456789; // ID del usuario a validar
    int is_valid_user = telegram_bot_authenticate_validate_user(auth, user_id, curl);

    if (is_valid_user) {
        printf("El usuario %ld es válido.\n", user_id);
    } else {
        printf("El usuario %ld no es válido.\n", user_id);
    }

    // Validar un grupo de Telegram
    long group_id = -123456789; // ID del grupo a validar
    int is_valid_group = telegram_bot_authenticate_validate_group(auth, group_id, curl);

    if (is_valid_group) {
        printf("El grupo %ld es válido.\n", group_id);
    } else {
        printf("El grupo %ld no es válido.\n", group_id);
    }

    // Liberar los recursos
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    telegram_bot_authenticate_free(auth);

    return 0;
}
