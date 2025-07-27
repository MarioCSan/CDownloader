#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // sleep() en Linux/macOS

#ifdef _WIN32
#include <windows.h> // Sleep() solo en Windows
#endif

int verify_yt_dlp_installed();
void install_yt_dlp();
int verify_if_empty(const char *file_name);

int main() {
    FILE *file;
    char url[256];
    const char *file_name = "urls.txt";

    file = fopen(file_name, "r");

    if (file == NULL) {
        printf("Error opening %s. File does not exist.\n", file_name);
        printf("Creating %s\n", file_name);
        sleep(2);
        FILE *archivo = fopen(file_name, "w");
        if (archivo) fclose(archivo);
        printf("Fill in urls.txt to be able to download the songs.\n");
        sleep(2);
        return 1;
    }

    // Detección del sistema operativo
    #ifdef _WIN32
        printf("Detected: Windows\n");
        system("mkdir mp3_downloads");
    #elif __APPLE__
        printf("Detected: macOS\n");
        system("mkdir -p mp3_downloads");
    #elif __linux__
        printf("Detected: Linux\n");
        system("mkdir -p mp3_downloads");
    #else
        printf("Unsupported operating system.\n");
        return 1;
    #endif

    if (!verify_yt_dlp_installed()) {
        install_yt_dlp();
    }

    if (verify_if_empty(file_name) == 0) {
        int counterTotales = 0;

        while (fgets(url, sizeof(url), file)) {
            if (url[0] == '#') continue;
            url[strcspn(url, "\n")] = 0; // quitar salto de línea

            char command[512];

            #ifdef _WIN32
                snprintf(command, sizeof(command), "yt-dlp.exe --extract-audio --audio-format mp3 -o \"mp3_downloads/%%(title)s.%%(ext)s\" \"%s\"", url);
            #else
                snprintf(command, sizeof(command), "yt-dlp --extract-audio --audio-format mp3 -o \"mp3_downloads/%%(title)s.%%(ext)s\" \"%s\"", url);
            #endif

            counterTotales++;
            system(command);
        }

        fclose(file);
        sleep(2);
        printf("Elements to download: %d.\nDownload completed.\n", counterTotales);
        return 0;
    } else {
        printf("There are no elements to download in %s.\n", file_name);
        sleep(2);
        return 1;
    }
}

int verify_yt_dlp_installed() {
    FILE *fp;
    char path[1035];

    // Verifica si yt-dlp está disponible
    fp = popen("yt-dlp --version", "r");
    if (fp == NULL) {
        printf("Error checking yt-dlp.\n");
        return 0;
    }

    if (fgets(path, sizeof(path), fp) != NULL) {
        pclose(fp);
        return 1;
    }

    pclose(fp);
    return 0;
}

void install_yt_dlp() {
    int result;

    #ifdef _WIN32
        printf("yt-dlp is not installed. Installing using winget...\n");
        result = system("winget install -e --id yt-dlp.yt-dlp");
    #elif __APPLE__
        printf("yt-dlp is not installed. Installing using Homebrew...\n");
        result = system("brew install yt-dlp");
    #elif __linux__
        printf("yt-dlp is not installed. Attempting installation using pip...\n");
        result = system("pip3 install yt-dlp --break-system-packages");
    #else
        printf("Unsupported OS. Install yt-dlp manually.\n");
        return;
    #endif

    if (result == 0) {
        printf("yt-dlp successfully installed.\n");
    } else {
        printf("Failed to install yt-dlp. Please install it manually.\n");
    }
}

int verify_if_empty(const char *file_name) {
    FILE *file = fopen(file_name, "r");

    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    if (size == 0) {
        printf("The file %s is empty. Fill in the file to continue.\n", file_name);
        return 1;
    }

    printf("The file %s is not empty.\n", file_name);
    return 0;
}
