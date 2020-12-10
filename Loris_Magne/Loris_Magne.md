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

Séance du 03/12/2020
- 
- On a remarqué un problème du nombre d'incrémentation d'interruptions sur l'anémomètre : 1x 1 interruption et 1x 4 interruption (peut être problème au niveau d'un aimant)  
- On a constaté le même problème sur le pluviomètre  
- Test du code en générant un signal carré avec le GBF --> code fonctionne bien pour le pluviomètre et l'anémomètre  
- Refabrication de l'adaptateur RJ11 pour éliminer un possible problème côté connecteur  

Séance du 04/12/2020
- 
- Poursuite de la recherche du problème au niveau du nombre d'interruptions  
- Solution --> mise en place d'un timer qui permet d'ignorer les interruptions s'il ne s'est pas passé assez de temps entre chacune  
- Test de l'anémomètre et du pluviomètre --> câblage et code fonctionnels
- Fabrication d'un nouvel adaptateur RJ11 pour connecter la girouette  
- Recherche du fonctionnement du mode deep sleep afin de pouvoir réveiller l'ESP32 lors d'une interruption provenant du pluviomètre ou au bout de 15min (timer)  
- Implémentation d'un premier code pour gérer la fonction d'endormissement et de réveil  

Séance du 10/12/2020
- 
- Poursuite de l'implémentation du code permettant de gérer la fonction d'endormissement et de réveil  
- Analyse et compréhension du PCB du groupe précédent  
