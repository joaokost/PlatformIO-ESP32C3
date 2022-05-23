#include <stdint.h>
#include <stdbool.h>
#include "driver/i2c.h"
#include "i2c_driver.h"


void i2c_init(void){
    //Configure I2C Driver
    i2c_config_t i2c_config = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = I2C_MSATER_SDA_IO,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_io_num = I2C_MASTER_SCL_IO,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = I2C_MASTER_FREQ_HZ
	};

    //Write Configuration
    i2c_param_config(I2C_MASTER_PORT, &i2c_config);

    //Install I2C Driver
    i2c_driver_install(0, i2c_config.mode, 0, 0, 0);

}

void i2c_write(uint8_t address, uint8_t *data, uint8_t size){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_MASTER_PORT, cmd, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
}

void i2c_read(uint8_t address, uint8_t *data, uint8_t size){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | READ_BIT, ACK_CHECK_EN);
    if(size > 1){
        i2c_master_read(cmd, *data, size -1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, (data +  size - 1), NACK_VAL);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_MASTER_PORT, cmd, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
}
	
	