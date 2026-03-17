#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#include "wokwi-api.h"
#pragma clang diagnostic pop

typedef struct {
  uint8_t control_reg;
  uint32_t configured_addr;
  pin_t pin_rst;
  pin_t pin_a0;
  pin_t pin_a1;
  pin_t pin_a2;
  i2c_dev_t i2c;
} chip_state_t;

static chip_state_t state;

static bool on_i2c_connect(void *user_data, uint32_t address, bool read) {
  chip_state_t *chip = (chip_state_t *)user_data;
  (void)read;
  return (address == chip->configured_addr); /* ACK only our address */
}

static uint8_t on_i2c_read(void *user_data) {
  chip_state_t *chip = (chip_state_t *)user_data;
  return chip->control_reg;
}

static bool on_i2c_write(void *user_data, uint8_t data) {
  chip_state_t *chip = (chip_state_t *)user_data;
  chip->control_reg = data;
  return true; /* ACK */
}

static void on_i2c_disconnect(void *user_data) {
  (void)user_data;
}

static void on_rst_change(void *user_data, pin_t pin, uint32_t value) {
  (void)pin;
  chip_state_t *chip = (chip_state_t *)user_data;
  if (value == LOW) {
    chip->control_reg = 0x00;
  }
}

void chip_init(void) {
  chip_state_t *chip = &state;
  chip->control_reg = 0x00;

  /* Initialize pins */
  pin_init("VIN", INPUT);
  pin_init("GND", INPUT);
  pin_t pin_sda = pin_init("SDA", INPUT);
  pin_t pin_scl = pin_init("SCL", INPUT);
  chip->pin_rst = pin_init("RST", INPUT_PULLUP);
  chip->pin_a0 = pin_init("A0", INPUT_PULLDOWN);
  chip->pin_a1 = pin_init("A1", INPUT_PULLDOWN);
  chip->pin_a2 = pin_init("A2", INPUT_PULLDOWN);
  pin_init("SD0", INPUT);
  pin_init("SC0", INPUT);
  pin_init("SD1", INPUT);
  pin_init("SC1", INPUT);
  pin_init("SD2", INPUT);
  pin_init("SC2", INPUT);
  pin_init("SD3", INPUT);
  pin_init("SC3", INPUT);
  pin_init("SD4", INPUT);
  pin_init("SC4", INPUT);
  pin_init("SD5", INPUT);
  pin_init("SC5", INPUT);
  pin_init("SD6", INPUT);
  pin_init("SC6", INPUT);
  pin_init("SD7", INPUT);
  pin_init("SC7", INPUT);

  /* Determine I2C address from A0, A1, A2 pins */
  uint32_t a0 = pin_read(chip->pin_a0);
  uint32_t a1 = pin_read(chip->pin_a1);
  uint32_t a2 = pin_read(chip->pin_a2);
  uint32_t addr = 0x70 | (a2 << 2) | (a1 << 1) | a0;
  chip->configured_addr = addr;

  /* Initialize I2C slave */
  const i2c_config_t i2c_config = {
    .address = addr,
    .sda = pin_sda,
    .scl = pin_scl,
    .connect = on_i2c_connect,
    .read = on_i2c_read,
    .write = on_i2c_write,
    .disconnect = on_i2c_disconnect,
    .user_data = chip,
  };
  chip->i2c = i2c_init(&i2c_config);

  /* Watch RST pin for falling edge (active-low reset) */
  const pin_watch_config_t rst_watch = {
    .edge = FALLING,
    .pin_change = on_rst_change,
    .user_data = chip,
  };
  pin_watch(chip->pin_rst, &rst_watch);
}
