#include<LiquidCrystal.h>
#include<IRremote.h>
LiquidCrystal lcd (12,11,5,4,3,2);
int r1=10000;
int r2=100000;
int recpin = 9;
IRrecv irrecv(recpin);
decode_results results;
#define code1 16582903	//button1 A
#define code2 16615543	//button2 V
#define code3 16599223	//button3 R
#define code4 16591063  //button4 C
#define code5 16623703  //button5 L
#define code6 16607383  //button6 

float voltage_value0;
float voltage_value1;
int Vol_val;
float temp;
float voltage;
int subraction_value;
float temp_val;
float current_value;
int Vin = 5;
int R1=1000;
int R2;
int res;
float buffer;
float Vout;

          
int chargePin = 13;         
int dischargePin = 10;        
#define resistorValue 10000.0F   

unsigned long startTime;
unsigned long elapsedTime;
float microFarads;                
float nanoFarads;


float capacitance;
float inductance;
int pulse;
float frequency;

void setup() 
{
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  irrecv.enableIRIn(); 
  pinMode(chargePin, OUTPUT);     
  digitalWrite(chargePin, LOW); 
  pinMode(8, INPUT);
  pinMode(7, OUTPUT);
  lcd.print("   MULTIMETER");
  lcd.setCursor(0,1);
  lcd.print(" 1A 2V 3R 4C 5L");
}

void loop() 
{
  
  if(irrecv.decode(&results))
    {
      unsigned int value=results.value;
      switch(value)
      {
        case code1:
          voltage_value0 = analogRead(A0);
  		  voltage_value1 = analogRead(A1);
          subraction_value =(voltage_value1 - voltage_value0);
          temp_val = (subraction_value*0.00488);
          current_value = (temp_val*10);
          Serial.println(current_value);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Current: ");
          lcd.setCursor(0,1);
          lcd.print (current_value);
          lcd.setCursor(14,1);
          lcd.print("mA");
        break;

        case code2:
          Vol_val = analogRead(A2);
          temp = (Vol_val * 5.0) / 1024.0; 
          voltage = temp*1.1;   //(r1+r2)/r2 = 1.1 
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Voltage: ");
          lcd.setCursor(0,1);               
          lcd.print(voltage);
          lcd.setCursor(15,1);
          lcd.print("V");
        break;

        case code3:
          res = analogRead(A3);
          buffer = res * Vin;
          Vout = (buffer)/1024.0;
          buffer = (Vin/Vout) - 1;
          R2 = (int)(R1*buffer);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Resistance: ");
          lcd.setCursor(0, 1);               
          lcd.print(R2);
          lcd.setCursor(13,1);
          lcd.print("ohm");

        break;

        case code4:
        lcd.clear();
        lcd.print("Capacitance:");
        digitalWrite(chargePin, HIGH);  
        startTime = millis();
        while(analogRead(A4) < 648){       
        }

        elapsedTime= millis() - startTime;
        microFarads = ((float)elapsedTime / resistorValue) * 1000;         
        if (microFarads > 1){
          lcd.setCursor(0,1);
          lcd.print((long)microFarads);
          lcd.setCursor(14,1);
          lcd.print("uF");  
        }

        else{
          nanoFarads = microFarads * 1000.0;
          lcd.setCursor(0,1);
          lcd.print((long)nanoFarads);
          lcd.setCursor(14,1);
          lcd.print("nF");    
          delay(50); 
        }

        digitalWrite(chargePin, LOW);            
        pinMode(dischargePin, OUTPUT);            
        digitalWrite(dischargePin, LOW);          
        while(analogRead(A4) > 0){         
        }
        pinMode(dischargePin, INPUT);            
        break;
        
        case code5:
        digitalWrite(7, HIGH);
        delay(5);
        digitalWrite(7,LOW);
        delayMicroseconds(100); 
        pulse = pulseIn(8,HIGH);
        if(pulse > 0.1){ 
        capacitance = 10.E-6;
        frequency = 1.E6/(2*pulse);
        inductance = 1000000.0;
        inductance/=capacitance*frequency*frequency*4.*3.14*3.14;
        inductance*=2;
        delay(10);
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.print("Inductance:");
        lcd.setCursor(0,1); 
        lcd.print(inductance);
        lcd.setCursor(14,1); 
        lcd.print("uH");          
        delay(10);
        }
        break;
        default:
        lcd.clear();
        lcd.print("Key not defined");
        lcd.setCursor(0,1);
        lcd.print(" 1A 2V 3R 4C 5L");
        break;
      }
     irrecv.resume();
    }
}