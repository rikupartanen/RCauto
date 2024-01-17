#include <RH_ASK.h>
//Radiohead-kirjaston sisällyttäminen, kirjaston avulla RF-lähettimet saadaan lähettämään datanpätkiä

//Takamoottorin hallinta(eteen taakse liikkuminen)
int motor1Pin1 = 7;
int motor1Pin2 = 8;
int enablePin1 = 3;

// Etäisyysanturin 2 pinniä ultraäänen lähettämiseen sekä vastaanottamiseen
int trigger = 2;
int echo = 13;

// Kaksi arvoa, ultraäänen matkustusaika sekä siitä laskettava kuljettu matka
long aika;
long cm;

//Etumoottorin hallinta(kääntyminen)
int motor2Pin1 = 4;
int motor2Pin2 = 5;
int enablePin2 = 6;

//Lähettimen antaman datan string sekä sen jakaminen 2 eri stringiin
String str_potent1;
String str_potent2;
String str_out;

// Vähennetään kauko-ohjaimessa stringiin lisätty määrä jotta data olisi koko ajan yhtä monta merkkiä
int potent1;
int potent2;

RH_ASK rf_driver;

void setup(){  
    //Hallitaan Arduinosta tulevia johtoja
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(enablePin1, OUTPUT);
    pinMode(enablePin2, OUTPUT);
    
    //enablePin 1 ja 2 on kiinnitetty L298H H-Bridgeen kiinni PWM-napoihin, jolla hallitaan moottoreille tulevaa virtaa välillä 0-255
    //(190 rajoitettu koska alkuperäinen kytkentä oli 4,5V ja meillä on 10,5V koska H-Bridge ja arduino vaativat virtaa)

    // Aloita ASK-toiminto
    rf_driver.init();

    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    
    Serial.begin(9600);
}

void loop() {

    // Annetaan oletetun viestin pituus
    uint8_t buf[7];
    uint8_t buflen = sizeof(buf);
    // Check if received packet is correct size
    if (rf_driver.recv(buf, &buflen))
    {

      // Saadun datan muuttaminen stringiksi jos viesti on oikean pituinen
      str_out = String((char*)buf);

      // Stringin jakaminen 2 osaan
      for (int i = 0; i < str_out.length(); i++) {
      if (str_out.substring(i, i+1) == ",") {
      str_potent1 = str_out.substring(0, i);
      str_potent2 = str_out.substring(i+1);
      break;}
    }
    
    potent1 = atoi( &str_potent1[0])-100;
    potent2 = atoi( &str_potent2[0])-100;
    }
    
    //Debug
    Serial.print("Potentiometer 1: ");
    Serial.print(potent1);
    Serial.print("  - Potentiometer 2: ");
    Serial.print(potent2);
    Serial.print("  - Matka: ");
    Serial.println(cm);



    //Liikkuminen eteen ja taakse

    //Määritetään paljonkon tietty potentiometrin asento antaa virtaa takamoottorille ja mihin suuntaan moottori pyörii

    if (potent1 >90 && potent1 <=150 && cm>30){
      eteenpain();}

      else if (potent1 <70 && potent1 >= 0 && cm>30){
      taaksepain();}

    else if (potent1 <=90 && potent1 >=70 && cm >30){
    analogWrite(enablePin1, 0);
    digitalWrite(7,LOW);
    digitalWrite(8,LOW);}


    //Ohjaus vasen ja oikea

    //Määritetään paljonkon tietty potentiometrin asento antaa virtaa etumoottorille ja mihin suuntaan moottori pyörii

      if (potent2 >90 && potent2 <= 150 && cm>30){
        oikea();}

      else if (potent2 <70 && potent2 >= 0 && cm>30){
        vasen();}


    else if (potent2 <=90 && potent2 >=70 && cm>30){
    analogWrite(enablePin2, 0);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);}

    digitalWrite(trigger, LOW);
    delayMicroseconds(5);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);
 
    pinMode(echo, INPUT);
    aika = pulseIn(echo, HIGH);

    //Lasketaan matka ajan perusteella. Koska ultraääni kulkee saman matkan 2 kertaa, jaetaan aika kahdella. Tämän jälkeen tulos jaetaan 29.1, joka on äänen nopeus.
    cm = aika/58.2;

    if(cm < 30 && cm >= 0 && (potent1 >90 || potent1 <70) ){
    etaisyys();}
    delay(20);
  
}

