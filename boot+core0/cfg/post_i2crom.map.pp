section 
{
  param_index    = 0
  boot_mode      = 40
  sw_pll_prediv	 = 0
  sw_pll_mult    = 0
  sw_pll_postdiv = 0
  options        = 1

  core_freq_mhz    = 100
  i2c_clk_freq_khz = 400

  dev_addr_ext = 0x51

  multi_i2c_id = 0
  my_i2c_id    = 1
  address_delay = 0
  exe_file = "boot.i2c.ccs"
}
section 
{
  param_index    = 1
  boot_mode      = 40
  sw_pll_prediv	 = 0
  sw_pll_mult    = 19
  sw_pll_postdiv = 2
   sw_pll_flags = 1
  options        = 1

  core_freq_mhz    = 1000
  i2c_clk_freq_khz = 200

  dev_addr_ext = 0x51

  multi_i2c_id = 0
  my_i2c_id    = 1
  address_delay = 0
  exe_file = "boot.i2c.ccs"
}





