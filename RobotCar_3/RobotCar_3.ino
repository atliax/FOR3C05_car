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

    // Servo on digital pinna ~10
    servoMain.attach(10);

    pinMode(sonarTrigger_, OUTPUT);

    // Allir inngangar verda ad hafa pullup vidnam
    // INPUT_PULLUP er internal pullup vidnam (20kohm) orgjorvans
    pinMode(sonarEcho_, INPUT_PULLUP);
    pinMode(TXspilari_, OUTPUT);
    pinMode(RXspilari_, INPUT_PULLUP);

    Serial.begin(9600);
    //******************** Setja upp MDFPlayer *************************
    mySerial.begin(9600);
    delay(500);

    mp3_set_volume(20);
    delay(100);

    mp3_play_track(17); // 17. Bíll í gang (gamli bíllinn)
    delay(5000);

    // mp3_play_track(1); // 1. Riding along in my automobile
    lagNr = 2;

    // Timar
    time = millis(); // Setur time breytuna á tíma liðinn frá starti
    // timeX = time;

    // snua SONAR beint framm, í þessum bíl er leiðrétt um X gráður - TODO
    reiknaPulsBreidd(-5, -1);

    // lengdX = lengd();
}

//*************************** Keyrslulykkjan **********************
void loop() {
    reiknaPulsBreidd(0, -1);
    delay(100);

    // Keyra bil afram
    driveCar();
    delay(100);

    // Ef Spilari er upptekinn þá er RXspilari = 0
    // RXspilari (D2) tengist BUSY tengi á spilara sjá mynd 17
    if (digitalRead(RXspilari_) == 1) {
        if (lagNr == 17) {
            lagNr = 1;
        }
        // spila naesta lag
        // mp3_play_track(lagNr++);
    }

    // ef það er aðskotahlutur minna en 40cm framan vid bilinn
    while (lengd() < 40) {
        backCar();

        delay(500);

        stopCar();

        int vinstri = 0;
        int haegri = 0;

        delay(1000);
        reiknaPulsBreidd(-85, -1);

        vinstri = lengd();

        delay(1000);
        reiknaPulsBreidd(60, -1);

        haegri = lengd();

        delay(1500);
        reiknaPulsBreidd(-12, -1);

        if (vinstri > haegri) {
            driveLeft();
        } else {
            driveRight();
        }

        // Gefa tíma til að beygja ca +/- 90°
        delay(350);
        driveCar();

        delay(1000);
    }
}
