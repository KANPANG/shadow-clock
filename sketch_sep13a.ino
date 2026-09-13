// Shadowplay Clock — Soft clock (not using RTC)
// The second hand jumps once every 5 seconds (0, 5, 10, ..., 55)
// The three lights display the "current time" only when the distance is approximately 7
cm (±1 cm); otherwise, they are off.
// Define pins used
#define TRIG 2
#define ECHO 3
#define DATA 8
#define CLOCK 9
#define LATCH 10
#define LEDPIN 13
//LED buffer
byte leds[5] = {0, 0, 0, 0, 0}; // 5字节=40位,实际用36位
// Soft Clock
//Time can be modified, h/m/s
int h = 1; //hour
int m = 45; //min
int s = 0; //sec
unsigned long lastTickMs = 0;
// Motion sensor control
unsigned long readPulse()
{
digitalWrite(TRIG, LOW);
delayMicroseconds(5);
digitalWrite(TRIG, HIGH);
digitalWrite(LEDPIN, HIGH);
delayMicroseconds(12);
digitalWrite(TRIG, LOW);
digitalWrite(LEDPIN, LOW);
return pulseIn(ECHO, HIGH, 60000UL); //60 ms timeout
}
//Distance should be 7cm;No echo should returned 1000
long readDistanceOnce()
{
unsigned long t = readPulse();
if (t == 0) return 1000;
return (long)(t * 0.0343 / 2.0);
}
// Take the median of the third iteration to obtain the steady state
long readDistanceMed3()
{
unsigned long t1 = readPulse();
unsigned long t2 = readPulse();
unsigned long t3 = readPulse();
unsigned long lo = min(t1, min(t2, t3));
unsigned long hi = max(t1, max(t2, t3));
unsigned long med = t1 + t2 + t3 - lo - hi;
if (med == 0) return 1000;
return (long)(med * 0.0343 / 2.0);
}
// 74HC595 helpers * 5
void writeLEDs()
{
digitalWrite(LATCH, LOW);
for (int i = 4; i >= 0; i--)
{
shiftOut(DATA, CLOCK, MSBFIRST, leds[i]);
}
digitalWrite(LATCH, HIGH);
}
void setLED(int index, bool on)
{
int chip = index / 8;
int bit = index % 8;
if (on) leds[chip] |= (1 << bit);
else leds[chip] &= ~(1 << bit);
}
void allOFF() { for (int i = 0; i < 36; i++) setLED(i, false); }
void allON() { for (int i = 0; i < 36; i++) setLED(i, true); }
// Show clock (12×3, Physical offset +6)
//index * 3 + 0 = hour level, +1 = minute level, +2 = second level
void showClock(int hh, int mm, int ss)
{
allOFF();
// Hours: 12-hour format, maintaining original +6 offset.
int hourPos = (hh % 12 + 6) % 12;
// Minutes: One unit every 5 minutes (0, 5, 10, ..., 55) plus a +6 offset.
int minPos = ((mm / 5) + 6) % 12;
// Second hand: Jumps once every 5 seconds
int secPos = ((ss / 5) % 12);
setLED(hourPos * 3 + 0, true); // hour
setLED(minPos * 3 + 1, true); // min
setLED(secPos * 3 + 2, true); // every 5 sec
}
//Soft clock tick
void tickClock()
{
unsigned long now = millis();
if (now - lastTickMs >= 1000UL)
{
unsigned long elapsed = (now - lastTickMs) / 1000UL; // full sec
lastTickMs += elapsed * 1000UL;
s += (int)elapsed;
if (s >= 60) { m += s / 60; s %= 60; }
if (m >= 60) { h += m / 60; m %= 60; }
if (h >= 24) { h %= 24; }
}
}
//Arduino setup/loop
void setup() {
Serial.begin(9600);
pinMode(TRIG, OUTPUT);
pinMode(ECHO, INPUT);
pinMode(LEDPIN, OUTPUT);
digitalWrite(TRIG, LOW);
digitalWrite(LEDPIN, LOW);
pinMode(DATA, OUTPUT);
pinMode(CLOCK, OUTPUT);
pinMode(LATCH, OUTPUT);
for (int i = 0; i < 5; i++) leds[i] = 0;
writeLEDs();
lastTickMs = millis();
}
void loop()
{
tickClock();
// Read distance
// Use the median value for stability first
long d = readDistanceMed3();
//shows when ≈ 7 cm
const float targetCm = 7.0;
const float tol = 1.0;
bool active = (d > 0) && (d >= targetCm - tol) && (d <= targetCm + tol);
// Testing output
Serial.print("time=");
if (h < 10) Serial.print('0'); Serial.print(h);
Serial.print(':');
if (m < 10) Serial.print('0'); Serial.print(m);
Serial.print(':');
if (s < 10) Serial.print('0'); Serial.print(s);
Serial.print(" dist=");
Serial.print(d);
Serial.print(" cm active=");
Serial.println(active ? "YES" : "NO");
digitalWrite(LEDPIN, active ? HIGH : LOW);
// Display time only when active, otherwise turn off.
if (active) showClock(h, m, s);
else allOFF();
writeLEDs();
delay(50);
}