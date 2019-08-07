#include <SPI.h> 
#include <Ethernet.h>
#include <Wire.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address 
byte ip[] = { 192, 168, 88, 99 }; // IP address in LAN – need to change according to your Network address 
byte gateway[] = { 192, 168, 88, 1 }; // internet access via router 
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask 
EthernetServer server(80); //server port
String readString; 
int R2811 = 9;
int acnbackfan = 8;
int frontfan = 7;
int lights = 6;


const int trigPin = 12;
const int echoPin = 11;
long reply, cm, inch;
    OneWire oneWire(ONE_WIRE_BUS); 
    DallasTemperature sensors(&oneWire);
    
void setup(){
    pinMode(trigPin, OUTPUT); // Prepare the trigger pin
    pinMode(echoPin, INPUT); // Prepare the echo pin
    sensors.begin(); 
    pinMode(R2811, OUTPUT);
    pinMode(acnbackfan, OUTPUT);
    pinMode(frontfan, OUTPUT);
    pinMode(lights, OUTPUT);
    digitalWrite(R2811, HIGH);
    digitalWrite(acnbackfan, HIGH);
    digitalWrite(frontfan, HIGH);
    digitalWrite(lights, HIGH);
    Ethernet.begin(mac, ip, gateway, subnet); 
    server.begin(); 

    
}

void loop(){ 

//time2Distance(reply);
    // Create a client connection 
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
    EthernetClient client = server.available(); 
    if (client) { 
        while (client.connected()) { 
            if (client.available()) { 
                char c = client.read();

                //read char by char HTTP request 
                if (readString.length() < 100) {

                    //store characters to string 
                    readString += c; 
                }

                //if HTTP request has ended– 0x0D is Carriage Return \n ASCII 
                if (c == 0x0D) { 
                    client.println("HTTP/1.1 200 OK"); //send new page 
                    client.println("Content-Type: text/html"); 
                    client.println();

                    client.println("<HTML>"); 
                    client.println("<HEAD>"); 
                    client.println("<TITLE>UiLO</TITLE>"); 
                    client.println("</HEAD>"); 
                    client.println("<BODY>"); 
                    client.println("<hr>"); 
                    client.println("<hr>"); 
                    client.println("<br>"); 
                    client.println("<H1 style=\"color:green;\">UNINTEGRATED LIGHTS OUT (UiLO)</H1>"); 
                    client.println("<H2 style=\"color:red;\">By M3NI</H2>");
                    client.println("<hr>"); 
                    client.println("<br>");

            
                    client.println("<H2><a href=\"/?2811ON\"\">Turn On 2811 Router</a><br></H2>"); 
                    client.println("<H2><a href=\"/?2811OFF\"\">Turn Off 2811 Router</a><br></H2>");

                    client.println("</BODY>"); 
                    client.println("</HTML>");

                    delay(10); 
                    //stopping client 
                    client.stop();

                    // control arduino pin 
                    if(readString.indexOf("?2811ON") > -1) //checks for LEDON 
                    { 
                        digitalWrite(R2811, LOW); // set pin low 
                    } 
                    else{ 
                        if(readString.indexOf("?2811OFF") > -1) //checks for LEDOFF 
                        { 
                            digitalWrite(R2811, HIGH); // set pin high 
                        } 
                    } 
                    //clearing string for next read 
                    readString="";

                } 
            } 
        } 
    } 
}
