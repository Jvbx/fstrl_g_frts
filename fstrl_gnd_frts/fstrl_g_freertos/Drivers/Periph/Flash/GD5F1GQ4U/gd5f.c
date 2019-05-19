#include "gd5f.h"


__STATIC_INLINE void gd5f_csn_set(gd5f* dev) {
    LL_GPIO_SetOutputPin(dev->hw_config.csn_port, dev->hw_config.csn_pin);
}

__STATIC_INLINE void gd5f_csn_reset(gd5f* dev) {
    LL_GPIO_ResetOutputPin(dev->hw_config.csn_port, dev->hw_config.csn_pin);
}

/* Driver/platform specific GPIO functions */

#ifdef  GD5F_USE_WP

__STATIC_INLINE void gd5f_wpn_set(gd5f* dev) {
    LL_GPIO_SetOutputPin(dev->hw_config.csn_port, dev->hw_config.csn_pin);
}

__STATIC_INLINE void gd5f_wpn_reset(gd5f* dev) {
    LL_GPIO_ResetOutputPin(dev->hw_config.wpn_port, dev->hw_config.wpn_pin);
}
#endif

#ifdef  GD5F_USE_HOLD

__STATIC_INLINE void gd5f_holdn_set(gd5f* dev) {
    LL_GPIO_SetOutputPin(dev->hw_config.rstn_port, dev->hw_config.rstn_pin);
}

__STATIC_INLINE static void gd5f_holdn_reset(gd5f* dev) {
    LL_GPIO_ResetOutputPin(dev->hw_config.rstn_port, dev->hw_config.rstn_pin);
}
#endif

#define GD5F_WAIT_BUSY() if (gd5f_wait_busy(dev) != GD5F_OK) {return GD5F_DEV_TIMEOUT;}
#define GD5F_WAIT_SPI_BUSY() if (gd5f_wait_spi_busy(dev, dev->hw_config.spi_retry_count) != GD5F_OK) { gd5f_spi_stop(dev); return GD5F_SPI_TIMEOUT;}
#define GD5F_WAIT_TX_EMPTY() if (gd5f_wait_tx_empty(dev, dev->hw_config.spi_retry_count) != GD5F_OK) { gd5f_csn_set(dev); gd5f_spi_disable(dev); return GD5F_SPI_TIMEOUT;}
#define GD5F_WAIT_RX_NOT_EMPTY() if (gd5f_wait_rx_not_empty(dev, dev->hw_config.spi_retry_count) != GD5F_OK) { gd5f_csn_set(dev); gd5f_spi_disable(dev); return GD5F_SPI_TIMEOUT;}


__STATIC_INLINE GD5F_RESULT gd5f_wait_tx_empty(gd5f* dev, uint16_t timeout_us) {
	uint16_t timeleft = timeout_us;
		while (LL_SPI_IsActiveFlag_TXE(dev->hw_config.spi) == 0)  {
			if ((timeleft--) == 0) return GD5F_SPI_TIMEOUT;
		}
		return GD5F_OK;
}

__STATIC_INLINE GD5F_RESULT gd5f_wait_rx_not_empty(gd5f* dev, uint16_t timeout_us) {
	uint16_t timeleft = timeout_us;
		while (LL_SPI_IsActiveFlag_RXNE(dev->hw_config.spi) == 0)  {
			if ((timeleft--) == 0) return GD5F_SPI_TIMEOUT;
		}
		return GD5F_OK;
}



__STATIC_INLINE void gd5f_spi_enable(gd5f* dev){
if (!LL_SPI_IsEnabled(dev->hw_config.spi))
{LL_SPI_Enable(dev->hw_config.spi);}
}

__STATIC_INLINE void gd5f_spi_disable(gd5f* dev){
if (LL_SPI_IsEnabled(dev->hw_config.spi))
{LL_SPI_Disable(dev->hw_config.spi);}
}

__STATIC_INLINE GD5F_RESULT gd5f_wait_spi_busy(gd5f* dev, uint16_t timeout_us) {
    uint16_t timeleft = timeout_us;

    while (LL_SPI_IsActiveFlag_BSY(dev->hw_config.spi) > 0)  {
            if ((timeleft--) == 0) return GD5F_SPI_TIMEOUT;
          }
        dev->gd5f_busy = 0;
        return GD5F_OK;
}


 __STATIC_INLINE void gd5f_spi_start(gd5f* dev){
gd5f_spi_enable(dev);
gd5f_csn_reset(dev);
}

 __STATIC_INLINE void gd5f_spi_stop(gd5f* dev){
 gd5f_csn_set(dev);
 gd5f_spi_disable(dev);
}





