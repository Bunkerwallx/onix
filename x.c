




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <curl/curl.h>
#include <wappalyzer.h>

// Función para obtener el título de una página web
char *obtener_titulo(char *url) {
  CURL *curl;
  CURLcode res;
  char *buffer = NULL;
  size_t size;
  Soup *soup;
  char *titulo = NULL;

  // Inicializar CURL
  curl = curl_easy_init();
  if (!curl) {
    return NULL;
  }

  // Configurar opciones de CURL
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &escribir_memoria);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

  // Ejecutar la solicitud
  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "Error al obtener el título: %s\n", curl_easy_strerror(res));
    goto cleanup;
  }

  // Parsear el contenido HTML
  soup = soup_new_from_buffer(buffer, size);
  if (!soup) {
    fprintf(stderr, "Error al parsear el HTML\n");
    goto cleanup;
  }

  // Extraer el título
  titulo = soup_get_title(soup);

cleanup:
  // Liberar recursos
  curl_easy_cleanup(curl);
  soup_free(soup);
  free(buffer);

  return titulo;
}

// Función para obtener los subdominios de una página web
char **obtener_subdominios(char *url) {
  CURL *curl;
  CURLcode res;
  char *buffer = NULL;
  size_t size;
  Soup *soup;
  SoupLink *link;
  char **subdominios = NULL;
  int n_subdominios = 0;

  // Inicializar CURL
  curl = curl_easy_init();
  if (!curl) {
    return NULL;
  }

  // Configurar opciones de CURL
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &escribir_memoria);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

  // Ejecutar la solicitud
  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "Error al obtener los subdominios: %s\n", curl_easy_strerror(res));
    goto cleanup;
  }

  // Parsear el contenido HTML
  soup = soup_new_from_buffer(buffer, size);
  if (!soup) {
    fprintf(stderr, "Error al parsear el HTML\n");
    goto cleanup;
  }

  // Extraer los subdominios
  for (link = soup->links; link; link = link->next) {
    char *href = link->href;
    if (href && strstr(href, "://")) {
      char *subdominio = strtok(href, "://");
      subdominio = strtok(subdominio, "/");
      if (subdominio) {
        subdominios = realloc(subdominios, sizeof(char *) * (n_subdominios + 1));
        subdominios[n_subdominios++] = strdup(subdominio);
      }
    }
  }

cleanup:
  // Liberar recursos
  curl_easy_cleanup(curl);
  soup_free(soup);
  free(buffer);

  return subdominios;
}

// ... (se continúa en la siguiente respuesta)
// Función para obtener elementos de interés de una página web
char **obtener_elementos(char *url) {
  CURL *curl;
  CURLcode res;
  char *buffer = NULL;
  size_t size;
  Soup *soup;
  SoupTag *tag;
  char **elementos = NULL;
  int n_elementos = 0;

  // Inicializar CURL
  curl = curl_easy_init();
  if (!curl) {
    return NULL;
  }

  // Configurar opciones de CURL
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &escribir_memoria);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

  // Ejecutar la solicitud
  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "Error al obtener los elementos: %s\n", curl_easy_strerror(res));
    goto cleanup;
  }

  // Parsear el contenido HTML
  soup = soup_new_from_buffer(buffer, size);
  if (!soup) {
    fprintf(stderr, "Error al parsear el HTML\n");
    goto cleanup;
  }

  // Extraer los elementos
  for (tag = soup->root; tag; tag = tag->next) {
    if (tag->type == SOUP_TAG_P) {
      char *texto = tag->content;
      if (texto && strlen(texto) > 50) {
        elementos = realloc(elementos, sizeof(char *) * (n_elementos + 1));
        elementos[n_elementos++] = strdup(texto);
      }
    }
  }

cleanup:
  // Liberar recursos
  curl_easy_cleanup(curl);
  soup_free(soup);
  free(buffer);

  return elementos;
}

