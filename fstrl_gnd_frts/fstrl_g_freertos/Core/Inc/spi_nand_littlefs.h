/*
 * spi_nand_littlefs.h
 *
 *  Created on: May 19, 2019
 *      Author: jvbx0
 */



#ifndef INC_SPI_NAND_LITTLEFS_H_
#define INC_SPI_NAND_LITTLEFS_H_


#include "gd5f.h"



#define USE_HARDWARE_ECC                 ON
#define USE_SPARE_AREA                   OFF

#define LFS_PHYSICAL_BLOCK_COUNT         GD5F_BLOCK_COUNT          // one chip has 1024 blocks
#define LFS_PHYSICAL_PAGES_PER_BLOCK     GD5F_PAGES_PER_BLOCK      // each block contains 64 pages
#define LFS_PHYSICAL_PAGESIZE_MAIN       GD5F_PAGE_MAIN_AREA_SIZE  // each page has main area consists of 2048 bytes



#if (USE_HARDWARE_ECC)
#define LFS_PHYSICAL_PAGESIZE_SPARE      GD5F_SPARE_SIZE_ECC_ON    // and additional 64 or 128 spare bytes
#else                                                              // (depends on hardware ECC status)
#define LFS_PHYSICAL_PAGESIZE_SPARE      GD5F_SPARE_SIZE_ECC_OFF
#endif

#if (USE_SPARE_AREA)
#define LFS_PHYSICAL_PAGESIZE_TOTAL      (LFS_PHYSICAL_PAGESIZE_MAIN + LFS_PHYSICAL_PAGESIZE_SPARE)
#else
#define LFS_PHYSICAL_PAGESIZE_TOTAL      (LFS_PHYSICAL_PAGESIZE_MAIN)
#endif

#define LFS_PHYSICAL_BLOCK_SIZE          (LFS_PHYSICAL_PAGESIZE_TOTAL * LFS_PHYSICAL_PAGES_PER_BLOCK)
#define LFS_PHYSICAL_FLASH_SIZE          (LFS_PHYSICAL_BLOCK_SIZE * LFS_PHYSICAL_BLOCK_COUNT)

#define LFS_LOGICAL_READ_SIZE            512
#define LFS_LOGICAL_PROG_SIZE            512
#define LFS_LOGICAL_BLOCK_SIZE           LFS_PHYSICAL_BLOCK_SIZE
#define LFS_LOGICAL_BLOCK_COUNT          LFS_PHYSICAL_BLOCK_COUNT

#define LFS_RES_BLOCK_CYCLES             512
#define LFS_RES_CACHE_SIZE               512
#define LFS_RES_LOOKAHEAD_SIZE           (LFS_LOGICAL_BLOCK_COUNT >> 3) // (LFS_LOGICAL_BLOCK_COUNT >> 3) = 1024 >> 3 = 1024/8 = 128


#endif /* INC_SPI_NAND_LITTLEFS_H_ */
