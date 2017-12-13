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
////  File name	: shevm_fpga.v			                          ////
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

module shevm_fpga (
		//GLOBAL
	input wire MAIN_48MHZ_CLK_R_p,
	input wire VCC3_AUX_PGOOD_p,

		//DSP SPI
//	output wire DSP_SSPCS1_p,
//	output wire DSP_SSPCK_p,
//	output wire DSP_SSPMISO_p,
//	input wire DSP_SSPMOSI_p,
	
		//POWER ON
	input wire VCC5_PGOOD_p,
	input wire VCC2P5_PGOOD_p,
	input wire VCC0P75_PGOOD_p,
	input wire VCC1P5_PGOOD_p,
	output wire SYS_PGOOD_p,
	input wire VCC1P8_PGOOD_p,
	output wire VCC1P5_EN_p,
	output wire VCC1P8_EN1_p,
	output wire VCC0P75_EN_p,
	output wire VCC2P5_EN_p,
	output wire VCC_5V_EN_p,
	
		//GPIO
	input wire [15:0]BM_GPIO_p,
	inout wire [15:0]DSP_GPIO_p,
    //output wire [15:0]DSP_GPIO_p,
	output wire USER_DEFINE_p,
	
		//DEBUG
	(*keep = "TRUE"*)output wire [3:0] DEBUG_LED_p,
	
		//DSP CONTROL
	output wire DSP_PACLKSEL_p,
	output wire DSP_LRESETNMIENZ_p,
	output wire [3:0]DSP_CORESEL_p,
	output wire DSP_NMIZ_p,
	output wire DSP_LRESETZ_p,
	input wire DSP_HOUT_p,
	input wire DSP_BOOTCOMPLETE_p,
	input wire DSP_SYSCLKOUT_p,
	output wire DSP_PORZ_p,
	output wire DSP_RESETFULLZ_p,
	output wire DSP_RESETZ_p,
	input wire DSP_RESETSTAT_np,
	
		//RESET
	input wire FULL_RESET_p,
	input wire WARM_RESET_p,
	input wire COLD_RESET_p,
	input wire BOARD_RESET_p,
	
		//MISC
    input wire DSP_VCL_1_p,
    inout wire DSP_VD_1_p,
	output wire NAND_WP_p,
//	XDS560_IL_p,
    output wire NOR_WP_np,
    input wire NORFLASH_RST_N_p,
	output wire EEPROM_WP_p,
	output wire PCA9306_EN_p,
//	TRGRSTZ_p, 
	output wire PCIESSEN_p,  
	output wire DSP_TIMI0_p,             
	
		//CLOCK
	output wire CLOCK2_SSPCS1_p,
	output wire CLOCK2_SSPCK_p,
	output wire CLOCK2_SSPSI_p,
	input wire CLOCK2_SSPSO_p,
	output wire REFCLK2_PD_np,
	input wire CLOCK2_PLL_LOCK_p,	
	output wire CLOCK3_SSPCS1_p,
	output wire CLOCK3_SSPCK_p,
	output wire CLOCK3_SSPSI_p,
	input wire CLOCK3_SSPSO_p,
	output wire REFCLK3_PD_np,
	input wire CLOCK3_PLL_LOCK_p,
	
		//UCD9222
	input wire UCD9222_PG1_p,
	input wire UCD9222_PG2_p,
	output wire UCD9222_ENA1_p,
	output wire UCD9222_ENA2_p,
	input wire PGUCD9222_p,
	output wire UCD9222_RST_np,
	
	output wire DSP_TSIP0_CLKA0_p,
	output wire DSP_TSIP0_CLKB0_p,
	output wire DSP_TSIP1_CLKA1_p,
	output wire DSP_TSIP1_CLKB1_p,
	output wire DSP_TSIP0_FSA0_p,
	output wire DSP_TSIP0_FSB0_p,
	output wire DSP_TSIP1_FSA1_p,
	output wire DSP_TSIP1_FSB1_p,
    
    output wire PMBUS_CTL
);

/*******************************/  
/**** 	GLOBAL	   *****/
/*******************************/
wire		main_48mhz_clk_r_i;
wire [21:0] debug_sig;
wire lock, vio_rstn;
wire ucd_en1, ucd_en2, ucd_pw1, ucd_pw2, ucd9222_rstn, ucd_rst_vion;
wire pmbus_ctl_d;
//wire [15:0] dsp_gpio_in;

//assign DEBUG_LED_p = {dsp_gpio_in[2], dsp_gpio_in[1], dsp_gpio_in[1:0]};

assign PMBUS_CTL = 1'b1;

