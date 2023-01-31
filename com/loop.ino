

String commandeAexecuter = "";
bool commandeEnCoursDeReception = true;

void loop() {
  
}
void loop2(){
  Serial.println("Loop Start in 10s");
  delay(10000);
  Serial.println("Loop Start");
  while(Serial.available() > 0) {
    
    char octetRecu = Serial.read();

    // On teste si l'octet reçu est égal au "caractère saut de ligne" (char 10)
    if(octetRecu == char(10)) {
            Serial.println("char10");

      //commandeEnCoursDeReception = false;     // Nous avons à présent pu récolter l'ensemble des octets constituant la commande extérieure
      executeCommande(commandeAexecuter);     // Il ne reste donc plus qu'à exécuter cette dernière !
      commandeAexecuter = "";
    }
    // Si ce n'est aucun de ces 2 caractères spéciaux, le caractère reçu est à concaténer aux précédents reçus (car cela signifie qu'une commande est en cours de réception)
    if((octetRecu != char(0)) && (octetRecu != char(10)) && commandeEnCoursDeReception) {
      commandeAexecuter = commandeAexecuter + octetRecu;
    }
  }

  loopLed();

  loopProximity();

  loopTemp();


}

void executeCommande(String texteDeLaCommande) {


  if(texteDeLaCommande == "BLINK") {
      isInternalLedBlinking = true;
      Serial.println("[Réponse ARDUINO] Clignotement démarré !");
  } 
  else if(texteDeLaCommande == "STOP") {
      isInternalLedBlinking = false;
      Serial.println("[Réponse ARDUINO] Clignotement arrêté");
  }
  else if(texteDeLaCommande == "BLUE") {
    displayBlueLed();
      Serial.println(" BLue");
  }
  else if(texteDeLaCommande == "RED") {
    displayRedLed();
      Serial.println(" Red");
  }
  else {
      Serial.println("[Réponse ARDUINO] Commande '" + texteDeLaCommande + "' inconnue…");
  }

}
