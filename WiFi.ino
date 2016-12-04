#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "bangul";
const char *password = "qwertyuiop";

ESP8266WebServer server(80);

void handleRoot() {
  server.send ( 200, "text/html", "root" );
}

void setup ( void ) {
  Serial.begin ( 9600 );
  equalizerSerial.begin(9600);
  if (!SD.begin(4)) {
    return;
  }
  startWiFi();
}

void startWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
      Serial.print ( "." );
      delay ( 500 );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  server.on ( "/", handleRoot );
  server.on("/play", []()
  {
    server.send(200, "text/html", "play");
    playMusic(server.arg(0)[0]);
  });

  server.on("/stop", []()
  {
    server.send(200, "text/html", "stop");
    isPlaying = false;
  });
  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop ( void ) {
  server.handleClient();
  sendNextLyric();  
}
