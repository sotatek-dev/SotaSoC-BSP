#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include "hw_registers.h"

// SPI driver return codes
#define SPI_OK 0
#define SPI_ERROR -1

// SPI driver API
// Chip select (CS) is not handled by this driver; use the GPIO driver to
// assert/deassert CS before/after transfers as required by your device.

/**
 * @brief Initialize SPI module
 * @param clock_divider Clock divider value (use SPI_DIV_1MHZ, SPI_DIV_2MHZ,
 *                      SPI_DIV_4MHZ, SPI_DIV_8MHZ, or custom 0..255)
 * @return SPI_OK on success
 */
int spi_init(int clock_divider);

/**
 * @brief Set SPI clock divider at runtime
 * @param clock_divider Divider value (SPI_CONFIG_DIV_MASK)
 * @return SPI_OK
 */
int spi_set_clock_divider(int clock_divider);

/**
 * @brief Set SPI mode (CPOL, CPHA). Preserves clock divider.
 * @param mode 0=SPI_MODE0, 1=SPI_MODE1, 2=SPI_MODE2, 3=SPI_MODE3
 * @return SPI_OK
 */
int spi_set_mode(int mode);

/**
 * @brief Transfer a single byte (full duplex)
 * @param tx_byte Byte to send
 * @return Byte received from slave
 */
unsigned char spi_transfer_byte(unsigned char tx_byte);

/**
 * @brief Transfer multiple bytes
 * @param tx_buf Data to send (may be NULL to send 0xFF for each byte)
 * @param rx_buf Buffer for received data (may be NULL to discard)
 * @param len Number of bytes to transfer
 * @return SPI_OK on success, SPI_ERROR if len <= 0
 */
int spi_transfer(const unsigned char *tx_buf, unsigned char *rx_buf, int len);

#endif  // SPI_DRIVER_H
