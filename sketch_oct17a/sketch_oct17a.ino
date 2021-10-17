#include "ArduinoJson.h"
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include <addons/TokenHelper.h>


#include <addons/RTDBHelper.h>
#define WIFI_SSID "HongSon"
#define WIFI_PASSWORD "hongson2002"
#define API_KEY "AIzaSyCEFjlvvhtILS01ELfUmilhCCZEKJTrc0o"
#define DATABASE_URL "https://c19tf-bc3cf-default-rtdb.asia-southeast1.firebasedatabase.app/" 
#define USER_EMAIL "memorizememorize6@gmail.com"
#define USER_PASSWORD "firebasedoantotnghiep"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void send_data_to_Firebase(int STT, String Status,String User_name, String CMND, String Address);
void get_user_info(int STT, char* JSONMessage);


void setup() {
 
 Serial.begin(115200);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h


  Firebase.begin(&config, &auth);

  //Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
  Firebase.deleteNode(fbdo, "/Test");
}

int STT;
void loop() {
     delay (1000);
     const char JSONMessage[] = " {\"Name\": \"Hai\", \"CMND\": \"25\", \"Address\":\"TPHCM\"}";
     if (Serial.available()){
      String command = Serial.readStringUntil('\n');
      if (command == "SEND"){
          call (STT, JSONMessage);
          STT +=1;
        }
    
     }
 

            
       
}

void call(int STT, const char* json_string){
        const size_t capacity = JSON_OBJECT_SIZE (3) + 30;
        DynamicJsonDocument doc (capacity);
        doc.clear();
        //doc.garbageCollect();
        DeserializationError error = deserializeJson(doc,json_string);
        if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
        }
      

      
   
       const char* Name = doc["Name"];
       const char* CMND = doc["CMND"];
       const char* Address = doc["Address"];

       
       String User_Status;
       String User_Name ;
       String User_CMND ;
       String User_Address ;
       User_Status  = "Not done";
       User_Name = String (Name);
       User_CMND = String (CMND);
       User_Address = String (Address);
       

       
        

        send_data_to_Firebase(STT,User_Status,User_Name,User_CMND,User_Address);  
}



void send_data_to_Firebase(int STT, String Status,String User_name, String CMND, String Address){
  Serial.printf("Set bool... %s\n", Firebase.setString(fbdo, "/Test/STT/"+String(STT)+"/Status",String (Status)) ? "ok" : fbdo.errorReason().c_str());
  Serial.printf("Set bool... %s\n", Firebase.setString(fbdo, "/Test/STT/"+String(STT)+"/Name", String (User_name)) ? "ok" : fbdo.errorReason().c_str());
  Serial.printf("Set bool... %s\n", Firebase.setString(fbdo, "/Test/STT/"+String(STT)+"/CMND",String (CMND)) ? "ok" : fbdo.errorReason().c_str());
  Serial.printf("Set bool... %s\n", Firebase.setString(fbdo, "/Test/STT/"+String(STT)+"/Adress",String (Address) ) ? "ok" : fbdo.errorReason().c_str());
};
