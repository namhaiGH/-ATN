///LIBRARY
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

///VARIABLE
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

BluetoothSerial SerialBT;

String message = "";
int STT;

void setup()
{
Serial.begin(115200);
SerialBT.begin();
 
WiFi.begin("Minh Hang", "minhhang1967");
config.api_key = "AIzaSyC09qbn1_x-GrB-S4YFNF-awCcejyO3Kds";
auth.user.email = "vuvantruong24251962@gmail.com";
auth.user.password = "123456";


config.database_url = "https://c19testform-default-rtdb.asia-southeast1.firebasedatabase.app/";
config.token_status_callback = tokenStatusCallback; 
Firebase.begin(&config, &auth);
Firebase.reconnectWiFi(true);
//Firebase.setDoubleDigits(5);
Firebase.deleteNode(fbdo, "/Test");

}

void loop()
{

if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      message += String(incomingChar);
    }
   
    }
else{
  if(message!=""){
    STT+=1;
 
    //call(STT,message)
     DynamicJsonDocument doc(128);
     deserializeJson(doc,message);
     JsonObject obj = doc.as<JsonObject>();

     String User_Name = obj["Name"];
     String User_CMND = obj["CMND"];
     String User_Adress = obj["Adress"];

 
    Serial.printf("Set bool... %s\n", Firebase.setString(fbdo, "/Test/STT/"+String(STT)+"/Status","Not done") ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set bool... %s\n", Firebase.setString(fbdo, "/Test/STT/"+String(STT)+"/Name", User_Name) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set bool... %s\n", Firebase.setString(fbdo, "/Test/STT/"+String(STT)+"/CMND",User_CMND) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set bool... %s\n", Firebase.setString(fbdo, "/Test/STT/"+String(STT)+"/Adress",User_Adress ) ? "ok" : fbdo.errorReason().c_str());
    //end decode
    message="";
    }
  }
}
