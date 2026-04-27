#include <TFT_eSPI.h>
#include "fundo_temp_noite.h"
#include "fundo_temp_dia.h"
#include "escala_RPM_uno_dia.h"
#include "escala_RPM_uno_noite.h"
#include "capa_ponteiro_uno.h"
#include "ponteiro_uno.h"
#include "apresentacao_uno.h"


void checa_botoes();
void checaSerial();
void ajusta_brilho();
static void apresentacao();
static void mostra_temperatura();
static void mostra_rpm();
static void mostra_hallmeter();

#define bt_hr 38
#define bt_min 34
#define sel_butt 21 //botão para troca de funções;
int filtro_botao=0; //filtro para o botao de seleção
int func = 0; //variavel para armazenar a função 


#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

//////////////////////////////////////////LCD///////////////////////////////////////////

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite mostrador = TFT_eSprite(&tft);
TFT_eSprite ponteiro_uno = TFT_eSprite(&tft);
TFT_eSprite capa_ponteiro = TFT_eSprite(&tft);
TFT_eSprite rascunho = TFT_eSprite(&tft);

uint16_t angle=0;

// setting PWM properties

const int freq = 5000;
const int ledChannel = 2;
const int resolution = 10;
int targetTime = 0;



int leitura_adc=0;
int filtro_adc=0;
float angulo_tmp=0;
float temperatura = 0;
char texto_temp[6];
int escala_temp_atual = 0; //indice para identificar qual escala de temperatura atual;
int escala_temp_ant = 0; //indice para identificar qual escala de temperatura anterior;
float AFR_value=0;
float temperatura_volts=0;
boolean carregou_fundo = false;
boolean led_state = false;
int temper_adc;
int contador=0;
int targeTime2=0;
int filtro_luz=0;
int luz=0;
int indice_luz=0;
int indice_temp=0;
int prefiltro_luz[20];
int prefiltro_temp[20];
int inten_pisca=0;
int dac_luz=0;
int temporiza_apresentacao = 0;
boolean apresentou=false;
boolean dia = false;

//timeval tv;//Cria a estrutura temporaria para funcao abaixo.

char hora_formatada[64];
char data_formatada[64];




float angulo_ponteiro=0;
int DAC_volt = 0;
int prefiltro_temperatura[20];
int indice_temperatura = 0;
float temperatura_ADC = 0;
float temperatura_volt = 0;
float AFR = 0;

void setup()
{

////////////////////////////////////////////Temporização/////////////////////////////
  
  targetTime = millis() + 100;
  Serial.begin(2000000);

////////////////////////////////////////Inicializa o timer 0/////////////////////////
/*
if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
{
  Serial.print(F("Starting  ITimer0 OK, millis() = ")); Serial.println(millis());
}
else
  Serial.println(F("Can't set ITimer0. Select another freq. or timer"));

    */
//////////////////////////Inicializa e cria Sprites para LCD TFT//////////////////////

    pinMode(sel_butt, INPUT_PULLUP);
    pinMode(bt_hr, INPUT_PULLUP);
    pinMode(bt_min, INPUT_PULLUP);

    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setPivot(120,120);  
    
    rascunho.createSprite(240,320);
    rascunho.setRotation(0);
    rascunho.setSwapBytes(true);
    rascunho.setPivot(115,115);
  

    ponteiro_uno.createSprite(7,174);
    ponteiro_uno.setSwapBytes(true);
    ponteiro_uno.pushImage(0,0,7,174,pont_uno);
    ponteiro_uno.setPivot(4,174);
    
    capa_ponteiro.createSprite(51,104);
    capa_ponteiro.setSwapBytes(true);
    capa_ponteiro.pushImage(0,0,51,104,capa_ponteiro_uno);
    capa_ponteiro.setPivot(0,52);

    func = 1;
    temporiza_apresentacao = millis() + 3000;
}

void loop()
{
  
  contador++;
  if (targetTime < millis())
  {
    targetTime = millis() + 30;
    contador=0;
  if (millis()>temporiza_apresentacao)
    {
      if (apresentou){carregou_fundo=false; apresentou = false;}
      switch (func)
      {
        case 0:
          //atualiza_relogio(); 
          //mostra_relogio();
          func=1;
          break;
        case 1:
          mostra_temperatura();
          break;
        case 2:
          mostra_rpm();
          break;
        case 3:
          mostra_hallmeter();
          break;
      }
    }
  else {apresentacao();}
    ajusta_brilho(); //ajusta o brilho uma vez por segundo;
  }
  if (targeTime2 < millis())
    {
      targeTime2 = millis()+5;
      checa_botoes();
      checaSerial();
    }
}


