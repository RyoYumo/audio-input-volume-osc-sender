# AudioInputVolumeOscSender

### Overview
This simple software can smooth the input volume from the selected audio device and send it to other applications via OSC.
It can accelerate the production of audio reactive contents.

### Install
Download this repo and add "AudioVolumeOscSender" directory to of_v0.9.8_osx_release/apps/myApps.

<img width="618" alt="screen shot 2018-11-27 at 16 01 57" src="https://user-images.githubusercontent.com/23047341/49064396-223beb80-f25e-11e8-8afb-3efd9bb215ad.png">

### Usage and UI
When activated, a dialog box with a list of available audio devices is displayed, and you can select the device to use by entering the ID.

<img width="422" alt="screen shot 2018-11-27 at 13 47 27" src="https://user-images.githubusercontent.com/23047341/49059386-fe6faa00-f24b-11e8-88ef-43b4c579fc5d.png">


The connected IP address, port and key info are displayed.

<img width="320" alt="screen shot 2018-11-27 at 13 47 40" src="https://user-images.githubusercontent.com/23047341/49059399-17785b00-f24c-11e8-9b00-02182243f662.png">

![ezgif com-optimize](https://user-images.githubusercontent.com/23047341/49064181-65498f00-f25d-11e8-87be-97c8789ddbba.gif)

### Dependency
- openFrameworks v0.9.8
- ofxOsc
- ofxPubSubOsc