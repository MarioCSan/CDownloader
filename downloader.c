#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int verificar_yt_dlp_instalado();
void instalar_yt_dlp();

int main()
{
    FILE *file;
    char url[256];

    file = fopen("urls.txt", "r");

    if (file == NULL)
    {
        printf("Error al abrir el archivo urls.txt. No existe el fichero.\n");
        printf("Creando urls.txt\n");
        sleep(2);
        FILE *archivo = fopen("urls.txt", "w");
        printf("Rellene urls.txt para poder descargar las canciones.\n");
        sleep(2);
        return 1;
    }

    #ifdef _WIN32
        // Comandos para windows
        printf("Detectado: Windows\n");
        system("mkdir mp3_downloads");
        
        if (verificar_yt_dlp_instalado()) {
            printf("yt-dlp ya ase encuentra instalado.\n");
        } else {
            // Si no está instalado, procede a instalarlo
            instalar_yt_dlp();
        }
            
    #elif __linux__
        // LINUX
        printf("Detectado: Linux\n");
        system("mkdir -p mp3_downloads");
    #endif

    int counterDescargas = 0;
    int counterTotales = 0;

    while (fgets(url, sizeof(url), file)){

        // ignore lines that starts with '#'
        if (url[0] == '#') {
            continue;
        }

        // delete return of line
        url[strcspn(url, "\n")] = 0;

        char command[512];


        #ifdef _WIN32
            snprintf(command, sizeof(command), "yt-dlp.exe --extract-audio --audio-format mp3 -o \"mp3_downloads/%%(title)s.%%(ext)s\" \"%s\"", url);
        #elif __linux__
            snprintf(command, sizeof(command), "yt-dlp --extract-audio --audio-format mp3 -o \"mp3_downloads/%%(title)s.%%(ext)s\" \"%s\"", url);
        #endif

        counterTotales++;
        system(command);
    }

    // cerrar fichero
    fclose(file);
    sleep(5);
    printf("Elementos a descargar: %i.\nDescargas completadas.\n", counterTotales);
    return 0;
}

int verificar_yt_dlp_instalado() {
    FILE *fp;
    char path[1035];

    // Ejecuta el comando yt-dlp --version para verificar si está instalado
    fp = popen("yt-dlp --version", "r");
    if (fp == NULL) {
        printf("Error al intentar ejecutar el comando.\n");
        return 0;
    }

    // Lee el resultado del comando
    if (fgets(path, sizeof(path), fp) != NULL) {
        // Si obtuvimos alguna salida, asumimos que está instalado
        pclose(fp);
        return 1;
    }

    pclose(fp);
    return 0;
}

// Función para instalar yt-dlp usando winget
void instalar_yt_dlp() {
    printf("yt-dlp no está instalado. Instalando usando winget...\n");
    int resultado = system("winget install yt-dlp");
    
    if (resultado == 0) {
        printf("yt-dlp instalado con éxito.\n");
    } else {
        printf("Error durante la instalación de yt-dlp.\n Intente ejecutar la app como administrador.\n");
    }
}
