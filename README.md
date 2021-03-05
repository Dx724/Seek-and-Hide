# Seek and Hide <!-- omit in toc -->
## A temporally-distanced hide and seek game. <!-- omit in toc -->
![Image of the Seek and Hide device](media/image_shelf.jpg)
_Note: a clean-background image is also available [here](media/image_plain.jpg). However I felt an image of the device in context was more fitting._

[Video Demo](https://youtu.be/XuIBb4wW9iY)

"Always stay six feet apart." That's the maxim echoed around the world these days. In such isolation, it's difficult to play games such as hide-and-seek or tag with your friends. However, the Seek and Hide aims to capture the joy of such games via distance in time rather than space.

A player connects to the device via Wi-Fi and is guided by a virtual personality with chats with, taunts, and encourages the player to find it. The device senses when the user is in the general area, passes directly by, or is completely lost. Using such information, the virtual personality will tactically jeer at the seeker until it is found.

Once the player finds the Seek and Hide, it's time to hide! The player moves the device to a new location and configures its difficulty, setting up a challenge for the next player and coming to the realization that their own journey was the design of some player prior.

# Setup
## Hardware
The device contains an ESP32 microcontroller which hosts the web server, processes sensor data, and runs the virtual personality. In this specific case, a FreeNove ESP32-WROVER-DEV module was used, although most ESP32 devices should work just as well.

The device also uses an ultrasonic sensor (HC-SR04), a PIR sensor (HC-SR501), and two buttons.

## ESP32 Wiring
__Ultrasonic Sensor__
1. Connect the sensor's VCC pin to the ESP32's 5V pin.
2. Connect the sensor's TRIG pin to the ESP32's pin 13.
3. Connect the sensor's ECHO pin to the ESP32's pin 14.
4. Connect the sensor's GND pin to the ESP32's GND pin.

__PIR Sensor__
1. Connect the sensor's VCC pin to the ESP32's 5V pin.
2. Connect the sensor's OUT pin to the ESP32's pin 15.
3. Connect the sensor's GND pin to the ESP32's GND pin.

__Buttons__
1. For each of the two buttons, connect one side to the ESP32's GND pin.
2. Connect the other side of the "upper" button (further away from the enclosing box) to the ESP32's GPIO 18.
3. Connect the other side of the "lower" button (closer to the enclosing box) to the ESP32's GPIO 19.