# SPBTLE-RF
Arduino library to support the Bluetooth (V4.1 compliant) SPBTLE-RF module

## API

The library provides a basic BLE class to configure and enable the Bluetooth module.
Each profile provides its own class. See Beacon and sensorDemo profiles.

## Examples

The library includes two sketches. They are very similar, one sketch provides a Beacon Service, an other a Sensor Service.

For the Beacon Service sketch, we can see on the monitor window all the initialization phase, and the message
"Beacon service start!" when the bluetooth module is started and ready.
Two mode are supported, UID mode and URL mode. On both mode, user can choose the bluetooth MAC address of the device by
configuring SERVER_BDADDR in the Arduino sketch.
On UID mode, user can choose the Namespace and the Instance. This data are sent to the associated device (for example your smartphone).
On URL mode, user can choose the webURL sended.

You can test this application by connecting it with your smartphone.
On Android, donwload the Beacon Scanner Apps (iBeacon & Eddystone Scanner by flurp laboratories). The Apps can
also be found here https://play.google.com/store/apps/details?id=de.flurp.beaconscanner.app
Then start the app, it will ask you to enable the bluetooth on your smartphone. Start scanning and you will see the the device.
If you use it on UID mode, you will the see the Namespace and the instance.
If you use it on URL mode, you will see the URL, you can click on it and you will send the web page.


For the Sensor Service sketch, we can see on the monitor window all the initialization phase, and a message for each service started.
Three services are started : Acc, Environnemental and Time.
For testing the sketch, you can download on the playstore the "BLE Monitor" application provided by WT Microelectronics.
Launch the application and enable Bluetooth on your smartphone. Connect it to the BLueNRG device. You will see all the services,
you can click on each one (for example St Sensor Env Service, then Temperature) and read the data.


The SPBTLE-RF uses SPI. You need to configure the pin used for spi link.
  SPIClass SPI_3(44, 43, 42);

Choose the SPI used by the SPBTLE-RF, and the pinout of the device. A cheep select pin, spiIRQ pin, reset PIN and a led (optional) are required.
  SPBTLERFClass BTLE(&SPI_3, 50, 57, 31, LED4);

Start the bluetooth module.
  BTLE.begin(DISABLE_LOW_POWER_MODE);

Start the service. For example the BeaconService in UID_TYPE.
  BeaconService.begin(SERVER_BDADDR, BEACON_ID, NameSpace);

## Documentation

You can find the source files at  
https://github.com/stm32duino/SPBTLE-RF

The SPBTLE-RF module datasheet is available at  
http://www.st.com/content/st_com/en/products/wireless-connectivity/bluetooth-bluetooth-low-energy/spbtle-rf.html