void etaisyys()
{
digitalWrite(7,HIGH);
digitalWrite(8,LOW);
analogWrite(enablePin1,180);
digitalWrite(4,LOW);
digitalWrite(5,HIGH);
analogWrite(enablePin2,240);}
    
void eteenpain(){
  if (potent1 >90 && potent1 <=100 && cm>30){
    analogWrite(enablePin1, 100);
    digitalWrite(7,LOW);
    digitalWrite(8,HIGH);}

    else if (potent1 >100 && potent1 <=110 && cm>30){
    analogWrite(enablePin1, 120);
    digitalWrite(7,LOW);
    digitalWrite(8,HIGH);}

    else if (potent1 >110 && potent1 <=120 && cm>30){
    analogWrite(enablePin1, 140);
    digitalWrite(7,LOW);
    digitalWrite(8,HIGH);}

    else if (potent1 >120 && potent1 <=130 && cm>30){
    analogWrite(enablePin1, 160);
    digitalWrite(7,LOW);
    digitalWrite(8,HIGH);}

    else if (potent1 >130 && potent1 <=140 && cm>30){
    analogWrite(enablePin1, 180);
    digitalWrite(7,LOW);
    digitalWrite(8,HIGH);}

    else if (potent1 >140 && potent1 <=150 && cm>30){
    analogWrite(enablePin1, 200);
    digitalWrite(7,LOW);
    digitalWrite(8,HIGH);}
}

void taaksepain(){

    if (potent1 <70 && potent1 >=60 && cm>30){
    analogWrite(enablePin1, 100);
    digitalWrite(7,HIGH);
    digitalWrite(8,LOW);}

    else if (potent1 <60 && potent1 >=50 && cm>30){
    analogWrite(enablePin1, 120);
    digitalWrite(7,HIGH);
    digitalWrite(8,LOW);}

    else if (potent1 <50 && potent1 >=40 && cm>30){
    analogWrite(enablePin1, 140);
    digitalWrite(7,HIGH);
    digitalWrite(8,LOW);}

    else if (potent1 <40 && potent1 >=30 && cm>30){
    analogWrite(enablePin1, 170);
    digitalWrite(7,HIGH);
    digitalWrite(8,LOW);}

    else if (potent1 <30 && potent1 >=20 && cm>30){
    analogWrite(enablePin1, 200);
    digitalWrite(7,HIGH);
    digitalWrite(8,LOW);}

    else if (potent1 <20 && potent1 >=0 && cm>30){
    analogWrite(enablePin1, 220);
    digitalWrite(7,HIGH);
    digitalWrite(8,LOW);}
}

void oikea(){
    if (potent2 >90 && potent2 <=100 && cm>30){
    analogWrite(enablePin2, 150);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);}

    else if (potent2 >100 && potent2 <=110 && cm>30){
    analogWrite(enablePin2, 160);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);}

    else if (potent2 >110 && potent2 <=120 && cm>30){
    analogWrite(enablePin2, 170);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);}

    else if (potent2 >120 && potent2 <=130 && cm>30){
    analogWrite(enablePin2, 190);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);}

    else if (potent2 >130 && potent2 <=140 && cm>30){
    analogWrite(enablePin2, 220);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);}

    else if (potent2 >140 && potent2<=150 && cm>30){
    analogWrite(enablePin2, 240);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);}
}

void vasen(){
    if (potent2 <70 && potent2 >=60 && cm>30){
    analogWrite(enablePin2, 150);
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);}

    else if (potent2 <60 && potent2 >=50 && cm>30){
    analogWrite(enablePin2, 160);
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);}

    else if (potent2 <50 && potent2 >=40 && cm>30){
    analogWrite(enablePin2, 170);
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);}

    else if (potent2 <40 && potent2 >=30 && cm>30){
    analogWrite(enablePin2, 190);
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);}

    else if (potent2 <30 && potent2 >=20 && cm>30){
    analogWrite(enablePin2, 220);
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);}

    else if (potent2 <20 && potent2 >= 0 && cm>30){
    analogWrite(enablePin2, 240);
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);}
}
