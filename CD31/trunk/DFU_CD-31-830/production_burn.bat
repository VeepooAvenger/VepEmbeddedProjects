@ECHO OFF 

set C_PROJECT=CD_31
set C_PROJECT_ID=830
set C_VESRION=00310300
set C_DATE=20190330




set HEX_ALL_PRODUCTION=.\%C_PROJECT%_%C_VESRION%_%C_PROJECT_ID%_product_all_%C_DATE%.hex


:START

@echo                   * * * * * * * * * * * * * * * * * * * * * * * *   
@echo                   *                                             *
@echo                   *    �� �� �� ά �� �� �� �� �� �� �� ˾      *
@echo                   *                                             *
@echo                   *            ������¼�ֻ��̼�(52832)          * 
@echo                   *                                             *
@echo                   * * * * * * * * * * * * * * * * * * * * * * * *  
@echo off                                                       
@echo.
@echo                                  ������¼,��ȴ� ......

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