assign UCD9222_ENA1_p = ucd_en1 | ucd_pw1;
assign UCD9222_ENA2_p = ucd_en2 | ucd_pw2;
assign UCD9222_RST_np = ucd9222_rstn & ucd_rst_vion;
assign FPGA_rst = (~vio_rstn) & lock & BOARD_RESET_p;

//assign DSP_GPIO_p = 16'h081b; i2c 0x51 boot param 0
//assign DSP_GPIO_p = 16'h1801; 

// Instantiate the module
shevm_fpga_core core (
    .main_48mhz_clk_r_i(main_48mhz_clk_r_i), 
    .fpga_rstn(FPGA_rst), 
    .vcc5_pgood_i(VCC5_PGOOD_p),   
    .vcc2p5_pgood_i(VCC2P5_PGOOD_p), 
    .vcc0p75_pgood_i(VCC0P75_PGOOD_p),
    .vcc1p5_pgood_i(VCC1P5_PGOOD_p), 
    .vcc1p8_pgood_i(VCC1P8_PGOOD_p), 

    .sys_pgood_o(SYS_PGOOD_p), 
    .vcc_5v_en_o(VCC_5V_EN_p), 
    .vcc2p5_en_o(VCC2P5_EN_p), 
    .vcc1p8_en1_o(VCC1P8_EN1_p), 
    .vcc1p5_en_o(VCC1P5_EN_p), 
    .vcc0p75_en_o(VCC0P75_EN_p), 
    .bm_gpio_i(BM_GPIO_p), 
    .dsp_gpio_io(DSP_GPIO_p), 
    .full_reset_i(FULL_RESET_p), 
    .cold_reset_i(COLD_RESET_p), 
    .warm_reset_i(WARM_RESET_p), 
    //.fpga_rst_n_i(BOARD_RESET_p), 
    .pciessen_i(PCIESSEN_p), 
    .user_define_i(USER_DEFINE_p), 
    .debug_led_o(DEBUG_LED_p), 
    .dsp_porz_o(DSP_PORZ_p), 
    .dsp_resetfullz_o(DSP_RESETFULLZ_p), 
    .dsp_resetz_o(DSP_RESETZ_p), 
    .dsp_lresetz_o(DSP_LRESETZ_p), 
    .dsp_coresel_o(DSP_CORESEL_p), 
    .dsp_resetstat_ni(DSP_RESETSTAT_np), 
    .dsp_nmiz_o(DSP_NMIZ_p), 
    .dsp_lresetnmienz_o(DSP_LRESETNMIENZ_p), 
    .dsp_bootcomplete_i(DSP_BOOTCOMPLETE_p), 
    .dsp_paclksel_o(DSP_PACLKSEL_p), 
    .dsp_hout_i(DSP_HOUT_p), 
    .dsp_sysclkout_i(DSP_SYSCLKOUT_p), 
    .dsp_timi0_o(DSP_TIMI0_p), 
    .ucd9222_pg1_i(UCD9222_PG1_p), 
    .ucd9222_pg2_i(UCD9222_PG2_p), 
    .ucd9222_ena1_o(ucd_pw1/*UCD9222_ENA1_p*/), 
    .ucd9222_ena2_o(ucd_pw2/*UCD9222_ENA2_p*/), 
    .pgucd9222_i(PGUCD9222_p), 
    .ucd9222_rst_no(ucd9222_rstn/*UCD9222_RST_np*/), 
    .dsp_vcl_1_i(DSP_VCL_1_p), 
    .dsp_vd_1_o(DSP_VD_1_p), 
    .pca9306_en_o(PCA9306_EN_p), 
    .nand_wp_o(NAND_WP_p), 
    .nor_wp_no(NOR_WP_np), 
    .eeprom_wp_o(EEPROM_WP_p), 
    .clock2_sspcs1_o(CLOCK2_SSPCS1_p), 
    .clock2_sspck_o(CLOCK2_SSPCK_p), 
    .clock2_sspsi_o(CLOCK2_SSPSI_p), 
    .clock2_sspso_i(CLOCK2_SSPSO_p), 
    .refclk2_pd_no(REFCLK2_PD_np), 
    .clock2_pll_lock_i(CLOCK2_PLL_LOCK_p), 
    .clock3_sspcs1_o(CLOCK3_SSPCS1_p), 
    .clock3_sspck_o(CLOCK3_SSPCK_p), 
    .clock3_sspsi_o(CLOCK3_SSPSI_p), 
    .clock3_sspso_i(CLOCK3_SSPSO_p), 
    .refclk3_pd_no(REFCLK3_PD_np), 
    .clock3_pll_lock_i(CLOCK3_PLL_LOCK_p),
    //.dsp_gpio_in(dsp_gpio_in),
    .debug_sig(debug_sig)
    );
    
    // Instantiate the module
