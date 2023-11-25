import os
import librosa
import pandas as pd
import numpy as np

# Define la ruta de la carpeta que contiene las subcarpetas con los audios
carpeta_principal = 'Audios'

# Lista para almacenar los vectores y los nombres de las carpetas
datos = []

# Iterar sobre cada subcarpeta y sus archivos
for carpeta in os.listdir(carpeta_principal):
    ruta_carpeta = os.path.join(carpeta_principal, carpeta)
    
    if os.path.isdir(ruta_carpeta):
        for archivo in os.listdir(ruta_carpeta):
            if archivo.endswith('.wav'):  # Asegúrate de que es un archivo de audio
                ruta_archivo = os.path.join(ruta_carpeta, archivo)
                
                # Cargar el archivo de audio
                y, sr = librosa.load(ruta_archivo)
                
                # Extraer características, por ejemplo, MFCC
                mfccs = librosa.feature.mfcc(y=y, sr=sr)
                mfccs_media = np.mean(mfccs, axis=1).tolist()  # Convertir a lista
                print(mfccs_media, carpeta)
                
                # Añadir el vector y el nombre de la carpeta a la lista
                datos.append([mfccs_media, carpeta])

# Crear un DataFrame de pandas y guardar en CSV
df = pd.DataFrame(datos, columns=['Vector', 'ActorDoblaje'])
df.to_csv('audio_features.csv', index=False)
