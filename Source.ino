// Change the pin numbers according to your needs //
// Coded by Morsine //
// feel free to edit anything you want //
#include <SPI.h> 
#include <Ethernet.h>
#include <Wire.h>
#define ONE_WIRE_BUS 2 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address 
byte ip[] = { 192, 168, 33, 90 }; // IP address in LAN – need to change according to your Network address 
byte gateway[] = { 192, 168, 33, 1 }; // internet access via router 
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask 
EthernetServer server(80); //server port. 80 is default.
String readString; 
int R1 = 9; 
int R2 = 6;

void setup(){
     pinMode(R1, OUTPUT);
     pinMode(R2, OUTPUT);
     digitalWrite(R1, LOW);
     digitalWrite(R2, LOW);
     Ethernet.begin(mac, ip, gateway, subnet); 
     server.begin(); 

    
}

void loop(){ 

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
                    client.println("<TITLE>PMS v2</TITLE>"); 
                    client.println("</HEAD>"); 
                    client.println("<BODY>"); 
                    client.println("<hr>"); 
                    client.println("<hr>"); 
                    client.println("<br>"); 
                    client.println("<H1 style=\"color:red;\">PMS-v2</H1>"); 
                    client.println("<hr>"); 
                    client.println("<br>");

            
                    client.println("<H2><a href=\"/?R1ON\"\">Turn On R1</a><br></H2>"); 
                    client.println("<H2><a href=\"/?R1OFF\"\">Turn Off R1</a><br></H2>");
                    client.println("<H2><a href=\"/?R2ON\"\">Turn On R2</a><br></H2>"); 
                    client.println("<H2><a href=\"/?R2OFF\"\">Turn Off R2</a><br></H2>");
                    client.println("</BODY>"); 
                    client.println("</HTML>");

                    delay(10); 
                    //stopping client 
                    client.stop();

                    // control arduino pin 
                    if(readString.indexOf("?R1ON") > -1) //checks for LEDON 
                    { 
                        digitalWrite(R1, HIGH); // set pin low 
                    } 
                    if(readString.indexOf("?R1OFF") > -1) //checks for LEDOFF 
                    { 
                        digitalWrite(R1, LOW); // set pin high 
                    } 
                    if(readString.indexOf("?R2ON") > -1) //checks for LEDON 
                    { 
                        digitalWrite(R2, HIGH); // set pin low 
                    } 
                    if(readString.indexOf("?R2OFF") > -1) //checks for LEDOFF 
                    { 
                        digitalWrite(R2, LOW); // set pin high 
                    }
                    //clearing string for next read 
                    readString="";

                } 
            } 
        } 
    } 
}
