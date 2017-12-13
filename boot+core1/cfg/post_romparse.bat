copy ..\Debug\core0.out .
copy ..\..\core1\Debug\core1.out .
copy ..\..\core2\Debug\core2.out .
copy ..\..\core3\Debug\core3.out .
copy ..\..\core4\Debug\core4.out .
copy ..\..\core5\Debug\core5.out .
copy ..\..\core6\Debug\core6.out .
copy ..\..\core7\Debug\core7.out .

.\tools\hex6x .\cfg\core0.rmd
.\tools\hex6x .\cfg\core1.rmd
.\tools\hex6x .\cfg\core2.rmd
.\tools\hex6x .\cfg\core3.rmd
.\tools\hex6x .\cfg\core4.rmd
.\tools\hex6x .\cfg\core5.rmd
.\tools\hex6x .\cfg\core6.rmd
.\tools\hex6x .\cfg\core7.rmd
.\tools\mergebtbl core0.btbl core1.btbl core2.btbl core3.btbl core4.btbl core5.btbl core6.btbl core7.btbl boot.btbl
.\tools\b2i2c_ex boot.btbl boot.i2c
.\tools\b2ccs_ex boot.i2c simple.i2c.ccs
.\tools\qfparse
.\tools\modify i2crom.ccs boot_le.dat
.\tools\ccs2bin boot_le.dat boot_le.bin
copy boot_le.bin ..\..\norflash\Debug