GD5F_RESULT gd5f_spi_sendbyte(gd5f* dev,           // device handler
                            uint8_t databyte)
{
     gd5f_spi_start(dev);
     GD5F_WAIT_TX_EMPTY();
     LL_SPI_TransmitData8(dev->hw_config.spi, databyte);
     GD5F_WAIT_RX_NOT_EMPTY();
     databyte = LL_SPI_ReceiveData8(dev->hw_config.spi);
     GD5F_WAIT_TX_EMPTY();
     GD5F_WAIT_SPI_BUSY();
     gd5f_spi_stop(dev);
  return GD5F_OK;
}



GD5F_RESULT gd5f_write_enable(gd5f* dev)
{
	return gd5f_spi_sendbyte(dev, GD5F_WRITE_ENABLE);
}

GD5F_RESULT gd5f_write_disable(gd5f* dev)
{
	return gd5f_spi_sendbyte(dev, GD5F_WRITE_DISABLE);
}

GD5F_RESULT gd5f_reset(gd5f* dev){
	if (gd5f_spi_sendbyte(dev, GD5F_RESET) != GD5F_OK) return GD5F_ERROR;
    GD5F_WAIT_BUSY();
    return GD5F_OK;
}



GD5F_RESULT gd5f_cmd(gd5f* dev,
              GD5F_COMMAND cmd,
                  uint32_t address,
                   uint8_t addrlen,
                   uint8_t *databuf,
                  uint16_t datalen,
                   uint8_t noreadback)
{
    uint8_t  cmd_len = (addrlen + 1);
    uint8_t  cmdbuf[cmd_len];
    cmdbuf[0] = cmd;

    gd5f_spi_start(dev);


     switch (addrlen) {
        case 3: {
                cmdbuf[1] = (uint8_t)((address & BYTE_2_MASK32) >> 16);
                cmdbuf[2] = (uint8_t)((address & BYTE_1_MASK32) >> 8);
                cmdbuf[3] = (uint8_t)((address) & BYTE_0_MASK32);
                break;}
        case 2: {
                cmdbuf[1] = (uint8_t)((address & BYTE_1_MASK32) >> 8);
                cmdbuf[2] = (uint8_t)((address) & BYTE_0_MASK32);
                break;}
        case 1: {
                cmdbuf[1] = (uint8_t)((address) & BYTE_0_MASK32);
                break;}
        case 0: {break;}

        default: {
        return GD5F_INVALID_ARGUMENT;
        break;}
        }


     //uint8_t  spi_rsp = 0;
     //transmitting command opcode and address if needed
     for (uint8_t curr_byte_num = 0; curr_byte_num < cmd_len; curr_byte_num++)
            {
               GD5F_WAIT_TX_EMPTY();
               LL_SPI_TransmitData8(dev->hw_config.spi, cmdbuf[curr_byte_num]);
               GD5F_WAIT_RX_NOT_EMPTY();
               LL_SPI_ReceiveData8(dev->hw_config.spi);
           }
     //transmitting databuf contetnts
     if (noreadback) {
         for (uint16_t curr_byte_num = 0; curr_byte_num < datalen; curr_byte_num++)
                {
                GD5F_WAIT_TX_EMPTY();
                LL_SPI_TransmitData8(dev->hw_config.spi, databuf[curr_byte_num]);
                GD5F_WAIT_RX_NOT_EMPTY();
               LL_SPI_ReceiveData8(dev->hw_config.spi);
                }
         } else {
         for (uint16_t curr_byte_num = 0; curr_byte_num < datalen; curr_byte_num++)
                {
                GD5F_WAIT_TX_EMPTY();
                LL_SPI_TransmitData8(dev->hw_config.spi, databuf[curr_byte_num]);
                GD5F_WAIT_RX_NOT_EMPTY();
                databuf[curr_byte_num] = LL_SPI_ReceiveData8(dev->hw_config.spi);
                }
         }

GD5F_WAIT_TX_EMPTY();
GD5F_WAIT_SPI_BUSY();

gd5f_spi_stop(dev);
return GD5F_OK;
}








GD5F_RESULT gd5f_get_features(gd5f* dev)
{
volatile GD5F_RESULT res = 0;

 res |= gd5f_cmd(dev, GD5F_GET_FEATURES, GD5F_PROTECTION, 1, &dev->registers.protection,  1, 0);
 res |= gd5f_cmd(dev, GD5F_GET_FEATURES, GD5F_FEATURE_1,  1, &dev->registers.feature_1,   1, 0);
 res |= gd5f_cmd(dev, GD5F_GET_FEATURES, GD5F_STATUS_1,   1, &dev->registers.status_1,    1, 0);
 res |= gd5f_cmd(dev, GD5F_GET_FEATURES, GD5F_FEATURE_2,  1, &dev->registers.feature_2,   1, 0);
 res |= gd5f_cmd(dev, GD5F_GET_FEATURES, GD5F_STATUS_2,   1, &dev->registers.status_2,    1, 0);
return res;
}

