from pytube import YouTube
from moviepy.editor import *

def download_video(url, path):
    yt = YouTube(url)
    video = yt.streams.filter(only_audio=True).first()
    out_file = video.download(output_path=path)
    return out_file

def convert_to_mp3(video_file, target_path):
    video_clip = VideoFileClip(video_file)
    audio_clip = video_clip.audio
    audio_clip.write_audiofile(target_path)
    audio_clip.close()
    video_clip.close()

# URL del video de YouTube
url = input("Ingresa la URL del video de YouTube: ")

# Directorio donde quieres guardar el archivo
save_path = 'Downloads/'

# Descargar video
video_file = download_video(url, save_path)

# Convertir a MP3
mp3_file = video_file.replace('.mp4', '.mp3')
convert_to_mp3(video_file, mp3_file)

print(f"Video convertido a MP3 y guardado en: {mp3_file}")
