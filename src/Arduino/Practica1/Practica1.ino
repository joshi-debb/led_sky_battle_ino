#include "LedControl.h"

#define DIN 4
#define CLK 2
#define LOAD 3
#define btnK 13
#define btnD 12
#define btnDer 11
#define btnIzq 10
#define LED 9
LedControl matriz = LedControl(DIN, CLK, LOAD, 1);

// Variables Globales
int estado = 0; // 0.Mensaje 1.Menu Principal 2.Juego 3.Pausa 4.Estadisticas 5.Configuración
int nivel = 1;
int vidas = 3;
int vidasinit = 3;
int velocidad;
int *arreglo = NULL;
int elementos = 0;
int suma = 0;
int puntos = 0;
int contDestrucciones = 0;
int edificios_rest = 0;
int velocidadtmp;

// pRA1-vJ-2023<gRUPO I>

void setup()
{
  randomSeed(90);
  Serial.begin(9600);
  // Inicializa la matriz con driver
  matriz.shutdown(0, false);
  matriz.setIntensity(0, 8);

  // Inicializa la matriz
  for (int i = 14; i < 22; i++)
    pinMode(i, OUTPUT);
  for (int j = 22; j < 30; j++)
    pinMode(j, OUTPUT);
  for (int i = 14; i < 22; i++)
    digitalWrite(i, HIGH);
  for (int j = 22; j < 30; j++)
    digitalWrite(j, LOW);

  pinMode(13, INPUT);
  pinMode(btnK, INPUT);
  pinMode(btnD, INPUT);
  pinMode(btnDer, INPUT);
  pinMode(btnIzq, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(LED, OUTPUT);

  velocidad = map(analogRead(A0), 0, 1000, 1, 10);
}

byte buffer[8][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

byte menu[8][16] = {
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0},
  {0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
};

byte tmps[8][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

bool Frase[8][161] ={
  {0,0,0,0,0,0,0, 0,1,1,1,1,1,0, 0,0,0,1,1,0,0, 0,0,0,1,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,1,1,1,1,0, 0,0,0,0,0,0,0, 0,0,1,1,1,1,0, 0,0,1,1,1,1,0, 0,0,1,1,1,1,0, 0,0,1,1,1,1,0, 0,0,0,0,1,1, 0,0,0,0,0,0,0, 0,1,1,1,1,1,0, 0,1,1,0,0,1,1, 0,1,1,1,1,1,0, 0,0,1,1,1,1,0, 0,0,0,0,0,0,0, 0,1,1,1,1,1,1, 0,1,1,0,0,0, },
  {0,0,0,0,0,0,0, 0,1,1,0,0,1,1, 0,0,1,1,1,1,0, 0,0,0,1,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,1,1,1,1,0, 0,0,0,0,0,0,0, 0,1,1,1,1,1,1, 0,1,1,0,0,1,1, 0,1,1,1,1,1,1, 0,1,1,1,1,1,1, 0,0,0,1,1,0, 0,0,0,0,0,0,0, 0,1,1,0,0,1,1, 0,1,1,0,0,1,1, 0,1,1,0,0,1,1, 0,1,1,1,1,1,1, 0,0,0,0,0,0,0, 0,1,1,1,1,1,1, 0,0,1,1,0,0, },
  {0,1,1,1,1,1,0, 0,1,1,0,0,1,1, 0,1,1,0,0,1,1, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0, 0,1,1,0,0,1,1, 0,0,0,0,1,1,0, 0,0,0,0,0,0,0, 0,1,1,0,0,1,1, 0,1,1,0,0,1,1, 0,1,1,0,0,1,1, 0,0,0,0,0,1,1, 0,0,1,1,0,0, 0,0,1,1,1,1,1, 0,1,1,0,0,1,1, 0,1,1,0,0,1,1, 0,1,1,0,0,1,1, 0,1,1,0,0,1,1, 0,0,0,0,0,0,0, 0,0,0,1,1,0,0, 0,0,0,1,1,0, },
  {0,1,1,0,0,1,1, 0,1,1,1,1,1,0, 0,1,1,0,0,1,1, 0,0,0,1,1,0,0, 0,1,1,1,1,1,1, 0,1,1,0,0,1,1, 0,0,0,0,1,1,0, 0,1,1,1,1,1,1, 0,0,0,0,0,1,1, 0,1,1,0,1,1,1, 0,0,0,0,0,1,1, 0,0,0,1,1,1,0, 0,1,1,0,0,0, 0,1,1,0,0,1,1, 0,1,1,1,1,1,0, 0,1,1,0,0,1,1, 0,1,1,0,0,1,1, 0,1,1,0,0,1,1, 0,0,0,0,0,0,0, 0,0,0,1,1,0,0, 0,0,0,0,1,1, },
  {0,1,1,0,0,1,1, 0,1,1,1,1,0,0, 0,1,1,1,1,1,1, 0,0,0,1,1,0,0, 0,1,1,1,1,1,1, 0,1,1,0,0,1,1, 0,0,0,0,1,1,0, 0,1,1,1,1,1,1, 0,0,0,0,1,1,0, 0,1,1,1,0,1,1, 0,0,0,0,1,1,0, 0,0,0,1,1,1,0, 0,1,1,0,0,0, 0,1,1,0,0,1,1, 0,1,1,1,1,0,0, 0,1,1,0,0,1,1, 0,1,1,1,1,1,0, 0,1,1,0,0,1,1, 0,0,0,0,0,0,0, 0,0,0,1,1,0,0, 0,0,0,0,1,1, },
  {0,1,1,1,1,1,0, 0,1,1,0,1,1,0, 0,1,1,0,0,1,1, 0,0,0,1,1,0,0, 0,0,0,0,0,0,0, 0,1,1,0,0,1,1, 0,1,1,0,1,1,0, 0,0,0,0,0,0,0, 0,0,0,1,1,0,0, 0,1,1,0,0,1,1, 0,0,0,1,1,0,0, 0,0,0,0,0,1,1, 0,0,1,1,0,0, 0,0,1,1,1,1,1, 0,1,1,0,1,1,0, 0,1,1,0,0,1,1, 0,1,1,0,0,0,0, 0,1,1,0,0,1,1, 0,0,0,0,0,0,0, 0,0,0,1,1,0,0, 0,0,0,1,1,0, },
  {0,1,1,0,0,0,0, 0,1,1,0,0,1,1, 0,1,1,0,0,1,1, 0,1,1,1,1,1,1, 0,0,0,0,0,0,0, 0,0,1,1,1,1,0, 0,1,1,1,1,1,0, 0,0,0,0,0,0,0, 0,0,1,1,1,1,1, 0,1,1,0,0,1,1, 0,0,1,1,1,1,1, 0,1,1,1,1,1,1, 0,0,0,1,1,0, 0,0,0,0,0,1,1, 0,1,1,0,0,1,1, 0,1,1,1,1,1,1, 0,1,1,0,0,0,0, 0,1,1,1,1,1,1, 0,0,0,0,0,0,0, 0,1,1,1,1,1,1, 0,0,1,1,0,0, },
  {0,1,1,0,0,0,0, 0,1,1,0,0,1,1, 0,1,1,0,0,1,1, 0,1,1,1,1,1,1, 0,0,0,0,0,0,0, 0,0,0,1,1,0,0, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0, 0,1,1,1,1,1,1, 0,0,1,1,1,1,0, 0,1,1,1,1,1,1, 0,0,1,1,1,1,0, 0,0,0,0,1,1, 0,0,1,1,1,1,0, 0,1,1,0,0,1,1, 0,0,1,1,1,1,0, 0,1,1,0,0,0,0, 0,0,1,1,1,1,0, 0,0,0,0,0,0,0, 0,1,1,1,1,1,1, 0,1,1,0,0,0, }
};

void pintarLED(int x, int y)
{
  digitalWrite(14 + y, LOW);
  digitalWrite(22 + x, HIGH);
  delay(1);
  digitalWrite(14 + y, HIGH);
  digitalWrite(22 + x, LOW);
}

int position = 146;
bool direccion = true; // DER-IZQ = false,  IZQ-DER = true
void mostrarMensaje()
{
  matriz.clearDisplay(0);
  if (digitalRead(btnD))
  {
    direccion = !direccion;
    delay(100);
  }

  if (direccion)
  {
    if (position <= 0)
      position = 161;
    position = position - 1;
  }
  else
  {
    position = position + 1;
  }
  // Pintar las filas - Matriz Driver
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      matriz.setLed(0, i, j, Frase[i][(j + position) % 161]);
    }
  }

  // Pintar las filas - Matriz sin Driver
  for (int k = 0; k < 3; k++)
  {
    for (int i = 0; i < 8; i++)
    {
      for (int j = 8; j < 16; j++)
      {
        if (Frase[i][(j + position) % 161])
          pintarLED(j - 8, i);
      }
    }
  }
  int velo = map(velocidad, 10, 400, 1, 10);
  int vel = 230 - (velo*100) / 100 * 23;
  delay(vel);
}

void clearBuff()
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      buffer[i][j] = 0;
    }
  }
}

