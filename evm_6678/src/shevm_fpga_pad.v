//////////////////////////////////////////////////////////////////////
////                                                              ////
////  Advantech Co., Ltd	FPGA Design                           ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
////  this is the sample code for design reference only           ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////  Project	: TI TMX320C6678 EVM FPGA                         ////
////  File name	: shevm_fpga_pad.v		                          ////
////  Description                                                 ////
////      		: TMX320C6678 EVM FPGA Core MODULE                ////
////  Created Date                                                ////
////			: 2010/12/09									  ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) Advantech Co., Ltd.  All Rights Reserved       ////
////                                                              ////
//////////////////////////////////////////////////////////////////////

	`include "params.v"


module shevm_fpga_pad(
	//GLOBAL
	input wire MAIN_48MHZ_CLK_R_p,
	input wire VCC3_AUX_PGOOD_p_i,
	
	output wire main_48mhz_clk_r_i,

	//GPIO
	input wire [15:0] BM_GPIO_p,
	inout wire [15:0] DSP_GPIO_p,
	input wire USER_DEFINE_p,

	//DEBUG
	output wire [3:0] DEBUG_LED_p,

	//OTHER
	input wire DSP_VCL_1_p,
	inout wire DSP_VD_1_p,
	output wire NAND_WP_p,
	output wire NOR_WP_np,
	output wire EEPROM_WP_p,
	output wire PCA9306_EN_p,
	
	output wire PCIESSEN_p, 
	output wire DSP_TIMI0_p,
	 
  output wire DSP_TSIP0_CLKA0_p, 
  output wire DSP_TSIP0_CLKB0_p, 
  output wire DSP_TSIP1_CLKA1_p, 
  output wire DSP_TSIP1_CLKB1_p,
  output wire DSP_TSIP0_FSA0_p, 
  output wire DSP_TSIP0_FSB0_p, 
  output wire DSP_TSIP1_FSA1_p, 
  output wire DSP_TSIP1_FSB1_p 	 
	 
);

/*******************************/  
/**** 	GLOBAL	   *****/
/*******************************/



/*******************************/  
/**** 	DSP CONTROL	   *****/
/*******************************/
//assign dsp_vcl_1_i = DSP_VCL_1_p;  //20101205
//assign DSP_VD_1_p = 1'bZ; //20101205
//assign NAND_WP_p = (~VCC0P75_PGOOD_p) ? 1'b0 : 1'b1;
//(*keep = "TRUE"*) reg nor_wp_d;
//assign NOR_WP_np = (~VCC0P75_PGOOD_p) ? 1'b0 : 1'b1;
//always @(posedge main_48mhz_clk_r_i)
//nor_wp_d <= NOR_WP_np;
//assign EEPROM_WP_p = (~VCC0P75_PGOOD_p) ? 1'b0 : 1'b1;
//assign PCA9306_EN_p = (~VCC0P75_PGOOD_p) ? 1'b0 : pca9306_en_o;
//assign DSP_TIMI0_p = (~VCC0P75_PGOOD_p) ? 1'b0 : dsp_timi0_o;


                    
endmodule 