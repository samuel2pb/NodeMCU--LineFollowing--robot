#define ENA   16            // Ativação/Velocidade motores da direita   GPIO16(D0)
#define ENB   14            // Ativação/Velocidade motores da esquerda  GPIO14(D5)

#define IN1   2             // L298N IN1 motores da direita             GPIO2 (D4)
#define IN2   0             // L298N IN2 motores da direita             GPIO0 (D3)
#define IN3   4             // L298N IN3 motores da esquerda            GPIO4 (D2)
#define IN4   5             // L298N IN4 motores da esquerda            GPIO5 (D1)

#define Sensor_esquerda 12  //Sensor TCR5000 lado esquerdo              GPIO12(D6)
#define Sensor_meio 13      //Sensor TCR5000 meio                       GPIO13(D7)
#define Sensor_direita 15   //Sensor TCR5000 lado direito               GPIO15(D8)

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Servo.h>

Servo servo1; 
Servo servo2;

String comando;             //Coleta do aplicativo o estado do comado
int velocidade = 800;       //Ajuste de 400 - 1023
int velocidade_curva = 2;
int velocidade_ctrl; 
bool esquerda, meio, direita;

const char* ssid = "NodeMCU Car";
ESP8266WebServer server(80);

void setup() {

 pinMode(ENA, OUTPUT);
 pinMode(ENB, OUTPUT);  
 pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);
 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT); 
 pinMode(Sensor_esquerda, INPUT);
 pinMode(Sensor_meio, INPUT);  
 pinMode(Sensor_direita, INPUT);
 pinMode(LED_BUILTIN, OUTPUT);
 
 //servo1.attach(3);
 servo2.attach(1);

 Serial.begin(115200);
  
 //Conectando via WiFi

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Ponte de acesso - Endereco IP: ");
  Serial.println(myIP);
 
 // Starting WEB-server 
  server.on ( "/", HTTP_handleRoot );
  server.onNotFound ( HTTP_handleRoot );
  server.begin();    
}

void Frente(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Tras(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void TrasDireita(int vel){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, vel);
  analogWrite(ENB, vel/velocidade_curva);
}

void Esquerda(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void TrasEsquerda(int vel){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, vel/velocidade_curva);
  analogWrite(ENB, vel);
}

void FrenteDireita(int vel){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, vel);
  analogWrite(ENB, vel/velocidade_curva);
}

void Direita(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void FrenteEsquerda(int vel){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, vel/velocidade_curva);
  analogWrite(ENB, vel);
}

void Parada(){  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
 }

void loop() {
  server.handleClient();
    
  comando = server.arg("Estado");
  
  if (comando == "Auto"){
    esquerda = digitalRead(Sensor_esquerda);
    meio = digitalRead(Sensor_meio);
    direita = digitalRead(Sensor_direita);
    
    if(esquerda == false && direita == false && meio == false){
      Frente();
      analogWrite(ENA, 0);
      analogWrite(ENB, 0);
      delay(500);
    }
    
    else if(meio == true){
      Frente();
      analogWrite(ENA, velocidade);
      analogWrite(ENB, velocidade);
     }
     
     else if(esquerda == true && direita == false){
      Frente();
      analogWrite(ENA, velocidade/velocidade_curva);
      analogWrite(ENB, velocidade);
      }
      
      else if(esquerda == false && direita == true){
       Frente();
       analogWrite(ENA, velocidade);
       analogWrite(ENB, velocidade/velocidade_curva);
      }
        
      else if(esquerda == true && meio == true){
       Direita();
       analogWrite(ENA, velocidade);
       analogWrite(ENB, velocidade);
      }
        
      else if(direita == true && meio == true){
       Esquerda();
       analogWrite(ENA, velocidade);
       analogWrite(ENB, velocidade);
      }
    }
    else{
     if (comando == "Frente"){         Frente();   analogWrite(ENA, velocidade_ctrl); analogWrite(ENB, velocidade_ctrl);} 
     else if (comando == "Tras"){           Tras();     analogWrite(ENA, velocidade_ctrl); analogWrite(ENB, velocidade_ctrl);}
     else if (comando == "Esquerda"){       Esquerda(); analogWrite(ENA, velocidade_ctrl); analogWrite(ENB, velocidade_ctrl);}
     else if (comando == "Direita"){        Direita();  analogWrite(ENA, velocidade_ctrl); analogWrite(ENB, velocidade_ctrl);}
     else if (comando == "0"){              velocidade_ctrl = 400;  servo2.write(50);}
     else if (comando == "1"){              velocidade_ctrl = 470;  servo2.write(55);}
     else if (comando == "2"){              velocidade_ctrl = 540;  servo2.write(60);}
     else if (comando == "3"){              velocidade_ctrl = 610;  servo2.write(65);}
     else if (comando == "4"){              velocidade_ctrl = 680;  servo2.write(70);}
     else if (comando == "5"){              velocidade_ctrl = 750;  servo2.write(75);}
     else if (comando == "6"){              velocidade_ctrl = 820;  servo2.write(80);}
     else if (comando == "7"){              velocidade_ctrl = 890;  servo2.write(85);}
     else if (comando == "8"){              velocidade_ctrl = 960;  servo2.write(90);}
     else if (comando == "9"){              velocidade_ctrl = 1023; servo2.write(100);}
     else if (comando == "Parada")          Parada();
     else if (comando == "Abre")            servo2.write(110);             
     else if (comando == "Fecha")           servo2.write(50);
    }       
}

void HTTP_handleRoot(void) {

if(server.hasArg("Estado")){
       Serial.println(server.arg("Estado"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