void clearTmp()
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      tmps[i][j] = 0;
    }
  }
}

void calcularSuma()
{
  suma = 0;
  int limite = elementos < 5 ? elementos : 5; // Establecer el límite en el mínimo entre elementos y 5
  for (int i = 0; i < limite; i++)
  {

    suma += arreglo[i];
  }
}

void porcentaje()
{

  int limite = elementos < 5 ? elementos : 5; // Establecer el límite en el mínimo entre elementos y 5

  for (int i = 0; i < limite; i++)
  {
    float porcentaje1 = ((float)(arreglo[i]) / suma * 100);
    // usar map
    int ledsEncendidos = map(porcentaje1, 0, 100, 0, 8);

    llenarBuffer(i, ledsEncendidos);
  }
}

void llenarBuffer(int pos, int leds)
{

  for (int i = 7; i >= 8 - leds; i--)
  {
    if (pos == 0)
    {
      buffer[i][1] = 1;
      buffer[i][2] = 1;
    }
    else if (pos == 1)
    {
      buffer[i][4] = 1;
      buffer[i][5] = 1;
    }
    else if (pos == 2)
    {
      buffer[i][7] = 1;
      buffer[i][8] = 1;
    }
    else if (pos == 3)
    {
      buffer[i][10] = 1;
      buffer[i][11] = 1;
    }
    else if (pos == 4)
    {
      buffer[i][13] = 1;
      buffer[i][14] = 1;
    }
  }
}

