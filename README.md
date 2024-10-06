# CDownloader
A C program for CLI to download videos in mp3 format in bulk. It uses yt-dlp to perform the download.

## Dependencies

Make sure you have the following dependencies installed:
- ffmpeg (only in linux)
- [yt-dlp](https://github.com/yt-dlp/yt-dlp?tab=readme-ov-file#installation)

## Common configuration 

In all systems where it is executable, there must be a file urls.txt containing the urls of the videos to be downloaded. 
Without this file it will not be possible to download any element as there are no urls available.

I recommend to have the executable in a folder with the exe and the urls.txt file.
The download folder will be created automatically if you don't have it.

## Use in Windows
Open a terminal in the folder where downloader.exe and urls.txt are located. 
Run the command downloader.exe and watch the files being downloaded. 

## Uses in Linux
Coming soon.
