
#ifndef gateway_http_h
#define gateway_http_h

#define WEB_SERVER_MAX_LEN 99
#define WEB_URL_MAX_LEN    32

//Your Domain name with path or IP address with path
const char  http_str[] = "http://"; // http:// or https://
char        web_server[WEB_SERVER_MAX_LEN];
const char  add_datas_url[WEB_URL_MAX_LEN]   = "pri/ins/add_datas.php"; // php script to insert datas in the database.
const char  reboot_mess_url[WEB_URL_MAX_LEN] = "pri/ins/reboot_mess.php"; // php script to insert datas in the database.
const char  wifi_mess_url[WEB_URL_MAX_LEN]   = "pri/ins/wifi_mess.php"; // php script to insert datas in the database.


#endif /* gateway_http_h */