void agregarDato(int nuevoDato)
{

  elementos++;
  arreglo = (int *)realloc(arreglo, elementos * sizeof(int));
  if (arreglo == NULL)
  {
    exit(1);
  }
  for (int j = elementos - 1; j > 0; j--)
  {
    arreglo[j] = arreglo[j - 1];
  }
  // Asignar el nuevo dato en la posición 0
  arreglo[0] = nuevoDato;
}

void pintartmp(int valor){
  switch(valor){
    case 1:
      for(int j=0;j<9;j++){
        tmps[j][7] = 1;
        tmps[j][8] = 1;
      }
      tmps[1][6] = 1;
      tmps[2][5] = 1;
      tmps[7][6] = 1;
      tmps[7][9] = 1;
      break;
    case 2:
      for(int j=0;j<6;j++){
        tmps[j][7] = 1;
        tmps[j][8] = 1;
      }
      for(int i=6;i<8;i++){
        tmps[i][5] = 1;
        tmps[i][6] = 1;
      }
      for(int k=7;k<10;k++){
        tmps[7][k] = 1;
      }
      tmps[5][6] = 1;
      tmps[2][5] = 1;
      tmps[1][6] = 1;
      tmps[5][7] = 1;
      tmps[1][9] = 1;
      tmps[2][9] = 1;
      tmps[3][9] = 1;
      break;
    case 3:
      for(int j=5;j<9;j++){
        tmps[0][j] = 1;
        tmps[1][j] = 1;
        tmps[3][j] = 1;
        tmps[4][j] = 1;
        tmps[6][j] = 1;
        tmps[7][j] = 1;
        if (j==8){
          tmps[1][j+1] = 1;
          tmps[2][j] = 1;
          tmps[2][j+1] = 1;

          tmps[6][j+1] = 1;
          tmps[5][j] = 1;
          tmps[5][j+1] = 1;
        }
      }
      break;
    case 4:
      for(int j=0;j<5;j++){
        tmps[j][6] = 1;
        tmps[j][9] = 1;
      }
      for(int j=4;j<8;j++){
        tmps[j][8] = 1;
        tmps[j][9] = 1;
      }
      tmps[4][7] = 1;
      break;
    case 5:
      for(int j=6;j<10;j++){
        tmps[0][j] = 1;
        tmps[1][j] = 1;
        tmps[4][j] = 1;
        tmps[7][j] = 1;
      }
      tmps[3][6] = 1;
      tmps[2][6] = 1;

      tmps[6][8] = 1;
      tmps[5][8] = 1;
      tmps[5][9] = 1;
      tmps[6][9] = 1;
      break;
    case 6:
      for(int j=6;j<10;j++){
        tmps[0][j] = 1;
        tmps[1][j] = 1;
        tmps[4][j] = 1;
        tmps[7][j] = 1;
      }
      tmps[3][6] = 1;
      tmps[2][6] = 1;

      tmps[5][6] = 1;
      tmps[6][6] = 1;

      tmps[5][9] = 1;
      tmps[6][9] = 1;
      break;
    case 7:
      for(int j=6;j<10;j++){
        tmps[0][j] = 1;
        tmps[1][j] = 1;
      }
      tmps[7][9] = 1;

      tmps[5][9] = 1;
      tmps[6][9] = 1;

      tmps[2][9] = 1;
      tmps[3][9] = 1;
      tmps[4][9] = 1;
      break;
    case 8:
      for(int j=6;j<10;j++){
        tmps[0][j] = 1;
        tmps[1][j] = 1;
        tmps[4][j] = 1;
        tmps[7][j] = 1;
      }
      tmps[3][6] = 1;
      tmps[2][6] = 1;

      tmps[5][6] = 1;
      tmps[6][6] = 1;

      tmps[5][9] = 1;
      tmps[6][9] = 1;

      tmps[2][9] = 1;
      tmps[3][9] = 1;
      break;
    case 9:
      for(int j=6;j<10;j++){
        tmps[0][j] = 1;
        tmps[1][j] = 1;
        tmps[4][j] = 1;
        tmps[7][j] = 1;
      }
      tmps[3][6] = 1;
      tmps[2][6] = 1;

      tmps[5][9] = 1;
      tmps[6][9] = 1;

      tmps[2][9] = 1;
      tmps[3][9] = 1;
      break;
    case 10:
      for(int j=0;j<9;j++){
        tmps[j][3] = 1;
        tmps[j][4] = 1;
      }
      tmps[1][2] = 1;
      tmps[2][1] = 1;
      tmps[7][2] = 1;
      tmps[7][5] = 1;

      for(int k=0;k<8;k++){
        tmps[k][10] = 1;
        tmps[k][13] = 1;
      }
      tmps[0][11] = 1;
      tmps[0][12] = 1;
      tmps[7][11] = 1;
      tmps[7][12] = 1;
      break;
    case 11:
      for(int j=0;j<9;j++){
        tmps[j][3] = 1;
        tmps[j][4] = 1;
        tmps[j][11] = 1;
        tmps[j][12] = 1;
      }
      tmps[1][2] = 1;
      tmps[2][3] = 1;
      tmps[7][2] = 1;
      tmps[7][5] = 1;
      tmps[1][10] = 1;
      tmps[2][9] = 1;
      tmps[7][10] = 1;
      tmps[7][13] = 1;
      break;
    case 12:
      // Uno
      for(int j=0;j<9;j++){
        tmps[j][3] = 1;
        tmps[j][4] = 1;
      }
      tmps[1][2] = 1;
      tmps[2][1] = 1;
      tmps[7][2] = 1;
      tmps[7][5] = 1;

      // Dos
      for(int j=0;j<6;j++){
        tmps[j][12] = 1;
        tmps[j][13] = 1;
      }
      for(int i=6;i<8;i++){
        tmps[i][10] = 1;
        tmps[i][11] = 1;
      }
      for(int k=7;k<10;k++){
        tmps[7][k+5] = 1;
      }
      tmps[5][11] = 1;
      tmps[2][10] = 1;
      tmps[1][11] = 1;
      tmps[5][12] = 1;
      tmps[1][14] = 1;
      tmps[2][14] = 1;
      tmps[3][14] = 1;
      break;
    case 13:
      // Uno
      for(int j=0;j<9;j++){
        tmps[j][3] = 1;
        tmps[j][4] = 1;
      }
      tmps[1][2] = 1;
      tmps[2][1] = 1;
      tmps[7][2] = 1;
      tmps[7][5] = 1;

      // Tres
      for(int j=5;j<9;j++){
        tmps[0][j+5] = 1;
        tmps[1][j+5] = 1;
        tmps[3][j+5] = 1;
        tmps[4][j+5] = 1;
        tmps[6][j+5] = 1;
        tmps[7][j+5] = 1;
        if (j==8){
          tmps[1][j+5+1] = 1;
          tmps[2][j+5] = 1;
          tmps[2][j+5+1] = 1;

          tmps[6][j+5+1] = 1;
          tmps[5][j+5] = 1;
          tmps[5][j+5+1] = 1;
        }
      }
      break;
    case 14:
      // Uno
      for(int j=0;j<9;j++){
        tmps[j][3] = 1;
        tmps[j][4] = 1;
      }
      tmps[1][2] = 1;
      tmps[2][1] = 1;
      tmps[7][2] = 1;
      tmps[7][5] = 1;

      //Cuatro
      for(int j=0;j<5;j++){
        tmps[j][11] = 1;
        tmps[j][14] = 1;
      }
      for(int j=4;j<8;j++){
        tmps[j][13] = 1;
        tmps[j][14] = 1;
      }
      tmps[4][12] = 1;
      break;
    case 15:
      // Uno
      for(int j=0;j<9;j++){
        tmps[j][3] = 1;
        tmps[j][4] = 1;
      }
      tmps[1][2] = 1;
      tmps[2][1] = 1;
      tmps[7][2] = 1;
      tmps[7][5] = 1;
      // Cinco
      for(int j=6;j<10;j++){
        tmps[0][j+5] = 1;
        tmps[1][j+5] = 1;
        tmps[4][j+5] = 1;
        tmps[7][j+5] = 1;
      }
      tmps[3][11] = 1;
      tmps[2][11] = 1;

      tmps[6][13] = 1;
      tmps[5][13] = 1;
      tmps[5][14] = 1;
      tmps[6][14] = 1;
      break;
    case 16:
      // Uno
      for(int j=0;j<9;j++){
        tmps[j][3] = 1;
        tmps[j][4] = 1;
      }
      tmps[1][2] = 1;
      tmps[2][1] = 1;
      tmps[7][2] = 1;
      tmps[7][5] = 1;

      // Seis
      for(int j=6;j<10;j++){
        tmps[0][j+5] = 1;
        tmps[1][j+5] = 1;
        tmps[4][j+5] = 1;
        tmps[7][j+5] = 1;
      }
      tmps[3][11] = 1;
      tmps[2][11] = 1;

      tmps[5][11] = 1;
      tmps[6][11] = 1;

      tmps[5][14] = 1;
      tmps[6][14] = 1;
      break;
    case 17:
      // Uno
      for(int j=0;j<9;j++){
        tmps[j][3] = 1;
        tmps[j][4] = 1;
      }
      tmps[1][2] = 1;
      tmps[2][1] = 1;
      tmps[7][2] = 1;
      tmps[7][5] = 1;

      //Siete
      for(int j=6;j<10;j++){
        tmps[0][j+5] = 1;
        tmps[1][j+5] = 1;
      }
      tmps[7][14] = 1;

      tmps[5][14] = 1;
      tmps[6][14] = 1;

      tmps[2][14] = 1;
      tmps[3][14] = 1;
      tmps[4][14] = 1;
      break;
    case 18:
      // Uno
      for(int j=0;j<9;j++){
        tmps[j][3] = 1;
        tmps[j][4] = 1;
      }
      tmps[1][2] = 1;
      tmps[2][1] = 1;
      tmps[7][2] = 1;
      tmps[7][5] = 1;

      // Ocho
      for(int j=6;j<10;j++){
        tmps[0][j+5] = 1;
        tmps[1][j+5] = 1;
        tmps[4][j+5] = 1;
        tmps[7][j+5] = 1;
      }
      tmps[3][11] = 1;
      tmps[2][11] = 1;

      tmps[5][11] = 1;
      tmps[6][11] = 1;

      tmps[5][14] = 1;
      tmps[6][14] = 1;

      tmps[2][14] = 1;
      tmps[3][14] = 1;
      break;
    case 19:
      // Uno
      for(int j=0;j<9;j++){
        tmps[j][3] = 1;
        tmps[j][4] = 1;
      }
      tmps[1][2] = 1;
      tmps[2][1] = 1;
      tmps[7][2] = 1;
      tmps[7][5] = 1;

      // Nueve
      for(int j=6;j<10;j++){
        tmps[0][j+5] = 1;
        tmps[1][j+5] = 1;
        tmps[4][j+5] = 1;
        tmps[7][j+5] = 1;
      }
      tmps[3][11] = 1;
      tmps[2][11] = 1;

      tmps[5][14] = 1;
      tmps[6][14] = 1;

      tmps[2][14] = 1;
      tmps[3][14] = 1;
      break;
    default:
      for(int j=0;j<8;j++){
        tmps[j][6] = 1;
        tmps[j][9] = 1;
      }
      tmps[0][7] = 1;
      tmps[0][8] = 1;
      tmps[7][7] = 1;
      tmps[7][8] = 1;
      break;
  }
}
// Muestra la matriz
unsigned long tiempoActual = 0;
void mostrarMatriz()
{
  // Pintar las filas - Matriz Driver
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      matriz.setLed(0, i, j, buffer[i][j] == 1 ? true : false);
    }
  }
  // Pintar las filas - Matriz sin Driver
  for (int k = 0; k < 4; k++)
  {
    for (int i = 0; i < 8; i++)
    {
      for (int j = 8; j < 16; j++)
      {
        if (buffer[i][j] == 1)
          pintarLED(j - 8, i);
        else if ((micros() - tiempoActual) >= 50)
        {
          tiempoActual = micros();
        }
      }
    }
  }
}

