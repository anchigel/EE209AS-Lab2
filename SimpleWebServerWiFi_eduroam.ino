/*
  WiFi Web Server LED Blink
 
 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 9.
 
 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off
 
 This example is written for a network using WPA encryption. For 
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 Circuit:
 * WiFi shield attached
 * LED attached to pin 9
 
 created 25 Nov 2012
 by Tom Igoe
 */
#include <SPI.h>
#include <WiFi.h>
#include <Servo.h> 

char ssid[] = "yourNetwork";      //  your network SSID (name) 
char pass[] = "secretPassword";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(50);

Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 

int pos = 0;    // variable to store the servo position 

void setup() {
  Serial.begin(9600);      // initialize serial communication
  //pinMode(13, OUTPUT);      // set the LED pin mode
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object 
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    while(true);        // don't continue
  } 

  String fv = WiFi.firmwareVersion();
  if( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    //status = WiFi.begin(ssid, pass);
    status = WL_CONNECTED;
    // wait 5 seconds for connection:
    delay(5000);
  } 
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  int times = 0;
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {  
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:    
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
			client.print("Servo Controls:<br>");
            client.print("Click <a href=\"/CCW\">here</a> to turn servo ccw<br>");
            client.print("Click <a href=\"/CW\">here</a> to turn servo cw<br>");
            client.print("Click <a href=\"/F180\">here</a> to rotate servo from current position to 180 and 180 to 0, ten times<br>");
            client.print("Click <a href=\"/F90\">here</a> to rotate servo from current position to 90 and 90 to 0, ten times<br>");
			client.print("Click <a href=\"/CCW2\">here</a> to turn servo ccw to 90 and back 45<br>");
            client.print("Click <a href=\"/CW2\">here</a> to turn servo cw to 0 and back 45<br>");
			client.print("Click <a href=\"/SEQ1\">here</a> for sequence 1<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;         
          } 
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }     
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Determine client request
        if (currentLine.endsWith("GET /CCW")) {
          //digitalWrite(13, HIGH);               // GET /H turns the LED on
          if(pos < 90) {
             for(pos = pos; pos < 90; pos += 1)  
            {                                  
              myservo.write(pos);              
              delay(5);                       
            } 
          }
        }
        if (currentLine.endsWith("GET /CW")) {
          //digitalWrite(13, LOW);                // GET /L turns the LED off
          if(pos > 1) {
            for(pos = pos; pos>=1; pos-=1)    
            {                                
              myservo.write(pos);             
              delay(5);                     
            } 
          } 
        }
        if(currentLine.endsWith("GET /F180")) {
            int loops = 0;
            times = 10;
            while(loops < times) {
				if(pos < 180) {
					for(pos = pos; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
					{                                  // in steps of 1 degree 
					  myservo.write(pos);              // tell servo to go to position in variable 'pos' 
					  delay(2);                       // waits 2ms for the servo to reach the position 
					} 
				}
                for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
                {                                
                  myservo.write(pos);              // tell servo to go to position in variable 'pos' 
                  delay(2);                       // waits 2ms for the servo to reach the position 
                } 
                loops += 1;
			}
        }
        if(currentLine.endsWith("GET /F90")) {
            int loops = 0;
            times = 10;
            while(loops < times) {
				if(pos < 90) {
					for(pos = 0; pos < 90; pos += 1)  
					{                                
					  myservo.write(pos);              
					  delay(3);                       
					} 
				}
                for(pos = 90; pos>=1; pos-=1)     
                {                                
                  myservo.write(pos);               
                  delay(3);                       
                } 
                loops += 1;
			}
        }
		if (currentLine.endsWith("GET /CCW2")) {
          if(pos < 90) {
            for(pos = pos; pos < 90; pos += 1)  
            {                                  
              myservo.write(pos);              
              delay(2);                       
            } 
		  }
		  for(pos = pos; pos > 45; pos -= 1)  
			{                                  
				myservo.write(pos);              
				delay(2);                       
			}
        }
        if (currentLine.endsWith("GET /CW2")) {
          if(pos > 1) {
            for(pos = pos; pos>=1; pos-=1)    
            {                                
              myservo.write(pos);             
              delay(2);                     
            } 
		  }
		  for(pos = pos; pos < 45; pos+=1)    
			{                                
				myservo.write(pos);             
				delay(2);                     
			} 
        }
		if (currentLine.endsWith("GET /SEQ1")) {
			for(int j = 0; j < 10; j++){
				for(int i = 0; i < 2; i++) {
					if(pos < 90) {
						for(pos = pos; pos < 90; pos += 1)  
						{                                  
						  myservo.write(pos);              
						  delay(4);                       
						} 
					}
					for(pos = pos; pos > 45; pos -= 1)  
					{                                  
						myservo.write(pos);              
						delay(4);                       
					}
				}
				for(int i = 0; i < 2; i++) {
					if(pos > 1) {
					for(pos = pos; pos>=1; pos-=1)    
						{                                
						  myservo.write(pos);             
						  delay(4);                     
						} 
					}
					for(pos = pos; pos < 45; pos+=1)    
					{                                
						myservo.write(pos);             
						delay(4);                     
					} 
				}
			}
		}
		
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
