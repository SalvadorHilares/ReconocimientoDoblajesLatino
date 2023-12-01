const express = require('express');
const multer = require('multer');
const { spawn } = require('child_process');
const { exec } = require('child_process');
const cors = require('cors');

const app = express();
const port = 8080;
const upload = multer({ dest: 'uploads/' });

app.use(cors()); // Habilita CORS

app.post('/upload', upload.single('audioFile'), (req, res) => {
    const audioFilePath = req.file.path;

    // Ejecutar el script de Python
    const pythonProcess = spawn('python3', ['../Processing/queryAudio.py', audioFilePath]);
    pythonProcess.stdout.on('data', (data) => {
        console.log(`Python stdout: ${data}`);
    });
    
    pythonProcess.stderr.on('data', (data) => {
        console.error(`Python stderr: ${data}`);
    });
    
    pythonProcess.on('close', (code) => {
        console.log(`Python script finished with code ${code}`);
    
        // Ejecutar el programa C++ independientemente de la salida del script Python
        exec('.././main', (error, stdout, stderr) => {
            console.log(`C++ stdout: ${stdout}`);
            if (error) {
                console.error(`C++ error: ${error.message}`);
                return res.status(500).send(error.message);
            }
            if (stderr) {
                console.error(`C++ stderr: ${stderr}`);
                return res.status(500).send(stderr);
            }
            // Enviar el resultado al frontend
            res.send(stdout);
        });
    });
});

app.listen(port, () => {
    console.log(`Servidor escuchando en http://localhost:${port}`);
});