void mostrarMatrizTmp()
{
  // Pintar las filas - Matriz Driver
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      matriz.setLed(0, i, j, tmps[i][j] == 1 ? true : false);
    }
  }
  // Pintar las filas - Matriz sin Driver
  for (int k = 0; k < 4; k++)
  {
    for (int i = 0; i < 8; i++)
    {
      for (int j = 8; j < 16; j++)
      {
        if (tmps[i][j] == 1)
          pintarLED(j - 8, i);
        else
          delayMicroseconds(200);
      }
    }
  }
}

void mostrarconfig()
{
  // Borrar config
  for (int i = 0; i < velocidadtmp; i++)
  {
    buffer[2][i] = 0;
  }
  for (int i = 0; i < vidas; i++)
  {
    buffer[5][i] = 0;
  }
  //---

  // Leer los potenciometros
  velocidad = map(analogRead(A0), 0, 1000, 10, 400);
  vidas = map(analogRead(A1), 0, 1000, 3, 10);
  vidasinit = vidas;
  //---

  // Pintar las barras
  velocidadtmp = map(velocidad, 10, 400, 1, 16);
  for (int i = 0; i < velocidadtmp; i++)
  {
    buffer[2][i] = 1;
  }
  for (int i = 0; i < vidas; i++)
  {
    buffer[5][i] = 1;
  }
  //---

  // Calculo valor real velocidad
  //velocidad = velocidad * 100;
  //---

  // Logs
  Serial.println(velocidad);
  Serial.println(vidas);

  // delay(1);
}

