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

static unsigned short spi_transfer_hword_internal(unsigned short tx_hword) {
  SPI_TX_DATA_REG = tx_hword;
  SPI_CTRL_REG = SPI_CTRL_LEN_2B | SPI_CTRL_START;
  spi_wait_done();
  return (unsigned short)(SPI_RX_DATA_REG & 0xFFFFu);
}

static unsigned int spi_transfer_word_internal(unsigned int tx_word) {
  SPI_TX_DATA_REG = tx_word;
  SPI_CTRL_REG = SPI_CTRL_LEN_4B | SPI_CTRL_START;
  spi_wait_done();
  return SPI_RX_DATA_REG;
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

unsigned short spi_transfer_hword(unsigned short tx_hword) {
  return spi_transfer_hword_internal(tx_hword);
}

int spi_transfer_hwords(const unsigned short *tx_buf, unsigned short *rx_buf,
                        int len) {
  if (len <= 0) {
    return SPI_ERROR;
  }

  for (int i = 0; i < len; i++) {
    unsigned short tx = (tx_buf != 0) ? tx_buf[i] : (unsigned short)0xFFFF;
    unsigned short rx = spi_transfer_hword_internal(tx);
    if (rx_buf != 0) {
      rx_buf[i] = rx;
    }
  }

  return SPI_OK;
}

unsigned int spi_transfer_word(unsigned int tx_word) {
  return spi_transfer_word_internal(tx_word);
}

int spi_transfer_words(const unsigned int *tx_buf, unsigned int *rx_buf,
                       int len) {
  if (len <= 0) {
    return SPI_ERROR;
  }

  for (int i = 0; i < len; i++) {
    unsigned int tx = (tx_buf != 0) ? tx_buf[i] : 0xFFFFFFFFu;
    unsigned int rx = spi_transfer_word_internal(tx);
    if (rx_buf != 0) {
      rx_buf[i] = rx;
    }
  }

  return SPI_OK;
}

int spi_transmit_fast(const unsigned char *tx_buf, int len) {
  if (len <= 0 || tx_buf == 0) {
    return SPI_ERROR;
  }

  unsigned int div = SPI_CONFIG_REG & SPI_CONFIG_DIV_MASK;

  if (div == SPI_DIV_8MHZ) {
    if (len % 2 != 0) {
      return SPI_ERROR; /* 8MHz: len must be divisible by 2 */
    }
    const unsigned short *tx = (const unsigned short *)tx_buf;
    int n = len / 2;
    for (int i = 0; i < n; i++) {
      SPI_TX_DATA_REG = tx[i];
      SPI_CTRL_REG = SPI_CTRL_LEN_2B | SPI_CTRL_START;
    }
  } else if (div == SPI_DIV_16MHZ) {
    if (len % 4 != 0) {
      return SPI_ERROR; /* 16MHz: len must be divisible by 4 */
    }
    const unsigned int *tx = (const unsigned int *)tx_buf;
    int n = len / 4;
    for (int i = 0; i < n; i++) {
      SPI_TX_DATA_REG = tx[i];
      SPI_CTRL_REG = SPI_CTRL_LEN_4B | SPI_CTRL_START;
    }
  } else {
    return SPI_ERROR; /* clock must be 8MHz or 16MHz */
  }

  return SPI_OK;
}