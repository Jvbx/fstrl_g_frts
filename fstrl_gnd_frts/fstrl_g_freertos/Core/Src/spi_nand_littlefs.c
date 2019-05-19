/*
 * spi_nand_littlefs.c
 *
 *  Created on: May 19, 2019
 *      Author: jvbx0
 */


#include "spi_nand_littlefs.h"
#include "gd5f.h"
#include "lfs.h"


gd5f  spi_nand_gd5f;


static uint8_t lfc_rxd_buf[LFS_RES_CACHE_SIZE] = {0};
static uint8_t lfc_txd_buf[LFS_RES_CACHE_SIZE] = {0};
static uint8_t lfc_lkhd_buf[LFS_RES_LOOKAHEAD_SIZE] __attribute__((aligned(8))) = {0};



static int gd5f_block_device_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size){

uint16_t page_num = (block * LFS_PHYSICAL_PAGES_PER_BLOCK) + (off/LFS_PHYSICAL_PAGESIZE_TOTAL);
uint16_t column_num = (off % LFS_PHYSICAL_PAGESIZE_TOTAL);

if (gd5f_random_read_page(&spi_nand_gd5f, page_num, column_num, buffer, size) != GD5F_OK) {return LFS_ERR_IO;}
return LFS_ERR_OK;
}

static int gd5f_block_device_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size){

uint16_t page_num = (block * LFS_PHYSICAL_PAGES_PER_BLOCK) + (off/LFS_PHYSICAL_PAGESIZE_TOTAL);
uint16_t column_num = (off % LFS_PHYSICAL_PAGESIZE_TOTAL);

if  (gd5f_random_write_page(&spi_nand_gd5f, page_num, column_num, (uint8_t*) buffer, size) != GD5F_OK) {return LFS_ERR_IO;}
return LFS_ERR_OK;
}


static int gd5f_block_device_erase(const struct lfs_config *c, lfs_block_t block){
if  (gd5f_blockerase(&spi_nand_gd5f, block) != GD5F_OK) {return LFS_ERR_IO;}
return LFS_ERR_OK;
}


static int gd5f_block_device_sync(const struct lfs_config *c){
if  (gd5f_get_features(&spi_nand_gd5f) != GD5F_OK) {return LFS_ERR_IO;}
return LFS_ERR_OK;
}


// configuration of the filesystem is provided by this struct
const struct lfs_config cfg = {
    // block device operations
    .read  = gd5f_block_device_read,
    .prog  = gd5f_block_device_prog,
    .erase = gd5f_block_device_erase,
    .sync  = gd5f_block_device_sync,

    // block device configuration
    .read_size        = LFS_LOGICAL_READ_SIZE,
    .prog_size        = LFS_LOGICAL_PROG_SIZE,
    .block_size       = LFS_LOGICAL_BLOCK_SIZE,
    .block_count      = LFS_LOGICAL_BLOCK_COUNT,
    .block_cycles     = LFS_RES_BLOCK_CYCLES,
    .cache_size       = LFS_RES_CACHE_SIZE,
    .lookahead_size   = LFS_RES_LOOKAHEAD_SIZE,
    .read_buffer      = lfc_rxd_buf,
    .prog_buffer      = lfc_txd_buf,
    .lookahead_buffer = lfc_lkhd_buf,
};

