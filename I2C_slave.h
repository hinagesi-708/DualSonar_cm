#define SDA(x)		x ## A2
#define SCL(x)		x ## A1

#define I2C_ADDR_DEV1    0x50

//signed char reseaved_data[4] = {0};
signed char reseaved_data[1] = {0};

unsigned char send_data[4] = {0};
//unsigned char send_data[4] = {0};

void I2C_init();
void set_send_string(signed char Str[8]);
