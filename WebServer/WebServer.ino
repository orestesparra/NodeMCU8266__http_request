/*
      NodeMCU8266 server
      This server create a wifi connection where you can check the status of 4 buttons, throw http request. 
      Also, you can check and control the status throw a web portal. IP Address 192.168.4.1
      ////////////////////////
      Title: Halloween Project
      Developer: Orestes Parra (orestesparra.com)
      License: MIT
      Version: 1.0      
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "halloween"
#define APPSK  "scare1234"
#endif

#define btn1 5
#define btn2 4            
#define btn3 2
#define btn4 14

const char *ssid = APSSID;
const char *password = APPSK;

//IP Address 192.168.4.1
ESP8266WebServer server(80);
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>orestesparra.com</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
        * {
            margin: 0;
            padding: 0;
        }
        html {
            font-family: Helvetica;
            display: inline-block;
            margin: 0px auto;
            text-align: center;
        }
        body {
            background-color: whitesmoke;
            margin: 0;
            padding: 0;
        }
        header {
            background-color: orange;
            height: 10vh;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        main{
          display: flex;
          height: 75vh;
            justify-content: center;
        }
        footer {
            background-color: orange;
            height: 10vh;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .button {
            display: block;
            width: 150px;
            background-color: #1abc9c;
            border: 1px black solid;
            color: white;
            padding: 10px;
            text-decoration: none;
            font-size: 20px;
            cursor: pointer;
            border-radius: 40px;
        }
        .button-on {
            background-color: lightseagreen;
        }

        .button-on:active {
            background-color: lightseagreen;
        }

        .button-off {
            background-color: lightcoral;
        }

        .button-off:active {
            background-color: lightcoral;
        }
        
   </style>
  </head><body>
  <div>
        <header>
            <h1>Halloween Project</h1>
        </header>
        <main>
            <div>
                <div id="Disconnected" style="background-color: red;">Server Disconnected</div>
                <div style="display: flex; justify-content: center;align-items: center;margin-top: 15px;margin-bottom: 15px;">
                    <p style="margin-right: 5px;">Use website only</p>
                    <input type="checkbox" onclick="handleCheckBoxChange(event)">
                </div>

                <div id="ButtonsSection">
                    <div>
                      <h3>Click on the buttons to activate the respective output</h3>
                    </div>
                    <div style="display: flex; justify-content: center;align-items: center;margin-top: 15px;">
               
                        <div id="Output1On" style="display: block;">
                            <div style="margin-right: 10px;">
                                <p>Output 1 Status: ON</p>
                            </div>
                            <div>
                                <button class="button button-off" onclick="handleButtonClick('/output1off')">Turn OFF</button>
                            </div>
                        </div>

                        <div id="Output1Off" style="display: block;">
                            <div style="margin-right: 10px;">
                                <p>Output 1 Status: OFF</p>
                            </div>
                            <div>
                                <button class="button button-on" onclick="handleButtonClick('/output1on')">Turn ON</button>
                            </div>
                        </div>
                    </div>

                    <div style="display: flex; justify-content: center;align-items: center;margin-top: 15px;">
                        <div id="Output2On" style="display: block;">
                            <div style="margin-right: 10px;">
                                <p>Output 1 Status: ON</p>
                            </div>
                            <div>
                                <button class="button button-off" onclick="handleButtonClick('/output2off')">Turn OFF</button>
                            </div>
                        </div>

                        <div id="Output2Off" style="display: block;">
                            <div style="margin-right: 10px;">
                                <p>Output 2 Status: OFF</p>
                            </div>
                            <div>
                                <button class="button button-on" onclick="handleButtonClick('/output2on')">Turn ON</button>
                            </div>
                        </div>
                    </div>

                    <div style="display: flex; justify-content: center;align-items: center;margin-top: 15px;">
                        <div id="Output3On" style="display: block;">
                            <div style="margin-right: 10px;">
                                <p>Output 3 Status: ON</p>
                            </div>
                            <div>
                                <button class="button button-off" onclick="handleButtonClick('/output3off')">Turn OFF</button>
                            </div>
                        </div>

                        <div id="Output3Off" style="display: block;">
                            <div style="margin-right: 10px;">
                                <p>Output 3 Status: OFF</p>
                            </div>
                            <div>
                                <button class="button button-on" onclick="handleButtonClick('/output3on')">Turn ON</button>
                            </div>
                        </div>
                    </div>
                    
                    <div style="display: flex; justify-content: center;align-items: center;margin-top: 15px;">
                        <div id="Output4On" style="display: block;">
                            <div style="margin-right: 10px;">
                                <p>Output 4 Status: ON</p>
                            </div>
                            <div>
                                <button class="button button-off" onclick="handleButtonClick('/output4off')">Turn OFF</button>
                            </div>
                        </div>

                        <div id="Output4Off" style="display: block;">
                            <div style="margin-right: 10px;">
                                <p>Output 4 Status: OFF</p>
                            </div>
                            <div>
                                <button class="button button-on" onclick="handleButtonClick('/output4on')">Turn ON</button>
                            </div>
                        </div>
                    </div>
                    
                </div>
                
            </div>
        </main>
        <footer>
            <span>
              Developer: orestesparra.com
             </span>
        </footer>
    </div>
</body><script>
    let isButtonsActive = false;
    console.log("running the code")
    setInterval(function () {
      readButtons()
      if(isButtonsActive){
        document.getElementById("ButtonsSection").style.opacity = 1;
        document.getElementById("ButtonsSection").disabled = false;
      }else{
        document.getElementById("ButtonsSection").style.opacity = 0.2;
        document.getElementById("ButtonsSection").disabled = true;
        document.getElementById("Disconnected").style.display = 'none';      
      }
    }, 500);

    async function handleCheckBoxChange(event) {
        const checked = event.target.checked;
        await activateWebsite(checked);
    }
    
    async function readButtons() {
        await fetch('/buttons')
            .then(response => response.text())
            .then(data => {
                console.log(data)
                document.getElementById("Disconnected").style.display = 'none';
                if (data[0] === "1") {
                    document.getElementById("Output1On").style.display = '';
                    document.getElementById("Output1Off").style.display = 'none';
                } else {
                    document.getElementById("Output1On").style.display = 'none';
                    document.getElementById("Output1Off").style.display = '';
                }
                if (data[1] === "1") {
                    document.getElementById("Output2On").style.display = '';
                    document.getElementById("Output2Off").style.display = 'none';
                } else {
                    document.getElementById("Output2On").style.display = 'none';
                    document.getElementById("Output2Off").style.display = '';
                }
                if (data[2] === "1") {
                    document.getElementById("Output3On").style.display = '';
                    document.getElementById("Output3Off").style.display = 'none';
                } else {
                    document.getElementById("Output3On").style.display = 'none';
                    document.getElementById("Output3Off").style.display = '';
                }
                if (data[3] === "1") {
                    document.getElementById("Output4On").style.display = '';
                    document.getElementById("Output4Off").style.display = 'none';
                } else {
                    document.getElementById("Output4On").style.display = 'none';
                    document.getElementById("Output4Off").style.display = '';
                }
            }).catch(error=>{
                document.getElementById("Disconnected").style.display = '';
            });
    }
    
    async function activateWebsite(checked) {
        await fetch('/activateWeb')
            .then(response => response.text())
            .then(data => {
                console.log(data)
                isButtonsActive = checked; 
            }).catch(error=>{
                document.getElementById("Disconnected").style.display = '';
            });
    }
    async function handleButtonClick(path){
      if(isButtonsActive){
        await fetch(path)
            .then(response => response.text())
            .then(data => {
                console.log(data)
            }).catch(error=>{
                document.getElementById("Disconnected").style.display = '';
            });
      }
    }
</script></html>)rawliteral";
uint8_t output1 = 0;
uint8_t output2 = 0;
uint8_t output3 = 0;
uint8_t output4 = 0;
uint8_t activateWeb = 0;
void setup() {
  Serial.begin(115200);
  Serial.println(ssid);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);
  
  WiFi.softAP(ssid, password);
  delay(100);

  server.on("/", handleOnConnect);
  server.on("/buttons", handleReadButtons);
  server.on("/activateWeb", handleActivateWeb);
  server.on("/output1on", handleOutput1on);
  server.on("/output1off", handleOutput1off);
  server.on("/output2on", handleOutput2on);
  server.on("/output2off", handleOutput2off);
  server.on("/output3on", handleOutput3on);
  server.on("/output3off", handleOutput3off);
  server.on("/output4on", handleOutput4on);
  server.on("/output4off", handleOutput4off);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void loop() {
  server.handleClient();
}

void handleOnConnect() {
  Serial.println("New Connection");
  server.send(200, "text/html", index_html);
}

void handleNotFound() {
  server.send(404, "text/plain", "Page Not Found");
}

void readButtonsValues() {
  if (digitalRead(btn1) == LOW) output1 = 1;
  if (digitalRead(btn2) == LOW) output2 = 1;
  if (digitalRead(btn3) == LOW) output3 = 1;
  if (digitalRead(btn4) == LOW) output4 = 1;

  if (digitalRead(btn1) == HIGH) output1 = 0;
  if (digitalRead(btn2) == HIGH) output2 = 0;
  if (digitalRead(btn3) == HIGH) output3 = 0;
  if (digitalRead(btn4) == HIGH) output4 = 0;
}

void handleReadButtons() {
  Serial.println("Hardware Request");
  if (activateWeb == 0) {
    readButtonsValues();
  }
  String data = "";
  if (output1) {
    data += "1";
  } else {
    data += "0";
  }
  if (output2) {
    data += "1";
  } else {
    data += "0";
  }
  if (output3) {
    data += "1";
  } else {
    data += "0";
  }
  if (output4) {
    data += "1";
  } else {
    data += "0";
  }
  server.send(200, "text/plain", data);
  delay(10);
}

void handleActivateWeb() {
  if (activateWeb) {
    activateWeb = 0;
  }
  else {
    activateWeb = 1;
  }
  server.send(200, "text/plain", "Done");
}
void handleOutput1on() {
  output1 = 1;
  server.send(200, "text/plain", "Done");
}
void handleOutput1off() {
  output1 = 0;
  server.send(200, "text/plain", "Done");
}
void handleOutput2on() {
  output2 = 1;
  server.send(200, "text/plain", "Done");
}
void handleOutput2off() {
  output2 = 0;
  server.send(200, "text/plain", "Done");
}
void handleOutput3on() {
  output3 = 1;
  server.send(200, "text/plain", "Done");
}
void handleOutput3off() {
  output3 = 0;
  server.send(200, "text/plain", "Done");
}
void handleOutput4on() {
  output4 = 1;
  server.send(200, "text/plain", "Done");
}
void handleOutput4off() {
  output4 = 0;
  server.send(200, "text/plain", "Done");
}
