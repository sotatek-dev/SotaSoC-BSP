#ifndef HW_REGISTERS_H
#define HW_REGISTERS_H

// Constants
#define CLK_HZ                 64000000 // 64MHz
#define CYCLES_PER_INSTRUCTION 8
#define MEMORY_CYCLES          7
#define NOP_INSTR              0x00000013

// Memory base addresses
#define FLASH_BASE_ADDR        0x00000000
#define PSRAM_BASE_ADDR        0x01000000

// Peripheral base addresses
#define UART_BASE_ADDR         0x40000000
#define GPIO_BASE_ADDR         0x40001000
#define TIMER_BASE_ADDR        0x40002000
#define PWM_BASE_ADDR          0x40003000
#define I2C_BASE_ADDR          0x40004000
#define SPI_BASE_ADDR          0x40005000

// UART registers
#define UART_TX_DATA            (UART_BASE_ADDR + 0x00)
#define UART_TX_CTRL            (UART_BASE_ADDR + 0x04)
#define UART_RX_DATA            (UART_BASE_ADDR + 0x08)
#define UART_RX_CTRL            (UART_BASE_ADDR + 0x0C)
#define UART_DIVIDER            (UART_BASE_ADDR + 0x10)
#define UART_TX_CTRL_BUSY       0x01u
#define UART_TX_CTRL_START      0x02u
#define UART_RX_CTRL_READY      0x01u
#define UART_RX_CTRL_START      0x02u
#define UART_RX_CTRL_BREAK      0x04u
#define UART_DIVIDER_MASK       0x03FFu

// GPIO register offsets
#define GPIO_DIR               (GPIO_BASE_ADDR + 0x00)  // Direction register (bidirectional pins)
#define GPIO_OUT               (GPIO_BASE_ADDR + 0x04)  // Output data register
#define GPIO_IN                (GPIO_BASE_ADDR + 0x08)  // Input data register (read-only)
#define GPIO_INT_EN            (GPIO_BASE_ADDR + 0x0C)  // Interrupt enable (rising edge)
#define GPIO_INT_PEND          (GPIO_BASE_ADDR + 0x14)  // Interrupt pending (read-only)
#define GPIO_INT_CLR           (GPIO_BASE_ADDR + 0x18)  // Interrupt clear (write 1 to clear)

// Timer register offsets
#define TIMER_MTIME_LO         (TIMER_BASE_ADDR + 0x00)
#define TIMER_MTIME_HI         (TIMER_BASE_ADDR + 0x04)
#define TIMER_MTIMECMP_LO      (TIMER_BASE_ADDR + 0x08)
#define TIMER_MTIMECMP_HI      (TIMER_BASE_ADDR + 0x0C)

// PWM register offsets per channel (16 bytes per channel)
#define PWM_CH0_CTRL           (PWM_BASE_ADDR + 0x00)
#define PWM_CH0_PERIOD         (PWM_BASE_ADDR + 0x04)
#define PWM_CH0_DUTY           (PWM_BASE_ADDR + 0x08)
#define PWM_CH0_COUNTER        (PWM_BASE_ADDR + 0x0C)

#define PWM_CH1_CTRL           (PWM_BASE_ADDR + 0x10)
#define PWM_CH1_PERIOD         (PWM_BASE_ADDR + 0x14)
#define PWM_CH1_DUTY           (PWM_BASE_ADDR + 0x18)
#define PWM_CH1_COUNTER        (PWM_BASE_ADDR + 0x1C)

// I2C base address and register offsets
#define I2C_CTRL               (I2C_BASE_ADDR + 0x00)
#define I2C_STATUS             (I2C_BASE_ADDR + 0x04)
#define I2C_DATA               (I2C_BASE_ADDR + 0x08)
#define I2C_PRESCALE           (I2C_BASE_ADDR + 0x0C)

// I2C Control register bits
#define I2C_CTRL_START         0x01  // bit 0: Generate START condition
#define I2C_CTRL_STOP          0x02  // bit 1: Generate STOP condition
#define I2C_CTRL_READ          0x04  // bit 2: Read mode (1=read, 0=write)
#define I2C_CTRL_ACK_EN        0x08  // bit 3: ACK enable (1=send ACK, 0=send NACK)
#define I2C_CTRL_RESTART       0x10  // bit 4: Generate REPEATED START condition

// I2C Status register bits
#define I2C_STATUS_BUSY        0x01  // bit 0: Transfer in progress
#define I2C_STATUS_ACK         0x02  // bit 1: ACK received (1=ACK, 0=NACK)
#define I2C_STATUS_ARB_LOST    0x04  // bit 2: Arbitration lost
#define I2C_STATUS_DONE        0x08  // bit 3: Transfer complete
#define I2C_STATUS_ERROR       0x10  // bit 4: Bus error

// I2C Prescaler values for common frequencies @ 64MHz clock
#define I2C_PRESCALE_100KHZ    159  // 64MHz / (4 * 160) = 100kHz
#define I2C_PRESCALE_400KHZ    39   // 64MHz / (4 * 40) = 400kHz

// SPI base address and register offsets
#define SPI_CTRL               (SPI_BASE_ADDR + 0x00)
#define SPI_STATUS             (SPI_BASE_ADDR + 0x04)
#define SPI_TX_DATA            (SPI_BASE_ADDR + 0x08)
#define SPI_RX_DATA            (SPI_BASE_ADDR + 0x0C)
#define SPI_CONFIG             (SPI_BASE_ADDR + 0x10)

// SPI Control register bits
#define SPI_CTRL_START         0x01  // bit 0: Start transfer

// SPI Status register bits
#define SPI_STATUS_BUSY        0x01  // bit 0: Transfer in progress
#define SPI_STATUS_DONE        0x02  // bit 1: Transfer complete

// SPI Config register bits
#define SPI_CONFIG_DIV_MASK    0xFF  // bits [7:0]: Clock divider

// SPI Clock divider values for common frequencies @ 64MHz
#define SPI_DIV_1MHZ           31   // 64MHz / (2 * 32) = 1MHz
#define SPI_DIV_2MHZ           15   // 64MHz / (2 * 16) = 2MHz
#define SPI_DIV_4MHZ           7    // 64MHz / (2 * 8) = 4MHz
#define SPI_DIV_8MHZ           3    // 64MHz / (2 * 4) = 8MHz

// SPI Mode 0 (only mode supported)
#define SPI_MODE0              0x000  // CPOL=0, CPHA=0

#endif // HW_REGISTERS_H
