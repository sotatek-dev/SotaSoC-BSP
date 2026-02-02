#include "spi_driver.h"

#define SPI_EN_REG      (*(volatile unsigned int *)SPI_EN)
#define SPI_CTRL_REG    (*(volatile unsigned int *)SPI_CTRL)
#define SPI_STATUS_REG  (*(volatile unsigned int *)SPI_STATUS)
#define SPI_TX_DATA_REG (*(volatile unsigned int *)SPI_TX_DATA)
#define SPI_RX_DATA_REG (*(volatile unsigned int *)SPI_RX_DATA)
#define SPI_CONFIG_REG  (*(volatile unsigned int *)SPI_CONFIG)

static void spi_wait_done(void) {
  while ((SPI_STATUS_REG & SPI_STATUS_DONE) == 0) {
    // Wait for transfer to complete
  }
  // Clear done flag by reading status
  (void)SPI_STATUS_REG;
}

unsigned char spi_transfer_byte_internal(unsigned char tx_byte) {
  SPI_TX_DATA_REG = tx_byte;
  SPI_CTRL_REG = SPI_CTRL_START;
  spi_wait_done();
  return (unsigned char)SPI_RX_DATA_REG;
}

int spi_init(int clock_divider) {
  SPI_EN_REG = SPI_EN_ENABLE;
  SPI_CONFIG_REG =
      ((clock_divider & SPI_CONFIG_DIV_MASK) | SPI_MODE0); /* MODE0 default */

  return SPI_OK;
}

int spi_set_clock_divider(int clock_divider) {
  unsigned int config = SPI_CONFIG_REG & ~SPI_CONFIG_DIV_MASK;
  SPI_CONFIG_REG = config | (clock_divider & SPI_CONFIG_DIV_MASK);
  return SPI_OK;
}

int spi_set_mode(int mode) {
  unsigned int config = SPI_CONFIG_REG & SPI_CONFIG_DIV_MASK;
  SPI_CONFIG_REG = config | (mode & (SPI_CONFIG_CPOL | SPI_CONFIG_CPHA));
  return SPI_OK;
}

unsigned char spi_transfer_byte(unsigned char tx_byte) {
  return spi_transfer_byte_internal(tx_byte);
}

int spi_transfer(const unsigned char *tx_buf, unsigned char *rx_buf, int len) {
  if (len <= 0) {
    return SPI_ERROR;
  }

  for (int i = 0; i < len; i++) {
    unsigned char tx = (tx_buf != 0) ? tx_buf[i] : (unsigned char)0xFF;
    unsigned char rx = spi_transfer_byte_internal(tx);
    if (rx_buf != 0) {
      rx_buf[i] = rx;
    }
  }

  return SPI_OK;
}
