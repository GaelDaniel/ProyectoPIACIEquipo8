// ------------------------------------
// PINES DEL JUEGO DE CIRUJANO
// ------------------------------------
const int PIN_SONDA = 2;        // Entrada: Sonda (Malla a GND con PULLUP)
const int PIN_ZUMBADOR = 3;     // SALIDA: Zumbador PASIVO
const int PIN_LED_VERDE = 4;    // SALIDA: LED Verde (Indicador de Vidas)
const int PIN_BOTON_REINICIO = 5; // ENTRADA: Botón de Reinicio (a GND)
const int PIN_LED_ROJO = 13;    // SALIDA: LED Rojo (Alarma y Game Over)

// ------------------------------------
// LÓGICA DEL JUEGO
// ------------------------------------
int vidasRestantes = 5; 
bool alarmaActivada = false; 

// ------------------------------------
// VARIABLES DE TIEMPO (Para el Monitor Cardíaco sin bloqueo)
// ------------------------------------
unsigned long tiempoUltimoBip = 0;
// Intervalo de pausa entre bips (1500 ms = 1.5 segundos)
const long INTERVALO_PAUSA_MS = 1500; 
// Duración real que suena el bip (100 ms)
const int DURACION_BIP_MS = 100; 
// Bandera para saber si el bip debe estar sonando ahora
bool bipActivo = false; 

// ------------------------------------
// FUNCIONES DE SONIDO (Mantienen delay para su duración fija)
// ------------------------------------

void sonidoReinicio() {
  tone(PIN_ZUMBADOR, 440, 200); 
  delay(200); 
  noTone(PIN_ZUMBADOR);
}

void sonidoPerdidaVida() {
  tone(PIN_ZUMBADOR, 200, 200); 
  delay(250); 
  noTone(PIN_ZUMBADOR);
}

// Secuencia dramática de Game Over (Bip-Bip-Bip... Biiiiip)
void alarmaGameOver() {
  const int TONO_ALERTA = 880; 
  const int DURACION_ALARMA_FINAL = 3000; 
  const int PAUSAS = 3; 

  // La alarma de Game Over es crítica y usa delay para asegurar la secuencia
  for (int i = 0; i < PAUSAS; i++) {
    tone(PIN_ZUMBADOR, TONO_ALERTA, 100); 
    digitalWrite(PIN_LED_ROJO, HIGH);
    delay(100); 
    noTone(PIN_ZUMBADOR);
    digitalWrite(PIN_LED_ROJO, LOW);
    delay(100); 
  }
  
  delay(1000); 
  
  tone(PIN_ZUMBADOR, TONO_ALERTA); 
  digitalWrite(PIN_LED_ROJO, HIGH);
  delay(DURACION_ALARMA_FINAL);
  
  noTone(PIN_ZUMBADOR);
  digitalWrite(PIN_LED_ROJO, HIGH); 
}

// ------------------------------------
// FUNCIÓN ESENCIAL SIN BLOQUEO
// ------------------------------------
void monitorBip() {
  const int TONO_BIP = 400; // Tono del monitor
  unsigned long tiempoActual = millis();

  // 1. Lógica para INICIAR el Bip: ¿Ha pasado suficiente tiempo de PAUSA?
  // NOTA: Se comprueba el tiempo total entre bips (INTERVALO_PAUSA_MS + DURACION_BIP_MS)
  if (!bipActivo && (tiempoActual - tiempoUltimoBip >= (INTERVALO_PAUSA_MS + DURACION_BIP_MS))) {
    // Es hora de iniciar un nuevo bip
    tone(PIN_ZUMBADOR, TONO_BIP);
    bipActivo = true;
    tiempoUltimoBip = tiempoActual; // Reinicia el tiempo de la duración del sonido
  }

  // 2. Lógica para DETENER el Bip: ¿Ha sonado el tiempo suficiente (DURACION_BIP_MS)?
  if (bipActivo && (tiempoActual - tiempoUltimoBip >= DURACION_BIP_MS)) {
    noTone(PIN_ZUMBADOR);
    bipActivo = false;
    tiempoUltimoBip = tiempoActual; // Reinicia el tiempo para la PAUSA
  }
}

// ------------------------------------
// FUNCIÓN PARA REINICIAR EL JUEGO
// ------------------------------------
void reiniciarJuego() {
  vidasRestantes = 5;
  noTone(PIN_ZUMBADOR); 
  digitalWrite(PIN_LED_ROJO, LOW);
  digitalWrite(PIN_LED_VERDE, HIGH); 
  alarmaActivada = false;
  bipActivo = false; // Reiniciar estado del bip
  tiempoUltimoBip = millis(); // Reiniciar tiempo del monitor
  sonidoReinicio(); 
  Serial.println("--- JUEGO REINICIADO ---");
}

// ------------------------------------
// SETUP
// ------------------------------------
void setup() {
  pinMode(PIN_ZUMBADOR, OUTPUT);
  pinMode(PIN_LED_ROJO, OUTPUT);
  pinMode(PIN_LED_VERDE, OUTPUT); 
  pinMode(PIN_SONDA, INPUT_PULLUP); 
  pinMode(PIN_BOTON_REINICIO, INPUT_PULLUP); 
  
  reiniciarJuego(); 
  
  Serial.begin(9600);
  Serial.println("--- Juego de Cirujano FINAL (v6.4 Millis) Inicializado ---");
}

// ------------------------------------
// LOOP PRINCIPAL
// ------------------------------------
void loop() {
  int estadoBoton = digitalRead(PIN_BOTON_REINICIO);

  if (estadoBoton == LOW) {
    reiniciarJuego();
    delay(200); 
    return; 
  }

  int estadoContacto = digitalRead(PIN_SONDA);
  
  // Condición de error: Contacto con sonda Y no hay alarma activa Y quedan vidas
  if (estadoContacto == LOW && !alarmaActivada && vidasRestantes > 0) {
    
    // --- PÉRDIDA DE VIDA ---
    vidasRestantes--; 
    alarmaActivada = true;
    Serial.print("¡ERROR! Vidas restantes: ");
    Serial.println(vidasRestantes);
    
    // Detenemos el bip del monitor
    noTone(PIN_ZUMBADOR); 
    
    digitalWrite(PIN_LED_VERDE, LOW); 
    digitalWrite(PIN_LED_ROJO, HIGH); 
    
    sonidoPerdidaVida(); 
    
    // Desactivar Alarma y restaurar estados
    if (vidasRestantes > 0) {
      digitalWrite(PIN_LED_ROJO, LOW);
      digitalWrite(PIN_LED_VERDE, HIGH); 
    } else {
      // Game Over: Ejecutamos la secuencia dramática de alarma
      alarmaGameOver();
    }
    
    alarmaActivada = false;
  } 
  
  // -----------------------------------
  // 3. Mantiene el estado de Game Over y Controla el Bip
  // -----------------------------------
  if (vidasRestantes == 0) {
    digitalWrite(PIN_LED_ROJO, HIGH); 
    digitalWrite(PIN_LED_VERDE, LOW);
    noTone(PIN_ZUMBADOR); 
  } else if (!alarmaActivada) {
    // 4. SONIDO DEL MONITOR CARDÍACO SIN BLOQUEO
    monitorBip();
  }
}