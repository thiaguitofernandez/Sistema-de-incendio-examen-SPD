# Sistema-de-incendio-examen-SPD# SPD-Examen-Montacargas

## Examen: Montacargas
![Tinkercad](./imagenes/arduino.png)
![Diagrama](./imagenes/arduino_plano_esquematico.jpg)

## objetivo del proyecto
El objetivo de este proyecto es realizar el "menu" interactivo de un montaargas el cual debe subir pisos y bajarlos uno a la vez.
## Funcionamiento.
para su funcionamiento se divide en diferentes partes las cuales son las siguientes:
* Configuracion y definicion de variables.
* Loop y funciones pincipales.
* Entrada de datos.
* Funciones para apagado y prendido de leds.

### Configuracion inicial y definicion de variables.
Aqui se establecen los puertos utilizados asignadoles una denominacion, se inicializan las variables y se establece la configuacion inicial indicando la modalidad de cada puerto como el estado de dichos puertos
* Establecimiento de nombre de los puertos.

Los puertos establecidos fueron de los puertos digitales 13 al puerto 5 y tambien se convitieron los puertos analogicos A0 a A2 en puertos digitales yendo de 14 a 16 en el mismo orden de antes.
Y estos fueron para los siguientes 
~~~~
#define BOTON_PARAR 16
#define BOTON_SUBIR 15
#define BOTON_BAJAR 14
#define	LUZ_MOVIENDO 13
#define	LUZ_PARADO 12
#define	ARRIBA_DERECHA 11
#define	ARRIBA 10
#define	ARRIBA_IZQUIERDA 9
#define	CENTRO 8
#define	ABAJO_IZQUIERDA 7
#define	ABAJO 6
#define	ABAJO_DERECHA 5
~~~~
* Inicializacion de variables.

Las variables son 4:
2 siendo banderas(**bandera_emergencia** y **bandera_emegencia_moviendo**) que se utilizan para habilitar y desabilitar el codigo por lo que poseen valores de HIGH y LOW.
Y las otras siendo piso una variable de tipo **int** que tiene valores de 0 a 9 y contador una variable de tipo **long** para tener el rango utilizable de la funcion millis.
~~~~
int bandera_emergencia = LOW;
int bandera_emergencia_moviendo = LOW;
int piso = 0;
long contador = 0;
~~~~
* Configuracion Inicial.

Aqui se confugura los puertos para indicar su funcion, ya sea de salidad como de entrada, se inicia el monitor serial con una velocidad de 9600 y se configura el estado inicial que los puerto deberian de tener.
~~~~
void setup()
{
  pinMode(BOTON_PARAR, INPUT);
  pinMode(BOTON_SUBIR, INPUT);
  pinMode(BOTON_BAJAR, INPUT);
  pinMode(LUZ_MOVIENDO, OUTPUT);
  pinMode(LUZ_PARADO, OUTPUT);
  pinMode(ABAJO_DERECHA, OUTPUT);
  pinMode(ABAJO, OUTPUT);
  pinMode(ABAJO_IZQUIERDA, OUTPUT);
  pinMode(CENTRO, OUTPUT);
  pinMode(ARRIBA_IZQUIERDA, OUTPUT);
  pinMode(ARRIBA, OUTPUT);
  pinMode(ARRIBA_DERECHA, OUTPUT);
  prende_numero_cero();
  prende_led(LUZ_PARADO);
  Serial.begin(9600);
  Serial.println("Usted se encuentra en el piso: 0");
}
~~~~

### Loop y funciones pincipales.
Dentro del loop se encuenta el codigo que se ejecuta continuamente y luego las funciones pincipales las cuales son las mas importantes.

* **Loop**
~~~~
void loop()
{
    bandera_emergencia = emergencia(bandera_emergencia);

    if (bandera_emergencia == LOW){
        montacargas();
    }
}
~~~~
El loop consiste de la asignacion de la varaible **bandera_emergencia** atravez de la funcion emergencia la varaible consigue un valo de HIGH o LOW.
Luego un if evalua esa misma varaible para ver si esta en LOW para poder acceder a la funcion montacargas.

#### funciones principales.

Dentro de las funciones pincipales se encuentran 2 funciones que llevan parte del parado de emergencia del montacargas, luego se encuentra la funcion **montacargas** la cual se encarga de gestionar las funciones necesarias para que este suba y baje reportando el piso en el que esta.

* Funciones para el paro de emergencia.
Las funciones para el paro de emergencia son:
* **emergencia**
* **movimiento**
------

* **emergencia:**