void checa_botoes()
{
  if (!digitalRead(bt_hr)) {filtro_botao++;}
  if (!digitalRead(bt_min)) {filtro_botao++;}
  if (!digitalRead(sel_butt)) {filtro_botao++;}
  else 
  {
    if (filtro_botao > 1 && digitalRead(bt_hr) && digitalRead(bt_min))
      {
        func++; carregou_fundo = false;
        if (func > 3) {func=0;}
        filtro_botao=0;
      }
  }
}

static void apresentacao()////////////////////////////Função para mostrar a temperatura/////////////////////////////// 
{
  if (carregou_fundo == false)
    {
      mostrador.deleteSprite();
      mostrador.createSprite(240,320);
      mostrador.setSwapBytes(true);
      mostrador.pushImage(0,0,240,320,apresentacao_mille_fire);
      mostrador.setPivot(0,160);
      rascunho.setPivot(0,160);
      mostrador.pushRotated(&rascunho, 0.0, TFT_BLACK); // mostrador is the destination Sprite
      rascunho.pushSprite(0, 0, TFT_BLACK);
      carregou_fundo=true;
      apresentou=true;
    }
}


static void mostra_temperatura()////////////////////////////Função para mostrar a temperatura/////////////////////////////// 
{
  if (carregou_fundo == false)
    {
      mostrador.deleteSprite();
      mostrador.createSprite(240,320);
      mostrador.setSwapBytes(true);
      if (dia){mostrador.pushImage(0,0,240,320,fundo_temp_dia);}
      else {mostrador.pushImage(0,0,240,320,fundo_temp_noite);}
      mostrador.setPivot(0,160);
      rascunho.setPivot(0,160);
      filtro_adc=110;
      carregou_fundo=true;
    }
  prefiltro_temperatura[indice_temperatura]=analogRead(7);// atenção!!! para funcionamento normal não pode dividir
  indice_temperatura++;
  if (indice_temperatura>19) indice_temperatura=0;
  for (int i = 0; i < 20; i++){temperatura_ADC += prefiltro_temperatura[i];}
  temperatura_ADC /= 20;
  temperatura_volt=temperatura_ADC*0.8058;
  temperatura_ADC=4095-temperatura_ADC;
  angulo_ponteiro=map(dac_luz, 40, 240, 136, 45);
  rascunho.fillRect(0, 0, 240, 320, 0x0001);
  mostrador.pushRotated(&rascunho, 0.0, TFT_BLACK); // mostrador is the destination Sprite
  ponteiro_uno.pushRotated(&rascunho, angulo_ponteiro, TFT_BLACK); // mostrador is the destination Sprite
  capa_ponteiro.pushRotated(&rascunho, 0, TFT_BLACK);
   Serial.print("temperatura volts = ");Serial.printf("%3.3f",temperatura_volt);
   Serial.print("mV      Angulo = "); Serial.printf("%3.2f",angulo_ponteiro);
   Serial.print("      ADC = "); Serial.printf("%4.2f",temperatura_ADC);
   Serial.print("      AFR = "); Serial.printf("%2.3f \n",AFR);

  rascunho.pushSprite(0, 0, TFT_BLACK); 
}

