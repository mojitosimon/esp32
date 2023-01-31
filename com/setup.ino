
void setup() {
  Serial.begin(9600);

  // Déclaration de la broche où est branchée la LED en sortie
  setupLed();

  setupProximity();

  setupTemp();

  setupScreen();

  // Initialisation de la liaison PC <-> Arduino Nano

}