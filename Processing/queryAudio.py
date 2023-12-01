import librosa
import numpy as np
import sys

# Verifica si se recibió el nombre del archivo como argumento
if len(sys.argv) < 2:
    print("Error: No se proporcionó un nombre de archivo")
    sys.exit(1)

# El primer argumento de la línea de comandos es el nombre del archivo
audio_path = sys.argv[1]

# Cargar el archivo de audio
y, sr = librosa.load(audio_path)

# Extraer algunas características
mfccs = librosa.feature.mfcc(y=y, sr=sr)
 
# Vectorización (en este caso, los MFCC ya están en forma de matriz)
audio_vector = np.mean(mfccs, axis=1)

# Convertir el vector a string para guardarlo
vector_str = ', '.join(map(str, audio_vector))

# Guardar en un archivo de texto
with open('Processing/query.txt', 'w') as file:
    file.write(vector_str)