**emergencia** es una funcion la cual recibe por parametro una bandera y la devuelve con un valor de HIGH o LOW.
Para esto lee el puerto asociado a **BOTON_PARAR** para determinar si se esta recibiendo una señal.Luego en conjunto con el valor que la bandera pueda tener determina si debe asignarle HIGH o LOW, siempre para cambiar el valor del mismo y no asignarle un valor que ya tiene.
Otra funcion que emergencia posee es la de invalitar la el resto de botones ya que mientras la bandera que este retorne sea HIGH no se ejecutara mas codigo que esta funcion.

------

* **movimiento:**

**movimiento** principalmente se encarga de determinar el tiempo durante el cual el ascensor se encuentra en movimiento.Para esto utiliza la funcion millis(la cual se encuentra explicada en la parte inferior de este documento) y un contador inicializado como long para poder guardar el valor que **millis** devuelve con un adicional de 3000 milisegundos (el tiempo que el montacargas se mueve de piso en piso).
Lo que permite a la funcion determinar cuanto timepo pasa es un while el cual compara a **contador** (quien tiene el valor del tiempo a la hora de apretar el boton de inicio del movimiento mas 3 segundos) a la funcion **millis** y cuando **millis** devuelva un valor mayor o igual al de **contador** este terminara la funcion.
Ahora dentro de este while se encuentra una referencia a la funcion emergencia la cual si el **BOTON_PARAR** es apretado guardaria el tiempo restante del contador para que este llegue a 3 segundos y si el mismo boton es presionado otra vez el valor que **millis** deberia alcanzar para que el montacargas llegue a su destino sera guardado en contador.
~~~~
int emergencia(int bandera){
    if( (digitalRead(BOTON_PARAR) == HIGH) && (bandera == HIGH)){
        bandera = LOW;
      	contador = contador + millis();
      	Serial.println("Parada de emergencia desactivada");
      	delay(150);
    }
  	else if (digitalRead(BOTON_PARAR) == HIGH){
        bandera = HIGH ;
      	contador = contador - millis();
      	Serial.println("Parada de emergencia activada, por favor contacte con mantenimiento");
    	delay(150);
    }
  	
  	return bandera;
}

