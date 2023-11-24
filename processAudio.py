import librosa
import numpy as np

# Cargar un archivo de audio
audio_path = 'goku.mp3'
y, sr = librosa.load(audio_path)

# Extraer algunas características
mfccs = librosa.feature.mfcc(y=y, sr=sr)

# Vectorización (en este caso, los MFCC ya están en forma de matriz)
audio_vector = np.mean(mfccs, axis=1)

# Ahora 'audio_vector' es tu representación vectorial del audio
print(audio_vector)