void mostrarpausa()
{
  //
}

void borrarconfig()
{
  for (int i = 0; i < (velocidad / 100); i++)
  {
    buffer[2][i] = 0;
  }
  for (int i = 0; i < vidas; i++)
  {
    buffer[5][i] = 0;
  }
}

void mostrarmenu()
{
  // Pintar las filas - Matriz Driver
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      matriz.setLed(0, i, j, menu[i][j] == 1 ? true : false);
    }
  }
  // Pintar las filas - Matriz sin Driver
  for (int k = 0; k < 4; k++)
  {
    for (int i = 0; i < 8; i++)
    {
      for (int j = 8; j < 16; j++)
      {
        if (menu[i][j] == 1)
          pintarLED(j - 8, i);
      }
    }
  }
}

void color_floor(short actual_pos, short quantity)
{
  for (int i = 0; i < quantity; i++)
  {
    buffer[7][actual_pos + i] = 1;
  }
}

void show_matrix()
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 16; j++)
    {

      if (j == 15)
      {
      }
    }
  }
}

void destroy_buildings(short pos_y)
{
  // Evitamos que agarre un punto inexistente
  if (buffer[7][pos_y] == 0)
  {
    return;
  }
  // Verificamos ambos extremos para hacer la verificación de un solo lado
  bool verification = true;
  short counter = 0;

  // Para destruir los edificios que estan ubicados en la primera posición
  if (pos_y == 0)
  {
    while (verification)
    {
      for (short i = 4; i < 8; i++)
      {
        buffer[i][pos_y] = 0;
      }
      pos_y++;
      if (buffer[7][pos_y] == 0)
      {
        verification = false;
        return;
      }
    }
  }
  // Para destruir los edificios que estan ubicados en la ultima posición
  if (pos_y == 15)
  {
    while (verification)
    {
      for (short i = 4; i < 8; i++)
      {
        buffer[i][pos_y] = 0;
      }
      pos_y--;
      if (buffer[7][pos_y] == 0)
      {
        verification = false;
        return;
      }
    }
  }
  // Cuando no este en una esquina, comenzamos viendo el lado izquierdo
  short pos_y_left_temp = pos_y;
  while (verification)
  {
    for (short i = 4; i < 8; i++)
    {
      buffer[i][pos_y_left_temp] = 0;
    }
    pos_y_left_temp--;
    if (pos_y_left_temp == 0 && buffer[7][pos_y_left_temp] == 1)
    {
      for (short i = 4; i < 8; i++)
      {
        buffer[i][pos_y_left_temp] = 0;
      }
      verification = false;
      break;
    }
    if (buffer[7][pos_y_left_temp] == 0)
    {
      verification = false;
      break;
    }
  }
  // Cuando no este en una esquina, continuamos con lado derecho
  verification = true;
  while (verification)
  {
    pos_y++;
    if (pos_y_left_temp == 15 && buffer[7][pos_y] == 1)
    {
      for (short i = 4; i < 8; i++)
      {
        buffer[i][pos_y] = 0;
      }
      verification = false;
      break;
    }
    if (buffer[7][pos_y] == 0)
    {
      verification = false;
      break;
    }
    for (short i = 4; i < 8; i++)
    {
      buffer[i][pos_y] = 0;
    }
  }
}