GD5F_RESULT gd5f_get_feature(gd5f* dev, GD5F_FEATURE_REGS feature_reg, uint8_t* feature_byte)
{
volatile GD5F_RESULT res = 0;

res = gd5f_cmd(dev, GD5F_GET_FEATURES, feature_reg, 1, feature_byte, 1, 0);
if (res != GD5F_OK) return res;

    switch (feature_reg) {
        case GD5F_PROTECTION:
            {
             dev->registers.protection = *feature_byte;
             break;
            }
        case GD5F_FEATURE_1:
            {
             dev->registers.feature_1 = *feature_byte;
             break;
            }
        case GD5F_STATUS_1:
            {
            dev->registers.status_1 = *feature_byte;
            break;
            }
        case GD5F_FEATURE_2:
            {
            dev->registers.feature_2 = *feature_byte;
            break;
            }
        case GD5F_STATUS_2:
            {
            dev->registers.status_2 = *feature_byte;
            break;
            }
        default:{
        return GD5F_INVALID_ARGUMENT;
        break;}
    }
return res;
}

__STATIC_INLINE GD5F_RESULT gd5f_set_feature(gd5f* dev, GD5F_FEATURE_REGS feature_reg, uint8_t *feature_byte)
{
   volatile GD5F_RESULT res = 0;
    res =  gd5f_cmd(dev, GD5F_SET_FEATURES, feature_reg, 1, feature_byte, 1, 1);
   return res;
}

uint8_t gd5f_is_busy(gd5f* dev)
{
uint8_t feature_byte = 0;
if (gd5f_get_feature(dev, GD5F_STATUS_1, &feature_byte) != GD5F_OK) {return 0xFF;}
return (feature_byte & GD5F_STATUS_1_OIP);
}

GD5F_RESULT gd5f_wait_busy(gd5f* dev)
{
uint8_t is_not_timeout = 1;
volatile uint32_t microseconds = GD5F_OP_TIMEOUT;
uint32_t clk_cycle_start = DWT->CYCCNT;
microseconds *= (SystemCoreClock / 1000000);
dev->gd5f_busy = 1;

while(gd5f_is_busy(dev) && is_not_timeout) {
    is_not_timeout = ((DWT->CYCCNT - clk_cycle_start) < microseconds);
    }

if (is_not_timeout) {
    dev->gd5f_busy = 0;
    return GD5F_OK;
    }
return GD5F_DEV_TIMEOUT;
}


GD5F_RESULT gd5f_get_id(gd5f* dev)
{
return gd5f_cmd(dev, GD5F_READ_ID, GD5F_ID_ADDR_MID_DID, 1, (uint8_t*)&dev->devid, 2, 0);
}

GD5F_RESULT gd5f_init(gd5f* dev)
{
volatile GD5F_RESULT res = 0;
    dev->hw_config.spi = SPI1;
    dev->hw_config.csn_port = FLASH_CSN_GPIO_Port;
    dev->hw_config.csn_pin = FLASH_CSN_Pin;
    dev->hw_config.spi_retry_count = GD5F_OP_TIMEOUT;
    res |= gd5f_get_id(dev);
    res |= gd5f_get_features(dev);
    gd5f_unblock(dev);
    gd5f_ecc_on(dev);

	return res;
}


__STATIC_INLINE GD5F_RESULT gd5f_read_to_cache(gd5f* dev,
                        uint32_t pageaddress)
{
return gd5f_cmd(dev, GD5F_PAGE_READ_TO_CACHE, pageaddress, 3, NULL, 0, 1);
}


__STATIC_INLINE GD5F_RESULT gd5f_read_from_cache(gd5f* dev, uint16_t byte_addr, uint8_t *rxbuf, uint16_t datalen)
{
if ((datalen + byte_addr) > 2175) {return GD5F_INVALID_ARGUMENT;}
return gd5f_cmd(dev, GD5F_PAGE_READ_FROM_CACHE, (byte_addr << 8), 3, rxbuf, datalen, 0);
}

__STATIC_INLINE GD5F_RESULT gd5f_program_load(gd5f* dev, uint16_t byte_addr, uint8_t *txbuf, uint16_t datalen)
{
if ((datalen + byte_addr) > 2175) {return GD5F_INVALID_ARGUMENT;}
return gd5f_cmd(dev, GD5F_PROGRAM_LOAD, byte_addr, 2, txbuf, datalen, 1);
}

__STATIC_INLINE GD5F_RESULT gd5f_program_load_random(gd5f* dev, uint16_t byte_addr, uint8_t *txbuf, uint16_t datalen)
{
if ((datalen + byte_addr) > 2175) {return GD5F_INVALID_ARGUMENT;}
return gd5f_cmd(dev, GD5F_PROGRAM_LOAD_RANDOM_DATA, byte_addr, 2, txbuf, datalen, 1);
}

