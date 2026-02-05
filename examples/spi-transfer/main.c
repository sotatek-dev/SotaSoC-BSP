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

static void test_spi_transfer_fast_hword(int mode) {
  const int BUF_SIZE = 256;
  spi_init(SPI_DIV_8MHZ);
  spi_set_mode(mode);

  // Configure GPIO[CS_N_PIN] as output for CS_N (active low)
  gpio_set_direction(CS_N_PIN, GPIO_OUTPUT);
  gpio_set_pin(CS_N_PIN);  // CS_N high (deasserted)

  unsigned short tx_buf[BUF_SIZE / 2];
  unsigned short rx_buf[BUF_SIZE / 2];

  unsigned char* tx_buf_bytes = (unsigned char*)tx_buf;
  unsigned char* rx_buf_bytes = (unsigned char*)rx_buf;

  for (int i = 0; i < BUF_SIZE; i++) {
    tx_buf_bytes[i] = i;
  }

  gpio_clear_pin(CS_N_PIN);  // Assert CS_N
  spi_transmit_fast(tx_buf_bytes, BUF_SIZE);
  gpio_set_pin(CS_N_PIN);

  gpio_clear_pin(CS_N_PIN);  // Assert CS_N
  spi_transmit_fast(rx_buf_bytes, BUF_SIZE);
  gpio_set_pin(CS_N_PIN);
}

static void test_spi_transfer_fast_word(int mode) {
  const int BUF_SIZE = 256;
  spi_init(SPI_DIV_16MHZ);
  spi_set_mode(mode);

  // Configure GPIO[CS_N_PIN] as output for CS_N (active low)
  gpio_set_direction(CS_N_PIN, GPIO_OUTPUT);
  gpio_set_pin(CS_N_PIN);  // CS_N high (deasserted)

  unsigned int tx_buf[BUF_SIZE / 4];
  unsigned int rx_buf[BUF_SIZE / 4];

  unsigned char* tx_buf_bytes = (unsigned char*)tx_buf;
  unsigned char* rx_buf_bytes = (unsigned char*)rx_buf;

  for (int i = 0; i < BUF_SIZE * 4; i++) {
    tx_buf_bytes[i] = i;
  }

  gpio_clear_pin(CS_N_PIN);  // Assert CS_N
  spi_transmit_fast(tx_buf_bytes, BUF_SIZE);
  gpio_set_pin(CS_N_PIN);

  gpio_clear_pin(CS_N_PIN);  // Assert CS_N
  spi_transmit_fast(rx_buf_bytes, BUF_SIZE);
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

static void spi_transfer_single_hword(int mode) {
  const int BUF_SIZE = 16;
  spi_init(SPI_DIV_8MHZ);
  spi_set_mode(mode);

  // Configure GPIO[CS_N_PIN] as output for CS_N (active low)
  gpio_set_direction(CS_N_PIN, GPIO_OUTPUT);
  gpio_set_pin(CS_N_PIN);  // CS_N high (deasserted)

  unsigned short tx_buf[BUF_SIZE];
  unsigned short rx_buf[BUF_SIZE];

  unsigned char* tx_buf_bytes = (unsigned char*)tx_buf;

  for (int i = 0; i < BUF_SIZE * 2; i++) {
    tx_buf_bytes[i] = i;
  }

  // Read 32 bytes from SPI slave
  for (int i = 0; i < BUF_SIZE; i++) {
    gpio_clear_pin(CS_N_PIN);  // Assert CS_N
    rx_buf[i] = spi_transfer_hword(tx_buf[i]);
    gpio_set_pin(CS_N_PIN);  // Deassert CS_N
  }

  // Send the received data back
  for (int i = 0; i < BUF_SIZE; i++) {
    gpio_clear_pin(CS_N_PIN);
    spi_transfer_hword(rx_buf[i]);
    gpio_set_pin(CS_N_PIN);
  }
}

static void spi_transfer_single_word(int mode) {
  const int BUF_SIZE = 8;
  spi_init(SPI_DIV_8MHZ);
  spi_set_mode(mode);

  // Configure GPIO[CS_N_PIN] as output for CS_N (active low)
  gpio_set_direction(CS_N_PIN, GPIO_OUTPUT);
  gpio_set_pin(CS_N_PIN);  // CS_N high (deasserted)

  unsigned int tx_buf[BUF_SIZE];
  unsigned int rx_buf[BUF_SIZE];

  unsigned char* tx_buf_bytes = (unsigned char*)tx_buf;

  for (int i = 0; i < BUF_SIZE * 4; i++) {
    tx_buf_bytes[i] = i;
  }

  // Read 32 bytes from SPI slave
  for (int i = 0; i < BUF_SIZE; i++) {
    gpio_clear_pin(CS_N_PIN);  // Assert CS_N
    rx_buf[i] = spi_transfer_word(tx_buf[i]);
    gpio_set_pin(CS_N_PIN);  // Deassert CS_N
  }

  // Send the received data back
  for (int i = 0; i < BUF_SIZE; i++) {
    gpio_clear_pin(CS_N_PIN);
    spi_transfer_word(rx_buf[i]);
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
    case 8:
      spi_transfer_single_hword(SPI_MODE0);
      break;
    case 9:
      spi_transfer_single_hword(SPI_MODE1);
      break;
    case 10:
      spi_transfer_single_hword(SPI_MODE2);
      break;
    case 11:
      spi_transfer_single_hword(SPI_MODE3);
      break;
    case 12:
      spi_transfer_single_word(SPI_MODE0);
      break;
    case 13:
      spi_transfer_single_word(SPI_MODE1);
      break;
    case 14:
      spi_transfer_single_word(SPI_MODE2);
      break;
    case 15:
      spi_transfer_single_word(SPI_MODE3);
      break;
    case 16:
      test_spi_transfer_fast_hword(SPI_MODE0);
      break;
    case 17:
      test_spi_transfer_fast_word(SPI_MODE0);
      break;
    default:
      spi_transfer_multiple_bytes(SPI_MODE0);
      break;
  }

  if (test_id <= 17) {
    // Use ecall to notify cocotb that the test is done
    __asm__ volatile("ecall");
  }

  while (1) {
    // Program completed
  }
}
