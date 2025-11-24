# ProyectoPIACIEquipo8
Codigo del proyecto integrador del equipo 8 de la clase de computo integrado en la UANL FIME
Catedratico
M.C. Héctor Hugo Flores Moreno
MATRICULA	NOMBRE	CARRERA
1998752	Jesús Osvaldo Hernández Villegas	ITS
2004685	Lerick Asael Juárez Prado 	ITS
2006000	Jaime Azahel Contreras Domínguez 	ITS
2006191	Gael Aldair Torres Carreño 	ITS
2008890	Ricardo Gael Daniel Lucero 	ITS


¿Qué hace el proyecto?
Propósito: crear un juego interactivo y divertido que ponga aprueba las habilidades manuales y concentración del jugador 
contexto: este proyecto surge en una asignatura escolar donde se busca integrar componentes electrónicos y código para en conjunto obtener un juego interactivo que pondrá aprueba las destrezas del jugador o usuario
Alcance: el proyecto incluye el diseño, construcción y programación del juego “operando” haciendo uso de Arduino considerando la integración de componentes electrónicos básicos, Diseño del circuito en protoboard
Para que sirve: el propósito principal es el entretenimiento de alta tensión mediante la simulación de un procedimiento médico. En un contexto educativo, sirve como demostración funcional de la integración de hardware y firmware, practicando la programación sin bloques en sistemas embebidos
¿Cómo funciona internamente?
1. Control del Microcontrolador y alimentación
El sistema utiliza el Arduino UNO como su unidad central de procesamiento (CPU), ejecutando el firmware para tomar decisiones de juego. La protoboard actúa como la placa de distribución para organizar las conexiones de componentes.
•	Fuente de Energía: El juego es portátil gracias a la Pila de 9V, que suministra energía al pin VIN del Arduino. Un regulador interno se encarga de reducir esos 9V a 5V seguros, que son distribuidos a todo el circuito (leds, zumbador, Sonda).
2. detección de Entradas (Pines D2 y D5)
La lógica de detección de eventos se basa en el principio de caída de voltaje a Tierra (GND), gestionada por la configuración INPUT_PULLUP en el código.
•	detección de Error (Sonda): El pin D2 (conectado a la pinza) se mantiene en estado HIGH (5V) por la resistencia interna. El Aluminio está conectado a GND (0V). Al tocar el aluminio, se fuerza una conexión entre 5V y 0V, lo que hace que el Pin D2 caiga a LOW. El código interpreta este cambio de HIGH a LOW como la señal de Error y penalización
•	Reinicio (botón D5): El botón de reinicio funciona de la misma manera: al presionarse, conecta el Pin D5 a GND, activando la función reiniciarJuego(). El Arduino procesa estas señales de manera separada porque provienen de pines distintos, aunque el evento físico sea el mismo (contacto a GND).
3. lógica de Firmware y Tiempo
El Código gestiona el juego de forma eficiente, asegurando que la detección de errores siempre funcione (programación sin bloqueo).
•	programación sin Bloqueo (millis()): La función monitorBip() utiliza la librería millis() para temporizar los sonidos ambientales (bips del monitor cardiaco) y controlar su intervalo (1.5 segundos) sin usar el comando delay(). Esto es crucial porque permite que el Arduino lea continuamente la sonda en cada ciclo de loop() mientras el bip está sonando, evitando que el juego se congele.
•	Control de Estado: La variable vidasRestantes es el centro de la lógica. La bandera alarmaActivada impone un tiempo de "castigo" (cooldown) después de cada error, evitando que la pérdida de vidas sea continua si la pinza permanece colocada.
4. Flujo de Juego y Secuencias de Sonido
El Código utiliza el Zumbador Pasivo (Pin D3) junto con la función tone() para generar feedback auditivo:
•	Perdida de Vida: Al detectar contacto, se llama a sonidoPerdidaVida() (tono corto de 200 Hz), se resta una vida, y el LED Verde se apaga temporalmente.
•	Game Over (Vidas = 0): Se ejecuta la función alarmaGameOver(), que reproduce la secuencia dramática de tres bips rápidos seguida del tono largo y continuo de 3 segundos (simulando asistolia), dejando el LED Rojo encendido de forma fija.
Diagrama de Conexiones Físicas
 
¿Como se usa o contribuye alguien más?
Instalar dependencias
•	Para que el juego pueda funcionar, se debe tener descargado el entorno de desarrollo de Arduino (Arduino IDE) ya que es lo que utilizaremos para escribir, verificar y cargar el código en la placa de Arduino
•	Solo utilizaremos funciones nativas del propio Arduino, así que no se requiere instalar librerías adicionales
Ejecutar el entorno local
•	se debe montar el hardware, es decir, armar el circuito del juego en la protoboard siguiendo el diagrama de conexiones físicas
•	se debe configurar el entorno de Arduino IDE seleccionando el modelo de Arduino y puerto que utilizaremos
•	por último, se coloca el código, se verifica y se carga a nuestro Arduino
Como correr tests
Para verificar que todos los módulos funcionen correctamente se debe corroborar los siguiente:
•	Prueba de encendido: se debe de desconectar el Arduino de la computadora y probar el botón de encendido. Lo primero que debe suceder es que el led verde se debe de encender y el zumbador debe emitir el bip simulando un monitor cardiaco
•	Prueba de la sonda: al tocar el aluminio con las pinzas, el sistema debe detener los bips del monitor cardiaco y reproducir un sonido diferente al del monitor cardiaco, al mismo tiempo el led rojo se encenderá y el led verde se apagará.
•	 Prueba de Game Over: después de perder todas las vidas (5), el juego debe ejecutar una secuencia dramática de alarma de asistolia (bip-bip-bip rápido-silencio-bip largo) y el led rojo debe de quedarse encendido fijo.
•	Prueba de reinicio: al perder, se presiona el botón de reinicio y el juego comenzará de nuevo, restableciendo las vidas (5), el led rojo se apagará, el led verde se encenderá y el monitor cardiaco debe reanudar los bips
Como contribuir (si es open source):
Para modificar los módulos del juego, ya sea agregando o quitando elementos, se debe de modificar el archivo principal del código
