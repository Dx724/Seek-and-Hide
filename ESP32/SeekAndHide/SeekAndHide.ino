#include <UltrasonicSensor.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "SeekAndHide";
const char* password = "wheretogo";

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);

UltrasonicSensor ultrasonic(13, 14);

String last_message = "Welcome!";

void handleRoot() {
  String data_message = "<h1 style='text-align: center; display: block; margin-top: 1em; font-family: monospace; font-size: 3em;'>" + String(last_distance) + "</h1>";
  // Using a meta tag to refresh data ensures good compatibility (no JavaScript engine required!)
  server.send(200, "text/html", "<html><head><meta http-equiv='refresh' content='1'/></head><body style='color: #3299ff; background-color: black; font-weight: bold'>" + data_message + "</body></html>");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting up.");
  
  ultrasonic.setTemperature(21); // ~70 degrees Fahrenheit
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(15, INPUT);

  WiFi.persistent(false); // Helps to avoid crashes
  WiFi.softAP(ssid, password);
  delay(2000); // Helps to prevent crashes related to timing issues within the WiFi library
  WiFi.softAPConfig(local_ip, gateway, subnet);

  server.on("/", handleRoot);
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found!");
  });
  
  server.begin();
  Serial.println("Network enabled.");
}

void setMessage(String msg) {
  last_message = msg;
  msg_unchanged_ticks = 0;
}

int probable_pass = 0;
int processPassEvent(int dist) {
  if (dist_average == -1) return;
  if (dist * 1.0/dist_average < 0.5) { // Pretty close!
    probable_pass = 1;
  }
  else {
    if (probable_pass) { // Getting close then far -- probably passed it
      cycleMessage("Oof, you walked right past me!", "Don't even have time to stop and say hi?", "Alright, just keep walking, what could go wrong?");
      probable_pass = 0;
      return 1;
    }
    probable_pass = 0;
  }
  return 0;
}

void cycleMessage(String msg1, String msg2, String msg3) {
  msg_unchanged_ticks += 1;
  if (msg_unchanged_ticks >= MSG_STAGNATE) { // setMessage will reset this counter
    switch (cycle_ctr) {
      case 0:
        setMessage(msg1);
        break;
      case 1:
        setMessage(msg2);
        break;
      case 2:
        setMessage(msg3);
        break;
    }
    cycle_ctr = (cycle_ctr + 1) % CYCLE_LEN;
  }
}

double dist_average = -1;
int dist_ct = 0;

int TICK_SPEED = 50;

// Don't keep any message there for too long
int MSG_STAGNATE = 10000 / TICK_SPEED; // 10 seconds
int msg_unchanged_ticks = 0;

// For cycling messages
int cycle_ctr = 0;
int CYCLE_LEN = 3;

int last_pir = 0;

int us_active = 1; // Ultrasonic distance sensor active

void loop() {
  int distance = ultrasonic.distanceInCentimeters();
  int button1 = digitalRead(18); // Yellow button
  int button2 = digitalRead(19); // Blue button

  int pir = digitalRead(15); // Infrared sensor (jumper set to high for repeatable trigger mode), time set to minimum (3s) for smooth but responsive behavior
  if (button1 && button2 && !pir) { // Not in room
    if (last_pir) { // Just left the room
      setMessage("Why'd you leave? Don't give up.");
    }
    else {
      cycleMessage("You're not even in the right room!", "Do you have any idea where you are?", "Maybe try a different room...");
    }
  }
  else {
    if (!us_active || processPassEvent(dist)) {
      cycleMessage("Hurry up. You know this is a kids' game, right?", "Either I'm really good at hiding or you're just bad at seeking.", "The last guy was way better than you.");
    }
    if (!last_pir) { // Just entered the room
      setMessage("Hey, I think I hear you!");
    }
    if (button1) { // Normal mode
      us_active = 1;
      setMessage("You found me! Now go hide me better than the last guy did...");
    }
    if (button2) { // Hard mode
      us_active = 0;
      setMessage("You found me! Hard mode activated, that'll be a real challenge for the next guy. Now hide me somewhere safe.");
    }
  }

  if (us_active) {
    if (dist_average = 0) {
      dist_average = dist;
      dist_ct = 1;
    }
    else {
      dist_ct += 1;
      dist_average = (dist_average * (dist_ct - 1) + dist) / dist_ct;
    }
  }

evt_done:
  Serial.printf("Distance: %d cm\tA: %d\tB:%d\tIR:%d\n", distance, button1, button2, pir);

  last_distance = distance;
  server.handleClient();
  last_pir = pir;
  delay(TICK_SPEED); // Run at high speed to ensure we can detect brief "string-crossing"
}