void check_matrix()
{
  short minimo_altura = 2;
  short maximo_altura = 5;
  short numeroAleatorio = 0;

  for (int j = 0; j < 16; j++)
  {
    if (buffer[7][j] == 1)
    {
      numeroAleatorio = random(minimo_altura, maximo_altura);
      for (int aleatorio = 0; aleatorio < numeroAleatorio; aleatorio++)
      {
        buffer[7 - aleatorio][j] = 1;
      }
    }
  }
}

// Genera los edificios correspondientes
void new_game()
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      buffer[i][j] = 0;
    }
  }

  short minimo_base = 1;
  short maximo_base = 4;
  short espacio_inicial = 0;
  short minimo_espacio = 1;
  short maximo_espacio = 3;
  short numeroAleatorio = 0;
  short max_space = 15;

  bool complete = true;
  short counter = 0;

  do
  {
    // Primer espacio
    if (counter == 0)
    {
      // Looking for first space
      numeroAleatorio = random(espacio_inicial, maximo_espacio);
      counter += numeroAleatorio;
    }
    else
    {
      numeroAleatorio = random(minimo_espacio, maximo_espacio);
      counter += numeroAleatorio;
    }

    if ((counter + maximo_base) > 16)
    {
      complete = false;
      break;
    }
    // Looking for fist base space
    numeroAleatorio = random(minimo_base, maximo_base);
    counter += numeroAleatorio;
    color_floor(counter - numeroAleatorio, numeroAleatorio);
  } while (complete);

  check_matrix();
}

long int t0 = 0;
long int t1 = 0;

//long int t2 = millis();
//long int t3 = millis();

int contador = 0;

bool presionadoK = false;

// **** variables para el JUEGO NO TOCAR ***********

class Avion
{
public:
  int cuerpoBala = 0;
  void borrarAvion(int yAvion, int xAvion)
  {
    buffer[yAvion][xAvion] = 0;
    buffer[yAvion + 1][xAvion] = 0;
    buffer[yAvion + 1][xAvion + 1] = 0;
    buffer[yAvion + 1][xAvion + 2] = 0;
  }
  void borrarAvion_Izquierda(int yAvion, int xAvion)
  {
    buffer[yAvion][xAvion] = 0;
    buffer[yAvion + 1][xAvion] = 0;
    buffer[yAvion + 1][xAvion - 1] = 0;
    buffer[yAvion + 1][xAvion - 2] = 0;
  }
  void pintarAvion(int yAvion, int xAvion)
  {
    if ((xAvion + 1) < 16)
    {
      cuerpoBala = xAvion + 1;
    }
    else if ((xAvion + 1) == 16)
    {
      cuerpoBala = 0;
    }
    buffer[yAvion][xAvion] = 1;
    buffer[yAvion + 1][xAvion] = 1;
    buffer[yAvion + 1][xAvion + 1] = 1;
    buffer[yAvion + 1][xAvion + 2] = 1;
  }

  void pintarAvion_Izquierda(int yAvion, int xAvion)
  {
    if ((xAvion - 1) > -1)
    {
      cuerpoBala = xAvion - 1;
      buffer[yAvion][xAvion] = 1;
      buffer[yAvion + 1][xAvion] = 1;
      buffer[yAvion + 1][xAvion - 1] = 1;
      if (xAvion > 1)
      {
        buffer[yAvion + 1][xAvion - 2] = 1;
      }
      else if (xAvion == 1)
      {
        buffer[yAvion + 2][15] = 1;
      }
    }
    else if ((xAvion - 1) == -1)
    {
      cuerpoBala = 15;
      buffer[yAvion][xAvion] = 1;
      buffer[yAvion + 1][xAvion] = 1;
      buffer[yAvion + 2][15] = 1;
      buffer[yAvion + 2][14] = 1;
    }
  }
  int getCuerpo()
  {
    return cuerpoBala;
  }
  void Sentido_1(int y)
  {
    buffer[y + 1][getCuerpo()] = 0;
    buffer[y + 1][getCuerpo() + 1] = 0;
  }
  void Sentido_2(int y)
  {
    buffer[y + 1][getCuerpo()] = 0;
    buffer[y + 1][getCuerpo() - 1] = 0;
  }
};

int xAvion = 0;    // Posicion hacia derecha en x
int xAvionIz = 15; // Posicion hacia izquierda en x
int yAvion = 0;    // Variable de caida de avion
// --
int Direccion = 0;         // indica si la direccion del avion 1: izquierda , 0: derecha
int DireccionBala = 0;     // Al momento de hacer el disparo obtine la posicion en la que se presiono el boton
bool RutaBala = false;     // controla el estado de ruta de la bala
Avion avion;               // clase avion
unsigned long TiempoB = 0; // tiempo de la bala
unsigned long TiempoD = 0; // tiempo de la direccio a x
unsigned long TiempoI = 0; // tiempo de la direccion a y
int AsPerdido = 0;

