// function fetchData() {
//     fetch('/status')
//         .then(response => response.json())
//         .then(data => {
//             // Atualiza o status do botão
//             document.getElementById('buttonStatus').textContent = data.buttonStatus;

//             // Atualiza a temperatura
//             document.getElementById('temperature').textContent = `Temperatura: ${data.temperature}°C`;
//         })
//         .catch(err => console.error('Erro ao obter dados: ', err));
// }

fetch('http://localhost:3000/status')
  .then(response => response.json())
  .then(data => {
    // Exibe os dados do botão e da temperatura
    document.getElementById('statusBotao').innerText = data.buttonStatus;
    document.getElementById('temperatura').innerText = data.temperatura + '°C';
  })
  .catch(error => console.error('Erro ao obter dados:', error));


// Atualiza os dados a cada 1 segundo
setInterval(fetchData, 1000);
