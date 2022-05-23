# BakkesMod plugin COM port connection

#### Created with template using Visual Studio 2019. Do not use VS 2022

This Bakkesmod application allows you to connect to a COM port which should allow you to connect to a microcontroller such as an Arduino or Raspberry Pi
This main purpose of this application is so that you can gather information about the game and store that information on a microcontroller that can eventually be manipulated or used to do cool things.

As of writing this, this application is currently the equivelent of  0.0.0.1 version and should be improved at a later date. 
At the moment though it does connect to a micro controller and does work. 
My testing which is this plug in right here allowes the microcontroller to see if the boost value has changed for the local car with some success. 

Some notes:

**Make sure your function calls are the right calls if you are to use GameWrapper::HookEventWithCaller or HookEventWithCallerPost**

It is commented out but looks for ALL functions that come from Rocket League. 
If you call the wrong function, your game WILL crash. To be safe just use HookEvent.

**As of writing this, using a function that is called too much can crash the game**

At the moment, testing crashed the game at players loading in at the start of the game or during a replay. If you want to send bulk data but dont need it in real time. My advice would be to store in the plugin and then at the end of the game to send to the arduino.

There are heaps of YouTube tutorials on how to load a pluging.

To use the plugin:

1) On starting the Game, head to RLDataGathering in the plugin tab
2) Enter the COM port you wish to use (if you are using anything above COM9 then you need to enter //.////COM10 or whatever port it is otherwise its just entered normally ("COM3"). Don't ask me, but it works lol)
3) The console should say "Connection established". If not, check the message where the error could have come from.

This was just a fun little project so if you want me to work on it more then I can otherwise I'm just gonna start adding to it to pull data in real time.
