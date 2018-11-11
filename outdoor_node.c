// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_DHT.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include "Adafruit_DHT.h"



#define DHTPIN D0     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11		// DHT 11 
#define DHTTYPE DHT22		// DHT 22 (AM2302)
//#define DHTTYPE DHT21		// DHT 21 (AM2301)

#define WATER_SENSOR A1             //WATER LEVEL SENSOR
#define WATER_LEVEL_INDICATOR D2    //WATER LEVEL INDICATOR

#define DISTANCE_SENSOR A0          //ANALOG DISTANCE MEASUREMENT SENSOR
#define DISTANCE_INDICATOR    D1    //DISTANCE INDICATOR

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

int WaterSensor_value;      // Stores the analog output from WATER LEVEL SENSOR
int mapped_ws = 0;          // Stores the mapped analog value of WATER LEVEL SENSOR

int DistSensor_value;       // Stores the analog output from DISTANCE MEASUREMENT SENSOR
int offset = 30;            
int mapped_ds = 0;          // Stores the mapped analog value of DISTANCE MEASUREMENT SENSOR

float prev_temp = 0;
float prev_humidity = 0;
int prev_dist = 0;
int curr_dist = 0;
int prev_ws = 0;

void setup() {
	Serial.begin(9600); 
	Serial.println("DHTxx test!");

    pinMode(WATER_SENSOR, INPUT);
    pinMode(DISTANCE_SENSOR, INPUT);
    pinMode(WATER_LEVEL_INDICATOR, OUTPUT);
    pinMode(DISTANCE_INDICATOR, OUTPUT);
    
	dht.begin();
}

void loop() {
// Wait a few seconds between measurements.
	delay(2000);

// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a 
// very slow sensor)
	float h = dht.getHumidity();
// Read temperature as Celsius
	float t = dht.getTempCelcius();
// Read temperature as Farenheit
	float f = dht.getTempFarenheit();
  
// Check if any reads failed and exit early (to try again).
	if (isnan(h) || isnan(t) || isnan(f)) {
		Serial.println("Failed to read from DHT sensor!");
		return;
	}

// Compute heat index
// Must send in temp in Fahrenheit!
	float hi = dht.getHeatIndex();
	float dp = dht.getDewPoint();
	float k = dht.getTempKelvin();
	
	WaterSensor_value = analogRead(A1);             //Read the output of WATER LEVEL SENSOR
	DistSensor_value = analogRead(DISTANCE_SENSOR); //Read the output of DISTANCE MEASUREMENT SENSOR
	mapped_ds = map(DistSensor_value, 0, 4095, 15, 80);
	
	if (WaterSensor_value > 500) {
        digitalWrite(WATER_LEVEL_INDICATOR, HIGH);
        
        if (WaterSensor_value > prev_ws)
            Particle.publish("HA_ws", "1");
            
        prev_ws = WaterSensor_value;
	}
    else {
        digitalWrite(WATER_LEVEL_INDICATOR, LOW);
        prev_ws = 0;
    }
    
    if (t != prev_temp)
    {
        char buffer[4];
        sprintf(buffer, "%f", t);
        Particle.publish("HA_temp", buffer);
    }
    prev_temp = t;
    
    if (h != prev_humidity)
    {
        char humidity[4];
        sprintf(humidity, "%f", h);
        Particle.publish("HA_humidity", humidity);
    }
    prev_humidity = h;
    
	Serial.print("Humid: "); 
	Serial.print(h);
	Serial.print("% - ");
	Serial.print("Temp: "); 
	Serial.print(t);
	Serial.print("*C ");
	Serial.print(f);
	Serial.print("*F ");
	Serial.print(k);
	Serial.print("*K - ");
	Serial.print("DewP: ");
	Serial.print(dp);
	Serial.print("*C - ");
	Serial.print("HeatI: ");
	Serial.print(hi);
	Serial.println("*C");
	Serial.println(Time.timeStr());
	Serial.print("Water Sensor: ");
	Serial.println(WaterSensor_value);
	
	if ((mapped_ds - 30)<15 )
    {
        Serial.print("Distance: ");
        Serial.print(mapped_ds - 30);
        Serial.println("cm\n");
        digitalWrite(DISTANCE_INDICATOR,HIGH);
        
        curr_dist = 1;
        if(curr_dist != prev_dist)
            Particle.publish("HA_motion", "1");
            
        prev_dist = 1;
    }
    else
    {
        digitalWrite(DISTANCE_INDICATOR,LOW);
        prev_dist = 0;
    }
    
	Serial.println("----------------------------------------------------------------------");
	
	Serial.println();
	Serial.println();
	
	delay(1);
}

