#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#ifdef _WIN32
#include <windows.h>
#include <libgen.h>
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <libgen.h>
#endif

#ifdef __linux__
#include <libgen.h>
#endif

int verify_yt_dlp_installed();
void install_yt_dlp();
int verify_ffmpeg_installed();
void install_ffmpeg();
int verify_if_empty(const char *file_name);

// Función portable para obtener el directorio del ejecutable
void get_executable_directory(char *buffer, size_t size)
{
#ifdef __APPLE__
    char exePath[PATH_MAX];
    uint32_t exePathSize = sizeof(exePath);
    if (_NSGetExecutablePath(exePath, &exePathSize) != 0)
    {
        fprintf(stderr, "Error obtaining executable path.\n");
        exit(1);
    }
    realpath(exePath, buffer);
    strcpy(buffer, dirname(buffer));
#elif __linux__
    ssize_t len = readlink("/proc/self/exe", buffer, size - 1);
    if (len == -1)
    {
        perror("readlink");
        exit(1);
    }
    buffer[len] = '\0';
    strcpy(buffer, dirname(buffer));
#elif _WIN32
    if (!GetModuleFileNameA(NULL, buffer, (DWORD)size))
    {
        fprintf(stderr, "Error obtaining executable path.\n");
        exit(1);
    }
    // Convertir \ a /
    for (char *p = buffer; *p; p++)
        if (*p == '\\')
            *p = '/';
    strcpy(buffer, dirname(buffer));
#else
    fprintf(stderr, "Unsupported OS.\n");
    exit(1);
#endif
}

int main()
{
    char exeDir[PATH_MAX];
    char downloads_dir[PATH_MAX];
    get_executable_directory(exeDir, sizeof(exeDir));
    snprintf(downloads_dir, sizeof(downloads_dir), "%s/mp3_downloads", exeDir);

    char file_name[PATH_MAX];
    snprintf(file_name, sizeof(file_name), "%s/urls.txt", exeDir);

    FILE *file;
    char url[256];

    file = fopen(file_name, "r");

    if (file == NULL)
    {
        printf("Error opening %s. File does not exist.\n", file_name);
        printf("Creating %s\n", file_name);
        sleep(2);
        FILE *archivo = fopen(file_name, "w");
        if (archivo)
            fclose(archivo);
        printf("Fill in urls.txt to be able to download the songs.\n");
        sleep(2);
        return 1;
    }

#ifdef _WIN32
    char mkdir_cmd[PATH_MAX + 20];
    snprintf(mkdir_cmd, sizeof(mkdir_cmd), "mkdir \"%s\"", downloads_dir);
    system(mkdir_cmd);
#elif __APPLE__ || __linux__
    char mkdir_cmd[PATH_MAX + 20];
    snprintf(mkdir_cmd, sizeof(mkdir_cmd), "mkdir -p \"%s\"", downloads_dir);
    system(mkdir_cmd);
#else
    printf("Unsupported operating system.\n");
    return 1;
#endif


    if (!verify_yt_dlp_installed())
    {
        install_yt_dlp();
    }

    if (!verify_ffmpeg_installed())
    {
        install_ffmpeg();
    }

    if (verify_if_empty(file_name) == 0)
    {
        int counterTotales = 0;

        while (fgets(url, sizeof(url), file))
        {
            if (url[0] == '#')
                continue;
            url[strcspn(url, "\n")] = 0;
            char command[512];

#ifdef _WIN32
            snprintf(command, sizeof(command),
                     "yt-dlp.exe --extract-audio --audio-format mp3 -o \"%s/%%(title)s.%%(ext)s\" \"%s\"",
                     downloads_dir, url);
#else
            snprintf(command, sizeof(command),
                     "yt-dlp --extract-audio --audio-format mp3 -o \"%s/%%(title)s.%%(ext)s\" \"%s\"",
                     downloads_dir, url);
#endif

            counterTotales++;
            system(command);
        }

        fclose(file);
        sleep(2);
        printf("Elements to download: %d.\nDownload completed.\n", counterTotales);
        return 0;
    }
    else
    {
        printf("There are no elements to download in %s.\n", file_name);
        sleep(2);
        return 1;
    }
}

