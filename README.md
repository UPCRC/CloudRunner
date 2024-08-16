[grayTextColor]: #7C7C7C

# The CloudRunner Project
The goal of this project is to lower the cost barrier to beginning competitive robotics
and in the long term, help elevate people access robotics as a field regardless of their
socio-economic and financial background. 

CHECKING PO 
1. correct? github library link at Downloading and Installing CloudRunner Library
2. add more references for PWM and something about arduino UwU

## This repo
This repository hosts all the necessary information about the current rendition of CloudRunner and provides ad 
beginner friendly library and examples for use with the CloudRunner kit. 

## Installation and Needed Materials 
There are  3 main components needed to start utilizing the CloudRunner: 
1. Arduino IDE (version 2.x.x was used in developing this documentation)
2. CloudRunner Software Library
3. CloudRunner Kit

### Installing the Arduino IDE
First, download the Arduino IDE 2.0 which you can find in the Arduino website (https://www.arduino.cc/en/software).  After downloading, open it and this is what you will see. 

![Opening Arduino IDE](<Images-ReadME/Arduino Start Up.png>)

### Downloading and Installing CloudRunner Library to the Arduino IDE 2.x.x
Access the library by going to the UPCRC CloudRunner GitHub page (https://github.com/UPCRC/CloudRunner) and download the contents by clicking the ***<> Code*** dropdown box > Click ***Download ZIP***.
![Downloading CloudRunner ZIP Folder](<Images-ReadME/Downloading CloudRunner Library.png>)

Next, include the CloudRunner library that you downloaded from Github in the Arduino IDE. Go to ***Sketch Tab*** >  ***Include Library*** > ***Add.ZIP Library*** option. Choose the ZIP file you downloaded from Github.
![Installing Library in Arduino IDE](<Images-ReadME/Installing ZIP Folder.png>)

Verify the installation by going to the ***Sketch tab*** again and you should be able to see now in the <span style='color: #7C7C7C;'> Contributed Libraries </span> portion, the ***CloudRunner library*** or for other newer versions, it can be seen under ***Sketch Tab*** >  ***Include Library*** > under <span style='color: #7C7C7C;'> Contributed Libraries </span>.
![Verifying Library Installation](<Images-ReadME/Verifying Library Installation.png>)


## Getting Started!
In this section, you learn the basics of robotics namely **(1) Sensor Interfacing** and **(2) Motor Actuation**

### Introduction to CloudRunner Development Board and its Important Parts

### Motor Tests

### Sensor Tests
First, go to ***File*** > ***Examples*** > ***CloudRunner*** option which will show you sketches from the library you included. For the Sensor Array Test, click the ***sensor_array_test***.

![Navigating to PreMade Sketches / Examples](<Images-ReadME/Navigating to Sensor Example.PNG>)

Once you open the file, this is what it will look like.
 
**NEEDS UPDATED PICTURE HERE**

Now, connect the board to your laptop using a micro-USB cable. Go to the dropdown menu at the upper-left corner and click ***Select other board and port...***.
![Board and Port Navigation](<Images-ReadME/Board and Port.PNG>)

A new window will appear similar to this.
![Configuring Board and Port Number](<Images-ReadME/Configuring Board and Port Number.PNG>)
On the boards search bar, Select ***Arduino Nano*** 

* If can not selected or not seen, you may need to install the board at the boards manager of the Arduino IDE specifically **Arduino AVR Boards by Arduino** which supports a wide array of development boards including the nano)
    ![Installing Arduino AVR Boards](<Images-ReadME/Installing Supported Boards.PNG>)

Next, Select the correct port number on the right side of the window.
* You may verify the correct number via pressing ***Windows + X Key*** > ***Device Manager*** > ***Under Ports*** then check which port number shows up when you connect the microUSB
* Some microUSB cables can not be recognized so make sure your laptop recognizes the cable! *Note: we have observed this problem to be frequent for charger cables included when you bought a phone*

After that click the ***Upload Button*** ![Upload Button](<Images-ReadME/Upload Button.PNG>) at the upper left corner and wait for it to complete compiling and uploading. Once done, the data that the sensor is detecting will be displayed in the Serial monitor *(make sure that the baud rate = 9600!)*. Observe the **Centroid or Position** being displayed while you move the robot from left to right in the line. Notice that higher position is shown when the line is at the left of the robot. 

##Output picture

After getting the centroid, take note of the centroid when the robot is at the center of the line since it will be later used in the PID Line Tracer Sketch/ Main Program. 
### Main Program

## Documentation / References

## Contact Information

## Acknowledgements


