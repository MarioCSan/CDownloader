![logo](https://github.com/user-attachments/assets/e2dcaba4-9344-43a1-acec-46e3a2e899bd)

# CDownloader
A C program for CLI to download videos in mp3 format in bulk. It uses yt-dlp to perform the download.

## Dependencies

Make sure you have the following dependencies installed:
- ffmpeg (only in linux)
- [yt-dlp](https://github.com/yt-dlp/yt-dlp?tab=readme-ov-file#installation)

## Common configuration 

I recommend to have the executable in a folder with the exe and the urls.txt file.

In all systems where it is executable, there must be a file urls.txt containing the urls of the videos to be downloaded. 
Without this file it will not be possible to download any element as there are no urls available.

The download folder will be created automatically if you don't have it.

## Use in Windows
In MS Windows you have to options:

- Open a terminal in the folder where downloader.exe and urls.txt are located.
- Run the command `downloader.exe` and watch the files being downloaded.

Or

- Double click on the executable

## Uses in Linux
Until the executable is created for debian/linux dist, you can use it in the following way:
- Download or clone this repository.
- Once inside the downloaded project folder, run `./downloader` in the terminal.
