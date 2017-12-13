hex6x core0.rmd
hex6x core1.rmd
mergebtbl core0.btbl core1.btbl boot.btbl
b2i2c_ex boot.btbl boot.btbl.i2c
b2ccs_ex boot.btbl.i2c boot.i2c.ccs
qfparse
modify i2crom.ccs spirom_le.dat