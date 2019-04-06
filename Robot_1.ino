/*
 Name:    Robot_1.ino
 Created: Сб 30.03.19 20:05:54
 Author:  Korbut Ivan
*/

// the setup function runs once when you press reset or power the board
#include <LiquidCrystal_I2C.h>
#include <HCSR04.h>
#include <Wire.h>
#define TrigPin 2
#define EchoPinF 10
#define EchoPinL 5
#define EchoPinR 6
#define EchoPinUP 8


HCSR04 hc_F(TrigPin, EchoPinF);
HCSR04 hc_R(TrigPin, EchoPinR);
HCSR04 hc_L(TrigPin, EchoPinL);

LiquidCrystal_I2C lcd(0x3F, 16, 2); // Указываем I2C адрес (наиболее распространенное значение), а также параметры экрана (в случае LCD 1602 - 2 строки по 16 символов в каждой 
//UltrPar ug_forward, ug_left, ug_right, ug_upf;
 struct UltrPar
{
	float u_forward;
	float u_left;
	float u_right;
	boolean u_upf; // Задний датчик
} ;
float g_Forvard ;
float g_Left;
float g_Riht;
bool g_FirstValue;

struct  UltrPar GetParametrUltrasonc() //Получение данных с Ultrasonic
{
	//UltrPar *up_g;
	UltrPar P;
	P.u_forward = hc_F.dist();
	delay(50);
	P.u_left = hc_L.dist();
	delay(50);
	P.u_right = hc_R.dist();
	P.u_upf = digitalRead(EchoPinUP);
	String StrDelim = " ";
	//lcd.setCursor(0, 0);              // Установка курсора в начало первой строки
	//lcd.print(P.u_forward + StrDelim + P.u_left + StrDelim + P.u_right);
	//Serial.println("Forward " + char(P.u_forward));
	//Serial.println("Right " + char(P.u_right));
	//Serial.println("Left " + char(P.u_left));
	return P;
}
struct UltrPar g;

void check_result() {
	Serial.println();
	Serial.print("_float_: ");
	Serial.print(g.u_forward);
	Serial.print(";  _float_: ");
	Serial.print(g.u_left);
	Serial.print(";  _float_: ");
	Serial.print(g.u_right);
	Serial.println();
}
void setup() {
	Serial.begin(9600);
	lcd.init();                      // Инициализация дисплея  
	lcd.backlight();                 // Подключение подсветки 
	pinMode(EchoPinUP, INPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {
	UltrPar* up_g;
	g = GetParametrUltrasonc();
	IF (g_FirstValue == False){
		g_Forvard = g.u_forward;

	}
	check_result();
	String StrDelim = " ";

	lcd.setCursor(0, 0);              // Установка курсора в начало первой строки

	lcd.print(g.u_forward + StrDelim + g.u_left + StrDelim + g.u_right);

}


