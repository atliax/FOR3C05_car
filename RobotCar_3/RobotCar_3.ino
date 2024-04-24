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
    delay(500); // delay herna er sennilega otharfi

    mp3_set_volume(20);
    delay(100); // delay herna er sennilega otharfi

    mp3_play_track(17); // 17. Bíll í gang (gamli bíllinn)
    delay(5000);        // delay herna er sennilega otharfi

    /*
    while (digitalRead(RXspilari_) == 0) {
        // bida eftir að hljod klari ad spilast
    }
    */

    // mp3_play_track(1); // 1. Riding along in my automobile
    lagNr = 2;

    // Timar
    time = millis(); // Setur time breytuna á tíma liðinn frá starti

    // snua SONAR beint fram, í þessum bíl er leiðrétt um 12 gráður - TODO
    turnSonar(-12, -1);
}

//*************************** Keyrslulykkjan **********************
void loop() {
    turnSonar(-12, -1);
    delay(100); // delay herna er kannski otharfi

    // Keyra bil afram
    driveForward();
    delay(100); // delay herna er kannski otharfi

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
    while (distanceFromSonar() < 40) {
        stopCar();
        delay(100); // i raun 200 vegna delay(100) i stopCar()

        driveBackwards();
        // delay(200);

        stopCar();
        // herna er 100ms delay vegna stopCar()

        int vinstri = 0;
        int haegri = 0;

        // delay(1000); // delay herna er sennilega otharfi
        turnSonar(-85, -1);
        delay(1000);
        vinstri = distanceFromSonar();

        // delay(1000);

        turnSonar(60, -1);
        delay(1000);
        haegri = distanceFromSonar();

        // delay(1500);

        turnSonar(-12, -1);

        if (vinstri > haegri) {
            turnLeft();
        } else {
            turnRight();
        }

        // Gefa tíma til að beygja ca +/- 90°
        // TODO - stadfesta ad timinn se godur
        delay(350);

        driveForward();

        delay(1000); // delay herna er kannski otharfi
    }
}
