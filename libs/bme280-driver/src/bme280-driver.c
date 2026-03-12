#include "bme280-driver.h"
#include "bme280-regs.h"
#include "stdio.h"

typedef struct
{
    bme280_i2c_read i2c_read;
    bme280_i2c_write i2c_write;
} bme280_ctx_t;

static bme280_ctx_t bme280_ctx = {0};

void bme280_init(bme280_i2c_read i2c_read, bme280_i2c_write i2c_write)
{
    bme280_ctx.i2c_read = i2c_read;
    bme280_ctx.i2c_write = i2c_write;

    uint8_t id_reg_buf[1] = {0};
    bme280_read_regs(BME280_REG_id, id_reg_buf, sizeof(id_reg_buf));

    if (id_reg_buf[0] != 0x60)
    {
        printf("BME280 ERROR: Invalid device ID (expected 0x60, got 0x%02X)\n", id_reg_buf[0]);
    }
    printf("BME280 detected (ID: 0x60)\n");

    uint8_t ctrl_hum_reg_value = 0;
    ctrl_hum_reg_value |= (0b001 << 0); // osrs_h[2:0] = oversampling 1
    bme280_write_reg(BME280_REG_ctrl_hum, ctrl_hum_reg_value);

    uint8_t config_reg_value = 0;
    config_reg_value |= (0b0 << 0);   // spi3w_en[0:0] = false
    config_reg_value |= (0b000 << 2); // filter[4:2] = Filter off
    config_reg_value |= (0b001 << 5); // t_sb[7:5] = 62.5 ms
    bme280_write_reg(BME280_REG_config, config_reg_value);

    uint8_t ctrl_meas_reg_value = 0;
    ctrl_meas_reg_value |= (0b001 << 5); // osrs_t[7:5] = oversampling temperature 1
    ctrl_meas_reg_value |= (0b001 << 2); // osrs_p[4:2] = oversampling pressure 1
    ctrl_meas_reg_value |= (0b11 << 0);  // mode[1:0] = normal mode
    bme280_write_reg(BME280_REG_ctrl_meas, ctrl_meas_reg_value);
}

void bme280_read_regs(uint8_t start_reg_address, uint8_t *buffer, uint8_t length)
{
    uint8_t data[1] = {start_reg_address};
    bme280_ctx.i2c_write(data, sizeof(data));
    bme280_ctx.i2c_read(buffer, length);
}

void bme280_write_reg(uint8_t reg_address, uint8_t value)
{
    uint8_t data[2] = {reg_address, value};
    bme280_ctx.i2c_write(data, 2);
}

uint32_t bme280_read_temp_raw() {
    uint8_t read[3] = {0};
    bme280_read_regs(BME280_REG_temp_msb, read, 3);
    uint32_t value = ((uint32_t)read[0] << 12) | ((uint32_t)read[1] << 4) | (read[2] >> 4);
    return value;
}

uint32_t bme280_read_pres_raw() {
    uint8_t read[3] = {0};
    bme280_read_regs(BME280_REG_press_msb, read, 3);
    uint32_t value = ((uint32_t)read[0] << 12) | ((uint32_t)read[1] << 4) | (read[2] >> 4);
    return value;
}

uint16_t bme280_read_hum_raw() {
    uint8_t read[2] = {0};
    bme280_read_regs(BME280_REG_hum_msb, read, 2);
    uint16_t value = ((uint16_t)read[0] << 8) | read[1];
    return value;
}