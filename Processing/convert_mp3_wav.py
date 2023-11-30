import os
from moviepy.editor import *

def convert_mp4_to_wav(folder_path):
    for filename in os.listdir(folder_path):
        if filename.endswith(".mp4"):
            video_path = os.path.join(folder_path, filename)
            audio_path = os.path.join(folder_path, filename[:-4] + '.wav')
            video = VideoFileClip(video_path)
            audio = video.audio
            audio.write_audiofile(audio_path)

# Reemplaza 'path_to_your_folder' con la ruta de tu carpeta de videos MP4
convert_mp4_to_wav('./Audios/Arturo Mercado')
