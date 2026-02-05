#include "hw_registers.h"
#include "i2c_driver.h"

/**
 * @brief Test: Read 1 byte from I2C slave device, then write it back
 * @param device_addr 7-bit I2C device address
 * @return I2C_OK if both read and write succeed, negative on error
 */
int test_read_write_byte(unsigned char device_addr) {
  unsigned char read_byte;

  // Read single byte from slave
  int result = i2c_read_byte(device_addr, &read_byte);

  if (result == I2C_OK) {
    // Read successful - read_byte contains the data
    // Now write the same value back to the slave
    result = i2c_write_byte(device_addr, read_byte);

    if (result == I2C_OK) {
      // Write successful - value has been written back
      return I2C_OK;
    } else {
      // Write failed
      return result;
    }
  } else {
    // Read failed - could be:
    // - No device at this address (I2C_NACK)
    // - Bus error (I2C_ERROR)
    // - Arbitration lost (I2C_ARB_LOST)
    // This is expected if no device is connected
    return result;
  }
}

/**
 * @brief Test: Repeated START - read 1 byte then write it back without STOP in
 * between. Transaction: START -> addr+read -> read 1 byte -> Sr -> addr+write
 * -> write byte -> STOP Implemented with direct register access (non-standard
 * sequence, not in driver).
 * @param device_addr 7-bit I2C device address
 * @return I2C_OK if both read and write succeed, negative on error
 */
int test_repeated_start_read_write_byte(unsigned char device_addr) {
  volatile int *ctrl = (volatile int *)I2C_CTRL;
  volatile int *status = (volatile int *)I2C_STATUS;
  volatile int *data = (volatile int *)I2C_DATA;

  /* Wait bus free */
  while ((*status & I2C_STATUS_BUSY) != 0);

/* Helper: wait DONE, return error or I2C_OK */
#define WAIT_DONE()                                           \
  do {                                                        \
    while ((*status & I2C_STATUS_DONE) == 0) {                \
      if (*status & I2C_STATUS_ERROR) return I2C_ERROR;       \
      if (*status & I2C_STATUS_ARB_LOST) return I2C_ARB_LOST; \
    }                                                         \
  } while (0)

  /* START + addr + read */
  *data = (int)((device_addr << 1) | 0x01);
  *ctrl = I2C_CTRL_START;
  WAIT_DONE();
  if ((*status & I2C_STATUS_ACK) == 0) {
    *ctrl = I2C_CTRL_STOP;
    WAIT_DONE();
    return I2C_NACK;
  }

  /* Read 1 byte, NACK (no STOP) */
  *ctrl = I2C_CTRL_READ | I2C_CTRL_START;
  WAIT_DONE();
  unsigned char byte = (unsigned char)*data;

  /* Repeated START + addr + write */
  *data = (int)((device_addr << 1) | 0x00);
  *ctrl = I2C_CTRL_RESTART | I2C_CTRL_START;
  WAIT_DONE();
  if ((*status & I2C_STATUS_ACK) == 0) {
    *ctrl = I2C_CTRL_STOP;
    WAIT_DONE();
    return I2C_NACK;
  }

  /* Write byte */
  *data = (int)byte;
  *ctrl = I2C_CTRL_START;
  WAIT_DONE();
  if ((*status & I2C_STATUS_ACK) == 0) {
    *ctrl = I2C_CTRL_STOP;
    WAIT_DONE();
    return I2C_NACK;
  }

  /* STOP */
  *ctrl = I2C_CTRL_STOP;
  WAIT_DONE();
  while ((*status & I2C_STATUS_BUSY) != 0);

#undef WAIT_DONE
  return I2C_OK;
}

/**
 * @brief Test: Read 32 bytes from I2C slave device, then write them back
 * @param device_addr 7-bit I2C device address
 * @return I2C_OK if both read and write succeed, negative on error
 */
int test_read_write_bytes(unsigned char device_addr) {
  const int BUF_SIZE = 16;
  unsigned char buffer[BUF_SIZE];

  // Read 32 bytes from slave
  int result = i2c_read(device_addr, buffer, BUF_SIZE);

  if (result == I2C_OK) {
    // Read successful - buffer contains the data
    // Now write the same 32 bytes back to the slave
    result = i2c_write(device_addr, buffer, BUF_SIZE);

    if (result == I2C_OK) {
      // Write successful - 32 bytes have been written back
      return I2C_OK;
    } else {
      // Write failed
      return result;
    }
  } else {
    // Read failed - could be:
    // - No device at this address (I2C_NACK)
    // - Bus error (I2C_ERROR)
    // - Arbitration lost (I2C_ARB_LOST)
    // This is expected if no device is connected
    return result;
  }
}

int main() {
  // Initialize I2C at 100kHz
  i2c_init(I2C_PRESCALE_100KHZ);

  // Run test: Read 1 byte then write it back
  unsigned char device_addr = 0x21;
  test_read_write_byte(device_addr);
  // test_read_write_bytes(device_addr);
  // test_repeated_start_read_write_byte(device_addr);

  // Infinite loop
  while (1) {
    // Program completed - test done
    __asm__ volatile("nop");
  }
}
