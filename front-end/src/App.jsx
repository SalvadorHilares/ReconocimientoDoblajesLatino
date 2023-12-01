import React, { useState } from 'react';
import './App.css';

function App() {
  const [error, setError] = useState('');

  const handleFileChange = async (event) => {
    const file = event.target.files[0];
    if (file) {
      const fileType = file.type;
      if (fileType === 'audio/mpeg' || fileType === 'audio/wav') {
        setError(''); // Limpia cualquier error previo
        
        // Crear un FormData para enviar el archivo
        const formData = new FormData();
        formData.append('audioFile', file);
  
        try {
          // Reemplaza 'http://localhost:8080/upload' con la URL de tu servidor
          const response = await fetch('http://localhost:8080/upload', {
            method: 'POST',
            body: formData,
          });
  
          if (!response.ok) {
            throw new Error(`Error HTTP: ${response.status}`);
          }
  
          const result = await response.text();
          console.log(result);
        } catch (error) {
          console.error("Error al cargar el archivo:", error);
        }
      } else {
        setError('Solo se permite archivos de audio como wav o mp3');
      }
    }
  };
  

  return (
    <>
      <img src="./img/doblajelatino.jpg_673822677.webp" alt="logo"/>
      <h1>Encuentra a tu artista favorito por el audio de un personaje</h1>
      <h2>Sube un audio y encuentra a tu artista favorito</h2>
      <input type="file" onChange={handleFileChange} />
      {error && <div className="error-message">{error}</div>}
      <div className='result-container'>
        <h3>Mario Castañeda</h3>
      </div>
    </>
  );
}

export default App;
