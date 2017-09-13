/*

 DISCO_IOT_SensorDemo

 This sketch provides a default example how to use the BLE module of the
 Discovery L475VG IoT board.

 For the Sensor Service sketch, 3 services are started : Acc, Environnemental and Time.
 For testing the sketch, you can download on the playstore the "BLE Monitor"
 application provided by WT Microelectronics.
 Launch the application and enable Bluetooth on your smartphone. Connect it to
 the BLueNRG device. You will see all the services, you can click on each one
 (for example St Sensor Env Service, then Temperature) and read the data.

 You can choose the bluetooth MAC address of the device by configuring SERVER_BDADDR.

 Accelerometer values are updated on user action (press user button).
 Environnemental values (Temperature, humidity and pressure) are updated each seconds.
 Each minute a notification is sent to the user.

 */

#include <SPI.h>
#include <SPBTLE_RF.h>
#include <sensor_service.h>

/* Configure SPI3
  MOSI: PC12
  MISO: PC11
  SCLK: PC10
  */
SPIClass SPI_3(44, 43, 42);

// Configure BTLE pins
SPBTLERFClass BTLE(&SPI_3, 50, 57, 31, LED4);

const char *name = "BlueNRG";
uint8_t SERVER_BDADDR[] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x03};

bool set_connectable = true;

AxesRaw_t axes_data;
uint32_t previousSecond = 0;

void setup() {
  int ret;

  Serial.begin(9600);

  if(BTLE.begin() == SPBTLERF_ERROR)
  {
    Serial.println("Bluetooth module configuration error!");
    while(1);
  }

  if(SensorService.begin(name, SERVER_BDADDR))
  {
    Serial.println("Sensor service configuration error!");
    while(1);
  }

  /* Configure the User Button in GPIO Mode */
  pinMode(USER_BTN, INPUT);

  ret = SensorService.Add_Acc_Service(&axes_data);

  if(ret == BLE_STATUS_SUCCESS)
    Serial.println("Acc service added successfully.");
  else
    Serial.println("Error while adding Acc service.");

  ret = SensorService.Add_Environmental_Sensor_Service();

  if(ret == BLE_STATUS_SUCCESS)
    Serial.println("Environmental Sensor service added successfully.");
  else
    Serial.println("Error while adding Environmental Sensor service.");

  /* Instantiate Timer Service with two characteristics:
   * - seconds characteristic (Readable only)
   * - minutes characteristics (Readable and Notifiable )
   */
  ret = SensorService.Add_Time_Service();

  if(ret == BLE_STATUS_SUCCESS)
    Serial.println("Time service added successfully.");
  else
    Serial.println("Error while adding Time service.");

  /* Instantiate LED Button Service with one characteristic:
   * - LED characteristic (Readable and Writable)
   */
  ret = SensorService.Add_LED_Service();
  pinMode(LED_BUILTIN, OUTPUT);

  if(ret == BLE_STATUS_SUCCESS)
    Serial.println("LED service added successfully.");
  else
    Serial.println("Error while adding LED service.");
}

void loop() {
  BTLE.update();

  if(SensorService.isConnected() == TRUE)
  {
    //Update accelerometer values
    User_Process(&axes_data);

    if((millis() - previousSecond) >= 1000)
    {
      //Update environnemental data
      previousSecond = millis();
      SensorService.Temp_Update(205);     //20,5°
      SensorService.Press_Update(1024);   //1024mBar
      SensorService.Humidity_Update(530); //53.0%

      //Update time
      SensorService.Update_Time_Characteristics();
    }

    //Update LED state
    digitalWrite(LED_BUILTIN, SensorService.LED_State());
  }
  else if(set_connectable)
  {
    SensorService.setConnectable();
    set_connectable = false;
  }
}

/**
 * @brief  Process user input (i.e. pressing the USER button on Nucleo board)
 *         and send the updated acceleration data to the remote client.
 *
 * @param  AxesRaw_t* p_axes
 * @retval None
 */
void User_Process(AxesRaw_t* p_axes)
{
  /* Check if the user has pushed the button */
  if(digitalRead(USER_BTN) == RESET)
  {
    while (digitalRead(USER_BTN) == RESET);

    /* Update acceleration data */
    p_axes->AXIS_X += 100;
    p_axes->AXIS_Y += 100;
    p_axes->AXIS_Z += 100;
    SensorService.Acc_Update(p_axes);
  }
}
