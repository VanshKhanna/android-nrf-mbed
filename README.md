# android-nrf-mbed
Basic set up to control the LEDs on a nRF51 DK using BLE. The board is advertising as well as able to scan for other devices.

##NOTE##
The app as well as the nRF file is just a small part of my research. This was an experimental app, with really naive android programming solely aimed at helping others in my group to understand android BLE API, with no previous knowledge of android programming or BLE devices. 

I felt that the explanation for the BLE API by Google was rather confusing. My fellow group members also shared my opinion.

You are more than welcome to fix the issues and make it a proper nice looking app. I will keep fixing the issues myslef.

If you are facing a problem note mentioned in the *issues* section below, please add it.

##BLETest
Contains the android app to allow you to connect directly to the board and change LEDs.

**NOTE:** The app is designed to automatically connect to my board for the device address of my board. You will have to change.

###mbed files
Contains the main.cpp for the **nRF51 DK**. This was developed using mbed. Remeber to include the following libraries:
  * BLE_API
  * nRF51822
  * mbed

##Issues##
  * Nordic recently changed its API a bit. The LED's are not exactly behaving as they should. For instance, turning on LED 2 also turns on LED 1.
  * This is less of an issue and more of a design prefernce. The disconnect button does not completely disconnect. To do so, either change the app or just turn off your bluetooth. When I say *completely disconnect*, I mean that the board's disconnection_callback wont be called unless you turn off your phone's bluetooth (or change the way the disconnect button works).
  * **Warning**: The menu is absolutely useless. I was using it earlier to experiment with certain things, but then switched to using buttons.
  * The app will *probably* crash if you do anything **weird**. Here, **weird** entails:
    * Using scan button in the menu.
    * Connecting with the Bluetooth off.
    * And a few more cases which I cant remeber as of now and will add as I experience them.
