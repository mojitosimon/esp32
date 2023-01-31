#include <Adafruit_SSD1306.h>

#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);


void setupScreen() {

 if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    Serial.println("Screen  initialisation FAILED");                               // Arrêt du programme (boucle infinie) si échec d'initialisation
  ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
  ecranOLED.setTextSize(2);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor(SSD1306_WHITE);
  screenPrint("Screen OK");
  
  Serial.println("Screen correctly initialized");
}


void screenPrint(char * text){
    ecranOLED.println(text);
    ecranOLED.display();                            // Transfert le buffer à l'écran
}