void movimiento(){
   contador = (millis() + 3000);
   while(contador >= millis()){
     bandera_emergencia_moviendo = emergencia(bandera_emergencia_moviendo);
     while (bandera_emergencia_moviendo == HIGH){
        bandera_emergencia_moviendo = emergencia(bandera_emergencia_moviendo);
     	delay(100);
      }
   }
~~~~ 
* Funcion para mover el montacargas:

**montacargas** es la funcion designada para deteminar el orden de las funciones logicas usadas asi logrando que estas funcionen sin problemas como tambien informa dentro del monitor serial el piso en el que se encuentra el montacargas a la hora de que este finalize el recorrido y solo lo hara cuando alguno de los botones designados para subir o bajar de pisos envie una señal de HIGH asi logrando que se repita el contenido de la funcion todo el timepo que el montacargas se encuentre inactivo.
~~~~
void montacargas(){
  if(digitalRead(BOTON_BAJAR) == HIGH|| digitalRead(BOTON_SUBIR) == HIGH){
      	control_movimiento();
    	Serial.print("Usted se encuentra en el piso: ");
    	Serial.println(piso);
     	instrucciones_segun_numero();
  }
}
~~~~

### Funciones logicas y procesamiento de datos.

Estas son las funciones que se encargan de determinar las condiciones que deben requeririse para que ciertas cosas pasen (el cambio de valor de la variable contado o la iluminacion de los leds segun el piso).

------

* instrucciones_segun_numero:

**instrucciones_segun_numero** se encarga de segun el valor de la variable piso indicar que se debera hacer con el display 7 segmentos para indicar el piso el piso en el que el montacargas se encuentra actualmente. Para mostrar el numero deseado hace referencia a las funciones **prender_numero_......** y **apagar_numero_.....** para apagar las partes del segmento que pertenece a los numeros anteriores que pudieron estar pendidos y luego prender las partes del segmento que representa el valor de signo.

~~~~          
void instrucciones_segun_numero(){
    switch (piso)
    {
      case (0):
        apaga_numero_uno();
        prende_numero_cero();
        break;
      case (1):
        apaga_numero_cero();
        apaga_numero_dos();
        prende_numero_uno();
        break;
      case (2):
        apaga_numero_uno();
        apaga_numero_tres();
        prende_numero_dos();
        break;
      case (3):
        apaga_numero_dos();
        apaga_numero_cuatro();
        prende_numero_tres();
        break;    
      case (4):
        apaga_numero_tres();
        apaga_numero_cinco();
        prende_numero_cuatro();
        break;
      case (5):
        apaga_numero_cuatro();
        apaga_numero_seis();
        prende_numero_cinco();
        break;
      case (6):
        apaga_numero_cinco();
        apaga_numero_siete();
        prende_numero_seis();
        break;
      case (7):
        apaga_numero_seis();
        apaga_numero_ocho();
        prende_numero_siete();
        break;
      case (8):
        apaga_numero_siete();
        apaga_numero_nueve();
        prende_numero_ocho();
        break;
      case (9):
        apaga_numero_ocho();
        prende_numero_nueve();
        break;
    }
}
~~~~ 
* **control_movimiento**

control_movimiento se encarga de apagar la luz referente al montacargas estando pausado y prender la luz referente a la luz en movimiento y luego llamar a la funcion **subir_piso** o **bajar_piso** para agregar o restar 1 a piso para luego llamar a la funcion movimiento para determinar cuando 3 segundos halla pasado  para luego apagar la luz referente a movimiento y prender la luz referente a el montacargas estando pausado.
~~~~ 
void control_movimiento(){
  apaga_led(LUZ_PARADO);
  prende_led(LUZ_MOVIENDO);
  subir_piso();
  bajar_piso();
  movimiento();
  apaga_led(LUZ_MOVIENDO);
  prende_led(LUZ_PARADO);
}
~~~~ 

### Entrada de datos.

Las funciones clasificadas como entrada de datos son:
* **subir_piso**
* **bajar_piso**

Estas se encargan de modificar el valor de piso sumandole o restandole 1 segun que boton es presionado y que su valor no sea 9 o 0 los cuales son los valores limite que esta variable puiede tener.
* **subir_piso**

**subir_piso** se encarga de sumarle 1 a la variable piso si la lectura del BOTON_SUBIR es HIGH y el valor de piso no es 9.
~~~~ 

void subir_piso(){
    if ( (digitalRead(BOTON_SUBIR) == HIGH) && (piso != 9)){
      	Serial.println("Subiendo..");
        piso++;
    }
}
~~~~
* **bajar_piso**

**bajar_piso** se encarga de restarle 1 a la variable piso si la lectura del BOTON_BAJAR es HIGH y el valorr de piso no es 0.
~~~~
void bajar_piso(){
    if ( (digitalRead(BOTON_BAJAR) == HIGH) && (piso != 0)){
      	Serial.println("Bajando..");
        piso--;
    }
}
~~~~ 
### Funciones para apagado y prendido de leds
Estas funciones son las encargadas de cambiar el estado de los puertos asignados a leds dividiendose en dos caterias:
* Las funciones para evitar la repeticion de DigitalWrite.
* Las funciones para determinar la combinacion de un numero en el display.

Ambas de estas categorias tienen variantes para prender y apagar los leds.

* Las funciones para evitar la repeticion de DigitalWrite:

Estas son una funcion que recibe por parametro un puerto y mediante digitalWrite cambia el puerto de HIGH a LOW o de LOW a HIGH.
~~~~ 
void apaga_led(int led){
  digitalWrite(led, LOW);
}
void prende_led(int led){  	
	digitalWrite(led, HIGH);
}
~~~~
* Las funciones para determinar la combinacion de un numero en el display:

Estas funciones utilizan las funciones anteriores para en el display 7 segmentos mostrar el numero que tienen en el nombre o para deja de mostrarlo.
~~~~
void prende_numero_cero(){
  prende_led(ARRIBA);
  prende_led(ARRIBA_DERECHA);
  prende_led(ARRIBA_IZQUIERDA);
  prende_led(ABAJO_DERECHA);
  prende_led(ABAJO_IZQUIERDA);
  prende_led(ABAJO);
}
void apaga_numero_cero(){
  apaga_led(ARRIBA);
  apaga_led(ARRIBA_DERECHA);
  apaga_led(ARRIBA_IZQUIERDA);
  apaga_led(ABAJO_DERECHA);
  apaga_led(ABAJO_IZQUIERDA);
  apaga_led(ABAJO);
}
  
void prende_numero_uno(){
  prende_led(ARRIBA_DERECHA);
  prende_led(ABAJO_DERECHA);
}
  void apaga_numero_uno(){
  apaga_led(ARRIBA_DERECHA);
  apaga_led(ABAJO_DERECHA);
}

void prende_numero_dos(){
  prende_led(ARRIBA);
  prende_led(ARRIBA_DERECHA);
  prende_led(CENTRO);
  prende_led(ABAJO_IZQUIERDA);
  prende_led(ABAJO);
}
void apaga_numero_dos(){
  apaga_led(ARRIBA);
  apaga_led(ARRIBA_DERECHA);
  apaga_led(CENTRO);
  apaga_led(ABAJO_IZQUIERDA);
  apaga_led(ABAJO); 
}

void prende_numero_tres(){
  prende_led(ARRIBA);
  prende_led(ARRIBA_DERECHA);
  prende_led(CENTRO);
  prende_led(ABAJO_DERECHA);
  prende_led(ABAJO);
}
void apaga_numero_tres(){
  apaga_led(ARRIBA);
  apaga_led(ARRIBA_DERECHA);
  apaga_led(CENTRO);
  apaga_led(ABAJO_DERECHA);
  apaga_led(ABAJO);  
}

void prende_numero_cuatro(){
  prende_led(ARRIBA_IZQUIERDA);
  prende_led(ARRIBA_DERECHA);
  prende_led(CENTRO);
  prende_led(ABAJO_DERECHA);
}
void apaga_numero_cuatro(){
  apaga_led(ARRIBA_IZQUIERDA);
  apaga_led(ARRIBA_DERECHA);
  apaga_led(CENTRO);
  apaga_led(ABAJO_DERECHA);  
}

void prende_numero_cinco(){
  prende_led(ARRIBA);
  prende_led(ARRIBA_IZQUIERDA);
  prende_led(CENTRO);
  prende_led(ABAJO_DERECHA);
  prende_led(ABAJO); 
}
void apaga_numero_cinco(){
  apaga_led(ARRIBA);
  apaga_led(ARRIBA_IZQUIERDA);
  apaga_led(CENTRO);
  apaga_led(ABAJO_DERECHA);
  apaga_led(ABAJO);   
}

void prende_numero_seis(){
  prende_led(ARRIBA);
  prende_led(ARRIBA_IZQUIERDA);
  prende_led(CENTRO);
  prende_led(ABAJO_DERECHA);
  prende_led(ABAJO_IZQUIERDA);
  prende_led(ABAJO);
}
void apaga_numero_seis(){
  apaga_led(ARRIBA);
  apaga_led(ARRIBA_IZQUIERDA);
  apaga_led(CENTRO);
  apaga_led(ABAJO_DERECHA);
  apaga_led(ABAJO_IZQUIERDA);
  apaga_led(ABAJO);
}

void prende_numero_siete(){
  prende_led(ARRIBA);
  prende_led(ARRIBA_DERECHA);
  prende_led(ABAJO_DERECHA);
}
void apaga_numero_siete(){
  apaga_led(ARRIBA);
  apaga_led(ARRIBA_DERECHA);
  apaga_led(ABAJO_DERECHA);
}

void prende_numero_ocho(){
  prende_led(ARRIBA);
  prende_led(ARRIBA_DERECHA);
  prende_led(ARRIBA_IZQUIERDA);
  prende_led(CENTRO);
  prende_led(ABAJO_DERECHA);
  prende_led(ABAJO_IZQUIERDA);
  prende_led(ABAJO);
}
void apaga_numero_ocho(){
  apaga_led(ARRIBA);
  apaga_led(ARRIBA_DERECHA);
  apaga_led(ARRIBA_IZQUIERDA);
  apaga_led(CENTRO);
  apaga_led(ABAJO_DERECHA);
  apaga_led(ABAJO_IZQUIERDA);
  apaga_led(ABAJO);
}

void prende_numero_nueve(){
  prende_led(ARRIBA);
  prende_led(ARRIBA_DERECHA);
  prende_led(ARRIBA_IZQUIERDA);
  prende_led(CENTRO);
  prende_led(ABAJO_DERECHA);
}
void apaga_numero_nueve(){
  apaga_led(ARRIBA);
  apaga_led(ARRIBA_DERECHA);
  apaga_led(ARRIBA_IZQUIERDA);
  apaga_led(CENTRO);
  apaga_led(ABAJO_DERECHA);
}
~~~~
### millis.
**millis** es una funcion predefinida dentro de arduino la cual retorna el tiempo desde que el pocesador de arduino se activo de forma exacta(mediante milisegunods) y con formato **long** por lo que tambien deberia ser de este formato la variable en la que se guarde (si es que se guarda)  ya que si se guarda en **int** a los 32 segundos de tiempo activo el numero seria mayor que el mayor numero que este tipo de variable puede contener y se desbordaria pasando al menor numeor que este puede guardar.
## Link al proyecto.
- [proyecto](https://www.tinkercad.com/things/b1FarQ0Edki)
