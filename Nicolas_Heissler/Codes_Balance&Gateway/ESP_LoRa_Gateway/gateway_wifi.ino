#include "gateway_wifi.h"

//=== WiFi =============================================================
void connectWifi() {
  char    t[] = {'-', '\\', '|', '/'};
  int8_t  idx = 0 ;
  int8_t  max_rssi_idx = -1 ;
  int32_t max_rssi = -1000 ;   // RFM95 sensitivity down to -144 dBm.
  int     scanned_network_nb = 0;
  int8_t  known_network_id = -1 ;
  int8_t  max_known_network_id = -1 ;

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  // wifi scan
  scanned_network_nb = WiFi.scanNetworks();

  if ( scanned_network_nb == 0 ) {
    DEBUGPRINTLN1("No networks found, restart in 10 s");
    oled_print(OC24, NO_WIFI1, NO_WIFI2, NO_WIFI3, "");
    delay(10000);
    ESP.restart();
  } else {
    DEBUGPRINT1("Networks found : ");DEBUGPRINTLN1(scanned_network_nb);
    for (idx = 0; idx < scanned_network_nb; idx++) {
      DEBUGPRINT1(idx); DEBUGPRINT1(" : "); DEBUGPRINT1(WiFi.SSID(idx)); DEBUGPRINT1(", "); DEBUGPRINT1(WiFi.RSSI(idx));
      DEBUGPRINT1(" dB, "); DEBUGPRINT1(WiFi.BSSIDstr(idx)); DEBUGPRINT1(", channel:"); DEBUGPRINT1(WiFi.channel(idx));
      DEBUGPRINTLN1((WiFi.encryptionType(idx) == WIFI_AUTH_OPEN)?" ":" *");
      known_network_id = is_known(WiFi.SSID(idx));
      if ( WiFi.RSSI(idx) > max_rssi && known_network_id > -1 ) {
        max_rssi_idx = idx ;
        max_rssi = WiFi.RSSI(idx);
        max_known_network_id = known_network_id;
      }
    }
  }

  if ( max_known_network_id < 0 ) { // no known network 
    ble_settings_wifi();
  } else { // connect to the better network
    if ( !connect_to_network(max_known_network_id) ) {
      DEBUGPRINTLN1("");
      DEBUGPRINTLN1("WiFi not connected, reboot");
      oled_print(OB16, "WiFi connect", ssids[max_known_network_id], "KO", NO_WIFI3);
      delay(6000);
      if (verbose)
        send_wifi_connect_mess(ssids[max_known_network_id],WIFI_FAILED);
      ESP.restart();
    }
    oled_print(OC16, "WiFi OK", (String)WiFi.localIP()[0] + "." + (String)WiFi.localIP()[1] + "." +
      (String)WiFi.localIP()[2] + "." + (String)WiFi.localIP()[3], WiFi.macAddress(), "RSSI " + (String)WiFi.RSSI() + " dBm");
    delay(AFF_TIME);
    Serial.println("========================");
    if (verbose)
      send_wifi_connect_mess(ssids[max_known_network_id],WIFI_CONNECTED);
  }
}

int8_t connect_to_network(int8_t  network_id) { // return 1 if connected, 0 if not connected after intervalWiFi
  char    t[] = {'-', '\\', '|', '/'};
  int8_t  idx;

  DEBUGPRINT1("Connecting to : "); DEBUGPRINTLN1(ssids[network_id]); 
  //DEBUGPRINTLN1(passwds[network_id]); 
  WiFi.begin(ssids[network_id], passwds[network_id]);
  previousTime = millis();
  currentTime = previousTime;
  idx = 0 ;
  while (WiFi.status() != WL_CONNECTED) {
    oled_print(OB16, "WiFi connect", ssids[network_id], (String)t[idx % 4], "");
    currentTime = millis();
    delay(300);
    DEBUGPRINT1(".");
    idx++;
    if (idx > 50) {
      idx = 0;
    }
    if (currentTime - previousTime > intervalWiFi) {
      return 0;
    }
  }
  DEBUGPRINTLN1("");
  DEBUGPRINTLN1("WiFi connected");
  DEBUGPRINT1("IP address: "); DEBUGPRINTLN1(WiFi.localIP());
  DEBUGPRINT1("signal strength (RSSI):"); DEBUGPRINT1(WiFi.RSSI()); DEBUGPRINTLN1(" dBm");
  WiFi.macAddress(mac_gateway);
  DEBUGPRINT1("gateway mac Addresss : "); DEBUGPRINTLN1(WiFi.macAddress());
  //  if ( debug ) {
  //    Serial.setDebugOutput(true);
  //    Serial.println("=== WiFi.printDiag : ===");
  //    WiFi.printDiag(Serial);
  //  }
  return 1;
}

//===  =============================================================
int8_t is_known( String ssid) {
  int8_t idx = 0 ;

  for (idx = 0; idx < network_nb; idx++) {
    if ( ssid == ssids[idx] ) {
      //DEBUGPRINT3("known network found : "); DEBUGPRINT1(ssids[idx]); DEBUGPRINT3(" (");  DEBUGPRINT1(idx); DEBUGPRINTLN3(")"); 
      return idx;
    }
  }
  return -1;
}

//===  =============================================================
void disconnectWiFi() {
  WiFi.disconnect(true);
}
