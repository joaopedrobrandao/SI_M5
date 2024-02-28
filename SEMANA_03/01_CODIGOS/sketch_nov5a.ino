#include <Arduino.h>

const int buttonPins[] = {2, 4, 5, 18}; // Pins dos botões
const int ledPins[] = {14, 12, 13, 15}; // Pins dos LEDs
const int resetButtonPin = 22;
const int numButtons = 4;

int sequence[100]; // Sequência de cores/sinais
int sequenceLength = 1; // Comprimento inicial da sequência
int currentStep = 0; // Passo atual da sequência

void setup() {
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
  randomSeed(analogRead(0)); // Inicializa o gerador de números aleatórios
  
  // Inicia a sequência
  for (int i = 0; i < 100; i++) {
    sequence[i] = random(0, numButtons);
  }

  delay(1000); // Aguarda um segundo antes de começar o jogo
  playSequence(); // Inicia o jogo
}

void loop() {
    if (digitalRead(resetButtonPin) == HIGH) {
    delay(50); // Debounce
    gameOver();
    return; // Retorna para sair do loop e começar o jogo do início
  }
  for (int i = 0; i < numButtons; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      while (digitalRead(buttonPins[i]) == LOW) {} // Espera o botão ser solto
      if (i == sequence[currentStep]) {
        currentStep++;
        if (currentStep == sequenceLength) {
          // O jogador completou a sequência
          sequenceLength++;
          currentStep = 0;
          delay(300);
          playSequence();
        }
      } else {
        // O jogador errou a sequência
        gameOver();
      }
    }
  }
}

void playSequence() {
  for (int i = 0; i < sequenceLength; i++) {
    digitalWrite(ledPins[sequence[i]], HIGH);
    delay(500); // Mantém a luz acesa por 0.5 segundo
    digitalWrite(ledPins[sequence[i]], LOW);
    delay(100); // Aguarda 0.1 segundo antes de passar para o próximo
  }
}

void gameOver() {
  // Pode adicionar efeitos visuais
  // Por exemplo, piscar os LEDs rapidamente
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < numButtons; j++) {
      digitalWrite(ledPins[j], HIGH);
    }
    delay(100);
    for (int j = 0; j < numButtons; j++) {
      digitalWrite(ledPins[j], LOW);
    }
    delay(100);
  }

  // Reinicia o jogo
  sequenceLength = 1;
  currentStep = 0;
  for (int i = 0; i < 100; i++) {
    sequence[i] = random(0, numButtons);
  }

  delay(1000); // Aguarda um segundo antes de começar o jogo novamente
  playSequence();
}
