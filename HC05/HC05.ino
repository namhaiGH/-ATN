#include <BluetoothSerial.h>
#include <ArduinoJson.h>

BluetoothSerial SerialBT;

String message = "";
char incomingChar;
void setup()
{
  Serial.begin(115200);
  SerialBT.begin();
  Serial.println("Bluetooth Started! Ready to pair...");
 
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
    String input = "{\"Name\":\"hai\"}";
    char buf[50];
    message.toCharArray(buf,50);

    char buf2[50];
    input.toCharArray(buf2,50);
    int i=0;
    for (i=0;i<message.length();i++){
      Serial.print(i);
      Serial.print("----");
      Serial.print(buf[i]);
      Serial.print("----");
      Serial.println(buf2[i]);

      
      
    }
    
    //Serial.println(message);
    
   
    call(message);
    message="";
    }
  
  delay(500);
}
}


void call(String json_string){
  DynamicJsonDocument doc(1024);
  deserializeJson(doc,json_string);
  JsonObject obj = doc.as<JsonObject>();
  String User_name = obj["Name"];
  String CMND = obj["CMND"];
  String Adress = obj["Adress"];
  if (User_name!="null"){ Serial.println(User_name);Serial.println(CMND);Serial.println(Adress);}
    
  
 
}     
