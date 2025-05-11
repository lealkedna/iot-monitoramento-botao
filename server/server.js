const express = require('express');
const app = express();

app.use(express.json());
app.use(express.static('public'));

app.get('/', (req, res) => {
  res.send('Olá, meu servidor');
});


app.post('/button', (req, res) => {
  const { status, timestamp } = req.body;
  console.log(`[${new Date().toISOString()}] Botão está: ${status}`);
  res.status(200).send({ ok: true });
});

// Variáveis de exemplo
let buttonStatus = 'SOLTO';  // Substitua com o status real do botão
let temperatura = 25; // Substitua com o valor real da temperatura

// Rota GET para fornecer o status do botão e a temperatura
app.get('/status', (req, res) => {
    res.json({
        buttonStatus: buttonStatus,
        temperatura: temperatura
    });
});

app.get('/joystick', (req, res) => {
  
  const joystickData = {
    x: Math.floor(Math.random() * 1024),  
    y: Math.floor(Math.random() * 1024),
    button: 'PRESSED'  
  };
  
  res.json(joystickData);
});

const PORT = 3000;
app.listen(PORT, () => {
  console.log(`Servidor ouvindo em http://localhost:${PORT}`);
  console.log('Olá, meu servidor — pronto para receber requisições!');
});
