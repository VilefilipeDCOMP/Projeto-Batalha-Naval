# 🚢 Projeto Batalha Naval (Arduino)

Bem-vindo ao repositório do **Projeto Batalha Naval**! Este é um jogo de Batalha Naval desenvolvido para plataformas **Arduino**, com suporte a **multijogador local** via comunicação Serial (UART). O projeto foi criado com foco em modularidade, jogabilidade interativa via LCD e feedback visual.

---

## ✨ Funcionalidades

- **Multijogador (x1)**: Jogue contra outro oponente conectado via Serial (fio ou simulação).
- **Posicionamento de Navios**: Interface interativa para posicionar seus 4 navios no tabuleiro 10x10.
- **Sistema de Turnos**: Controle automático de turnos com `handshake` inicial para decidir quem começa.
- **Feedback Visual**:
  - Interface rica em **LCD 16x2** (I2C).
  - Animações exclusivas para **Tiro**, **Acerto (Explosão)** e **Erro (Água)**.
  - Sons/Feedback (se implementado hardware de som, visualmente via LCD).
- **Recordes**: Sistema de persistência de vitórias usando **EEPROM**.
- **Mecânica "Crivar"**: Confirme suas ações com um botão dedicado.

## 🛠️ Hardware Necessário

Para montar o sistema completo (2 jogadores), você precisará duplicar os itens abaixo (um kit para cada jogador):

- **1x Arduino UNO** (ou compatível).
- **1x Display LCD 16x2** com módulo **I2C**.
- **5x Botões** (Push-buttons) para controle:
  - Cima, Baixo, Esquerda, Direita, Confirmar (Crivar).
- **Resistores** (para pull-up dos botões, se não usar INPUT_PULLUP interno, mas o código usa `INPUT_PULLUP`).
- **Fios Jumper**.

### 🔌 Conexões

**Pinos dos Botões (Arduino):**
- **UP**: Pino 2
- **RIGHT**: Pino 3
- **DOWN**: Pino 4
- **LEFT**: Pino 5
- **CRIVAR (Confirmar)**: Pino 6

**Comunicação entre Arduinos (SoftwareSerial):**
- **RX**: Pino 7 (Conectar no TX do outro Arduino)
- **TX**: Pino 8 (Conectar no RX do outro Arduino)
- **GND**: Conectar os GNDs dos dois Arduinos juntos.

**Display LCD (I2C):**
- **SDA**: Pino A4 (ou SDA dedicado)
- **SCL**: Pino A5 (ou SCL dedicado)
- **VCC/GND**: 5V e GND.

---

## 🚀 Instalação e Compilação

Este projeto utiliza uma estrutura modular (arquivos `.h` e `.cpp` na pasta `biblioteca/`). Para compilar, você tem duas opções:

### Opção 1: Arduino IDE (Local)
1. Baixe o repositório.
2. Abra o arquivo principal (se você for usar a estrutura de biblioteca, certifique-se de que a IDE reconheça os arquivos na pasta `biblioteca`).
3. Instale as bibliotecas necessárias: `LiquidCrystal_I2C`.

### Opção 2: Tinkercad / Simuladores (Script de Build)
Como plataformas como o Tinkercad geralmente aceitam apenas um único arquivo `.ino`, incluímos um script Python para unificar o código.

1. Certifique-se de ter **Python 3** instalado.
2. Execute o script de build:
   ```bash
   python build_thinkercad.py
   ```
3. O script irá gerar um arquivo chamado **`Tinkercad_Final.ino`**.
4. Copie o conteúdo de `Tinkercad_Final.ino` e cole no editor de código do Tinkercad.

---

## 🎮 Como Jogar

1. **Menu Inicial**:
   - Use as setas para ver recordes ou pressione **CRIVAR** para iniciar.
   - (Opcional) Resete os recordes segurando a seta para a esquerda no menu.
2. **Posicionamento**:
   - Você deve posicionar 4 navios.
   - Use as **Setas** para mover o navio.
   - Pressione **Confirmar (Crivar)** para girar ou fixar a posição (conforme a lógica de `escolherOrientacao`).
     - *Dica*: O jogo valida se a posição é válida (não sobrepõe outro navio e não sai do mapa).
3. **Batalha**:
   - Se for seu turno, selecione as coordenadas do alvo (Linha/Coluna) e atire!
   - Se for o turno do oponente, aguarde o ataque.
   - O LCD mostrará se você acertou (`HIT`), errou (`MISS`) ou venceu (`WIN`).
4. **Vitória**:
   - O jogo acaba quando um jogador afunda todos os navios do adversário.
   - O resultado é salvo na memória permanente.

---

## 👥 Autores

Equipe responsável pelo desenvolvimento:

- **Sidnei**: Lógica de conexão e envio de dados (`conectarPlacas`, `enviarTiro`).
- **Danilo**: Gerenciamento do mapa e validação de vitórias (`iniciarMapaVazio`, `verificarVitoria`).
- **Arthur**: Posicionamento e validação de navios (`colocarNavioDeLadinho`, `CadastroCompletao`).
- **Filipe**: Persistência de dados e Recordes (`salvarNovaVitoria`, `EEPROM`).
- **Brunão**: Interface de controle e hardware (`lerDirecional`, `botoes`).

---

> *Projeto desenvolvido para a disciplina de Microcontroladores / Programação Embarcada.*
