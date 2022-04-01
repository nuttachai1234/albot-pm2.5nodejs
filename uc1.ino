#include <WiFi.h>


const char* WIFI_NAME = "vivo 1718";
const char* WIFI_PASSWORD = "1234567890";

WiFiServer server(80);

String header;

String LED_ONE_STATE = "off";
String LED_TWO_STATE = "off";


const int GPIO_PIN_NUMBER_12 = 22;
const int GPIO_PIN_NUMBER_13 = 23;


void setup() {
Serial.begin(115200);
pinMode(GPIO_PIN_NUMBER_12, OUTPUT);
pinMode(GPIO_PIN_NUMBER_13, OUTPUT);

digitalWrite(GPIO_PIN_NUMBER_12, LOW);
digitalWrite(GPIO_PIN_NUMBER_13, LOW);


Serial.print("Connecting to ");
Serial.println(WIFI_NAME);
WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print("Trying to connect to Wifi Network");
}
Serial.println("");
Serial.println("Successfully connected to WiFi network");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();
}

void loop() {
WiFiClient client = server.available();
if (client) {
Serial.println("New Client is requesting web page");
String current_data_line = "";
while (client.connected()) {
if (client.available()) {
char new_byte = client.read();
Serial.write(new_byte);
header += new_byte;
if (new_byte == '\n') {

if (current_data_line.length() == 0)
{

client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();

if (header.indexOf("G-LED=ON") != -1)
{
Serial.println("GPIO13 LED is ON");
LED_ONE_STATE = "on";
digitalWrite(GPIO_PIN_NUMBER_13, HIGH);
}
if (header.indexOf("G-LED=OFF") != -1)
{
Serial.println("GPIO13 LED is OFF");
LED_ONE_STATE = "off";
digitalWrite(GPIO_PIN_NUMBER_13, LOW);
}
if (header.indexOf("Y-LED=ON") != -1)
{
Serial.println("GPIO12 LED is ON");
LED_TWO_STATE = "on";
digitalWrite(GPIO_PIN_NUMBER_12, HIGH);
}
if (header.indexOf("Y-LED=OFF") != -1)
{
Serial.println("GPIO12 LED is OFF");
LED_TWO_STATE = "off";
digitalWrite(GPIO_PIN_NUMBER_12, LOW);
}

client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
client.println(".button { background-color: #4CAF50; border: 2px solid #4CAF50;; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
// Web Page Heading
client.println("</style></head>");
client.println("<body><center><h1>ESP32 Web Server Control</h1></center>");
client.println("<form><center>");
client.println("<p>Light is " + LED_ONE_STATE + "</p>");
// If the PIN_NUMBER_xx State is off, it displays the ON button
client.println("<center> <button class=\"button\" name=\"G-LED\" value=\"ON\" type=\"submit\">G-LED ON</button>") ;
client.println("<button class=\"button\" name=\"G-LED\" value=\"OFF\" type=\"submit\">G-LED OFF</button><br><br>");
client.println("<p>Fan is " + LED_TWO_STATE + "</p>");
client.println("<button class=\"button\" name=\"Y-LED\" value=\"ON\" type=\"submit\">Y-LED ON</button>");
client.println("<button class=\"button\" name=\"Y-LED\" value=\"OFF\" type=\"submit\">Y-LED OFF</button> <br><br>");
client.println("</center></form></body></html>");
client.println();
break;
}
else
{
current_data_line = "";
}
}
else if (new_byte != '\r')
{
current_data_line += new_byte;
}
}
}
// Clear the header variable
header = "";
// Close the connection
client.stop();
Serial.println("Client disconnected.");
Serial.println("");
}
}
