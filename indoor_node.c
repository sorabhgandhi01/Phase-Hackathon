#include <stdio.h>
#include <stdlib.h>

int counter = 0;
char *message;

int detected_motion = 0;
int Light = D1;
int interrupt_PIR = D5;


int detected_flame = 0;
int interrupt_flame = D6;


int noise_input = A0;
int Sound_value = 0;
int open_door = 0;
int buzzer = 0;

Servo myServo;

Timer timer(3000,turn_off_light,true);

void setup() {
    Particle.subscribe("HA_temp", handler_temp);
    Particle.subscribe("HA_motion", handler_motion);
    Particle.subscribe("HA_ws", handler_water);
	Serial.begin(9600);
	pinMode(Light, OUTPUT);
    pinMode(interrupt_PIR, INPUT);
    digitalWrite(Light, LOW);
    attachInterrupt(interrupt_PIR,PIR_ISR,RISING);
    
    pinMode(Light, OUTPUT);
    
    myServo.attach( D3 );
    
    pinMode(D0,OUTPUT);

    pinMode(interrupt_flame, INPUT);
    attachInterrupt(interrupt_flame,flame_ISR,RISING);

}


void loop() {
    if(detected_motion == 1)
    {
        digitalWrite(Light, HIGH);
        timer.start();
        Serial.println("Motion Detected");
        delay(40);
        detected_motion = 0;
        
              // move servo to 25°
    }
    
    if(open_door == 1)
    {
        Serial.println("Door Opened");
        myServo.write(0);
        delay(500);             // wait 100 ms
        myServo.write(90);
        delay(500);
        myServo.write(0);
        open_door = 0;
    }
    if( buzzer == 1)
    {
        tone(D0,2300,1000);
        buzzer = 0;
    }
    if(detected_flame ==1)
    {
        tone(D0,2300,1000);
        detected_flame = 0;
    }
    
    Loudness_sensor();
    
   
}

void PIR_ISR()
{
    
    detected_motion = 1;

}

void flame_ISR()
{
    detected_flame = 1;
}

void Loudness_sensor()
{
    Sound_value = analogRead(noise_input);
    //Serial.println(Sound_value);
    if(Sound_value > 700)
    {
        digitalWrite(Light, HIGH);
        timer.start();
        Serial.println("Sound Detected");
        delay(40);
    }
    
}

void handler_temp(const char *event, const char *data)
{
        Serial.println(event);
        Serial.print("Temperature :");
        if(data)
        {
         Serial.println(data);

        }

    
}

void handler_motion(const char *event, const char *data)
{
        Serial.println(event);
        Serial.print("Data :");
        if(data)
        {
         Serial.println(data);
         int data_num = atoi(data);
         
         if(data_num == 1)
         {
            Serial.println("Open door");
            open_door = 1;
         }

        }

    
}
void handler_water(const char *event, const char *data)
{
        Serial.println(event);
        Serial.print("Data :");
        if(data)
        {
         Serial.println(data);
         int data_num = atoi(data);
         
         if(data_num == 1)
         {
            Serial.println("Water level high");
            buzzer = 1;
         }
        }

    
}

void turn_off_light()
{
    digitalWrite(Light, LOW);

}