dcm dcm_inst (
    .CLKIN_IN(MAIN_48MHZ_CLK_R_p), 
    .RST_IN(),
    .CLKIN_IBUFG_OUT(), 
    .CLK0_OUT(main_48mhz_clk_r_i), 
    .LOCKED_OUT(lock)
    );
	//BUFG BUFG_main_clk ( .O(main_48mhz_clk_r_i), .I(MAIN_48MHZ_CLK_R_p));
/*********************************/  
/**** 	Differential 2 Single*****/
/*********************************/

  diff_2_sig_clk DIFF( .FPGA_48MHz(main_48mhz_clk_r_i)       ,.FPGA_rst(FPGA_rst) 
                      ,.S_TDM_CLKA(S_TDM_CLKA)               ,.S_TDM_CLKB(1'b0)        
                      ,.S_TDM_CLKC(S_TDM_CLKC)               ,.S_TDM_CLKD(1'b0)
                      ,.DSP_TSIP0_CLKA0_o(DSP_TSIP0_CLKA0_t) ,.DSP_TSIP0_CLKB0_o(DSP_TSIP0_CLKB0_t)
                      ,.DSP_TSIP1_CLKA1_o(DSP_TSIP1_CLKA1_t) ,.DSP_TSIP1_CLKB1_o(DSP_TSIP1_CLKB1_t)
                      ,.DSP_TSIP0_FSA0_o(DSP_TSIP0_FSA0_t)   ,.DSP_TSIP0_FSB0_o(DSP_TSIP0_FSB0_t)  
                      ,.DSP_TSIP1_FSA1_o(DSP_TSIP1_FSA1_t)   ,.DSP_TSIP1_FSB1_o(DSP_TSIP1_FSB1_t));



assign DSP_TSIP0_CLKA0_p  = (~VCC0P75_PGOOD_p) ? 1'b0 : DSP_TSIP0_CLKA0_t; 
assign DSP_TSIP0_CLKB0_p  = (~VCC0P75_PGOOD_p) ? 1'b0 : DSP_TSIP0_CLKB0_t; 
assign DSP_TSIP1_CLKA1_p  = (~VCC0P75_PGOOD_p) ? 1'b0 : DSP_TSIP1_CLKA1_t; 
assign DSP_TSIP1_CLKB1_p  = (~VCC0P75_PGOOD_p) ? 1'b0 : DSP_TSIP1_CLKB1_t; 
assign DSP_TSIP0_FSA0_p   = (~VCC0P75_PGOOD_p) ? 1'b0 : DSP_TSIP0_FSA0_t ; 
assign DSP_TSIP0_FSB0_p   = (~VCC0P75_PGOOD_p) ? 1'b0 : DSP_TSIP0_FSB0_t ; 
assign DSP_TSIP1_FSA1_p   = (~VCC0P75_PGOOD_p) ? 1'b0 : DSP_TSIP1_FSA1_t ; 
assign DSP_TSIP1_FSB1_p   = (~VCC0P75_PGOOD_p) ? 1'b0 : DSP_TSIP1_FSB1_t ; 

wire [35:0] CONTROL0, CONTROL1;
wire [7:0] SYNC_IN, SYNC_OUT;
wire [127:0] TRIG0;

assign SYNC_IN = {4'd0, 4'b1111};
assign vio_rstn = SYNC_OUT[0];
assign ucd_en1 = SYNC_OUT[1];
assign ucd_en2 = SYNC_OUT[2];
assign ucd_rst_vion = !SYNC_OUT[3];
assign TRIG0 = {95'd0, pmbus_ctl_d, ucd_en1, ucd_en2, vio_rstn, lock, debug_sig, FPGA_rst, DEBUG_LED_p[3:0], DSP_PORZ_p, DSP_RESETFULLZ_p, DSP_RESETZ_p};

vio vio_inst (
    .CONTROL(CONTROL0), // INOUT BUS [35:0]
    .CLK(main_48mhz_clk_r_i), // IN
    .SYNC_IN(SYNC_IN), // IN BUS [7:0]
    .SYNC_OUT(SYNC_OUT) // OUT BUS [7:0]
);
icon icon_inst (
    .CONTROL0(CONTROL0), // INOUT BUS [35:0]
    .CONTROL1(CONTROL1) // INOUT BUS [35:0]
);

ila ila_inst (
    .CONTROL(CONTROL1), // INOUT BUS [35:0]
    .CLK(main_48mhz_clk_r_i), // IN
    .TRIG0(TRIG0) // IN BUS [127:0]
);
endmodule 

