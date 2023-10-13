/* INFO / COPYRIGHT
Código desenvolvido pelo aluno Yuri Andrade dos Anjos(https://Github.com/YuriXbr/)
O material em questão é Open Source e pode ser usado por qualquer pessoa desde que
seja dado os devidos créditos.

O código pode ser facilmente ajustado por qualquer pessoa com conhecimentos bascos na área
- Ajuste as variaveis dos pinos ligados no seu arduino:
- Ajuste o pino de entrada dos leds e dos botões;
- Ajuste a quantidade maxima de leds e de efeitos;
- Para adicionar um novo efeito basta adicionar um "while (efeito == x) {};"
OBS: Lembre-se de no final da lógica do efeito, adicionar a validação da vairavel efeito para sair do loop.
OBS²: Evite o uso de muitos leds simultaneos no Tinkercad, o simulador não aguenta.
*/

#include <Adafruit_NeoPixel.h>

//PINOS (Representam a ligação física dos pnios do arduino):
#define PIN_POTRED A0      // PINO DE CONTROLE DO POTENCIOMETRO
#define PIN_POTGREEN A1    // PINO DE CONTROLE DO POTENCIOMETRO
#define PIN_POTBLUE A2     // PINO DE CONTROLE DO POTENCIOMETRO
#define PIN_POTALPHA A3    // PINO DE CONTROLE DO POTENCIOMETRO
#define PIN_BUTTON 2       // PINO DE CONTROLE DO BOTÃO
#define PIN_RESETBUTTON 3  // PINO DE CONTROLE DO BOTÃO DE RESET
#define PIN_LED 10         // PINO DE CONTROLE DO LED

//CONSTANTES (Não são pinos, apenas valores que não serão alterados.):
#define MAX_POT_VALUE 1023 // VALOR MAX DO POT 250k
#define NUM_LEDS 6         // QUANTIDADE DE LEDS NA FITA
#define MAX_EFFECTS 3

// Biblioteca de controle dos Leds
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_LED, NEO_RGB + NEO_KHZ800);

//VARIAVEIS:
int buttonState = 1;
int resetButtonState = 1;
int efeito = 0;


/* INICIALIZAÇÃO:
- Configura PINMODE do  botão.
- Cofigura ação de interrupção dos botões.
- Liga a fita com os Leds desligados usando o strip.show().
*/
void setup() { 
    
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    pinMode(PIN_RESETBUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), effectState, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_RESETBUTTON), effectState, FALLING);
    strip.begin();
    strip.show(); 

}

/* SETAR EFEITOS:
- Ler pino do botão e do botão de reset
- Garantir que a variavel efeito está na faixa funcional
- Se o botão de reset foi pressionado, variavel efeito = 0.
- Se o botão de efeito foi pressionado, variavel efeito incrimenta 1 se menor que limite de efeitos, se não variavel efeito = 0.
*/
void effectState() {

    buttonState = digitalRead(PIN_BUTTON);
    resetButtonState = digitalRead(PIN_RESETBUTTON);

    if(efeito > MAX_EFFECTS || efeito < 0) efeito = 0;
    
    if(resetButtonState == 0) {
        efeito = 0; 
        return;

    } else if(buttonState == 0){
        
        if(efeito < MAX_EFFECTS){
            efeito++;
            return;
        
        } else {
            efeito = 0;
            return;
        }
    }

}

/* FUNÇÕES DOS POTENCIOMETROS
- Leem o valor bruto do pino do potenciometro correspondente
- Retornam o valor convertido para escala de 0-255
*/
int Red(void) {
    int potValueRED = analogRead(PIN_POTRED);
    int redValue = map(potValueRED, 0, MAX_POT_VALUE, 0, 255);
    return redValue;
}
int Green(void) {
    int potValueGREEN = analogRead(PIN_POTGREEN);
    int greenValue = map(potValueGREEN, 0, MAX_POT_VALUE, 0, 255);
    return greenValue;
}      
int Blue(void) {
    int potValueBLUE = analogRead(PIN_POTBLUE);
    int blueValue = map(potValueBLUE, 0, MAX_POT_VALUE, 0, 255);
    return blueValue;
}   
int Alpha(void) {
    int potValueALPHA = analogRead(PIN_POTALPHA);
    int alphaValue = map(potValueALPHA, 0, MAX_POT_VALUE, 0, 255);
    return alphaValue;
    
}   

/* ROTINA DE EFEITOS:
- Ajusta o brilho para o valor correto
- Se variavel efeito = 0: Leds controlados pelos potenciometros.
- Se variavel efeito = 1: Leds alternam entre vermelho-amarelo em pares.
- Se variavel efeito = 2: Leds piscam em verde de 2 em 2.
- Se variavel efeito = 3: Leds com padrão arco-iris infinito.
- No fim da rotina do efeito, valida o estado da variavel efeito, se estiver diferente do efeito correspondente, sai do laço.
*/
void loop() {

    strip.setBrightness(Alpha());

    while (efeito == 0) { // Potenciometros
        
        for(int i = 0; i < NUM_LEDS; i++) {	
            strip.setPixelColor(i, Green(), Red(), Blue());
            strip.setBrightness(Alpha());
            strip.show();
            if(efeito != 0) return;
        }
        
    }

    while (efeito == 1) { // Vermelho-Amarelo
        
        for(int i = 0; i < NUM_LEDS; i++) {
        strip.setBrightness(Alpha());
        strip.setPixelColor(i, 0, 255, 0);
        strip.setPixelColor(i+1, 150, 255, 0);
        strip.show();
        delay(300);
        strip.setPixelColor(i, 0, 0, 0);
        strip.setPixelColor(i+1, 0, 0, 0);
        strip.show();
        if(efeito != 1) return;
        }
        
    }

    while (efeito == 2) { // Verde-Verde
        
        for(int i = 0; i < NUM_LEDS; i++) {
            strip.setBrightness(Alpha());
            strip.setPixelColor(i, 255, 0, 0);
            strip.setPixelColor(i+1, 255, 0, 0);
            strip.show();
            delay(300);
            strip.setPixelColor(i, 0, 0, 0);
            strip.setPixelColor(i+1, 0, 0, 0);
            strip.show();
            if(efeito != 2) return;
        }
        
    } 

    while (efeito == 3) { // Arco-Iris

            for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
                if(efeito != 3) return;

                for(int i=0; i<NUM_LEDS; i++) { 
                    int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
                    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
                    strip.setBrightness(Alpha());
                    if(efeito != 3) return;
            }

                strip.show();
                delay(10);
            }
        
    }

}