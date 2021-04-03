Lucile COMBA--ANTONETTI 

Rapport individuel de suivi de projet :
==
Séance du 10/09/20  
- 
- installation et prise en main de github et git (tests pull/push)  
- prise de connaissance du travail à faire  
- prise de connaissance des travaux réalisés par le groupe de l'an dernier 

Séance du 25/09/20  
- 
- rédaction du cahier des charges : terminé
- étude du fonctionnement du capteur de pluviométrie (avec Loris)
- lecture des Datasheets pour réaliser les cablages suivants
- mise en place du cablage du capteur BME 280, du microcontrolleur ESP32 et du driver FTDI 232 sur la plaque Labdec  

Séance du 08/10/20  
- 
- test du capteur BME 280  
- mise en place du cablage du capteur TSL 2591  
- test du capteur TSL 2591  
- téléchargement de librairie ESP32  
- lecture de la datasheet du pluviomètre  

Séance du 22/10/20  
- 
- réalisation du code arduino du pluviomètre  
- lecture de la datasheet de l'anémomètre   
- réalisation du code arduino du pluviomètre 
- mise en place du cablage du pluviomètre (avec bouton poussoir)  

Séance du 19/11/20  
-    
- test du code du pluviomètre (avec bouton poussoir)  
- problèmes de cablage : pouton poussoir défectueux + problème de pin 
- les modifcations apportées ont résolu les problèmes

Séance du 26/11/20  
- 
- mise en place du cablage de l'anémomètre (avec bouton poussoir)  
- test du code de l'anémomètre (avec bouton poussoir)
- cablage de l'anémometre avec l'adaptateur RJ11
- test avec ce cablage : il ne fonctionne pas, il faut le ressouder
- test sans le RJ11 (en tenant les cables) : problèmes de rebonds
- mise en place d'un condensateur (220 pF) en parallèle de l'anémomètre  

Séance du 03/12/20  
- 
- test du capteur anémomètre avec le condensateur : constatation d'un problème d'interruption (1 puis 4)  
- test à l'oscilloscope : aucun rebond détectable  
- test externe avec un GBF -> ça marche
- même test en remplancant l'anémomètre par le pluviomètre : aussi des problèmes d'interruptions  
- ressoudure d'un nouveau RJ-11 


Séance du 04/12/20  
- 
- correction du code du pluviomètre et de l'anénomètre pour empécher les multiples interuptions sur un seul front montant  
- test du nouveau code pour les deux capteurs -> ça marche  
- lecture de la datasheet de la girouette  
- mise en place du clablage de la girouette  
- réalisation du code de la girouette : utilistion dans le code de la fonction analogRead 
- test du code de la girouette : problème CAN non linéaire pour ESP32   

Séance du 10/12/20  
- 
- participation à l'implémentation du code pour gérer la fonction sleep de l'ESP32
- recherche pour corriger le problème de non linéarité de l'acd  
- modification du code de la girouette  

Séance du 17/12/20  
- 
- participation aux recherches pour modifier la fonction sleep de l'ESP32 (problème du timer)  
- test du nouveau code de la girouette -> marche pas  
- modification du code de la girouette  

Séance du 08/01/21  
- 
- réalisation du Powerpoint de présentation  
- création des ordinogrammes des codes   

Séance du 29/01/21
-
- réalisation du tableau excel pour améliorer le tableau d'avancement  
- avancement du code de la girouette (mais matériel pas disponible pour tester durant la séance)  
- réalisation des tests du module LoRa, cablâge revue, en fin de séance le module était initialisé (envoi testé pas reception)  
- configuration du code de récéption de LoRa pour le module TTGO (LoRa externe pour test)  

Séance du 12/02/21
-
- finition du tableau d'avancement  
- problème d'initialisation du module Lora => Debuggage du câblage du module LoRa : breadBoard mère non fonctionnelle  
- tests du bon fonctionnement de nouveaux cables 
- participation au câblage d'un module RFM95 sur une nouvelle breadBoard
- tests d'échanges entre le RFM95 et le module TTGO -> tests fonctionnels  

Séance du 19/02/21
-
- cablage pour tester le code pour gérer 2 modules LoRa
- tests de communication entre la balance, les modules LoRa et la gateway

Séance du 05/03/21
-
- utilisation de l'ancienne plaque Labdec pour tester le code de la girouette  
- problème au niveau de la valeur de la tension retournée sur l'ADC1
- réalisation de mesures des tensions sur la breadboard au multimètre ainsi que sur le connecteur RJ11 de la Girouette  
- source du problème : câblage de l'adaptateur RJ45  
- réalisation d'un tableau de valeur de la tesion au borne de la résistance variable de la girouette en fonction de l'angle pour un Vcc=3,3V  

Séance du 12/03/21
-
- test de l'anénmomètre après réparation : OK  
- vérification/comparaison entre les valeurs du tableau, la tension mesurée avec le multimetre et celles visible sur le moniteur série  
- détection d'un problème au niveau du channel et de l'atténuation  
- modification du code de la girouette  
- variation de la tension d'alimentation (potentiomètre) => relevé de la tention de la résistence interne de la girouette avec un analogread() puis avec l'aténuation  

Séance du 19/03/21
-
- analyse des tracés de la séance précédente  
- calcule de la résistance de référence permettant des deltats de tension les plus élevés  
- power point de présentation complété  
- prise en compte de la variation de ta tension d'alimentation dans le code de la girouette  

Séance de 01/04/21
-
- poursuite du code de la girouette (récupération de la tension d'alimentation et calcul du )  
- tests post création de carte  
- Tests de continuité  
- Suppression de plans de masse sous les composants CMS  


