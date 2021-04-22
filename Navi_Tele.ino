#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "Marilu Moraes";
const char* senha = "31197618";

void setup() {
  Serial.begin(115200);
  Serial.println("Boot");
  
  WiFi.mode(WIFI_STA); //definindo unidade como estação
  WiFi.begin(ssid, senha);

  //Tratamento para erro de conexão
  while (WiFi.waitForConnectResult() != WL_CONNECTED){
    if (WiFi.status() == WL_NO_SSID_AVAIL){
      Serial.println("Rede não encontrada! Verifique ponto de rede ou senha");
    }

    if (WiFi.status() == WL_CONNECTION_LOST){
      Serial.println("Conexão perdida temporariamente");
    }

    if (WiFi.status() == WL_IDLE_STATUS){
      Serial.println("Impossível conectar");
    }
    
    Serial.println("Conexão Falhou! Aguardando Boot");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  //Definindo FOTA
  ArduinoOTA.onStart([](){Serial.println("Iniciando FOTA");});
  ArduinoOTA.onEnd([](){Serial.println("Finalizando FOTA");});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
    Serial.printf("Progresso: %u%%r", (progress/(total/100)));
  });
  ArduinoOTA.onError([](ota_error_t error){Serial.printf("Erro [%u]: ", error);
  if (error == OTA_BEGIN_ERROR){
    Serial.println("Autenticação falhou");
  } else if (error == OTA_BEGIN_ERROR){
    Serial.println("Inicialização falhou");
  } else if (error == OTA_CONNECT_ERROR){
    Serial.println("Falha de conexão");
  } else if (error == OTA_RECEIVE_ERROR){
    Serial.println("Falha de recepção");
  } else if (error == OTA_END_ERROR){
    Serial.println("Falha na finalização");
  } else {
    Serial.println("Erro desconhecido");
  }
  });

  //Inicializando FOTA
  ArduinoOTA.begin();
  Serial.println("Pronto para FOTA");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  ArduinoOTA.handle();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