__STATIC_INLINE GD5F_RESULT gd5f_program_execute(gd5f* dev, uint32_t pageaddress)
{
return gd5f_cmd(dev, GD5F_PROGRAM_EXECUTE, pageaddress, 3, NULL, 0, 1);
}



//1 page = 2048 bytes + 64/128 spare
//1 block = 64 pages
//1G device = 1024 blocks
//total pages = 1024*64 = 65536

//total 512 subpages = 65536*4 = 262144

//address format:
// |__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|
//  15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^|^^^^^^^^^^^^^^^^
//       block number (0..1023)  | page num in block (0..63)
//

GD5F_RESULT gd5f_random_read_page(gd5f* dev, uint32_t page_num, uint16_t start_bytenum, uint8_t *rxbuf, uint16_t datalen)
{
 GD5F_WAIT_BUSY();
 if (gd5f_read_to_cache(dev, page_num) != GD5F_OK) {return GD5F_DEV_TIMEOUT;}
 GD5F_WAIT_BUSY();
 return gd5f_read_from_cache(dev, start_bytenum, rxbuf, datalen);
 gd5f_get_features(dev);
}

GD5F_RESULT gd5f_random_write_page(gd5f* dev, uint32_t page_num, uint16_t start_bytenum, uint8_t *txbuf, uint16_t datalen)
{
    GD5F_WAIT_BUSY();
    if (gd5f_read_to_cache(dev, page_num) != GD5F_OK) {return GD5F_DEV_TIMEOUT;}
    GD5F_WAIT_BUSY();
    if (gd5f_write_enable(dev) != GD5F_OK) {return GD5F_DEV_TIMEOUT;}
    GD5F_WAIT_BUSY();
    if (gd5f_program_load_random(dev, start_bytenum, txbuf, datalen) != GD5F_OK) {return GD5F_DEV_TIMEOUT;}
    if (gd5f_program_execute(dev, page_num) != GD5F_OK) {return GD5F_DEV_TIMEOUT;}
    GD5F_WAIT_BUSY();
    return gd5f_wait_busy(dev);
    gd5f_get_features(dev);
}


GD5F_RESULT gd5f_write_page(gd5f* dev, uint32_t page_num, uint16_t start_bytenum, uint8_t *txbuf, uint16_t datalen)
{
    GD5F_WAIT_BUSY();
    if (gd5f_write_enable(dev) != GD5F_OK) {return GD5F_DEV_TIMEOUT;}
    if (gd5f_program_load(dev, start_bytenum, txbuf, datalen) != GD5F_OK) {return GD5F_DEV_TIMEOUT;}
    if (gd5f_program_execute(dev, page_num) != GD5F_OK) {return GD5F_DEV_TIMEOUT;}
    GD5F_WAIT_BUSY();
    return gd5f_get_features(dev);
}

GD5F_RESULT gd5f_blockerase(gd5f* dev, uint32_t block_num)
{
    GD5F_WAIT_BUSY();
    if (gd5f_write_enable(dev) != GD5F_OK) {return GD5F_DEV_TIMEOUT;}
    GD5F_WAIT_BUSY();
    if (gd5f_cmd(dev, GD5F_BLOCK_ERASE, ((block_num << 6) & 0x0000FFC0), 3, NULL, 0, 1) != GD5F_OK) {return GD5F_DEV_TIMEOUT;}
    return gd5f_wait_busy(dev);
}

GD5F_RESULT gd5f_unblock(gd5f* dev)
{
GD5F_WAIT_BUSY();
gd5f_get_features(dev);
uint8_t feature_byte = 0x00; //dev->registers.protection & ((~(GD5F_PROTECTION_BP0|GD5F_PROTECTION_BP1|GD5F_PROTECTION_BP2))&(~GD5F_PROTECTION_RESERVED));
gd5f_set_feature(dev, GD5F_PROTECTION, &feature_byte);

gd5f_get_features(dev);
return gd5f_wait_busy(dev);
}

GD5F_RESULT gd5f_ecc_off(gd5f* dev)
{
GD5F_WAIT_BUSY();
gd5f_get_features(dev);
uint8_t feature_byte = 0x00;
gd5f_set_feature(dev, GD5F_FEATURE_1, &feature_byte);
GD5F_WAIT_BUSY();
gd5f_get_features(dev);
return gd5f_wait_busy(dev);
}

GD5F_RESULT gd5f_ecc_on(gd5f* dev)
{
GD5F_WAIT_BUSY();
gd5f_get_features(dev);
dev->registers.feature_1 |= GD5F_FEATURE_1_ECC_EN;
gd5f_set_feature(dev, GD5F_FEATURE_1, &dev->registers.feature_1);
GD5F_WAIT_BUSY();
gd5f_get_features(dev);
return gd5f_wait_busy(dev);
}

