// Copyright Nat Weerawan 2015-2016
// MIT License

#include <ESP8266WiFi.h>
#include <WiFiConnector.h>
#include <ESP8266WebServer.h>
#include "FS.h"

/*
  WIFI INFO
  DELETE ALL IF YOU WANT TO USE FULL FUNCTION OF SMARTCONFIG
*/

#define WIFI_SSID        "Opendream"
#define WIFI_PASSPHARSE  "gfkgvkgv'2016!!!!"


WiFiConnector wifi = WiFiConnector(WIFI_SSID, WIFI_PASSPHARSE);

ESP8266WebServer server(80);


void fail(const char* msg) {
  Serial.println(msg);
  while (true) {
    yield();
  }
}

void init_hardware()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("BEGIN");


  if (!SPIFFS.begin()) {
    fail("SPIFFS init failed");
  }

  // SETUP CALLBACKS
  wifi.on_connecting([&](const void* message)
  {
    char buffer[70];
    sprintf(buffer, "[%d] connecting -> %s ", wifi.counter, (char*) message);
    Serial.println(buffer);
    delay(500);
  });

  wifi.on_connected([&](const void* message)
  {
    // Print the IP address
    Serial.print("WIFI CONNECTED => ");
    Serial.println(WiFi.localIP());

    Dir root = SPIFFS.openDir("/");
    Serial.println("READING ROOT");
    while (root.next()) {
      String fileName = root.fileName();
      File f = root.openFile("r");
      Serial.printf("%s: %d\r\n", fileName.c_str(), f.size());
    }

    server.on ( "/inline", []() {
      server.send ( 200, "text/plain", "this works as well" );
    } );
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
  });

}

void setup()
{
  init_hardware();

  Serial.print("CONNECTING TO ");
  Serial.println(wifi.SSID() + ", " + wifi.psk());

  wifi.connect();

}

void loop()
{
  wifi.loop();
  server.handleClient();
}
