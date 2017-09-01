#include <SPI.h>
#include <SPBTLE_RF.h>
#include <sensor_service.h>

/* Configure SPI3:
  MOSI: PC12
  MISO: PC11
  SCLK: PC10
  */
SPIClass SPI_3(44, 43, 42);
SPBTLERFClass BTLE(&SPI_3, 50, 57, 31, LED4);

const char *name = "BlueNRG";
uint8_t SERVER_BDADDR[] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x03};

AxesRaw_t axes_data;

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
    Serial.println("Acc service added successfully.\n");
  else
    Serial.println("Error while adding Acc service.\n");

  ret = SensorService.Add_Environmental_Sensor_Service();

  if(ret == BLE_STATUS_SUCCESS)
    Serial.println("Environmental Sensor service added successfully.\n");
  else
    Serial.println("Error while adding Environmental Sensor service.\n");

  /* Instantiate Timer Service with two characteristics:
   * - seconds characteristic (Readable only)
   * - minutes characteristics (Readable and Notifiable )
   */
  ret = SensorService.Add_Time_Service();

  if(ret == BLE_STATUS_SUCCESS)
    Serial.println("Time service added successfully.\n");
  else
    Serial.println("Error while adding Time service.\n");
}

void loop() {
  BTLE.update();
  User_Process(&axes_data);
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
  SensorService.setConnectable();

  if(SensorService.isConnected() == TRUE)
  {
    SensorService.Update_Time_Characteristics();

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
}
