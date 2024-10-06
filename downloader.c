#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *file;
    char url[256];

    file = fopen("urls.txt", "r");

    if (file == NULL)
    {
        printf("Error al abrir el archivo urls.txt\n");
        return 1;
    }

    #ifdef _WIN32
        // Comandos para windows
        printf("Detectado: Windows\n");
        system("mkdir mp3_downloads");
    #elif __linux__
        // LINUX
        system("mkdir -p mp3_downloads");
    #endif

    int counterDescargas = 0;
    int counterTotales = 0;

    while (fgets(url, sizeof(url), file)){
        
        url[strcspn(url, "\n")] = 0;

        char command[512];


        #ifdef _WIN32
            snprintf(command, sizeof(command), "yt-dlp.exe --extract-audio --audio-formt mp3 -o \"mp3_downloads/%%(title)s.%%(ext)s\" \"%s\"", url);
        #elif __linux__
            snprintf(command, sizeof(command), "yt-dlp --extract-audio --audio-format mp3 -o \"mp3_downloads/%%(title)s.%%(ext)s\" \"%s\"", url);
        #endif

        counterTotales++;
        system(command);
    }

    // cerrar fichero
    fclose(file);
    printf("Elementos a descargar: %i\n. Descargas completadas.\n", counterTotales);
    return 0;
}