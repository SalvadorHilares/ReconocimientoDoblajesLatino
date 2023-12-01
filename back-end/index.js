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
    const pythonProcess = spawn('python', ['../Processing/queryAudio.py', audioFilePath]);

    pythonProcess.stdout.on('data', (data) => {
        const paramForCppFunction = data.toString();
        exec(`./path_to_cpp_executable ${paramForCppFunction}`, (error, stdout, stderr) => {
            if (error) {
                console.error(`exec error: ${error}`);
                return res.status(500).send(`Error en la función C++: ${error.message}`);
            }
            res.send(stdout);
        });
    });

    pythonProcess.stderr.on('data', (data) => {
        console.error(`stderr: ${data}`);
        res.status(500).send(`Error en el script de Python: ${data}`);
    });
});

app.listen(port, () => {
    console.log(`Servidor escuchando en http://localhost:${port}`);
});