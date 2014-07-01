#include <EEPROM.h>
int led_out=5;
int bat_lvl_low=4;
int bat_lvl_mid=7;
int bat_lvl_high=8;
int temp_led=10;
int idle_in=0;
int temp_in=2;
int v_in=3;
int fan_out=6;
int pwm_in=1;
int trig_in=9;
//int trig_in=0;
int cellcount=6;
//5v = 1024;
//3v minimum
//10:1 voltage division
int vlow=55*cellcount;
int vwarn=62*cellcount;
int vmid=68*cellcount;
int vhigh=76*cellcount;
// MCP9701A
// Vzero=400mv=82 from AnalogrRead()
// 19.7 (4 1024th's of 5v) per degree C
// Max temp 50C = 282
// warning temp = 45C 262
int twarn=262;
int tmax=282;
int tfan=202;
int idle_thresh=512;
int idle_min=8;
int idle_max=32;
int idle_div=400;
int min_bright=8;
int throttle_bright=160;
int tempblink=0;
int batblink=0;
int voltage_avg=0;
int lasttrig=1;
int temp_avg=0;
int batthrottle=0;
int batthrottlemargin=4;


void setup() {
  cellcount=EEPROM.read(0);
  vlow=EEPROM.read(4)*cellcount;
  vwarn=EEPROM.read(3)*cellcount;
  vhigh=EEPROM.read(1)*cellcount;
  vmid=EEPROM.read(2)*cellcount;
  batthrottlemargin=EEPROM.read(5);
  idle_thresh=EEPROM.read(0x0A)*4;
  idle_min=EEPROM.read(0x0B);
  idle_max=EEPROM.read(0x0C);
  idle_div=EEPROM.read(0x0D)*4;
  min_bright=EEPROM.read(0x0E);
  throttle_bright=EEPROM.read(0x0F);
  int tbase=EEPROM.read(0x10);
  twarn=tbase+EEPROM.read(0x12);
  tmax=tbase+EEPROM.read(0x11);
  tfan=tbase+EEPROM.read(0x13);
  pinMode(temp_in,INPUT);
  pinMode(pwm_in,INPUT);
  pinMode(trig_in,INPUT_PULLUP);
  pinMode(v_in,INPUT);
  pinMode(temp_led,OUTPUT);
  pinMode(led_out,OUTPUT);
  digitalWrite(led_out,0);
  pinMode(bat_lvl_low,OUTPUT);
  pinMode(bat_lvl_high,OUTPUT);
  pinMode(bat_lvl_mid,OUTPUT);
  pinMode(fan_out,OUTPUT);
  pinMode(idle_in,INPUT);
  voltage_avg=analogRead(v_in);
  temp_avg=analogRead(temp_in);
}


void loop() {
  int trig=digitalRead(trig_in);
  int voltage=analogRead(v_in);
  int temp=analogRead(temp_in);
  temp_avg=(temp_avg*9+temp)/10;
  voltage_avg=(voltage_avg*9+voltage)/10;
  int idle=analogRead(idle_in);
  //temp_avg=temp;
  //voltage_avg=voltage;
  if ((trig==1 && idle < idle_thresh) || temp_avg >= tmax || voltage_avg <= vlow) {
    digitalWrite(led_out,0);
  } else {
    if (temp_avg < tmax && ((voltage_avg > vlow) && ( lasttrig==0 || trig==1)) || (voltage_avg > vwarn)) { //don't let them turn it on if it's below vwarn but above vlow, but let them keep it on until it reaches vlow.
      int pwm_adjust=analogRead(pwm_in);
      pwm_adjust=max(min(pwm_adjust/3,255),min_bright);
      if (trig==1) { //idle mode 
        int tidle=idle-idle_thresh;
        tidle=(tidle*(idle_max-idle_min))/idle_div;
        tidle+=idle_min;
        tidle=min(tidle,idle_max);
        pwm_adjust=tidle;
      }
      if (temp_avg > twarn ||  voltage_avg < vwarn || (voltage_avg < (vwarn+batthrottlemargin*cellcount) && batthrottle==1)) {
        pwm_adjust=min(pwm_adjust,throttle_bright);
        
         if (voltage_avg < vwarn) {
           batthrottle=1;
         }
      } else   if (voltage_avg > (vwarn+batthrottlemargin*cellcount)) {
         batthrottle=0;
      }
      if (pwm_adjust==255) {
        digitalWrite(led_out,1);
      } else {
        analogWrite(led_out,pwm_adjust);
      }
    }
  }
  if (temp_avg > twarn) {
    digitalWrite(temp_led,1);
  } else {
    digitalWrite(temp_led,0);
  }
  if (voltage_avg > vhigh) {
    digitalWrite(bat_lvl_high,1);
    digitalWrite(bat_lvl_mid,0);
    digitalWrite(bat_lvl_low,0);
  }else if (voltage_avg > vmid) {
    int brt=(255*(voltage_avg-vmid))/(vhigh-vmid);
    analogWrite(bat_lvl_high,brt);
    analogWrite(bat_lvl_mid,255-brt);
    digitalWrite(bat_lvl_low,0);
  } else if (voltage_avg > vwarn) {
    digitalWrite(bat_lvl_mid,1);
    digitalWrite(bat_lvl_low,0);
    digitalWrite(bat_lvl_high,0);
  } else if (voltage_avg > vlow) {
    int brt=(255*(voltage_avg-vlow))/(vwarn-vlow);
    digitalWrite(bat_lvl_low,1);
    analogWrite(bat_lvl_mid,brt);
    digitalWrite(bat_lvl_high,0);
  } else {
    digitalWrite(bat_lvl_low,1);
    digitalWrite(bat_lvl_mid,0);
    digitalWrite(bat_lvl_high,0);
  }
  if (temp_avg > twarn) {
    digitalWrite(fan_out,1); //turn fan on full blast!
  } else if (temp_avg > tfan) {
    analogWrite(fan_out,134+(temp_avg-tfan)*2);
  } else {
    digitalWrite(fan_out,0);
  }
}





