import librosa
import numpy as np

# Cargar un archivo de audio
s = input('Ingrese el nombre del archivo de audio: ')
audio_path = s + '.wav'
y, sr = librosa.load(audio_path)

# Extraer algunas características
mfccs = librosa.feature.mfcc(y=y, sr=sr)

# Vectorización (en este caso, los MFCC ya están en forma de matriz)
audio_vector = np.mean(mfccs, axis=1)

# Convertir el vector a string para guardarlo
vector_str = ', '.join(map(str, audio_vector))

# Guardar en un archivo de texto
with open('query.txt', 'w') as file:
    file.write(vector_str)
