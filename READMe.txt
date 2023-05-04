This project consists of obtaining key point data from user's pose through Googles TensorFlow-based pose detection API.
-----------------------------------------------------------------------------------------------------------------------
All backend data managment will be handled by node red flows which help calculate real time pose while also publishing data ,which is stored in MongoDB database, through
an MQTT topic. ESP-32 will be subscribed to said topic and manage LED switching with corresponding poses from input data.

Libraries used in controller --> 
1. WiFi.h
2. PubSubClient.h
3. string.h

ESP-32 Setup --> 
2x 330 Ohm Resistors
3x 1k Ohm Resistors
Left - GPIO 26
Right - GPIU 27
RGB, Green - GPIO 32
RGB, Blue - GPIO 33
RGB, Red - GPIO 2
Double A Battery Pack - Connected to 3V3 voltage regulator and GRND Pin

Note: 
*  Node red pose determination code is done by calculating the distance from the users wrists to their hip. After this distance corsses a specified threshold a pose is determined.


