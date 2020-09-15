# temp-track
Temp-track is a IOT temperature sensor designed for use in the transition back to office during the pandemic. Created as a summer stretch project for Cisco, Temp-track allows for custom API intergrations with it's Wifi capabilities, and features the work of Tyler Patel, Tara Rafi, Olivia Xia, Michael Fitzgerald and Kelvin Cui.

<img src="https://i.ibb.co/FbZ2crr/covid.jpg" alt="covid" border="0">

## Background info
With the advent of the Coronavirus, the entire world shifted to working from home, in an effort to limit the possibilities of infection. COVID-19 is highly contagious, and the current best way to prevent infection is through screening. An obvious symptom of COVID-19 is a fever, with a raised internal temperature. Temperature-based screening is the most common solution right now to quickly screen people before they are allowed entry, often seen at storefronts and airports. Since COVID-19 is easily transmitted through bodily fluids, the FDA recommends "telethermographic temperature sensors", or temperature sensors that can assess skin temperature remotely. 

### Existing Solutions
There are two popular solutions that currently exist for this purpose : simple IR sensor-based forehead thermometers, and advanced IR camera-based screening stations.

<img src="https://i5.walmartimages.com/asr/cf1244e1-c9be-4a1f-a942-5abf91fb68e5.a9cd36071ce4695b630caee80f58c265.jpeg?odnHeight=450&odnWidth=450&odnBg=ffffff" alt="covid" border="0">
The point-and-shoot forehead thermometers are simple to use, but they can be very inaccurate, often featuring a variance of 0.5 degrees celcius. They're also standalone units, and allows for zero intergration.

<img src="https://www.lastmilegear.com/wp-content/uploads/Seek-YW-AAA-300x300.jpg" alt="covid" border="0">
The camera-based screening stations meanwhile are quite advanced, featuring a variance of 0.3 degrees celcius, and allow for intergrations. However, they are very expensive, are difficult to operate, and take up a lot of space.

## Proposed Solution
We believe that our solution features the best of both currently popular solutions. It features the ease of use of the forehead thermometers, while maintaining the accuracy of the IR cameras with it's medical grade IR Sensor. It also features an ESP-01 Chip, allowing for custom API intergrations, and best of all, costs the same on average as the forehead thermometers.

## How it works

## Hardware
The hardware is based on an Arduino Nano, as well as an ESP-01 Wifi Chip. The temperature is recorded using the Medical Grade MLX90614 IR Sensor, allowing for 0.2 degrees celcius variance. When the button is pressed, the temperature is recorded, and then sent over Wifi to a server of your choice. If the temperature is below 38 degrees celcius, then a green light flashes, indicating a pass for the screening. Conversely, if the temperature is above 38, then a red light flashes. Our current example is set up to send the temperature info to our Node Red Server through a GET call, which brings us into...

## Software
In our example, our Node Red server is set up to take the temperature info, and using the Webex Teams API, send that temperature as a message to a user through Teams.

## Our Vision
Our vision is to create an all-in-one screening and contact tracing solution, utilizing two key APIs: DNASpaces and Webex Teams. With DNASpaces, we're able to annonymusly track the location of any user connected to a Cisco router in a building, and determine which people have come into contact with each other. With Webex Teams, we're also able to update the building manager of any alerts, send helpful resources to anyone who's registered a high temperature, and even allow people to self-report if they've tested positive. 

### Example Workflow
An example workflow for any office with Cisco routers could be the following. An employee would scan their ID badge, and get their temperatures taken. If their temperature is abnormally high, they are prevented entry into the building. The building management gets an alert that one of their employees have registered a high temperature, and that employee would be sent helpful resources, and recommended to get tested, both through Webex Teams. If the test returns positive, the employee may self-report through Teams. If they self-report, the employee's stored location data from DNASpaces is automatically compared, and any other employee who they've come into contact with will get a notification through Teams, alerting them that someone they've come into contact with has tested positive.

## Lessons Learned
This was a very educational project, and there are a few things that we'd do differently in the future. In terms of hardware, instead of using an Arduino Nano and ESP-01, an ESP-32 is a single board that accomplishes the same thing, with less cost. Using the ESP-32 would also allow us to power the device using a Li-Po, making the device standalone instead of having to be powered over USB. The ESP-32 also has a deep-sleep mode, so that it would only draw power when the button is clicked, allowing long battery life. Furthermore, some ESP-32s feature a built-in OLED screen, which would allow for a display of the actual temperature. In terms of the server, instead of sending API calls directly to Node Red, we'd set up a MQTT server that would act as a bridge between the two to allow for more efficent communications.

