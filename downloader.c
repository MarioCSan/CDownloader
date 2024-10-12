#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int verify_yt_dlp_installed();
void install_yt_dlp();

int main()
{
    FILE *file;
    char url[256];

    file = fopen("urls.txt", "r");

    if (file == NULL)
    {
        printf("Error opening urls.txt file. File does not exist.\n");
        printf("Creating urls.txt\n");
        sleep(2);
        FILE *archivo = fopen("urls.txt", "w");
        printf("Fill in urls.txt to be able to download the songs.\n");
        sleep(2);
        return 1;
    }

    #ifdef _WIN32
        // Comandos para windows
        printf("Detected: Windows\n");
        system("mkdir mp3_downloads");
        
        if (!verify_yt_dlp_installed()) {
            // Si no está instalado, procede a instalarlo
           install_yt_dlp();
        }
            
    #elif __linux__
        // LINUX
        printf("Detected: Linux\n");
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
    printf("Elements to download: %i.\nDownload completed.\n", counterTotales);
    return 0;
}

int verify_yt_dlp_installed() {
    FILE *fp;
    char path[1035];

    // Ejecuta el comando yt-dlp --version para verificar si está instalado
    fp = popen("yt-dlp --version", "r");
    if (fp == NULL) {
        printf("Error when trying to execute the command.\n");
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
void install_yt_dlp() {
    printf("yt-dlp is not installed. Installing using winget...\n");
    int resultado = system("winget install yt-dlp");
    
    if (resultado == 0) {
        printf("yt-dlp successfully installed.\n");
    } else {
        printf("Error while installing yt-dlp.\n Try running the app as administrator and check your internet connection.\n");
    }
}
