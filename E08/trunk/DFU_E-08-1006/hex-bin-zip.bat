@ECHO OFF 

set C_PROJECT=E_08
set C_PROJECT_ID=1006
set C_VESRION=00080004
set C_DATE=20190725




:: V_01
::set HEX_PATH=F:\Work_52\V_01\examples\ble_peripheral\ble_app_hrs_freertos\pca10040\s132\arm5_no_packs\_build
:: V_01 test 
::set HEX_PATH=F:\Work_52\V_01_20161022\v-01\examples\ble_peripheral\ble_app_hrs_freertos\pca10040\s132\arm5_no_packs\_build
::set HEX_NAME=nrf52832xxaa_s132_app_v_01.hex
::set ZIP_NAME=nrf52832xxaa_s132_v_01.zip

:: EP_01
::set HEX_PATH=F:\Work_52\EP_02\EP-02-code\examples\ble_peripheral\ble_app_hrs_freertos\pca10040\s132\arm5_no_packs\_build
set HEX_PATH=.\hex
set HEX_NAME=%C_PROJECT%_%C_PROJECT_ID%.hex
set ZIP_NAME=%C_PROJECT%_%C_VESRION%_%C_PROJECT_ID%_%C_DATE%.zip


set KEY_FILE=.\src\priv.pem
set HW_VERSION=52
set APPLICATION_VERSION=0xff
set SD_REQ=0x8C

set HEX_BOOT_SETTINGS=settings.hex
set HEX_PRODUCTION=%C_PROJECT%_%C_VESRION%_product.hex

::set HEX_BOOTLOADER=F:\Work_52\Bootloader\examples\dfu\bootloader_secure\pca10040\arm5_no_packs\_build\nrf52832_xxaa_s132_boot.hex
set HEX_BOOTLOADER=.\src\nrf52832_xxaa_s132_boot_0807.hex
set HEX_SOFTDEVICE=.\src\s132_nrf52_3.0.0_softdevice.hex

set HEX_ALL_PRODUCTION=%C_PROJECT%_%C_VESRION%_%C_PROJECT_ID%_product_all_%C_DATE%.hex


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



del .\*.zip %HEX_ALL_PRODUCTION%  %HEX_BOOT_SETTINGS% %HEX_PRODUCTION%

::boot setting**************************
.\tool\nrfutil.exe settings generate --family NRF52 --application %HEX_PATH%\%HEX_NAME% --application-version %APPLICATION_VERSION% --bootloader-version 1 --bl-settings-version 1 %HEX_BOOT_SETTINGS%


::Production of lossless application file*************
mergehex -m %HEX_PATH%\%HEX_NAME% %HEX_BOOT_SETTINGS% -o %HEX_PRODUCTION%

::Production of lossless all file*************
mergehex -m %HEX_PATH%\%HEX_NAME% %HEX_BOOTLOADER% -o t_app_boot.hex
mergehex -m t_app_boot.hex %HEX_BOOT_SETTINGS% -o t_app_boot_setting.hex
mergehex -m %HEX_SOFTDEVICE% t_app_boot_setting.hex  -o %HEX_ALL_PRODUCTION%

del .\t_app_boot.hex t_app_boot_setting.hex %HEX_BOOT_SETTINGS% %HEX_PRODUCTION%


::dfu file××××××××××××××××*************
::52 debug 
::nrfutil.exe pkg generate --debug-mode --application %HEX_PATH%\%HEX_NAME% --key-file priv.pem %ZIP_NAME%  

::52 application
.\tool\nrfutil.exe pkg generate --hw-version %HW_VERSION% --sd-req %SD_REQ% --application-version %APPLICATION_VERSION% --application %HEX_PATH%\%HEX_NAME% --key-file %KEY_FILE% %ZIP_NAME%


@echo off                                                       
@echo.
   
pause

cls
goto  START
 
::pause

::cls
::goto  START