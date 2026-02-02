/**
 * This example demonstrates how to use the SPI driver to transfer data between
 * the SotaSoC01 and the SPI slave.
 *
 * It only runs with cocotb. However, you can refer to this example to
 * understand how to use the SPI driver.
 */

#include "gpio_driver.h"
#include "spi_driver.h"

#define CS_N_PIN 2

// This magic number is set by cocotb to identify the test to run
#define SPI_TEST_ID (*(volatile unsigned int *)0x01400000)

static void spi_transfer_multiple_bytes(int mode) {
  const int BUF_SIZE = 256;
  spi_init(SPI_DIV_8MHZ);
  spi_set_mode(mode);

  // Configure GPIO[CS_N_PIN] as output for CS_N (active low)
  gpio_set_direction(CS_N_PIN, GPIO_OUTPUT);
  gpio_set_pin(CS_N_PIN);  // CS_N high (deasserted)

  unsigned char rx_buf[BUF_SIZE];

  gpio_clear_pin(CS_N_PIN);  // Assert CS_N
  // Read 32 bytes from SPI slave
  for (int i = 0; i < BUF_SIZE; i++) {
    rx_buf[i] = spi_transfer_byte((unsigned char)i);
  }
  gpio_set_pin(CS_N_PIN);  // Deassert CS_N

  gpio_clear_pin(CS_N_PIN);
  // Send the received data back
  for (int i = 0; i < BUF_SIZE; i++) {
    spi_transfer_byte(rx_buf[i]);
  }
  gpio_set_pin(CS_N_PIN);
}

static void spi_transfer_single_byte(int mode) {
  const int BUF_SIZE = 32;
  spi_init(SPI_DIV_8MHZ);
  spi_set_mode(mode);

  // Configure GPIO[CS_N_PIN] as output for CS_N (active low)
  gpio_set_direction(CS_N_PIN, GPIO_OUTPUT);
  gpio_set_pin(CS_N_PIN);  // CS_N high (deasserted)

  unsigned char rx_buf[BUF_SIZE];

  // Read 32 bytes from SPI slave
  for (int i = 0; i < BUF_SIZE; i++) {
    gpio_clear_pin(CS_N_PIN);  // Assert CS_N
    rx_buf[i] = spi_transfer_byte((unsigned char)i);
    gpio_set_pin(CS_N_PIN);  // Deassert CS_N
  }

  // Send the received data back
  for (int i = 0; i < BUF_SIZE; i++) {
    gpio_clear_pin(CS_N_PIN);
    spi_transfer_byte(rx_buf[i]);
    gpio_set_pin(CS_N_PIN);
  }
}

int main(void) {
  int test_id = SPI_TEST_ID;
  switch (test_id) {
    case 0:
      spi_transfer_multiple_bytes(SPI_MODE0);
      break;
    case 1:
      spi_transfer_multiple_bytes(SPI_MODE1);
      break;
    case 2:
      spi_transfer_multiple_bytes(SPI_MODE2);
      break;
    case 3:
      spi_transfer_multiple_bytes(SPI_MODE3);
      break;
    case 4:
      spi_transfer_single_byte(SPI_MODE0);
      break;
    case 5:
      spi_transfer_single_byte(SPI_MODE1);
      break;
    case 6:
      spi_transfer_single_byte(SPI_MODE2);
      break;
    case 7:
      spi_transfer_single_byte(SPI_MODE3);
      break;
    default:
      spi_transfer_multiple_bytes(SPI_MODE0);
      break;
  }

  if (test_id <= 7) {
    // Use ecall to notify cocotb that the test is done
    __asm__ volatile("ecall");
  }

  while (1) {
    // Program completed
  }
}
