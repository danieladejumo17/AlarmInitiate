//NODEMCU -NRF24L01
//D2       CSN
//D4       CE
//3.3      V+
//GND      GND
//D7       MOSI
//D6       MISO
//D5       CSK o SCK

// BUG: Routecallbacks still call their callbacks, irrespective of their middleware's result.
// TODO: Chnge implementattion to store char* for route/path instead of String. strcmp can be used instead of ==

// FOR WIFI FUNCTIONALITIES
#include "ArduinoExpress.h"
#include "anyClient.h"
#include "users.h"
#include "admin.h"
#include "LocalStorage.h"

// RADIO FUNCTIONALITIES
#include "radio.h"


// Setup WiFi
const char* ssid = "Alarm AP";
//WiFiServer server(80);


void setup()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();

  Serial.begin(115200);
  radio.begin();

  auto app = ArduinoExpress();

  app.use([](Req &req, Res &res, Next next)->void*{ // FIXME: Add this middleware to ArdiunoExpress
    DynamicJsonBuffer buffer(200);
    if(buffer.parseObject(req.body).success()){
      next();
    }else{
      res.send(400, "text/plain", "Invalid JSON body");
    }
  });

  app.get("/", [](Req &req, Res &res)->void*{
    logdebug("Root got request");
    res.send(200, "text/plain", "Hello World!");
  });

  app.post("/raise", [](Req &req, Res &res)->void*{
    logdebug("Raise got request");
    res.send(200, "text/plain", "Hello AlarmInitiate!");
  });

  // app.use(&routers::any);
  // app.use(&routers::users);
  // app.use("/admin", &routers::admin);


  app.listen(80, []()->void{}); // TODO: Only provide this callback as an extra feature
}

void loop()
{}
// ORIGINAL LOOP TEMPORARILY SUSPENDED

/*
void loop()
{
  // put your main code here, to run repeatedly:
  //  server.handleClient();

  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("[Client Connected]");
    HTTP_Request request = HTTP_Request();
    

    // GET THE REQUEST LINE
    while(client.connected())
    {
      if (client.available())
      {
        request.method = client.readStringUntil(' ');
        request.route = client.readStringUntil(' ');
        request.method.trim();
        request.route.trim();
        client.readStringUntil('\r');
        break;
      }
    }
    

    // GET THE REQUEST HEADERS
    int count = 0;
    while(client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        Serial.print(line.length()); Serial.print("---"); Serial.println(line[0] == '\n'? "True" : "False");

        if (line.length() == 1 && line[0] == '\n')
        {
          Serial.println("[End of Reading Headers]");

//          client.println("HTTP/1.1 200 OK");
//          client.println("Content-type:text/plain");
//          client.println("Connection: close");
//          client.println();
//          
//          client.println("Thanks for reaching out");
//          client.println();
          break;
        }

        if (count < request.MAX_HEADERS_COUNT)
        {
          int colonIndex = line.indexOf(':');
          request.headers[count].key = line.substring(0, colonIndex);
          request.headers[count].value = line.substring(colonIndex+1);
          request.headers[count].key.trim();
          request.headers[count].value.trim();
          count = count + 1;
        }
      }
    }


    // GET THE REQUEST BODY
    while (client.available()) {
      // but first, let client finish its request
      // that's diplomatic compliance to protocols
      // (and otherwise some clients may complain, like curl)
      // (that is an example, prefer using a proper webserver library)
      request.body += client.readStringUntil('\r');
    }

    request.printToSerial();

    // Send a response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/plain");
    client.println("Connection: close");
    client.println();
    
    client.println("Thanks for reaching out");
    client.println();

    // close the connection:
    client.stop();
    Serial.println("[Client disconnected]");
  }
}






/*
void HTTP_handleRoot()
{
  if (server.hasArg("command"))
  {
    Serial.println(server.arg("command"));
  }

  server.send(200, "text/html", "Community Alarm");
  delay(10);
}

void HTTP_handleAlarm()
{
  if (server.hasArg("on"))
  {
    String alarmOn = server.arg("on");

    if (alarmOn == "True")
    {
      const char* message = "Swithching Alarm On";
      triggerAlarmOn(true);
      Serial.println(message);
      server.send(200, "text/html", message);
    }
    else if (alarmOn == "False")
    {
      const char* message = "Swithching Alarm Off";
      triggerAlarmOn(false);
      Serial.println(message);
      server.send(200, "text/html", message);
    }
    else
    {
      server.send(400, "text/html", "Invalid Request");
    }
  }else{
      server.send(400, "text/html", "Invalid Request - No Parameters");  
  }
}

void HTTP_handleNotFound()
{
  if (server.hasArg("command"))
  {
    Serial.println(server.arg("command"));
  }

  server.send(404, "text/html", "Invalid Request");
  delay(10);
}

void triggerAlarmOn(bool on)
{
  //  TO READ
  //  if (radio.available()) {
  //    char text[32] = "";
  //    radio.read(&text, sizeof(text));
  //    Serial.println(text);
  //  }

  //  TO WRITE
  if (on)
  {
    const String message = R"=====(GET /users HTTP/1.1
    Content-Type: text/plain
    Content-Length: 15
    Authorization: 8430F-GHJT-K53A-CDK0-MD7

    My People
    Good
    
    )=====";
    char text[message.length()]; // = "on";
    message.toCharArray(text, message.length());
    radio.write(&text, sizeof(text));
  } else {
    const char text[] = "off";
    radio.write(&text, sizeof(text));
  }

  delay(1000);
}
*/
