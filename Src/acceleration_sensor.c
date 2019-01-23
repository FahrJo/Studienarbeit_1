#include "acceleration_sensor.h"

// Schreibt Daten an ein Register eines I2C Ger�tes
// HAL_OK, HAL_ERROR, HAL_BUSY, HAL_TIMEOUT -> HAL_StatusTypeDef;
// 
HAL_StatusTypeDef i2c_write_register(I2C_HandleTypeDef hi2c3, uint8_t device_slave_adress, uint8_t register_pointer 
																		,uint16_t register_data_to_write, uint16_t number_bytes_to_write)
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_I2C_Mem_Write(&hi2c3, device_slave_adress, (uint16_t)register_pointer, I2C_MEMADD_SIZE_8BIT 
															,(uint8_t*)(&register_data_to_write), number_bytes_to_write, 100); 

    /* Check the communication status */
    if(status != HAL_OK)
    {
        // Error handling, for example re-initialization of the I2C peripheral
    }
		return status;
}

// beliebiges Register lesen. 
HAL_StatusTypeDef i2c_read_register(I2C_HandleTypeDef hi2c3,uint8_t device_slave_adress, uint8_t register_pointer
																		,uint8_t *register_data_read_buffer, uint8_t number_bytes_to_read)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	status = HAL_I2C_Mem_Read(&hi2c3, device_slave_adress, (uint16_t)register_pointer, 
														I2C_MEMADD_SIZE_8BIT, (uint8_t*) register_data_read_buffer, 
														number_bytes_to_read, 200);
	/* Check the communication status */
	if(status != HAL_OK)
	{
			// Error handling, for example re-initialization of the I2C peripheral
	}
	return status;
}

//Accelerometer alle Werte auslesen. Returns HAL_OK when successfull
HAL_StatusTypeDef getAllAccelerometerValues(I2C_HandleTypeDef hi2c3, s_accelerometerValues *accValues)
{
	HAL_StatusTypeDef status = HAL_OK;
	int8_t receiveBuffer[6];
	
	status = i2c_read_register(hi2c3, ACCELEROMETER_I2C_ADRESS, ACC_OUT_X_MSB, (uint8_t*)&receiveBuffer, 6);
	
	if (status!=HAL_OK)return status; // bei Fehler Funktion hier mit status Rueckgabe verlassen
//X_Wert_14_0 = (X_MSB<<5) || (X_LSB>>2)
//bei Fehlerhaften Werten, evtl vor "<<" typecast auf int16 
	accValues->x_Value = receiveBuffer[0]<<5 || receiveBuffer[1]>>2;
	accValues->y_Value = receiveBuffer[2]<<5 || receiveBuffer[3]>>2;
	accValues->z_Value = receiveBuffer[4]<<5 || receiveBuffer[5]>>2;

	return	HAL_OK;
}

// Convert Received Data in Acceleration float
// Pram: breiteInBit ist entweder 8 oder 12bit / messbereich +-2, 4, 8 g
// f�r das speichern der Werte w�re uint16_t sinnvoller als float
float convertAccelToFloat(uint16_t rohDaten, uint8_t breiteInBit, uint8_t messbereich)
{
	// 0111 1111 1111 = +1.999 / 3.998 / 7.996
	// 0000 0000 0001 = 0.001 / 0.002 / 0.004
	// 1111 1111 1111 = -0.001 / -0.002 / -0.004
	// 1000 0000 0000 = -2 / -4 / -8
	return 0;
}