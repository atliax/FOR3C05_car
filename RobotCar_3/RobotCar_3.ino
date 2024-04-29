//**************************** RobotCar_3 ********************************
// Only files with the .ino extension are scanned for include files.     *
// This is by design, for simplicity for one thing and so that a         *
// library can't include another library that includes another library   *
// and so on.                                                            *
// Athugid ad setja snuningsAtt i RobotFunctions.h a -1 ef servomotor    *
// er orvhentur                                                          *
//***********************************************************************/
#include "Arduino.h"
#include "MyDFPlayer.h"
#include "RobotFunctions.h"

//************************** uppsetning a forritinu *****************
void setup() {
    pinMode(motorVpwm_, OUTPUT);
    pinMode(motorVgir_, OUTPUT);
    pinMode(motorHpwm_, OUTPUT);
    pinMode(motorHgir_, OUTPUT);

    servoMain.attach(servo_pin);

    pinMode(sonarTrigger_, OUTPUT);

    // Allir inngangar verda ad hafa pullup vidnam
    // INPUT_PULLUP er internal pullup vidnam (20kohm) orgjorvans
    pinMode(sonarEcho_, INPUT_PULLUP);
    pinMode(TXspilari_, OUTPUT);
    pinMode(RXspilari_, INPUT_PULLUP);

    //******************** Setja upp MDFPlayer *************************
    Serial.begin(9600);
    mySerial.begin(9600);
    delay(500);

    mp3_set_volume(20);
    delay(100);

    mp3_play_track(17); // 17. Bíll í gang (gamli bíllinn)
    delay(5000);

    mp3_play_track(lagNr++); // 1. Riding along in my automobile

    time = millis(); // Setur time breytuna á tíma liðinn frá starti

    // snua SONAR beint fram, í þessum bíl er leiðrétt um 12 gráður
    turnSonar(SONAR_FORWARD, -1);
}

//*************************** Keyrslulykkjan **********************
void loop() {
    turnSonar(SONAR_FORWARD, -1);
    delay(100);

    // Keyra bil afram
    driveForward();
    delay(100);

    // Ef Spilari er upptekinn þá er RXspilari = 0
    // RXspilari (D2) tengist BUSY tengi á spilara sjá mynd 17
    if (digitalRead(RXspilari_) == 1) {
        if (lagNr == 17) {
            lagNr = 1;
        }

        // spila naesta lag
        mp3_play_track(lagNr++);
    }

    int delaysec = 15;

    // ef það er aðskotahlutur minna en 40cm framan vid bilinn
    while (distanceFromSonar() < 40 || (millis() - time >= (delaysec * 1000))) {
        stopCar();
        delay(100); // i raun 200 vegna delay(100) i stopCar()

        driveBackwards();
        if (millis() - time >= (delaysec * 1000)) {
            delay(1000);
        }

        stopCar();
        // herna er 100ms delay i endanum a stopCar()

        int vinstri = 0;
        int haegri = 0;

        turnSonar(SONAR_LEFT, -1);
        delay(1000);
        vinstri = distanceFromSonar();

        turnSonar(SONAR_RIGHT, -1);
        delay(1000);
        haegri = distanceFromSonar();

        turnSonar(SONAR_FORWARD, -1);

        if (vinstri > haegri) {
            turnLeft();
        } else {
            turnRight();
        }

        // Gefa tíma til að beygja ca +/- 90°
        delay(375);

        driveForward();

        delay(1000); // delay herna er kannski otharfi

        time = millis();
    }
}
