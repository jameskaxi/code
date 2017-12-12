////////////////////////////////////////////////////////////////////
////                                                              ////
////  Advantech Co., Ltd	FPGA Design                           ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
////  this is the sample code for design reference only           ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) Advantech Co., Ltd.  All Rights Reserved       ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
`define	debouncer_counter	16'h61a8                  
`define	PowerOn_idle_time	24'h75300   
`define	PowerOn_fail_time	24'h75300   
`define	PowerOn_v2p5_time	24'h3a980
`define	PowerOn_pmbs_time	24'h3a980
`define	PowerOn_pmb2_time	24'h3a980
`define	PowerOn_v1p8_time	24'h3a980
`define	PowerOn_v1p5_time	24'h3a980
`define	PowerOn_v075_time	24'h3a980
`define	PowerOn_clkg_time	24'h3a980
`define	PowerOn_wait_time	24'h3a980
`define	PowerOk_fail_time	8'h20
`define	PowerOff_delay_time	24'h3a980
`define	Powerfail_time		24'h3a980
`define	Set_Clk2_time		24'hbb80
`define	Set_Clk3_time		24'hbb80

`define DSP_GPIO_Output_delay_time 32'hf

`define POR_assert_delay_time			24'h3a980	
`define POR_release_delay_time			24'h40
`define Resetfull_check_delay_time		24'h61a8
`define Resetfull_assert_delay_time		24'h3a980
`define Resetfull_release_delay_time	24'h40
`define Hardreset_check_delay_time		24'h61a8
`define Hardreset_assert_delay_time		24'h61a8
`define Hardreset_release_delay_time	24'h40
`define Softreset_check_delay_time		24'h61a8
`define Softreset_assert_delay_time		24'h61a8
`define Softreset_release_delay_time	24'h40

`define FPGA_ireset_delay_time			16'h61a8

`define PRNG_CLK_CNT		12'h95F

`define FPGA_UPDATE_UNLOCK_CODE	8'h68

