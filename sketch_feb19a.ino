//Matthieu SAJOT / Leo ROLLAND TD08

#include <LiquidCrystal.h>     //Bibliotheque pour l'ecran LCD
#include <Servo.h>             //Bibliotheque pour le servomoteur
#include <IRremote.h>          //Bibliotheque pour la telecommande

const int led_verte = 8;      //pin LED RVB
const int led_bleue = 9;      //pin LED RVB
const int led_rouge = 10;     //pin LED RVB
const char DIN_RECEPTEUR_INFRAROUGE = 2;    //pin pour le module ir

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);      //pin pour l'ecran lcd

Servo monServomoteur;  //declaration du servo

IRrecv monRecepteurInfraRouge (DIN_RECEPTEUR_INFRAROUGE); //declaration du module ir
decode_results messageRecu;   //stock le message recu par le module ir

int mode = 0; //0 pour mode automatique, 1 pour mode commander
int mode_commande = 0; //0 pour mode jour, 1 pour mode nuit
/******************************************************************************************************************************************************************************/
void setup()
{
  Serial.begin(9600);
  pinMode(led_rouge, OUTPUT);
  pinMode(led_verte, OUTPUT);
  pinMode(led_bleue, OUTPUT);
  monServomoteur.attach(3);
  monRecepteurInfraRouge.enableIRIn();    //initialisation du module ir
  monRecepteurInfraRouge.blink13(true);   //clignotement dune led sur l'arduino a la reception du module (facultatif)
}
/******************************************************************************************************************************************************************************/
void loop() {
  /**********************************************************************************************/
  if (monRecepteurInfraRouge.decode(&messageRecu))      //receptionne la valeur du module IR
  {
    if (messageRecu.value == 0xFD10EF)
    {
      Serial.println("Touche GAUCHE");
      mode = false;
      lcd.clear();
      lcd.print("MODE AUTOMATIQUE");
      delay(2000);
    }
    if (messageRecu.value == 0xFD50AF)
    {
      Serial.println("Touche DROITE");
      mode = true;  
      lcd.clear();
      lcd.print("MODE COMMANDE");
      delay(2000);
    }
    if (messageRecu.value == 0xFDA05F)
    {
      Serial.println("Touche HAUT");
      mode_commande = false;
    }
    if (messageRecu.value == 0xFDB04F)
    {
      Serial.println("Touche BAS");
      mode_commande = true;
    }
    monRecepteurInfraRouge.resume();
  }
  /********************************************************************************************/
  if (mode == 0)                                        //MODE AUTOMATIQUE
  {
    int valeur = analogRead(A0);      //lire la valeur de la lumiere
    int intensite;
    intensite = map(valeur,0,1023,0,255);
    
    Serial.println(valeur);           //affiche cette valeur
    Serial.println(intensite);
    if (intensite > 125)
    {
      digitalWrite(led_rouge, 0);
      digitalWrite(led_verte, 0);
      digitalWrite(led_bleue, 0);
      lcd.clear();
      lcd.print("C'EST LE JOUR");
      monServomoteur.write(0);
    }

    if (intensite < 125)
    {
      digitalWrite(led_rouge, 255);
      digitalWrite(led_verte, 0);
      digitalWrite(led_bleue, 150);
      lcd.clear();
      lcd.print("C'EST LA NUIT");
      monServomoteur.write(180);

    }
  }
  /***************************************************************************************/
  if (mode == 1)                                                     //MODE COMMANDER
  {
    if (mode_commande == 0)
    {
      digitalWrite(led_rouge, 0);
      digitalWrite(led_verte, 0);
      digitalWrite(led_bleue, 0);
      monServomoteur.write(0);
      lcd.clear();
      lcd.print("JOOOUUUURRR");
    }

    if (mode_commande == 1)
    {
      digitalWrite(led_rouge, 200);
      digitalWrite(led_verte, 255);
      digitalWrite(led_bleue, 30);
      monServomoteur.write(180);
      lcd.clear();
      lcd.print("NUUIIIITTT");
    }
  }
  /*******************************************************************************/
  delay(100);
}
