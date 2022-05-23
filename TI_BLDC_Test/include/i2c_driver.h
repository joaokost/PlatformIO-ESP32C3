#include <stdint.h>
#include <stdbool.h>


#define WRITE_BIT                   0
#define READ_BIT                    1
#define ACK_CHECK_EN                1
#define ACK_VAL                     0                             
#define NACK_VAL                    1                            
#define I2C_MSATER_SDA_IO           2
#define I2C_MASTER_SCL_IO           3
#define I2C_MASTER_FREQ_HZ          100000
#define I2C_MASTER_PORT             0


void i2c_init(void);
void i2c_write(uint8_t address, uint8_t *data, uint8_t size);
void i2c_read(uint8_t address, uint8_t *data, uint8_t size);