int verify_yt_dlp_installed()
{
    FILE *fp;
    char path[1035];

    // Verifica si yt-dlp está disponible
    fp = popen("yt-dlp --version", "r");
    if (fp == NULL)
    {
        printf("Error checking yt-dlp.\n");
        return 0;
    }

    if (fgets(path, sizeof(path), fp) != NULL)
    {
        pclose(fp);
        return 1;
    }

    pclose(fp);
    return 0;
}

int verify_ffmpeg_installed()
{
    FILE *fp;
    char path[1035];

#ifdef _WIN32
    fp = _popen("ffmpeg -version", "r");
#else
    fp = popen("ffmpeg -version", "r");
#endif

    if (fp == NULL)
    {
        return 0;
    }

    if (fgets(path, sizeof(path), fp) != NULL)
    {
#ifdef _WIN32
        _pclose(fp);
#else
        pclose(fp);
#endif
        return 1;
    }

#ifdef _WIN32
    _pclose(fp);
#else
    pclose(fp);
#endif

    return 0;
}

void install_ffmpeg()
{
    int result;

#ifdef _WIN32
    printf("FFmpeg is not installed. Installing using winget...\n");
    result = system("winget install -e --id Gyan.FFmpeg");

#elif __APPLE__
    printf("FFmpeg is not installed. Installing using Homebrew...\n");
    const char *brew_path = "/opt/homebrew/bin/brew";
    if (access(brew_path, X_OK) != 0)
    {
        printf("Homebrew is not installed. Installing Homebrew first...\n");
        system("/bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\"");
        system("echo 'eval \"$(/opt/homebrew/bin/brew shellenv)\"' >> ~/.zprofile");
        system("eval \"$(/opt/homebrew/bin/brew shellenv)\"");
    }
    result = system("/opt/homebrew/bin/brew install ffmpeg");

#elif __linux__
    printf("FFmpeg is not installed. Installing using apt...\n");
    // Puedes cambiar a yum/dnf según la distro
    result = system("sudo apt update && sudo apt install -y ffmpeg");

#else
    printf("Unsupported OS. Install FFmpeg manually.\n");
    return;
#endif

    if (result == 0)
    {
        printf("FFmpeg successfully installed.\n");
    }
    else
    {
        printf("Failed to install FFmpeg. Please install it manually.\n");
    }
}

void install_yt_dlp()
{
    int result;

#ifdef _WIN32
    printf("yt-dlp is not installed. Installing using winget...\n");
    result = system("winget install -e --id yt-dlp.yt-dlp");

#elif __APPLE__
    printf("yt-dlp is not installed. Installing using Homebrew...\n");

    const char *brew_path = "/opt/homebrew/bin/brew";

    if (access(brew_path, X_OK) != 0)
    {
        printf("Homebrew is not installed. Installing Homebrew first...\n");
        system("/bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\"");
        system("echo 'eval \"$(/opt/homebrew/bin/brew shellenv)\"' >> ~/.zprofile");
        system("eval \"$(/opt/homebrew/bin/brew shellenv)\"");
    }

    result = system("/opt/homebrew/bin/brew install yt-dlp");

#elif __linux__
    printf("yt-dlp is not installed. Attempting installation using pip...\n");
    result = system("pip3 install yt-dlp --break-system-packages");

#else
    printf("Unsupported OS. Install yt-dlp manually.\n");
    return;
#endif

    if (result == 0)
    {
        printf("yt-dlp successfully installed.\n");
    }
    else
    {
        printf("Failed to install yt-dlp. Please install it manually.\n");
    }
}

int verify_if_empty(const char *file_name)
{
    FILE *file = fopen(file_name, "r");

    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    if (size == 0)
    {
        printf("The file %s is empty. Fill in the file to continue.\n", file_name);
        return 1;
    }

    printf("The file %s is not empty.\n", file_name);
    return 0;
}
