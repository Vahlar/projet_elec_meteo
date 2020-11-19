#ifndef gateway_language_h
#define gateway_language_h

#if ( LANGUAGE == 'F') 
 #define INIT_MSG1 "Gateway"
 #define INIT_MSG2 "rucher"

 #define INIT_SYS1 "RESET"
 #define INIT_SYS2 "GLOBAL"
 #define INIT_SYS3 "(EEPROM)"


// #define INFO1 "BATTERIE"
// #define INFO2 " V"
// #define INFO3 " %"

 #define NO_WIFI1  "Pas de réseau"
 #define NO_WIFI2  "WiFi"
 #define NO_WIFI3  "Redémarrage"

 #define CHOOSE "choisir avec (->)"
 #define VALID  "(<-) puis validez"
 
 #define BLE_SERVER  "Gateway"
 #define BLE_INVIT1 "press PROG btn"
 #define BLE_INVIT2 "pour entrer"
 #define BLE_INVIT3 "en mode"
 #define SETTING "REGLAGES"
 #define SAVED   "ENREGISTRES"

 #define BLE_CONNECT1 "Connection au"
 #define BLE_CONNECT2 "serveur bluetooth"
 #define BLE_CONNECT3 "attendue"
 #define BLE_CONNECT4 "établie"

 #define GENERAL  "Général"
 #define LORA     "LoRa"
 #define WIFI     "WiFi"
 #define RST_EEPROM "Init EEPROM"
 #define CHOICE    "choix:"

 #define STRING_TO_LONG "trop long"
 
 #define LR_TXPOWER "Puissance (dB)"
 #define LR_FREQ    "Fréquence (Mhz)"
 #define LR_SF      "SF (portée)"
 #define LR_SBW     "SBW (kHz)"
//
// #define MODE "Mode ...."
// #define CHOICES "Gén. LoRa Cal."
// const char *choice[] = {"LoRa","Calibration","Général"};



 #define LIST_NETWORK   "Lister les réseaux enregistrés"
 #define ADD_NETWORK    "Ajouter un réseau"
 #define DELETE_NETWORK "Supprimer un réseau"
 #define SEARCH_NETWORK "Rechercher les réesaux disponibles"
 #define EXIT           "Quitter"
 #define NO_KNOWN_NETWORK "pas de réseaux enregistrés"
 #define NO_NETWORK_FOUND "pas de réseaux disponibles"
 #define NETWORKS         " réseaux : "
 #define CHOOSE_NETWORK   "Sélectionnez un réseau : "

 #define TOO_MUCH_NETWORKS "Nombre maxi de réseaux enregistrés atteint,\n supprimez un réseau pour en enregistrer un nouveau"
 #define ENTER_SSID        "ssid ?"
 #define ENTER_PASSWD      "Mot de passe"

 #define NETWORKS_TO_DEL_ID "Réseau à supprimer ('q' pour quitter')"

 #define CONNECT_NETWORKS_FAILED "Erreur de connection au réseau "
 #define CONNECT_NETWORKS_OK "Réseau enregistré : "

 #define WEB_HTACCESS "web (htaccess)"
 #define ENTER_LOGIN  "web login ?"
 #define ENTER_WEB_SERVER  "web server ?\n168.192.0.1/path/bees/\nxxx.xxxx.xxx/path/bees/\nxxx.xxxx.xxx:80/path/bees"

 #define GO_TO_SLEEP "Mise en sommeil pour "
 #define GO_TO_SLEEP1 "Mise en"
 #define GO_TO_SLEEP2 "sommeil"
 #define MN " mn"
 #define MINUTES " minutes"
  
 #define LOGOUT "Déconnexion"

 #define NO_SENSOR  "Pas de capteur"  // max length 20
 #define DATA_OK    "Données OK"
 #define DATA_KO    "Données KO"
 #define HIVE  "Ruche"
 
#else /* LANGAGE */
 // the same in GB

#endif /* LANGAGE */



#endif /* gateway_language_h */