/*
void loop() {
  int trig=digitalRead(trig_in);
  int voltage=analogRead(v_in);
  int temp=analogRead(temp_in);
  temp_avg=(temp_avg*9+temp)/10;
  voltage_avg=(voltage_avg*9+voltage)/10;
  int idle=analogRead(idle_in);
  //int idle=0;
  //temp_avg=temp;
  //voltage_avg=voltage;
  if ( (trig==1 || idle > idle_thresh) || temp_avg >= tmax || voltage_avg <= vlow) {
    digitalWrite(led_out,0);
  } else {
    if (temp_avg < tmax && ((voltage_avg > vlow) && lasttrig==0) || (voltage_avg > vwarn)) { //don't let them turn it on if it's below vwarn but above vlow, but let them keep it on until it reaches vlow.
      int pwm_adjust=analogRead(pwm_in);
      if (trig==1) { //idle mode 
        int tidle=idle-idle_thresh;
        tidle=(tidle*(idle_max-idle_min))/idle_div;
        tidle+=idle_min;
        tidle=min(tidle,idle_max);
        pwm_adjust=tidle;
      }
      pwm_adjust=max(min(pwm_adjust/3,255),min_bright);
      if (temp_avg > twarn ||  voltage_avg < vwarn || (voltage_avg < (vwarn+batthrottlemargin*cellcount) && batthrottle==1)) {
        pwm_adjust=min(pwm_adjust,throttle_bright);
         if (voltage_avg < vwarn) {
           batthrottle=1;
         }
      } else   if (voltage_avg > (vwarn+batthrottlemargin*cellcount)) {
         batthrottle=0;
      }
      if (pwm_adjust==255) {
        digitalWrite(led_out,1);
      } else {
        analogWrite(led_out,pwm_adjust);
      }
    }
  }
  if (temp_avg > twarn) {
    digitalWrite(temp_led,1);
  } else {
    digitalWrite(temp_led,0);
  }
  if (voltage_avg > vhigh) {
    digitalWrite(bat_lvl_high,1);
    digitalWrite(bat_lvl_mid,0);
    digitalWrite(bat_lvl_low,0);
  }else if (voltage_avg > vmid) {
    int brt=(255*(voltage_avg-vmid))/(vhigh-vmid);
    analogWrite(bat_lvl_high,brt);
    analogWrite(bat_lvl_mid,255-brt);
    digitalWrite(bat_lvl_low,0);
  } else if (voltage_avg > vwarn) {
    digitalWrite(bat_lvl_mid,1);
    digitalWrite(bat_lvl_low,0);
    digitalWrite(bat_lvl_high,0);
  } else if (voltage_avg > vlow) {
    int brt=(255*(voltage_avg-vlow))/(vwarn-vlow);
    digitalWrite(bat_lvl_low,1);
    analogWrite(bat_lvl_mid,brt);
    digitalWrite(bat_lvl_high,0);
  } else {
    digitalWrite(bat_lvl_low,1);
    digitalWrite(bat_lvl_mid,0);
    digitalWrite(bat_lvl_high,0);
  }
  if (temp_avg > twarn) {
    digitalWrite(fan_out,1); //turn fan on full blast!
  } else if (temp_avg > tfan) {
    analogWrite(fan_out,134+(temp_avg-tfan)*2);
  } else {
    digitalWrite(fan_out,0);
  }
}
*/

