#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "smart-bus-edge-4ba97.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "6j969Hz60cKjrO7ddqvwfU7rhHZnRScXvk6Skpi3"            // the secret key generated from firebase

const char* ssid = "Redmi Note 9";
const char* password = "Dynamics";

//code for bidirectional passenger counter
int available_seat = 3;
int total_seat = 3;
int count=0;
char stop;
String available_seat1, count1, total_seat1, myString1, myString2, seat,myString3,total;
void setup() 
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Total seats = 3");
  delay(2000);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  Serial.print("occupied seats = ");
  Serial.print(count);
  Serial.println();
  Serial.print("Available_Seat = ");
  Serial.print(available_seat);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
}

void loop() 
{
  int in_value = digitalRead(6);
  int out_value = digitalRead(7);
  total=String(total_seat);
  total_seat1=total;
  Firebase.setString("Seat/Bus/Total Number Of Seats", total_seat1);                                  //setup path and send readings
  
   if(in_value == LOW)
   {
    Serial.println();
    if(count<total_seat)
    {
    count++;
    Serial.print("occupied seats = ");
    Serial.print(count);
    Serial.println();
    myString1= String(count);
    count1=myString1;
    Firebase.setString("Seat/Bus/Seat Occupied", count1);
    available_seat = total_seat - count;
    Serial.print("Available_Seat = ");
    Serial.print(available_seat);
    Serial.println();
    myString2= String(available_seat);
    available_seat1=myString2;
    Firebase.setString("Seat/Bus/Seat Available", available_seat1);
    delay(1000);
   }
   else
   {  
    Serial.print("seats are not available");
    Serial.println();
    myString3= String("seats are not available");
    seat=myString3;
    delay(5000); 
   }
   
  }

   if(out_value == LOW)
   {
    if(count>0)
    {
    Serial.println();
    count--;
    Serial.print("occupied seats = ");
    Serial.print(count);
    Serial.println();
    myString1= String(count);
    count1=myString1;
    Firebase.setString("Seat/Bus/Seat Occupied", count1);
    available_seat = total_seat - count;
    Serial.print("Available_Seat = ");
    Serial.print(available_seat);
    Serial.println();
    myString2= String(available_seat);
    available_seat1=myString2;
    Firebase.setString("Seat/Bus/Seat Available", available_seat1);
    delay(1000); 
   }
   }
}
