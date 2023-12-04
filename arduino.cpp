#include <LiquidCrystal.h>

char respuestasCorrectas[] = {'A', 'B'};
int longitud = sizeof(respuestasCorrectas) / sizeof(respuestasCorrectas[0]);
int i = 0;
float resultado = 0;


// Inicializar el objeto lcd con los pines correspondientes
LiquidCrystal lcd_1(10, 9, 7, 6, 5, 4);

// Definir pines para la bombilla, botón, motores, LEDs y fotoresistencias
const int bombillaPin = 8;
const int BOTON_PIN = 2;
const int motorcito = 3;
const int ledR = 13;
const int ledG = 11;
const int ledB = 12;

int estadoBoton = HIGH;  // Estado inicial del botón (no presionado)
int ultimoEstadoBoton = HIGH;

const int ftPines[] = {0, 1, 2, 3}; // Pines de las fotoresistencias

int valoresFotoresistencias[4] = {0};

bool estadoEncendido1 = false;

// Función para controlar los LEDs RGB
void controlarFocoRGB(int valorR, int valorG, int valorB) {
  analogWrite(ledR, valorR);
  analogWrite(ledG, valorG);
  analogWrite(ledB, valorB);
}

// Funciones para encender y apagar la bombilla
void encenderBombilla() {
  digitalWrite(bombillaPin, HIGH);
}

void apagarBombilla() {
  digitalWrite(bombillaPin, LOW);
}


int verificarRespuesta(char letraEnviada) {
    apagarBombilla();
    analogWrite(motorcito, 1);
    delay(5000);
    analogWrite(motorcito, 0);
  
  // Lectura de valores de las fotoresistencias
  for (int i = 0; i < 4; i++) {
    valoresFotoresistencias[i] = analogRead(ftPines[i]);
  }

  // Encontrar la fotoresistencia con menos luz
  int indiceMenosLuz = 0;
  int valorMenosLuz = valoresFotoresistencias[0];

  for (int i = 1; i < 4; i++) {
    if (valoresFotoresistencias[i] < valorMenosLuz) {
      valorMenosLuz = valoresFotoresistencias[i];
      indiceMenosLuz = i;
    }
  }
  
  encenderBombilla();
  delay(500);

  // Verificar si todas las fotorresistencias tienen el mismo valor
  bool todasIguales = true;
    for (int i = 1; i < 4; i++) {
        if (valoresFotoresistencias[i] != valoresFotoresistencias[0]) {
            todasIguales = false;
            break;  // No es necesario seguir verificando si encontramos una diferencia
    }
  }

    int count = 0;
    for (int i = 0; i < 4; i++) {
      if (valoresFotoresistencias[i] < 381) {
        for (int j = i + 1; j < 4; j++) {
          if (valoresFotoresistencias[i] == valoresFotoresistencias[j]) {
            count++;
            break;
          }
        }
      }
    }
    
  if (todasIguales || count > 0) {
    // Todas las fotorresistencias tienen el mismo valor
   // La respuesta no está rellenada
      lcd_1.clear();
      lcd_1.print("Respuesta no rellenada");
      delay(8000);
      return 0;

  } else {
    // Las fotorresistencias no tienen el mismo valor
    // Verificar si la respuesta está rellenada (valor menor a 381)
    if (valorMenosLuz < 381) {
      // La respuesta seleccionada es la opción correspondiente a la fotoresistencia con menos luz
      char respuestaSeleccionada = 'A' + indiceMenosLuz;

      // Mostrar la respuesta seleccionada en la pantalla LCD o realizar otras acciones según sea necesario
      lcd_1.clear();
      lcd_1.print("Respuesta: ");
      lcd_1.print(respuestaSeleccionada);
      delay(8000);
      if (letraEnviada == respuestaSeleccionada) {
        return 1; // Devolver 1 si la letra enviada es igual a la respuesta seleccionada
      } else {
        return 0; // Devolver 0 si la letra enviada es diferente a la respuesta seleccionada
      }    
    } else {
      // La respuesta no está rellenada
      lcd_1.clear();
      lcd_1.print("Respuesta no rellenada");
      delay(8000);
      return 0;
    }
  }

}


void setup() {
  // Configurar pines como entrada o salida según corresponda
  pinMode(BOTON_PIN, INPUT);
  pinMode(motorcito, OUTPUT);
  pinMode(bombillaPin, OUTPUT);
  
  // Inicializar la pantalla LCD
  lcd_1.begin(16, 2);
  lcd_1.print("");

  // Configurar pines de las fotoresistencias como entrada
  for (int i = 0; i < 4; i++) {
    pinMode(ftPines[i], INPUT);
  }

  // Configurar pines de los LEDs como salida
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
}

void loop() {
  
  encenderBombilla();
  delay(500);
  
    lcd_1.clear();
    lcd_1.print("Pregunta #");
    lcd_1.print(i+1);
    delay(5000);
  
  
  // Espera hasta que el botón sea presionado
  while (digitalRead(BOTON_PIN) == HIGH) {   
    // Código que se ejecutará mientras el botón esté presionado
    lcd_1.clear();
    lcd_1.print("Encendido");
    controlarFocoRGB(224, 112, 0);
    delay(500); 
    
    

  
    resultado = verificarRespuesta(respuestasCorrectas[i]) + resultado;

    i++;
  }

  if (!(i < longitud)) {
    lcd_1.clear();
    lcd_1.print("Resultado : ");
    lcd_1.print(resultado * (10/(longitud)));
    controlarFocoRGB(255, 0, 127);
	delay(10000);

  } else{

    if (i > 0) {
      lcd_1.clear();
      lcd_1.print("Continuar?");
      delay(500);
      controlarFocoRGB(0, 0, 224);
  } else {
      lcd_1.clear();
      lcd_1.print("Apagado?");
      delay(500);
      controlarFocoRGB(0, 0, 224);
  }


  }



}

