@ECHO OFF 

set C_PROJECT=CD_31
set C_PROJECT_ID=851
set C_VESRION=01310600
set C_DATE=20190329




set HEX_ALL_PRODUCTION=.\%C_PROJECT%_%C_VESRION%_%C_PROJECT_ID%_product_all_%C_DATE%.hex


:START

@echo                   * * * * * * * * * * * * * * * * * * * * * * * *   
@echo                   *                                             *
@echo                   *    深 圳 市 维 亿 魄 科 技 有 限 公 司      *
@echo                   *                                             *
@echo                   *            正在烧录手环固件(52832)          * 
@echo                   *                                             *
@echo                   * * * * * * * * * * * * * * * * * * * * * * * *  
@echo off                                                       
@echo.
@echo                                  正在烧录,请等待 ......

@echo on


.\tool\nrfjprog --eraseall
.\tool\nrfjprog --program %HEX_ALL_PRODUCTION%   --reset
:: --verify %HEX_ALL_PRODUCTION%

.\tool\nrfjprog --reset

@echo off                                                       
@echo.
  
pause

cls
goto  START