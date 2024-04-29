//*************************** Robot Functions **********************************

// Servo er innbyggður í Arduino en það þarf að stofna servoMain sem er þá
// eintak af Servo
// servoMain hefur t.d. fallið write(int stefna)
#include <Servo.h>
Servo servoMain;

//******************* Skilgreining á tengjum og breytum **************

int motorVpwm_ = 5;
int motorVgir_ = 9;
int motorVpwm; // Geymir stöðu motorV  (analogWrite(motorVpwm_,motorVpwm);)

int motorHpwm_ = 6;
int motorHgir_ = 12;
int motorHpwm; // Geymir stöðu motorH  (analogWrite(motorHpwm_,motorHpwm);)
int servo_pin = 10; // Geymir númer servo tengis

int VgrunnV = 150; // Upphafsgildi mótoranna
int VgrunnH = 148;

int TXspilari_ = 4; // Sendir skipanir til mp3 spilarans
int RXspilari_ = 2; // Tekur við skipunum frá mp3 spilara

int sonarTrigger_ = 7; // Sendir 10us breiðan 40kHz sónarpúls
int sonarEcho_ = 8;    // Tekur á móti SONAR púlsinum
int lagNr = 1;             // Synir hvaða lag er spilad

#define SONAR_LEFT -85
#define SONAR_FORWARD -12
#define SONAR_RIGHT 60

unsigned long time; 

// ****************************** Föll *****************************************

// Reikna púlsbreidd frá gráðum og kalla síðan á servoMain.write(stefna)
void turnSonar(int gradur, int snunAtt) // réttsaelis=1 rangsaelis=-1
{
    int stefna;
    if (snunAtt >= 0)
        stefna = gradur + 90;
    else
        stefna = -1 * gradur + 90;
    if (stefna < 0)
        stefna = 0;
    if (stefna > 180)
        stefna = 180;
    servoMain.write(stefna);
}

// Reiknar fjarlaegd ad endurvarpi
// Athugid ad nota Tetta fall ekki nema SONAR se
// tengdur annars bídur tolvan endalaust
unsigned int distanceFromSonar() // Reikna lengd ad endurvarpi
{
    unsigned int tmp;

    // Setja trigger utgang í hastoðu (Byrja StartPuls)
    digitalWrite(sonarTrigger_, HIGH);

    // bíða í 10 us  (Start púls er 10us breiður)
    delayMicroseconds(10);

    // Setja trigger útgang í lastodu (enda startpuls)
    digitalWrite(sonarTrigger_, LOW);

    // taka vid pulsinum
    tmp = pulseIn(sonarEcho_, HIGH);

    // reikna puls yfir i cm
    tmp = tmp * 0.01715; // tmp*34300cm/s /2 =tmp*0,01715cm/us

    // Skila ut lengd i cm
    return tmp;
}

// keyra afram
void driveForward() {
    digitalWrite(motorVgir_, HIGH);
    digitalWrite(motorHgir_, LOW);
    analogWrite(motorVpwm_, VgrunnV);
    analogWrite(motorHpwm_, VgrunnH);
}

// stodva mjuklega
void stopCar() {
    analogWrite(motorVpwm_, VgrunnV);
    analogWrite(motorHpwm_, VgrunnH);

    for (int i = 120; i >= 0; i -= 20) {
        delay(100);
        analogWrite(motorVpwm_, i);
        analogWrite(motorHpwm_, i);
    }
    delay(100);
}

// keyra afturabak
void driveBackwards() {
    digitalWrite(motorVgir_, LOW);
    digitalWrite(motorHgir_, HIGH);
    analogWrite(motorVpwm_, VgrunnV);
    analogWrite(motorHpwm_, VgrunnH);
}

// snua til vinstri 
void turnLeft() {
    digitalWrite(motorVgir_, LOW);
    digitalWrite(motorHgir_, LOW);
    analogWrite(motorVpwm_, VgrunnV);
    analogWrite(motorHpwm_, VgrunnH);
}

// snua til haegri 
void turnRight() {
    digitalWrite(motorVgir_, HIGH);
    digitalWrite(motorHgir_, HIGH);
    analogWrite(motorVpwm_, VgrunnV);
    analogWrite(motorHpwm_, VgrunnH);
}
