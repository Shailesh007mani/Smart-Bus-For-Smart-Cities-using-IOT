#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>

TinyGPSPlus gps;  // The TinyGPS++ object

SoftwareSerial ss(4, 5); // The serial connection to the GPS device  Rx-D1(5) TX-D2(4)

#define FIREBASE_HOST "smart-bus-edge-4ba97.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "6j969Hz60cKjrO7ddqvwfU7rhHZnRScXvk6Skpi3"            // the secret key generated from firebase

const char* ssid = "Redmi Note 9";
const char* password = "Dynamics";

int available_seat = 3;
int total_seat = 3;
int count=0;
char stop;
String available_seat1, count1, total_seat1, myString11, myString21, seat,myString3,total, lat1, long1, lat2, long2;
float latitude , longitude;
float s =45;
float t, time01;
float distanceinMETER, a, c, dlon, dlat, distanceinKILOMETER;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str, myString1, myString2;
int pm;

WiFiServer server(80);
void setup()
{
  Serial.begin(115200);
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
  ss.begin(9600);
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

  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  Serial.println(gps.satellites.value());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  pinMode(D3,INPUT);
  pinMode(D4,INPUT);
}

void loop()
{
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        myString1 = lat_str;
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        myString2 =lng_str;
        Firebase.setString("Current Location/Latitude", myString1);                                  //setup path and send readings
        Firebase.setString("Current Location/Longitude", myString2); //setup path and send readings
        
        float lat1=latitude*3.14/180; //value of latitude in radians
        float long1=longitude*3.14/180; //value of longitude in radians
        float lat2=28.6712*3.14/180; //bus stand location
        float long2=77.3177*3.14/180; //bus stand location
        dlon = long2 - long1;
        dlat = lat2 - lat1 ;
        a = sq(sin(dlat / 2)) + cos(lat1) * cos(lat2) * sq(sin(dlon / 2));
        c = 2 * atan2( sqrt(a), sqrt(1 - a) );
        distanceinMETER = 6378137.0 * c; //distance in meters
        distanceinKILOMETER = distanceinMETER/1000;
           //speed= distance/time
  t=distanceinKILOMETER/s;
  time01=60*t;
  
  Serial.print("Distance In Meter: ");
  Serial.print(distanceinMETER);
  Serial.print("m ");
  Serial.print("\n");
  
  Serial.print("Distance In Kilometer: ");
  Serial.print(distanceinKILOMETER);
  Serial.print("km ");
  Serial.print("\n");

  Serial.print("Speed: ");
  Serial.print(s);
  Serial.print("km/hr ");
  Serial.print("\n");

  Serial.print("Time: ");
  Serial.print(time01);
  Serial.print("min ");
  Serial.print("\n");
      }

      if (gps.date.isValid())
      {
        date_str = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();

        if (date < 10)
          date_str = '0';
        date_str += String(date);

        date_str += " / ";

        if (month < 10)
          date_str += '0';
        date_str += String(month);

        date_str += " / ";

        if (year < 10)
          date_str += '0';
        date_str += String(year);
      }

      if (gps.time.isValid())
      {
        time_str = "";
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();

        minute = (minute + 30);
        if (minute > 59)
        {
          minute = minute - 60;
          hour = hour + 1;
        }
        hour = (hour + 5) ;
        if (hour > 23)
          hour = hour - 24;

        if (hour >= 12)
          pm = 1;
        else
          pm = 0;

        hour = hour % 12;

        if (hour < 10)
          time_str = '0';
        time_str += String(hour);

        time_str += " : ";

        if (minute < 10)
          time_str += '0';
        time_str += String(minute);

        time_str += " : ";

        if (second < 10)
          time_str += '0';
        time_str += String(second);

        if (pm == 1)
          time_str += " PM ";
        else
          time_str += " AM ";

      }

    }
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>GPS Interfacing with NodeMCU</title> <style>";
  s += "a:link {background-color: YELLOW;text-decoration: none;}";
  s += "table, th, td {border: 1px solid black;} </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER> GPS Interfacing with NodeMCU</h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
  s += "width:50%";
  s += "> <tr> <th>Latitude</th>";
  s += "<td ALIGN=CENTER >";
  s += lat_str;
  s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
  s += lng_str;
  s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";
  s += date_str;
  s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";
  s += time_str;
  s += "</td>  </tr> </table> ";
 
 
  if (gps.location.isValid())
  {
     s += "<p align=center><a style=""color:RED;font-size:125%;"" href=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
    s += lat_str;
    s += "+";
    s += lng_str;
    s += """ target=""_top"">Click here!</a> To check the location in Google maps.</p>";
  }

  s += "</body> </html> \n";

  client.print(s);
  delay(100);
  pinMode(D3,INPUT);
  pinMode(D4,INPUT);
  int in_value = digitalRead(D3);
  int out_value = digitalRead(D4);
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
    myString11= String(count);
    count1=myString11;
    Firebase.setString("Seat/Bus/Seat Occupied", count1);
    available_seat = total_seat - count;
    Serial.print("Available_Seat = ");
    Serial.print(available_seat);
    Serial.println();
    myString21= String(available_seat);
    available_seat1=myString21;
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
    myString11= String(count);
    count1=myString11;
    Firebase.setString("Seat/Bus/Seat Occupied", count1);
    available_seat = total_seat - count;
    Serial.print("Available_Seat = ");
    Serial.print(available_seat);
    Serial.println();
    myString21= String(available_seat);
    available_seat1=myString21;
    Firebase.setString("Seat/Bus/Seat Available", available_seat1);
    delay(1000); 
   }
  }
}
