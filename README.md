# Iot-Switch
Iot Switch that can be controlled by using mqtt pub client and webhooks. IFTTT service can be used to integrate Google Assistance and Cortana to trigger our switch.
Make sure you have shell access to your linux server.

## Requirement

ESP32
MQTT Broker by [eclipse](https://github.com/eclipse/mosquitto)
Linux Webhook by [Ananh](https://github.com/adnanh/webhook)
IFTTT account (IFTTT)(https://ifttt.com)

## Installation

1) git clone to your server
'''
  git clone https://github.com/QrioDev/Iot-Switch.git
'''

2) Installing MQTT Broker (mosquitto)
'''
    sudo apt-add-repository ppa:mosquitto-dev/mosquitto-ppa
    sudo apt-get update
    sudo apt-get install mosquitto
'''
Use crontab to start your mosquitto service at reboot.

3) Installing Webhook

Webhook allows you to create HTTP endpoints which can be used by IFTTT
'''
    sudo apt-get install webhook
'''

After webhook is installed, run the iotswitch1.json by using this command. Make sure to change the directory
'''
  webhook -hooks <directory of your json file>/iotswitch1.json -port 9005
'''

And make sure the directory of iotswtch1.sh is matched inside your iotswitch1.json.
After your webhook is deployed, it will create an endpoint at
'''
  http://<yourip>:9005/hooks/iotswitch1
'''

The switch can be controlled via webook by putting a query at the end of the webhook
'''
  http://<yourip>:9005/hooks/iotswitch1?status=on //for turning the switch on
  http://<yourip>:9005/hooks/iotswitch1?status=off // " off
'''

4) Uploading the arduino code into your ESP32. Make sure to change the SSID, password and the ip of your broker inside the code.

5) Integrating with IFTTT

Create an Applet, using Google Assistant or Cortana as trigger, and choose webhook (make web request) as action.

Inside the "What do you want to say" textfield, I preferred to use "Turn $" to make it simple. $ will become a variable that will be sent at the end of your webhook below. So the valid command will be "Turn on" and "Turn off"
Inside the URL of the web request, put
'''
http://<your-ip>:9005/hooks/iotswitch1?status= {{TextField}}
'''
