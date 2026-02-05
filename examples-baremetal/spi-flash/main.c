#include <stdio.h>

#include "gpio_driver.h"
#include "hw_registers.h"
#include "spi_driver.h"

#define CS_N_PIN 2

// SPI NOR Flash commands (standard JEDEC)
#define CMD_READ_DATA  0x03
#define CMD_READ_JEDEC 0x9F

static void flash_cs_low(void) {
  gpio_clear_pin(CS_N_PIN);
}

static void flash_cs_high(void) {
  gpio_set_pin(CS_N_PIN);
}

/**
 * Read bytes from SPI flash at 24-bit address.
 * Uses Read Data command (0x03).
 */
static void flash_read(unsigned int addr, unsigned char *buf, unsigned int len) {
  flash_cs_low();

  spi_transfer_byte(CMD_READ_DATA);
  spi_transfer_byte((unsigned char)((addr >> 16) & 0xFF));
  spi_transfer_byte((unsigned char)((addr >> 8) & 0xFF));
  spi_transfer_byte((unsigned char)(addr & 0xFF));

  for (unsigned int i = 0; i < len; i++) {
    buf[i] = spi_transfer_byte(0xFF);
  }

  flash_cs_high();
}

/**
 * Read JEDEC ID (3 bytes: Manufacturer, Memory Type, Capacity).
 * Optional, for verifying flash presence.
 */
static void flash_read_id(unsigned char *id_out) {
  flash_cs_low();
  spi_transfer_byte(CMD_READ_JEDEC);
  id_out[0] = spi_transfer_byte(0xFF);
  id_out[1] = spi_transfer_byte(0xFF);
  id_out[2] = spi_transfer_byte(0xFF);
  flash_cs_high();
}

int main(void) {
  printf("Reading SPI flash...\n");
  
  gpio_set_direction(CS_N_PIN, GPIO_OUTPUT);
  flash_cs_high();

  spi_init(SPI_DIV_1MHZ);

  printf("Reading JEDEC ID...\n");
  unsigned char jedec_id[3];
  flash_read_id(jedec_id);
  printf("JEDEC ID: %02X %02X %02X\n", jedec_id[0], jedec_id[1], jedec_id[2]);
  /* id_out[0]=Manufacturer (e.g. 0xEF Winbond), [1]=Type, [2]=Capacity */

  printf("Reading flash data ...\n");
  unsigned int address = 0;
  unsigned char buf[4];
  for (unsigned int i = 0; i < 16; i++) {
    flash_read(address, buf, sizeof(buf));
    printf("Flash data[%08X]: %02X %02X %02X %02X\n", address, buf[0], buf[1], buf[2], buf[3]);
    address += 4;
  }


  while (1) {
  }
}
