Tâches projet Loris Magne

Séance du 10/09/2020 :  
- 
- Prise de connaissance du travail éffectué les années précédentes  
- Prise en main de Github et création des différents fichiers et de l'espace de travail  
- Prise en main des commandes pull / push dans le Git Bash  
- Prise en main de la synthaxe Markdown

Séance du 25/09/2020 :
- 
- Etude du fonctonnement des différents capteurs, notamment celui qui mesure la pluviométrie  
- Prise de connaissance du code existant  
- Mise à jour du code existant et création d'une nouvelle version V1.1, intégrant les bonnes librairies pour utiliser nos capteurs (avec Nicolas H.)  
- Création d'un fichier test pour le capteur BME (température, pression et humidité) (avec Nicolas H.)  

Séance du 08/10/2020 :
-
- Câblage des capteurs BME et TSL (ok)  
- Réalisation des tests sur le capteur BME (température, pression et humidité) et le capteur TSL2591 (luminosité)  
- Recherche et installation des différentes librairies pour faire le code LoRa sur Arduino  
- Commencement du code LoRa afin de tester l'envoi de trames avec le module Lora RFM96  

Séance du 22/10/2020 (isolé car cas contact COVID) :
-  
- Recherche et documentation pour le câblage du module RF96.
- Finalisation d'un code permettant l'envoie de trames LoRa.
- Elaboration d'un code permettant la réception de trames LoRa.
- Câblage du module LoRa et test des premiers codes (envoie de trames)

Séance du 19/11/2020 :
-  
- Analyse et description du fonctionnement de la Gateway
- Câblage et test du pluviomètre
- Analyse du câblage car problème de téléversement lorsqu'on branche MISO et MOSI
- Test envoi trames LoRa

Séance du 26/11/2020
-  
- Test du code de l'anémomètre avec un bouton poussoir simulant le nombre d'interruptions (tours)  
- Câblage de l'anémomètre avec l'adaptateur RJ11  
- Recherche du problème au niveau du nombre d'interruption --> adaptateur non fonctionnel  
- Test en direct sur l'anémomètre avec une résistance de tirage --> interruption ok mais problème de rebond  
- Pour palier le problème de rebond, mise en place d'une capacité en parrallèle avec le switch  
