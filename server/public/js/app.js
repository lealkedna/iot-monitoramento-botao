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


document.addEventListener("DOMContentLoaded", () => {
    const xValue = document.getElementById('xValue');
    const yValue = document.getElementById('yValue');
    const buttonState = document.getElementById('buttonState');
    
    // Função para buscar o status do joystick do servidor
    function fetchJoystickData() {
        fetch('http://localhost:3000/joystick')
            .then(response => response.json())
            .then(data => {
                // Atualiza a página com os dados do joystick
                xValue.textContent = data.x;
                yValue.textContent = data.y;
                buttonState.textContent = data.button;
            })
            .catch(error => {
                console.error('Erro ao buscar dados do joystick:', error);
            });
    }

    // Atualiza a cada 1 segundo
    setInterval(fetchJoystickData, 1000);
});


// Função para simular os dados do joystick
function updateJoystickData() {
  // Gerando valores simulados para X e Y
  let x = Math.floor(Math.random() * 4096);  // Valor entre 0 e 4095
  let y = Math.floor(Math.random() * 4096);  // Valor entre 0 e 4095
  let buttonPressed = Math.random() > 0.5;  // Aleatório entre 0 e 1

  // Atualizando os elementos da interface
  document.getElementById('x-value').textContent = x;
  document.getElementById('y-value').textContent = y;
  document.getElementById('button-status').textContent = buttonPressed ? "Pressionado" : "Solto";
}

// Atualizar a cada 1 segundo
setInterval(updateJoystickData, 1000);