int prueba = 0;
short prueba2 = 1; // niveles
int PosicionActualBala = 0;

void loop()
{
  t1 = millis();
  if (estado == 0)
  {
    mostrarMensaje();
    // Mover el mensaje a la Derecha
    if (digitalRead(btnDer))
    {
      Serial.println("Derecha");
      clearBuff();
      direccion = true;
    }
    // Mover el mensaje a la Izquierda
    if (digitalRead(btnIzq))
    {
      Serial.println("Izquierda");
      clearBuff();
      direccion = false;
    }
    if (digitalRead(btnK))
    {
      if (!presionadoK)
      {
        presionadoK = true;
        t0 = millis();
      }
      else
      {
        long int dif = t1 - t0;
        if (dif >= 1800 && dif <= 2200)
        {
          Serial.println("Presionado");
          digitalWrite(LED, HIGH);
          delay(500); // Que suelte el botón
          digitalWrite(LED, LOW);
          if (!digitalRead(btnK))
          {
            estado = 1; // Cambia al Menu Principal
            clearBuff();
            Serial.println("Cambia al menu principal");
          }
        }
      }
    }
    else
    {
      presionadoK = false;
    }
  }
  else if (estado == 1)
  {
    contDestrucciones = 0;
    vidas = vidasinit;
    puntos = 0;
    mostrarmenu();
    if (digitalRead(btnD))
    {
      // Muestra las Estadísticas
      Serial.println("Estadisticas");
      clearBuff();
      calcularSuma();
      porcentaje();
      mostrarMatriz();
      estado = 4;
      matriz.clearDisplay(0);
      delay(100);
    }
    else if (digitalRead(btnK))
    {
      // Regresa al mensaje inicial
      Serial.println("Mensaje Inicial");
      estado = 0;
      delay(100);
    }
    else if (digitalRead(btnDer))
    {
      // Muestra el menú de configuración
      Serial.println("Configuracion");
      matriz.clearDisplay(0);
      estado = 5;
      delay(100);
    }
    else if (digitalRead(btnIzq))
    {
      // Inicia el Juego
      Serial.println("Juego");
      matriz.clearDisplay(0);
      estado = 2;
      delay(100);
    }
  }
  else if (estado == 2)
  {
    PlayGame();
    if (digitalRead(btnK))
    {
      // Pausa El Juego
      pintartmp(vidas);
      estado = 3;
      delay(600);
    }
  }
  else if (estado == 3)
  {
    clearBuff();
    mostrarMatrizTmp();
    if (digitalRead(btnK))
    {
      if (!presionadoK)
      {
        presionadoK = true;
        t0 = millis();
      }
      else
      {
        long int dif = t1 - t0;
        if (dif >= 1800 && dif <= 2300)
        {
          Serial.println("2 segundos");
          digitalWrite(LED, HIGH);
          delay(450);
          digitalWrite(LED, LOW);
          if (!digitalRead(13))
          {
            clearTmp();
            matriz.clearDisplay(0);
            estado = 2;
            Serial.println("Regresa al Juego");
          }
        }
        if (dif >= 2800 && dif <= 3300)
        {
          Serial.println("3 segundos");
          digitalWrite(LED, HIGH);
          delay(500);
          digitalWrite(LED, LOW);
          if (!digitalRead(13))
          {
            clearTmp();
            matriz.clearDisplay(0);
            estado = 1;
          }
        }
      }
    }
    else
    {
      presionadoK = false;
    }
  }
  else if (estado == 4)
  {
    if (digitalRead(btnK))
    {
      if (!presionadoK)
      {
        presionadoK = true;
        t0 = millis();
      }
      else
      {
        long int dif = t1 - t0;
        if (dif >= 2800 && dif <= 3200)
        {
          Serial.println("Regresa al Menu Principal");
          digitalWrite(LED, HIGH);
          delay(500);
          digitalWrite(LED, LOW);
          if (!digitalRead(13))
          {
            clearTmp();
            clearBuff();
            estado = 1;
          }
        }
      }
    }
    else
    {
      presionadoK = false;
      mostrarMatriz();
    }
  }
  else if (estado == 5)
  {
    mostrarconfig();
    mostrarMatriz();
    if (digitalRead(btnK))
    {
      if (!presionadoK)
      {
        presionadoK = true;
        t0 = millis();
      }
      else
      {
        long int dif = t1 - t0;
        if (dif >= 2800 && dif <= 3200)
        {
          Serial.println("Regresa al Menu Principal");
          digitalWrite(LED, HIGH);
          delay(500);
          digitalWrite(LED, LOW);
          if (!digitalRead(13))
          {
            clearTmp();
            clearBuff();
            estado = 1;
          }
        }
      }
    }
    else
    {
      presionadoK = false;
    }
  }
}

bool Colicion(int direccionx, int direcciony)
{
  if (buffer[direcciony + 1][direccionx + 3] == 1)
  {
    AsPerdido = direccionx;
    return true;
  }
  return false;
}

bool Colicion_Izquierda(int direccionx, int direcciony)
{
  if (buffer[direcciony + 1][direccionx - 3] == 1)
  {
    AsPerdido = direccionx;
    return true;
  }
  return false;
}
// MARIANO
//  **************   PARA GENERAR EDIFICIOS Y DERRUMBAR TORRES  ******************

