Séance du 10/09 :  
	- Choix du projet  
	- Prise en main de git et du push  
	- Lecture du document qui a été effectué les années précédentes  
	- Analyse des données présentent dans le rapport des années précédentes  
	
Séance du 25/09 :  
	- Rédaction du cahier des charges  
	- Lecture des datasheets    
	- Branchement du BME 280  
	- Branchement du FTDI 232  
	- Câblage du montage pour téléverser sur l'ESP 32  
	
Séance du 08/10 :  
	- Modifiaction du câblage pour le montage de téléversement  
	- Récupération du schéma sur Eagle  
	- Test des capteurs BME 280 et TSL2591  
	- Prise en main du logiciel Eagle sur la partie routage  

22/10/2020 :  
-Branchement du pluviometre sur la breadBoard  
-Branchement d'un module LoRa sur la breadBoard  
-Analyse de la datasheet du microcontrôleur pour savoir quelles pin permettent de faire du SPI  
-Problème avec le module de téléversement  

19/11/2020 :  
-Rectification du câblage, pour réactivé le téléversement  
-Vérification que le pluviometre fonctionne / test de continuité  
-Modification du câblage du module LoRa (le branchement n'était pas bon nous utilisions les modules SPI interne)  
-Test de téléversement pour le module LoRa (Fonctionnel)  
	
26/11/2020 :  
-Vérification du bon fonctionnement de l'anémomètre  
-Test de l'addaptateur (il ne fonctionne pas, il y a un problème de soudure)  
-Le code de l'anémomètre fonctionne avec le bouton poussoir  
-Nous avons repérer un problème de rebond (mise en place d'une capa de 220 PF en parrallèle du switch)  

03/12/2020 :  
-Test du code de l'anémomètre en rajoutant une capacité pour éviter les rebonds (vérification à l'oscilloscope, il n'y a pas de rebond)  
-Test du code de l'anémomètre avec un GBF qui symbolyse les intérruptions (le code fonctionne)  
-L'anémomètre ne semble pas fonctionner nous avons toujours des problèmes avec les intérruptions( valeur irrégulière)  
-Soudure d'un nouvel addaptateur (en attendant le nouvel anémomètre)  

04/12/2020 :  
-Rajout d'une condition pour empêcher les multiples intéruptions sur un seul front montant  
-Câblage de la girouette (girouette pas encore fonctionnel problème de code)  
-Test de la girouette avec un voltmètre (la girouette fonctionne)  
-test de la fonction deepsleep avec le pluviomètre (pas encore fonctionnel)  

10/12/2020 :  
-Prise en main du logiciel Eagle pour la gestion de schéma et de pcb  
-Recherche des librairies qui ont été utilisé par le groupe précedent (il faut avoir exactement les mêmes librairies)  
-Analyse du PCB pour voir s'il est possible de rajouter les composants qui nous manque (problème de place)  
-Mise en place de la première ébauche de la fonction sleep sur le pluviomètre (ça ne fonctionne pas pour le momment)  


17/12/2020 :  
-Modification du schéma initial sur Eagle    
-rajout des capteurs sur Eagle    

