Nicolas Heissler

10/09/2020 :
- 
-Installation et prise en main de git (commande bash + test pull/push)  
-Prise de connaissance du projet et des travaux à faire  
-Prise de connaissance des travaux réalisé par les précédents groupes      

25/09/2020 :
- 
-Etude du code de l'équipe précèdente  
-Mise en forme de tout le code déjà présent et modification afin d'améliorer la lisibilité  
-Installation de toutes les librairies nécessaire pour utiliser les différents capteurs  
-Création d'un code afin de tester le capteur BME (température, pression, humidité)  
-Test du capteur BME sur labdec  

08/10/2020 :
-
-Branchement des capteurs BME et TSL (branchement fonctionnel)  
-Test du capteur BME (seul)  
-Test du capteur TSL (seul)  
-Test des deux capteurs en simultané (i2c maître-esclave)  
-Installation de la librarie LoRa puis début d'un code de test pour envoyer des trames avec le module LoRa  

22/10/2020 :
-
-Réalisation du code pour envoyer des trames avec un module LoRa  
-Réalisation du code pour recevoir des trames avec un module LoRa  
-Branchement d'un module LoRa sur la breadBoard (module "sender")  
-Utilisation d'une autre carte programmable avec module LoRa intégré pour recevoir les trames envoyées par notre premier module  
- Premiers tests et debuggage des échanges entre modules LoRa  

19/11/2020 :
-
-Réalisation du code pour tester le capteur de pluviometrie 
-Câblage du banc de test pour tester le capteur de température 
-Câblage du banc de test pour tester le module LoRa 
-Début de test du module LoRa et comparaison avec le code de la balance et de la gateway 

26/11/2020 :
-
-Vérification du bon fonctionnement du code de l'anémometre (test avec bouton poussoir) 
-Test de l'adapteur (RJ-11) -> ne fonctionne pas, il faut en ressouder un 
-Vérification du bon fonctionnement de l'anémomètre (sans le RJ11 -> en tenant les câbles ) 
-> détection problème de rebond 
-> mise en place d'une capa de 220pF en parallèle du switch 

03/12/2020 :
-
-Test de l'anémometre -> problème d'interruption interne (test externe avec GBF du code -> OK) 
-Soudure d'un nouvel adaptateur RJ-11 car ancien défectueux 
-Modification du code de test de l'anémomètre 
-Modification du code de test du pluviomètre 

04/12/2020 :
-
-Changement du code de l'anémometre -> fonctionnel 
-Changement du code de pluviometre -> fonctionnel 
-Test du mode light_sleep -> le fonctionnement n'est pas adapté à notre projet 
-Réalisation d'un code de test pour le deepSleep sur le pluviometre (reveil pour les interruption et/ou un timer) 
-> Un réveil d'interruption reset le timer pour le wakeup (voir comment trouver le temps entre le début du timer et l'interruption) 

10/12/2020 :
-
-Etude sur le mode deepsleep pour fonctionnement du système complet sur ce mode 
-Modification du code de test du pluviometre en mode deepSleep 
-Aide pour réalisation du code de la girouette avec Lucile 
-Test de différent réveil de l'esp32 en mode deepSleep (interrupt, timer, entryLevel) 

17/12/2020 :
-
-Modification du code de deepsleep pour fonctionner avec timer+interruption (oui pendant 4h) 

08/01/2021 :
-
-Création d'un code de gestion du deepsleep avec une autre méthode car la méthode utilisé est obsolète 
-Création d'ordinogramme pour tout les codes de tests pour les intégrer dans le diaporama de présentation de mi parcours du projet 
-Création et modification du diaporama de présentation de mi parcours du projet 

29/01/2021 :
-
-Finition du code de test de la gestion du deepsleep avec une méthode non obsolete (variable time_t, timediff etc...) 
-Début des tests du module LoRa, le câblage à été revu et le code également, en fin de séance le module était initialisé (envoi testé pas reception) 
-Configuration du code de récéption de LoRa pour le module TTGO (LoRa externe pour test) 