void new_buildings(int level)
{
  short max_space = 16;
  short min_space = 0;
  short base_randomizer = 0;
  short height_randomizer = 0;
  short min_height = 1;
  short max_height = 5;
  for (short i = 0; i < level; i++)
  {
    bool check_space = true;
    do
    {
      base_randomizer = random(min_space, max_space);
      if (buffer[7][base_randomizer] == 0)
      {
        buffer[7][base_randomizer] = 1;
        check_space = false;
        height_randomizer = random(min_height, max_height);
        new_height(base_randomizer, height_randomizer);
        break;
      }
    } while (check_space);
  }
}
// Para eliminar edificio
void erradicate_building(short next_pos_x)
{
  for (short i = 0; i < 4; i++)
  {
    buffer[7 - i][next_pos_x] = 0;
  }
}
// Para ponerle altura al edificio
void new_height(short next_pos_x, short height)
{
  for (short i = 0; i < height; i++)
  {
    buffer[7 - i][next_pos_x] = 1;
  }
}
// Para terminar una partida
bool check_game_status()
{
  for (int i = 0; i < 16; i++)
  {
    if (buffer[7][i] == 1)
    {
      return false;
    }
  }
  return true;
}

void PlayGame()
{
  if (contDestrucciones == 5)
  {
    vidas++;
    contDestrucciones = 0;
  }
  if (vidas == 0 || prueba2 > 16)
  {
    clearBuff();
    Serial.println("Perdiste! :(");
    prueba = 0;
    prueba2 = 1;
    xAvion = 0;
    xAvionIz = 15;
    yAvion = 0;
    TiempoD = 0;
    TiempoI = 0;
    vidas = vidasinit;
    Direccion = 0;
    estado = 0; // Regresa al mensaje inicial
    position = 146;
    agregarDato(puntos);
    //  TE REGRESA AL MENSAJE INICIAL
  }
  else
  {
    if (prueba == 0)
    {
      new_buildings(prueba2);
      prueba2++;
      prueba++;
    }
    else if (check_game_status())
    {
      prueba = 0;
      clearBuff();
      pintartmp(prueba2);
      for (int y=0;y<35;y++){
        mostrarMatrizTmp();
      }
      delay(1000);
      clearTmp();
    }
    // EMPIEZA LO MIO ---------------------------
    if (digitalRead(btnD))
    {
      RutaBala = true;
      DireccionBala = yAvion;
      PosicionActualBala = avion.getCuerpo();
      if (buffer[7][PosicionActualBala] == 1)
      {
        contDestrucciones++;
        puntos++;
        controlVidas();
        Serial.println();
        Serial.println("Dest:");
        Serial.println(contDestrucciones);
        Serial.println("Vidas:");
        Serial.println(vidas);
        Serial.println();
      }
      delay(200);
    }

    if (RutaBala)
    {
      int temp = DireccionBala + 1;
      if (DireccionBala < 8)
      {
        buffer[temp - 1][PosicionActualBala] = 0;
        if ((millis() - TiempoB) >= 30)
        {
          TiempoB = millis();
          DireccionBala++;
        }
        if (DireccionBala < 8)
        {
          buffer[temp][PosicionActualBala] = 1;
        }
      }
      else if (DireccionBala == 8)
      {
        DireccionBala = 0;
        RutaBala = false;
      }
    }

    if (digitalRead(btnIzq))
    {
      Direccion = 1;
      xAvionIz = xAvion;
      avion.Sentido_1(yAvion);
    }
    else if (digitalRead(btnDer))
    {
      Direccion = 0;
      xAvion = xAvionIz;
      avion.Sentido_2(yAvion);
    }

    if (Direccion == 1)
    {
      if (Colicion_Izquierda(avion.getCuerpo(), yAvion))
      {
        avion.borrarAvion_Izquierda(yAvion, AsPerdido + 1);
        vidas--;
        yAvion -= 2;
      }

      if (xAvionIz > -1)
      {
        avion.borrarAvion_Izquierda(yAvion, xAvionIz);
        if ((millis() - TiempoI) >= velocidad)
        {
          TiempoI = millis();
          xAvionIz--;
        }

        avion.pintarAvion_Izquierda(yAvion, xAvionIz);
        mostrarMatriz();
      }
      else if (xAvionIz == -1)
      {
        xAvionIz = 15;
        yAvion++;
        if (yAvion == 7)
        {
          yAvion = 0;
        }
      }
    }
    else if (Direccion == 0)
    {
      if (Colicion(avion.getCuerpo(), yAvion))
      {
        avion.borrarAvion(yAvion, AsPerdido - 1);
        vidas--;
        yAvion -= 2;
      }
      if (xAvion < 16)
      {
        avion.borrarAvion(yAvion, xAvion);
        if ((millis() - TiempoD) >= velocidad)
        {
          TiempoD = millis();
          xAvion++;
        }
        avion.pintarAvion(yAvion, xAvion);
        mostrarMatriz();
      }
      else if (xAvion == 16)
      {
        xAvion = 0;
        yAvion++;

        if (yAvion == 7)
        {

          yAvion = 0;
        }
      }
    }
  }
}

void controlVidas()
{
  if (contDestrucciones == 5)
  {
    vidas++;
    contDestrucciones = 0;
  }
  if (vidas == 0)
  {
    estado = 0; // Regresa al mensaje inicial
    position = 146;
    clearBuff();
  }
}