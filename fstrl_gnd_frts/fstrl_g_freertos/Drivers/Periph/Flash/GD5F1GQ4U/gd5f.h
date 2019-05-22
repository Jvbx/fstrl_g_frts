
#ifndef GD5F1G_DEF_h
#define GD5F1G_DEF_h
#ifdef __cplusplus
 extern "C" {
#endif


#include <string.h>
#include "main.h"
#include "spi.h"

#define BYTE_0_MASK32   (0x000000FF)
#define BYTE_1_MASK32   (0x0000FF00)
#define BYTE_2_MASK32   (0x00FF0000)
#define BYTE_3_MASK32   (0xFF000000)

/* Uncomment below to use hardware WriteProtect and HOLD functions
 * if not used, corresponding pins of Flash IC should be pulled up to VCC */

//#define  GD5F_USE_WP
//#define  GD5F_USE_HOLD

/* ReadID  related constants */
#define GD5F_MANUFACTURER_ID          0xC8  //valid for GD5FxxQ4UBxIG or GD5FxxQ4RBxIG chips
#define GD5F_3V3_VERSION_ID           0xD1  //GD5F1GQ4UBxIG chip
#define GD5F_1V8_VERSION_ID           0xC1  //GD5F1GQ4RBxIG chip
#define GD5F_ID_ADDR_MID_DID          0x00  //address for GD5F_READ_ID command; first response byte - Manufacturer ID (MID), second - Device ID (DID)
#define GD5F_ID_ADDR_DID_MID          0x01  //first response byte - DID, second - MID

/* Memory array structure */
#define GD5F_PAGE_MAIN_AREA_SIZE     (2048)  //main page size
#define GD5F_SPARE_SIZE_ECC_OFF      (128)   //adittional space, avaliable when hardware ECC is OFF, so full page size would be 2048 + 128 = 2176 bytes
#define GD5F_SPARE_SIZE_ECC_ON       (64)    //adittional space, avaliable when hardware ECC is ON. 2112 total bytes
#define GD5F_MAX_VALID_BYTE_ECC_OFF  (GD5F_PAGE_MAIN_AREA_SIZE + GD5F_SPARE_SIZE_ECC_OFF - 1)  //Max valid value for 12-bit CA (Column Address - byte number within the page). Hardware ECC Disabled
#define GD5F_MAX_VALID_BYTE_ECC_ON   (GD5F_PAGE_MAIN_AREA_SIZE + GD5F_SPARE_SIZE_ECC_ON - 1)   //Max valid value for 12-bit CA. Hardware ECC Enabled
#define GD5F_PAGES_PER_BLOCK         (64)    //GD5F1GQ4UBYIG chip is 1 Gbit (128 Megabytes) chip.
#define GD5F_BLOCK_COUNT             (1024)  //GD5F1GQ4UBYIG chip is 1 Gbit (128 Megabytes) chip.
#define GD5F_BAD_BLOCK_MARK_ADDRESS  (2048)  //located at the first byte of spare-area on the first page of each block
#define GD5F_BAD_BLOCK_MARK          (0x00)  //default value after erase - 0xFF for NAND.


/* ECC Protection page sheme*/
// ECC Protected main areas:
#define GD5F_MAIN_0_MIN_ADDR          0x0000
#define GD5F_MAIN_0_MAX_ADDR          0x01FF
#define GD5F_MAIN_1_MIN_ADDR          0x0200
#define GD5F_MAIN_1_MAX_ADDR          0x03FF
#define GD5F_MAIN_2_MIN_ADDR          0x0400
#define GD5F_MAIN_2_MAX_ADDR          0x05FF
#define GD5F_MAIN_3_MIN_ADDR          0x0600
#define GD5F_MAIN_3_MAX_ADDR          0x07FF

// ECC Protected spare areas:
#define GD5F_SPARE_0_MIN_ADDR         0x0804
#define GD5F_SPARE_0_MAX_ADDR         0x080F
#define GD5F_SPARE_1_MIN_ADDR         0x0814
#define GD5F_SPARE_1_MAX_ADDR         0x081F
#define GD5F_SPARE_2_MIN_ADDR         0x0824
#define GD5F_SPARE_2_MAX_ADDR         0x082F
#define GD5F_SPARE_3_MIN_ADDR         0x0834
#define GD5F_SPARE_3_MAX_ADDR         0x083F

//Internal ECC Parity Data:
#define GD5F_ECCPARITY_MIN_ADDR       0x0840
#define GD5F_ECCPARITY_MAX_ADDR       0x087F

// ECC Unprotected spare areas:
#define GD5F_SPARE_0_MIN_ADDR_UNPRO   0x0800  // 0x0800 is reserved for initial BadBlock mark
#define GD5F_SPARE_0_MAX_ADDR_UNPROT  0x0803
#define GD5F_SPARE_1_MIN_ADDR_UNPROT  0x0810
#define GD5F_SPARE_1_MAX_ADDR_UNPROT  0x0813
#define GD5F_SPARE_2_MIN_ADDR_UNPROT  0x0820
#define GD5F_SPARE_2_MAX_ADDR_UNPROT  0x0823
#define GD5F_SPARE_3_MIN_ADDR_UNPROT  0x0830
#define GD5F_SPARE_3_MAX_ADDR_UNPROT  0x0833



//#define GD5F_SPI_PORT                SPI1    //spi port
#define GD5F_OP_TIMEOUT               10000



/* Commands */
typedef enum {
    GD5F_WRITE_ENABLE                = 0x06,  // 1 byte (including command)
    GD5F_WRITE_DISABLE               = 0x04,  // 1 byte
    GD5F_GET_FEATURES                = 0x0F,  // 3 bytes, then output updated until CS# is driven high
    GD5F_SET_FEATURES                = 0x1F,  // 4 bytes, C, A, D, dummy
    GD5F_PAGE_READ_TO_CACHE          = 0x13,
    GD5F_PAGE_READ_FROM_CACHE        = 0x03,  //  Note: A0 should be 0, but this is not sure - newer revision datasheet says nothing about it
    GD5F_FAST_READ_FROM_CACHE        = 0x0B,
    GD5F_READ_FROM_CACHE_X2          = 0x3B,
    GD5F_READ_FROM_CACHE_X4          = 0x6B,
    GD5F_READ_FROM_CACHE_DUAL_IO     = 0xBB,
    GD5F_READ_FROM_CACHE_QUAD_IO     = 0xEB,
    GD5F_READ_ID                     = 0x9F,   //address 0x00 - first outputs ManufacturerID then DevID, 0x01 - DevID goes first
    GD5F_PROGRAM_LOAD                = 0x02,
    GD5F_PROGRAM_LOAD_X4             = 0x32,
    GD5F_PROGRAM_EXECUTE             = 0x10,
    GD5F_PROGRAM_LOAD_RANDOM_DATA    = 0x84,
    GD5F_PROGRAM_LOAD_RANDOM_DATA_X4 = 0xC4,   // or 0x34
    GD5F_PROG_LOAD_RAND_DATA_QUAD_IO = 0x72,
    GD5F_BLOCK_ERASE                 = 0xD8,   // block size equals 128K
    GD5F_RESET                       = 0xFF
} GD5F_COMMAND;


/* Device registers adresses, acessible by GD5F_GET_FEATURES (0x0F)
 * and GD5F_SET_FEATURES (0x1F) commands */
typedef enum {
    GD5F_PROTECTION            = 0xA0,
    GD5F_FEATURE_1             = 0xB0,
    GD5F_STATUS_1              = 0xC0,
    GD5F_FEATURE_2             = 0xD0,
    GD5F_STATUS_2              = 0xF0
} GD5F_FEATURE_REGS;

/* Registers bitmasks: */

//GD5F_PROTECTION (0xA0)
#define GD5F_PROTECTION_RESERVED  0x41   //0b0100 0001
#define GD5F_PROTECTION_CMP       0x02   //0b0000 0010
#define GD5F_PROTECTION_INV       0x04   //0b0000 0100
#define GD5F_PROTECTION_BP0       0x08   //0b0000 1000
#define GD5F_PROTECTION_BP1       0x10   //0b0001 0000
#define GD5F_PROTECTION_BP2       0x20   //0b0010 0000
#define GD5F_PROTECTION_BWRD      0x80   //0b1000 0000

//GD5F_FEATURE_1 (0xB0)
#define GD5F_FEATURE_1_RESERVED   0x2E   //0b0010 1110
#define GD5F_FEATURE_1_QE         0x01   //0b0000 0001
#define GD5F_FEATURE_1_ECC_EN     0x10   //0b0001 0000
#define GD5F_FEATURE_1_OTP_EN     0x40   //0b0100 0000
#define GD5F_FEATURE_1_OTP_PRT    0x80   //0b1000 0000

//GD5F_STATUS_1 (0xC0)
#define GD5F_STATUS_1_RESERVED    0xC0   //0b1100 0000
#define GD5F_STATUS_1_OIP         0x01   //0b0000 0001
#define GD5F_STATUS_1_WEL         0x02   //0b0000 0010
#define GD5F_STATUS_1_E_FAIL      0x04   //0b0000 0100
#define GD5F_STATUS_1_P_FAIL      0x08   //0b0000 1000
#define GD5F_STATUS_1_ECCS0       0x10   //0b0001 0000
#define GD5F_STATUS_1_ECCS1       0x20   //0b0010 0000

//GD5F_FEATURE_2 (0xD0)
#define GD5F_FEATURE_2_RESERVED   0x9F   //0b1001 1111
#define GD5F_FEATURE_2_DS_S0      0x20   //0b0010 0000
#define GD5F_FEATURE_2_DS_D1      0x40   //0b0100 0000

//GD5F_STATUS2 (0xF0)
#define GD5F_STATUS_2_RESERVED    0x3F   //0b1100 1111
#define GD5F_STATUS_2_ECCSE0      0x10   //0b0001 0000
#define GD5F_STATUS_2_ECCSE1      0x20   //0b0010 0000

typedef enum { GD5F_OK               = (int8_t) 0x00,
               GD5F_READY            = (int8_t) 0x01,
               GD5F_ERROR            = (int8_t) 0x80,
               GD5F_INVALID_ARGUMENT = (int8_t) 0x81,
               GD5F_BUSY             = (int8_t) 0x82,
               GD5F_DEV_TIMEOUT      = (int8_t) 0x84,
               GD5F_SPI_BUSY         = (int8_t) 0x90,
               GD5F_SPI_TIMEOUT      = (int8_t) 0xA0,

             } GD5F_RESULT;


typedef struct {
    SPI_TypeDef *    spi;               //hardware spi module, flash chip is connected to
        uint32_t     spi_retry_count;   //used with STM32 HAL and LL library.

    GPIO_TypeDef*    csn_port;          //CS#, software driven chip select line, active low
        uint32_t     csn_pin;

#ifdef  GD5F_USE_WP
    GPIO_TypeDef*    wpn_port;      //WP#, write protect line, active low. Could be not used, in that case should be pulled up to VCC
    uint32_t         wpn_pin;
#endif

#ifdef  GD5F_USE_HOLD
    GPIO_TypeDef*    holdn_port;    //HOLD#, hold function, active low. May be not used, in that case should be pulled up to VCC
    uint32_t         holdn_pin;
#endif
} gd5f_hw_config;


typedef struct {
     uint8_t        protection;
     uint8_t        status_1;
     uint8_t        status_2;
     uint8_t        feature_1;
     uint8_t        feature_2;
} gd5f_registers;

typedef struct {
         gd5f_hw_config      hw_config;      //spi interface handler and physical pins
         gd5f_registers      registers;      //important registers "mirror"
//         uint16_t            curr_page;      //current page we are working with
//         uint16_t            curr_offset;    //current byte number in page (2048 + spare 128 or 64 bytes). 0..2175 (ECC disabled) or 0..2111 (ECC Enabled)
//         uint16_t            pagesize;       //if flash properly inited, this would contain pagesize in bytes. 2112 (default, ECC ON) or 2176
         uint8_t             devid[2];       //devid array. devid[0] - manufacturer ID, devid[1] - chip ID. should be 0x1F for AT45DB flash chip.
//         uint8_t             rtxbuf[2182];    //some internal buffer. Stil have no idea why do i need it.
//         uint8_t             addrshift;      //10 for 528 pagesize or 9 for 512. Used to maintain different address format.
//         uint8_t             chipsize;
volatile uint8_t             gd5f_busy;      //0 - flash is free for action. else we have to wait
} gd5f;




GD5F_RESULT gd5f_get_id(gd5f* dev);
GD5F_RESULT gd5f_get_features(gd5f* dev);
GD5F_RESULT gd5f_init(gd5f* dev);
GD5F_RESULT gd5f_reset(gd5f* dev);
GD5F_RESULT gd5f_wait_busy(gd5f* dev);
GD5F_RESULT gd5f_random_read_page(gd5f* dev, uint32_t page_num, uint16_t start_bytenum, uint8_t *rxbuf, uint16_t datalen);
GD5F_RESULT gd5f_random_write_page(gd5f* dev, uint32_t page_num, uint16_t start_bytenum, uint8_t *txbuf, uint16_t datalen);
GD5F_RESULT gd5f_write_page(gd5f* dev, uint32_t page_num, uint16_t start_bytenum, uint8_t *txbuf, uint16_t datalen);
GD5F_RESULT gd5f_blockerase(gd5f* dev, uint32_t page_num);
GD5F_RESULT gd5f_unblock(gd5f* dev);
GD5F_RESULT gd5f_ecc_off(gd5f* dev);
GD5F_RESULT gd5f_ecc_on(gd5f* dev);

#ifdef __cplusplus
}
#endif
#endif /* GD5F1G_DEF_h */