static void mostra_rpm()////////////////////////////////Função para mostrar o relogio////////////////////////////////// 
{
  if (carregou_fundo == false)
    {
      mostrador.deleteSprite();
      mostrador.createSprite(240,320);
      mostrador.setSwapBytes(true);
      if (dia){mostrador.pushImage(0,0,240,320,fundo_RPM_dia);}
      else {mostrador.pushImage(0,0,240,320,fundo_RPM_noite);}
      mostrador.setPivot(0,160);
      rascunho.setPivot(0,160);
      filtro_adc=110;
      carregou_fundo=true;
    }
  prefiltro_temperatura[indice_temperatura]=analogRead(7);// atenção!!! para funcionamento normal não pode dividir
  indice_temperatura++;
  if (indice_temperatura>19) indice_temperatura=0;
  for (int i = 0; i < 20; i++){temperatura_ADC += prefiltro_temperatura[i];}
  temperatura_ADC /= 20;
  temperatura_volt=temperatura_ADC*0.8058;
  temperatura_ADC=4095-temperatura_ADC;
  angulo_ponteiro=map(dac_luz, 40, 240, 45, 136);
  rascunho.fillRect(0, 0, 240, 320, 0x0001);
  mostrador.pushRotated(&rascunho, 0.0, TFT_BLACK); // mostrador is the destination Sprite
  ponteiro_uno.pushRotated(&rascunho, angulo_ponteiro, TFT_BLACK); // mostrador is the destination Sprite
  capa_ponteiro.pushRotated(&rascunho, 0, TFT_BLACK);
   Serial.print("temperatura volts = ");Serial.printf("%3.3f",temperatura_volt);
   Serial.print("mV      Angulo = "); Serial.printf("%3.2f",angulo_ponteiro);
   Serial.print("      ADC = "); Serial.printf("%4.2f",temperatura_ADC);
   Serial.print("      AFR = "); Serial.printf("%2.3f \n",AFR);

  rascunho.pushSprite(0, 0, TFT_BLACK); 
}

static void mostra_hallmeter()
{
 func=1;
}


void ajusta_brilho()////////////////////////Função que ajusta o brilho automaticamente//////////////////////////////////
{
  prefiltro_luz[indice_luz]=analogRead(7);
  indice_luz++;
  if (indice_luz>19) indice_luz=0;
  luz=0;
  for (int i = 0; i < 20; i++){luz += prefiltro_luz[i];}
  luz /= 20;
  int mapeado=0;
  if (luz>4095){luz=4095;}
  if (filtro_luz > luz) {filtro_luz = filtro_luz - ((filtro_luz - luz) / 5); filtro_luz--;}
  if (filtro_luz < luz) {filtro_luz = filtro_luz + ((luz - filtro_luz) / 5); filtro_luz++;}
  mapeado = map(filtro_luz,0,4095,40,240);
  inten_pisca = map(filtro_luz,0,4095,1,20);
  dac_luz=mapeado;
  if ((dac_luz>100) && (!dia)){dia = true; carregou_fundo = false;}
  if ((dac_luz<=100) && (dia)){dia = false; carregou_fundo = false;}
  analogWrite(2,mapeado);
}

bool TimerHandler0(struct repeating_timer *t)
{
    if (led_state == false) {led_state = true; analogWrite(25, inten_pisca);} //Liga o led de status
    else {led_state = false; analogWrite(25, 0);} //desiga o led de status    
  return true;
}

void checaSerial() /////////////////////////////Chegada de caracteres na porta serial////////////////////////////////////
{
  char ch; // caracter recebido
  int pos=0; // indica a posição atual do buffer;
  char buff[5]; //buffer de recepção de dados seriais
  int resultado=0;
  int set=0;
  while (Serial.available()) 
  {
    // recebe um novo caracter:
    ch = (char)Serial.read();
    if ((ch=='h') || (ch=='m') || (ch=='f') || (ch=='v')){pos=0;} //se o caracter recebido = c, coloca o buffer na posição inicial
    if (ch=='h')set=1;
    if (ch=='m')set=2;
    if (ch=='v')set=3;
    if (ch=='f'){func++; if (func>3)func=0; carregou_fundo = false;}
    buff[pos]=ch;
    pos++;
    if (pos>2 && set<3) 
    {
      pos=0;
      resultado=((int)buff[1]-48)*10+ ((int)buff[2]-48);
      if (set==1)
        {
          if (resultado >=0 && resultado <=23)
          {

            break;
          }
        }
      if (set==2)
        {
          if (resultado >=0 && resultado <=59)
          {

            break;
          }
        }
    Serial.flush();
    }
    if (pos>3)
    {
      pos=0;
      resultado=((int)buff[1]-48)*100 + ((int)buff[2]-48)*10 + ((int)buff[3]-48);
      if (resultado>1000){resultado=1000;}
      DAC_volt=map(resultado, 0, 1000, 0, 77);
      //analogWrite(20,DAC_volt);
      Serial.println(DAC_volt);
      Serial.flush();
      break;
    }
  }
}
