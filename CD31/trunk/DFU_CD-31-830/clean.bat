@ECHO OFF 

::set HEX_PATH=F:\Work_52\V_01\examples\ble_peripheral\ble_app_hrs_freertos\pca10040\s132\arm5_no_packs\_build
set HEX_PATH=F:\Work_52\V_01_20161022\v-01\examples\ble_peripheral\ble_app_hrs_freertos\pca10040\s132\arm5_no_packs\_build
set HEX_NAME=nrf52832_xxaa.hex
set ZIP_NAME=nrf52832xxaa_s132_v_01.zip

set KEY_FILE=priv.pem
set HW_VERSION=52
set APPLICATION_VERSION=0xff
set SD_REQ=0x8C

set HEX_BOOT_SETTINGS=settings.hex
set HEX_PRODUCTION=nrf52832_xxaa_product.hex

set HEX_BOOTLOADER=F:\Work_52\Bootloader\examples\dfu\bootloader_secure\pca10040\s132\arm5_no_packs\_build\nrf52832_xxaa_s132_boot.hex
set HEX_SOFTDEVICE=s132_nrf52_3.0.0_softdevice.hex

set HEX_ALL_PRODUCTION=nrf52832_xxaa_product_all.hex

:START

@echo                   * * * * * * * * * * * * * * * * * * * * * * * *   
@echo                   *                                             *
@echo                   *    深 圳 市 维 亿 魄 科 技 有 限 公 司      *
@echo                   *                                             *
@echo                   *            正在生成手环代码(52832)          * 
@echo                   *                                             *
@echo                   * * * * * * * * * * * * * * * * * * * * * * * *  
@echo off                                                       
@echo.
@echo                                  正在转换,请等待 ......

@echo on

del .\*.zip 
::%HEX_ALL_PRODUCTION%  %HEX_BOOT_SETTINGS% %HEX_PRODUCTION%
del .\*.hex

@echo off                                                       
@echo.
  
::pause

::cls
::goto  START