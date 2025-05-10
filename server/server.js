const express = require('express');
const app = express();

app.use(express.json());

app.get('/', (req, res) => {
  res.send('Olá, meu servidor');
});


app.post('/button', (req, res) => {
  const { status, timestamp } = req.body;
  console.log(`[${new Date().toISOString()}] Botão está: ${status}`);
  res.status(200).send({ ok: true });
});


const PORT = 3000;
app.listen(PORT, () => {
  console.log(`Servidor ouvindo em http://localhost:${PORT}`);
  console.log('Olá, meu servidor — pronto para receber requisições!');
});