// Función para obtener metadatos de una página web
char **obtener_metadatos(char *url) {
  CURL *curl;
  CURLcode res;
  char *buffer = NULL;
  size_t size;
  Soup *soup;
  SoupMeta *meta;
  char **metadatos = NULL;
  int n_metadatos = 0;

  // Inicializar CURL
  curl = curl_easy_init();
  if (!curl) {
    return NULL;
  }

  // Configurar opciones de CURL
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &escribir_memoria);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

  // Ejecutar la solicitud
  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "Error al obtener los metadatos: %s\n", curl_easy_strerror(res));
    goto cleanup;
  }

  // Parsear el contenido HTML
  soup = soup_new_from_buffer(buffer, size);
  if (!soup) {
    fprintf(stderr, "Error al parsear el HTML\n");
    goto cleanup;
  }

  // Extraer los metadatos
  for (meta = soup->meta; meta; meta = meta->next) {
    char *nombre = meta->name;
    char *contenido = meta->content;
    if (nombre && contenido) {
      char *metadato = malloc(strlen(nombre) + strlen(contenido) + 3);
      sprintf(metadato, "%s: %s", nombre, contenido);
      metadatos = realloc(metadatos, sizeof(char *) * (n_metadatos + 1));
      metadatos[n_metadatos++] = metadato;
    }
  }

cleanup:
  // Liberar recursos
  curl_easy_cleanup(curl);
  soup_free(soup);
  free(buffer);

  return metadatos;
// Función para obtener las tecnologías utilizadas en una página web
char **obtener_tecnologias(char *url) {
  Wappalyzer wappalyzer;
  wappalyzer_init(&wappalyzer);

  // Analizar la página web
  wappalyzer_analyze(&wappalyzer, url);

  // Obtener las tecnologías
  char **tecnologias = NULL;
  int n_tecnologias = 0;
  for (int i = 0; i < wappalyzer.technologies_count; i++) {
    tecnologias = realloc(tecnologias, sizeof(char *) * (n_tecnologias + 1));
    tecnologias[n_tecnologias++] = strdup(wappalyzer.technologies[i].name);
  }

  // Liberar recursos
  wappalyzer_destroy(&wappalyzer);

  return tecnologias;
}

// Función para guardar la información de una página web en una base de datos
void guardar_informacion(char *url, char *titulo, char **subdominios, char **elementos, char **metadatos, char **tecnologias) {
  sqlite3 *db;
  sqlite3_stmt *stmt;
  char *sql;

  // Abrir la base de datos
  if (sqlite3_open("webs_visitadas.db", &db) != SQLITE_OK) {
    fprintf(stderr, "Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
    return;
  }

  // Preparar la consulta SQL
  sql = "INSERT INTO webs (url, titulo, subdominios, elementos, metadatos, tecnologias) VALUES (?, ?, ?, ?, ?, ?)";
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return;
  }

  // Enlazar los parámetros
  sqlite3_bind_text(stmt, 1, url, -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, titulo, -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, ",", -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, ",", -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 5, ",", -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 6, ",", -1, SQLITE_TRANSIENT);

  // Ejecutar la consulta
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
  }

  // Finalizar la consulta
  sqlite3_finalize(stmt);

  // Cerrar la base de datos
  sqlite3_close(db);
}

// Función principal del programa
int main() {
  char *url = "https://www.ejemplo.com/";
  char *titulo = obtener_titulo(url);
  char **subdominios = obtener_subdominios(url);
  char **elementos = obtener_elementos(url);
  char **metadatos = obtener_metadatos(url);
  char **tecnologias = obtener_tecnologias(url);

  // Imprimir resultados
  printf("**Título:** %s\n", titulo);
  printf("**Subdominios:** ");
  for (int i = 0; subdominios[i]; i++) {
    printf("%s ", subdominios[i]);
  }
  printf("\n**Elementos de interés:** ");
  for (int i = 0; elementos[i]; i++) {
    printf("%s ", elementos[i]);
  }
  printf("\n**Metadatos:** ");
  for (int i = 0; metadatos[i]; i++) {
    printf("%s ", metadatos[i]);
  }
  printf("\n**Tecnologías:** ");
  for (int i = 0; tecnologias[i]; i++) {
    printf("%
  printf("\n");

  // Guardar en base de datos
  guardar_informacion(url, titulo, subdominios, elementos, metadatos, tecnologias);

  // Liberar recursos
  free(titulo);
  for (int i = 0; subdominios[i]; i++) {
    free(subdominios[i]);
  }
  free(subdominios);
  for (int i = 0; elementos[i]; i++) {
    free(elementos[i]);
  }
  free(elementos);
  for (int i = 0; metadatos[i]; i++) {
    free(metadatos[i]);
  }
  free(metadatos);
  for (int i = 0; tecnologias[i]; i++) {
    free(tecnologias[i]);
  }
  free(tecnologias);

  return 0;
}
