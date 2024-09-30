#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

// Configurações - variáveis editáveis
const char* default_SSID = "Wokwi-GUEST"; 
const char* default_PASSWORD = ""; 
const char* default_BROKER_MQTT = "18.209.40.125"; 
const int default_BROKER_PORT = 1883; 
const char* default_TOPICO_SUBSCRIBE = "/TEF/lamp001/cmd"; 
const char* default_TOPICO_PUBLISH_1 = "/TEF/lamp001/attrs"; 
const char* default_TOPICO_PUBLISH_2 = "/TEF/lamp001/attrs/l"; 
const char* default_ID_MQTT = "fiware_001"; 
const int default_D4 = 2; 

int lcdColums = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColums, lcdRows);
#define BUTTON_PIN 19
int lastButtonState = HIGH; // Estado anterior do botão
int currentButtonState;

const char* topicPrefix = "lamp001";
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
int D4 = default_D4;

WiFiClient espClient;
PubSubClient MQTT(espClient);
char EstadoSaida = '0';

// Variável global para armazenar a mensagem recebida
String mensagemRecebida = ""; // Inicialmente vazia

// Estrutura para armazenar as informações dos pilotos
struct Piloto {
  String nome;
  int volta;
  int posicao;
};

// Array de pilotos
Piloto pilotos[10] = {
  {"Ayrton Senna", 1, 8},
  {"Michael Schumacher", 2, 1},
  {"Lewis Hamilton", 3, 3},
  {"Alain Prost", 1, 5},
  {"Nelson Piquet", 2, 2},
  {"Sebastian Vettel", 3, 4},
  {"Fernando Alonso", 1, 6},
  {"Niki Lauda", 2, 7},
  {"James Hunt", 3, 9},
  {"Kimi Raikkonen", 1, 10}
};

int pilotoIndex = 0; // Índice do piloto atual

void initSerial() {
  Serial.begin(115200);
}

void initWiFi() {
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");
  reconectWiFi();
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  InitOutput();
  initSerial();
  initWiFi();
  initMQTT();
  lcd.init();
  lcd.backlight();
  delay(5000);
  MQTT.publish(TOPICO_PUBLISH_1, "s|on");
  currentButtonState = digitalRead(BUTTON_PIN);
}

void loop() {
  VerificaConexoesWiFIEMQTT();
  EnviaEstadoOutputMQTT();
  handleLuminosity();
  MQTT.loop();

  // Exibe a mensagem recebida no LCD se houver uma nova mensagem
  if (mensagemRecebida != "") {
    deslizarMensagem(1, mensagemRecebida, 250);
    mensagemRecebida = ""; // Limpa a mensagem após exibi-la
  }

  // Lê o estado atual do botão
  currentButtonState = digitalRead(BUTTON_PIN);

  // Verifica se o botão foi pressionado
  if (lastButtonState == HIGH) {
    mostrarProximoPiloto();
  }

  // Atualiza o estado do botão
  lastButtonState = currentButtonState;
}

void mostrarProximoPiloto() {
  // Exibe o piloto atual no LCD
  String mensagem = "Volta " + String(pilotos[pilotoIndex].volta) + " - " +
                    pilotos[pilotoIndex].nome + " - Posicao " +
                    String(pilotos[pilotoIndex].posicao);
  deslizarMensagem(1, mensagem, 250);

  // Avança para o próximo piloto, voltando ao início se passar do último
  pilotoIndex = (pilotoIndex + 1) % 10;
}

void deslizarMensagem(int row, String postmanMsg, int delayTime) {
  int larguraLcd = 16;

  for (int i = 0; i < larguraLcd; i++) {
    postmanMsg = " " + postmanMsg;
  }

  postmanMsg = postmanMsg + " ";

  for (int pos = 0; pos < postmanMsg.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(postmanMsg.substring(pos, pos + lcdColums));
    delay(delayTime);
  }
}

void reconectWiFi() {
  if (WiFi.status() == WL_CONNECTED)
    return;
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("IP obtido: ");
  Serial.println(WiFi.localIP());

  digitalWrite(D4, LOW);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
  }
  Serial.print("- Mensagem recebida: ");
  Serial.println(msg);

  // Armazena a mensagem recebida na variável global para ser exibida no LCD
  mensagemRecebida = msg;

  // Exemplo de ação com a mensagem recebida (pode ser ajustado conforme necessário)
  if (msg == "s|on") {
    EstadoSaida = '1';
    digitalWrite(D4, LOW); // Liga o LED
  } else if (msg == "s|off") {
    EstadoSaida = '0';
    digitalWrite(D4, HIGH); // Desliga o LED
  }
}

void VerificaConexoesWiFIEMQTT() {
  if (!MQTT.connected())
    reconnectMQTT();
  reconectWiFi();
}

void EnviaEstadoOutputMQTT() {
  if (EstadoSaida == '1') {
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
    Serial.println("- Led Ligado");
  }

  if (EstadoSaida == '0') {
    MQTT.publish(TOPICO_PUBLISH_1, "s|off");
    Serial.println("- Led Desligado");
  }
  Serial.println("- Estado do LED onboard enviado ao broker!");
  delay(1000);
}

void InitOutput() {
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);
  boolean toggle = false;

  for (int i = 0; i <= 10; i++) {
    toggle = !toggle;
    digitalWrite(D4, toggle);
    delay(200);
  }
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPICO_SUBSCRIBE);
    } else {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Haverá nova tentativa de conexão em 2s");
      delay(2000);
    }
  }
}

void handleLuminosity() {
  const int potPin = 34;
  int sensorValue = analogRead(potPin);
  int luminosity = map(sensorValue, 0, 4095, 0, 100);
  String mensagem = String(luminosity);
  Serial.print("Valor da luminosidade: ");
  Serial.println(mensagem.c_str());
  MQTT.publish(TOPICO_PUBLISH_2, mensagem.c_str());
}
