// C++ code
//
#include <LiquidCrystal.h>
#include <IRremote.h>
#include <Servo.h>


#define led 8
#define led_servo_mov 13
#define sensorIR 9

#define Tecla_1 0xEF10BF00

int temperatura = 0;
int pos = 0;
int rojo = 0;
int verde = 0;
int azul = 0;

LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);
Servo servo_6;

void setup()
{
  	lcd_1.begin(16, 2);
  	lcd_1.print("Primavera");
    lcd_1.setCursor(0, 1);
  	lcd_1.print("temp");
	pinMode(led, OUTPUT);

    Serial.begin(9600);
    IrReceiver.begin(sensorIR, DISABLE_LED_FEEDBACK);
	servo_6.attach(6, 500, 2500);
  	servo_6.write(pos);
}

void loop(){
    temperatura = analogRead(A0);
    temperatura = map(temperatura,20,358,-40,125);  
  	if(temperatura < 43 && digitalRead(led) == LOW){
    	determinar_estacion();
	}
  	control_hacer();
  	incendio();
 	
  	delay(1000);

}
void determinar_estacion(){
  	lcd_1.clear();
    if  (temperatura < 7){
        lcd_1.print("Invierno");
    }else if (temperatura < 15){
      	lcd_1.print("Otonio");
    }else if (temperatura < 26){
       	lcd_1.print("Primavera");
    }else if (temperatura < 42){
        lcd_1.print("Verano");
    }else incendio();
  	lcd_1.setCursor(0, 1);
  	lcd_1.print("temp  ");
    lcd_1.print(temperatura);
    
	




}

void control_hacer(){
	if (IrReceiver.decode()){
      	if (IrReceiver.decodedIRData.decodedRawData == Tecla_1 && digitalRead(led) == HIGH){
			digitalWrite(led, LOW);
      	}else if (IrReceiver.decodedIRData.decodedRawData == Tecla_1){
			digitalWrite(led, HIGH);
      	}
		IrReceiver.resume();
	}
}

void incendio(){
    if( (temperatura <= 42 && digitalRead(led) == LOW) && pos > 0 ){
        mensaje_cerrando();
        cerrar_valvula();
    }else if(( temperatura >= 42 || digitalRead(led) == HIGH) && pos == 0 ){
        mensaje_abriendo();
        abrir_valvula();
    }
}




void abrir_valvula(){
  digitalWrite(led_servo_mov, HIGH);
    for (pos = 0; pos <= 180; pos += 1) {
        servo_6.write(pos);
        delay(5);
    }
  digitalWrite(led_servo_mov, HIGH);
}
void mensaje_abriendo() {
  lcd_1.clear();
  lcd_1.print("abriendo");
  lcd_1.setCursor(0, 1);
  lcd_1.print("valvula");

}
void mensaje_incendio(){
  lcd_1.clear();
  lcd_1.print("sistema");
  lcd_1.setCursor(0, 1);
  lcd_1.print("activado");
  
}

void cerrar_valvula(){
  	digitalWrite(led_servo_mov, HIGH);
    for (pos = 180; pos >= 0; pos -= 1) {
      servo_6.write(pos);
      delay(5);
    }
    digitalWrite(led_servo_mov, HIGH);
}
void mensaje_cerrando(){
  lcd_1.clear();
  lcd_1.print("cerando");
  lcd_1.setCursor(0, 1);
  lcd_1.print("valvula");
  
}