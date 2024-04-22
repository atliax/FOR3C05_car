//*************************** Robot Functions **********************************

// Servo er innbyggður í Arduino en það þarf að stofna servoMain sem er þá
// eintak af Servo
// servoMain hefur t.d. fallið write(int stefna)
#include <Servo.h>
Servo servoMain;

//********************** Skilgreining á tengjum og breytum *********************

int motorVpwm_ = 5;
int motorVgir_ = 9;
int motorVpwm; // Geymir stöðu motorV  (analogWrite(motorVpwm_,motorVpwm);)

int motorHpwm_ = 6;
int motorHgir_ = 12;
int motorHpwm;   // Geymir stöðu motorH  (analogWrite(motorHpwm_,motorHpwm);)
int servo_ = 10; // Geymir númer servo tengis

int VgrunnV = 150; // Upphafsgildi mótoranna
int VgrunnH = 150;

int TXspilari_ = 4; // Sendir skipanir til mp3 spilarans
int RXspilari_ = 2; // Tekur við skipunum frá mp3 spilara

int sonarTrigger_ = 7; // Sendir 10us breiðan 40kHz sónarpúls
int sonarEcho_ = 8;    // Tekur á móti SONAR púlsinum
int lagNr;             // Synir hvaða lag er spilad
int randomTurn = 1;

//*************************** Lengdir og Tímar *********************************

// unsigned int lengdV;
// unsigned int lengdH;
unsigned int lengdMax = 0;
// int lengdX = 0;
unsigned long time; // Notuð með millis() fallinu til að ákveða spilatíma laga
// unsigned long timeX;

// ****************************** Föll *****************************************

// Reikna púlsbreidd frá gráðum og kalla síðan á servoMain.write(stefna)
void reiknaPulsBreidd(int gradur, int snunAtt) // réttsaelis=1 rangsaelis=-1
{
    int stefna = gradur + 90;

    if (snunAtt < 0) {
        stefna *= -1;
    }

    if (stefna < 0) {
        stefna = 0;
    }

    if (stefna > 180) {
        stefna = 180;
    }

    servoMain.write(stefna);
}

// Reiknar fjarlaegd ad endurvarpi
// Athugid ad nota Tetta fall ekki nema SONAR se
// tengdur annars bidur tolvan endalaust
unsigned int lengd() {
    unsigned int tmp;

    // Setja trigger utgang i hastodu (Byrja StartPuls)
    digitalWrite(sonarTrigger_, HIGH);

    // bida i 10 us  (Start puls er 10us breidur)
    delayMicroseconds(10);

    // Setja trigger utgang i lagstodu (enda startpuls)
    digitalWrite(sonarTrigger_, LOW);

    tmp = pulseIn(sonarEcho_, HIGH);
    tmp = tmp * 0.01715; // tmp*34300cm/s / 2 = tmp*0,01715cm/us

    // skila ut lengd
    return tmp;
}

// keyra bil afram
void startCar() {
    digitalWrite(motorVgir_, HIGH);
    digitalWrite(motorHgir_, LOW);
    analogWrite(motorVpwm_, VgrunnV);
    analogWrite(motorHpwm_, VgrunnH);
}

// bremsa bil
void breakCar() {
    analogWrite(motorVpwm_, 0);
    analogWrite(motorHpwm_, 0);
}

// stoppa bil
void stopCar() {
    digitalWrite(motorVgir_, HIGH);
    digitalWrite(motorHgir_, LOW);

    for (int i = VgrunnH; i <= 0; i -= 10) {
        analogWrite(motorVpwm_, i);
        analogWrite(motorHpwm_, i);
        delay(100);
    }
}

// bakka bil
void backCar() {
    stopCar();
    delay(500);

    digitalWrite(motorVgir_, LOW);
    digitalWrite(motorHgir_, HIGH);
    analogWrite(motorVpwm_, VgrunnV);
    analogWrite(motorHpwm_, VgrunnH);
}

// snua bil til vinstri
void driveLeft() {
    digitalWrite(motorVgir_, LOW);
    digitalWrite(motorHgir_, LOW);
    analogWrite(motorVpwm_, VgrunnV);
    analogWrite(motorHpwm_, VgrunnH);
}

// snua bil til haegri
void driveRight() {
    digitalWrite(motorVgir_, HIGH);
    digitalWrite(motorHgir_, HIGH);
    analogWrite(motorVpwm_, VgrunnV);
    analogWrite(motorHpwm_, VgrunnH);
}
