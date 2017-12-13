////////////////////////////////////////////////////////////////////
////																															////
////	Advantech	Co., Ltd	FPGA Design														////
////																															////
//////////////////////////////////////////////////////////////////////
////																															////
////	this is	the	sample code	for	design reference only						////
////																															////
//////////////////////////////////////////////////////////////////////
////	Project	:	TI TMX320C6678 EVM FPGA													////
////	File name	:	shevm_fpga_core.v															////
////	Description																									////
////					:	TMX320C6678	EVM	FPGA Core	MODULE								////
////	Created	Date																								////
////			:	2010/12/09										////
//////////////////////////////////////////////////////////////////////
////																															////
//// Copyright (C) Advantech Co.,	Ltd.	All	Rights Reserved				////
////																															////
//////////////////////////////////////////////////////////////////////

`include "params.v"


`define	Rev_ID	16'h000B

module shevm_fpga_core(
           //GLOBAL
           input wire main_48mhz_clk_r_i,   	//FPGA 48MHZ Reference clock source
           input wire fpga_rstn,     	//FPGA power good indication
            input wire	vcc5_pgood_i,
            input wire	vcc2p5_pgood_i,
            input wire	vcc0p75_pgood_i,
            input wire	vcc1p5_pgood_i,
            input wire	vcc1p8_pgood_i,
					 
           output wire sys_pgood_o,			//system power good indication
           output reg vcc_5v_en_o,    		//5V power enable
           output reg vcc2p5_en_o,			//2.5V power enable
           output reg vcc1p8_en1_o,          	//1.8V power enable
           output reg vcc1p5_en_o,			//1.5V power enable
           output reg vcc0p75_en_o,			//0.75V power enable

           //GPIO
           input wire [15:0] bm_gpio_i,	           //Boot Mode switch inputs
           inout wire [15:0] dsp_gpio_io,				//DSP GPIO outputs for boot strapping
           //output wire [15:0] dsp_gpio_io,				//DSP GPIO outputs for boot strapping

           //RESET	buttons & input switch
           input wire full_reset_i,			//full reset button input
           input wire cold_reset_i,			//hard(cold) reset button input
           input wire warm_reset_i,			//warm reset button input
           //input wire fpga_rst_n_i,			//FPGA JTAG reset input
           //trgrstz_i, 				//trgrst from emulation card
           input wire pciessen_i,				//PCIESSEN switch for boot strapping
           input wire user_define_i,  		//User Defined switch input

           //DEBUG
           output wire [3:0]debug_led_o,			//Debug LEDs

           //DSP Interface
           output wire dsp_porz_o,				//DSP POR
           output wire dsp_resetfullz_o,		//DSP Full Reset
           output wire dsp_resetz_o,			//DSP Reset
           output wire dsp_lresetz_o,			//DSP local reset
           output wire dsp_coresel_o,			//DSP core select
           input wire dsp_resetstat_ni,		//DSP resetstat
           output wire dsp_nmiz_o,				//DSP NMI
           output wire dsp_lresetnmienz_o,		//DSP Lreset and NMI enable
           input wire dsp_bootcomplete_i,		//DSP boot complete indication
           output wire dsp_paclksel_o,			//DSP PA clock select
           input wire dsp_hout_i,				//DSP HOUT
           input wire dsp_sysclkout_i,		//DSP SYSCLKOUT
           output reg dsp_timi0_o,			//DSP Timier 0 and PCIESSEN boot strapping

           //UCD9222
           input wire ucd9222_pg1_i,			//ucd9222 #1 power good and also for the CVDD DSP core power good
           input wire ucd9222_pg2_i,			//ucd9222 #2 power good and also for the VCC1V0 DSP core power good
           output reg ucd9222_ena1_o,   		//ucd9222 #1 power and the CVDD DSP core power enable
           output reg ucd9222_ena2_o,			//ucd9222 #2 power and the VCC1V0 DSP core power enable
           input wire pgucd9222_i,			//both ucd9222 and the VCC1V0/CVDD DSP core power are valid
           output reg ucd9222_rst_no,			//ucd9222 reset

           //PMBUS	& Smart-Reflex
           input wire dsp_vcl_1_i,			//DSP smart-reflex clock
           output wire dsp_vd_1_o,				//DSP smrat-reflex data
           output wire pca9306_en_o,			//smart-reflex buffer enable

           //MISC
           output wire nand_wp_o,				//NAND flash write protect
           output wire nor_wp_no,				//NOR flash write protect
           output wire eeprom_wp_o,			//EEPROM write protect

           //CLOCK Generator SPI interface & Control
           output wire clock2_sspcs1_o,
           output wire clock2_sspck_o,
           output wire clock2_sspsi_o,
           input wire clock2_sspso_i,
           output reg refclk2_pd_no,
           input wire clock2_pll_lock_i,
           output wire clock3_sspcs1_o,
           output wire clock3_sspck_o,
           output wire clock3_sspsi_o,
           input wire clock3_sspso_i,
           output reg refclk3_pd_no,
           input wire clock3_pll_lock_i,
           //output wire [15:0] dsp_gpio_in,
           output wire [21:0] debug_sig
       );
/************************/
/****		GPIO			*****/
/************************/
reg		dsp_gpio_en;
reg		[31:0]	dsp_gpio_ctrl_delay;
reg		clock2_sspcs1_d1;
wire	fpga_update_lock_reset_n;	 //FPGA	Update Lock	Register Reset

//(*KEEP = "TRUE" *) 

reg		[23:0]	PW_c;
reg		[7:0]	fail_power_c;
reg		power_fail;
reg		all_power;

reg		[23:0]	set_clk2_c;
reg		[23:0]	set_clk3_c;
reg		set_clk2;
reg		set_clk3;

reg		half_clk;

// power good	signals	sync to	48MHz	clock
reg		[1:0]	vcc5_pgood_d;
reg		[1:0]	vcc2p5_pgood_d;
reg		[1:0]	vcc1p8_pgood_d;
reg		[1:0]	vcc1p5_pgood_d;
reg		[1:0]	vcc0p75_pgood_d;
reg		[1:0]	ucd9222_pg1_d;
reg		[1:0]	ucd9222_pg2_d;
reg		[1:0]	pgucd9222_d;
reg		[1:0]	clock2_pll_lock_d;
reg		[1:0]	clock3_pll_lock_d;
reg		[1:0]	dsp_resetstat_nd;

reg		[15:0]	bm_gpio;
reg		[15:0]	dsp_gpio;
reg		fpga_rst_n;
reg		cold_reset_s1;
reg		cold_reset_s2;
reg		warm_reset_s1;
reg		warm_reset_s2;
reg		full_reset_s1;
reg		full_reset_s2;
reg		user_define;	//DC switch	with 1 sync	should be	OK
reg		pciessen;
//reg		trgrstz_s1;
//reg		trgrstz;
reg		dsp_resetstat_n;	//stable signal	with 1 sync	should be	OK
reg		spi2_busy_s1;
reg		spi2_busy_s2;
reg		spi3_busy_s1;
reg		spi3_busy_s2;
reg		set_spi_reg_128;
reg		spi_reg_128_s1;
reg		spi_reg_128;
reg		set_spi_reg_256;
reg		spi_reg_256_s1;
reg		spi_reg_256;


reg		[3:0]	PG_ST;

reg		clock3_sspcs1_d1;

reg		fullreset_n_48s1;
reg		fullreset_n_48s2;
reg		warmreset_n_48s1;
reg		warmreset_n_48s2;
reg		coldreset_n_48s1;
reg		coldreset_n_48s2;
reg		dsp_resetstat_n_48s1;
reg		dsp_resetstat_n_48s2;
reg		fpga_rst_n_48s1;
reg		fpga_rst_n_48s2;

///////////////////////////
reg		fpga_update_lock_reset_n_48s1;
reg		fpga_update_lock_reset_n_48s2;
reg		fpga_update_lock_reset_n_48s3;
////////////////////////////

reg		por_req_n;
reg		resetfull_req_n;
reg			hardreset_req_n;
reg			softreset_req_n;

wire		por_out_n;
wire		resetfull_out_n;
wire		reset_out_n;

wire	spi_rw;
wire	spi_cs;
wire	spi_rdy;
wire	[7:0]	spi_w;
wire	[7:0]	spi_a;

wire	[31:0]	clock2_datao;
wire	[31:0]	clock3_datao;
wire	spi2_busy;
wire	spi3_busy;
wire	[7:0]	clock2_baudi;
wire	[31:0]	clock2_datai;
wire	[7:0]	clock3_baudi;
wire	[31:0]	clock3_datai;

wire	clock2_pll_lock_status;
wire	clock3_pll_lock_status;

wire	clock2_pll_lock;
wire	clock3_pll_lock;
wire		lock_reset;									// reset lock	value	during the POR & RESETFULL
wire	power_fail_check;

reg		[7:0]	latch_pgs;
reg		 [27:0]	freerun_counter;
reg		 [3:0] heart_beat;

wire [15:0] dsp_gpio_in;
wire [15:0] dsp_gpio_d;

(*mark_debug = "true"*)reg		[3:0]	PW_SM;		//Power	Sequence State-Machine

assign debug_sig = {dsp_gpio_en, dsp_gpio_in, PW_SM[3:0], nand_wp_o};

always@(posedge	main_48mhz_clk_r_i or	negedge	fpga_rstn) begin
        if (~fpga_rstn) begin
                freerun_counter	<= 28'h0;
                heart_beat <=	4'h0;
            end
        else if(freerun_counter[23:0]==24'h0) begin
                freerun_counter	<= freerun_counter + 1'b1;
                heart_beat <=	heart_beat + 1'b1;
            end
        else begin
                freerun_counter	<= freerun_counter + 1'b1;
            end
    end

//`endif

always@(freerun_counter	or latch_pgs) begin
        case(freerun_counter[27:26])
            2'b00	:
                PG_ST[3:0] <=	{2'b00,	latch_pgs[1:0]};
            2'b01	:
                PG_ST[3:0] <=	{2'b01,	latch_pgs[3:2]};
            2'b10	:
                PG_ST[3:0] <=	{2'b10,	latch_pgs[5:4]};
            2'b11	:
                PG_ST[3:0] <=	{2'b11,	latch_pgs[7:6]};
        endcase
    end




parameter	SM_IDLE	=4'h0;
parameter	SM_V2P5	=4'h1;
parameter	SM_PMBS	=4'h2;
parameter	SM_PMB2	=4'h3;
parameter	SM_V1P8	=4'h4;
parameter	SM_V1P5	=4'h5;
parameter	SM_V075	=4'h6;
parameter	SM_CLKG	=4'h7;
parameter	SM_PORWR	=4'h8;
parameter	SM_WAIT	=4'h9;
parameter	SM_PWOK	=4'ha;
parameter	SM_POFF1	=4'hb;
parameter	SM_POFF2	=4'hc;
parameter	SM_FAIL	=4'hd;	 //PM_Fail //20110103
////////////////////////////////////////////////////////////
/***********************/
/****	Assignments	 *****/
/***********************/
assign	dsp_coresel_o						=	(PW_SM>=SM_CLKG &&	PW_SM<=SM_PWOK) ? 4'h0	:	4'h0;
assign	dsp_paclksel_o 					= 1'b1;
assign	dsp_lresetnmienz_o 			= (PW_SM>=SM_CLKG	&& PW_SM<=SM_PWOK)	?	1'b1 : 1'b0;
assign	dsp_nmiz_o 							= (PW_SM>=SM_CLKG && PW_SM<=SM_POFF1)	?	1'b1 : 1'b0;
assign	dsp_lresetz_o						=	(PW_SM>=SM_CLKG &&	PW_SM<=SM_POFF1)? 1'b1	:	1'b0;

assign	dsp_porz_o 							= (PW_SM>=SM_PORWR&& PW_SM<=SM_PWOK)	?	por_out_n	:	1'b0;
assign	dsp_resetfullz_o 				= (PW_SM>=SM_WAIT	&& PW_SM<=SM_POFF1)	?	resetfull_out_n	:	1'b0;
assign	dsp_resetz_o 						= (PW_SM>=SM_CLKG	&& PW_SM<=SM_POFF1)	?	reset_out_n	:	1'b0;


//Emif boot 
//assign	dsp_gpio_d 					= (dsp_gpio_en) ? 16'h1821 : 16'hzzzz;
//I2c master boot addr = 0x51
//assign	dsp_gpio_io 					= 16'h182b;
//I2c master boot addr = 0x50
assign	dsp_gpio_io 					= (dsp_gpio_en) ? 16'h140d : 16'hzzzz;//spi boot

assign dsp_gpio_in = dsp_gpio_io;

assign	debug_led_o	= {dsp_gpio_in[2], dsp_gpio_in[3], dsp_gpio_in[1], dsp_gpio_in[0]};

assign	power_fail_check 				= (PW_SM==SM_POFF1	|| PW_SM==SM_POFF2 ||	PW_SM==SM_FAIL)?	1'b1 : 1'b0;


assign	nand_wp_o								=	(PW_SM==4'ha &&	dsp_resetstat_ni==1'b1 &&	dsp_gpio_en==1'b0) ? 1'b1 : 1'b0;
assign	nor_wp_no								=	(PW_SM==4'ha &&	dsp_resetstat_ni==1'b1 &&	dsp_gpio_en==1'b0) ? 1'b1	:	1'b0;
assign	eeprom_wp_o							=	(PW_SM==4'ha &&	dsp_resetstat_ni==1'b1 &&	dsp_gpio_en==1'b0) ? 1'b0	:	1'b0;
assign	pca9306_en_o	 					= (PW_SM==4'ha) ? 1'b1	:	1'b0;

assign	sys_pgood_o	=	all_power;

assign	clock2_baudi = 8'd2;
assign	clock2_datai = 32'd0;
assign	clock3_baudi = 8'd2;
assign	clock3_datai = 32'd0;

assign	fpga_update_lock_reset_n = (dsp_porz_o==1'b0 ||	dsp_resetfullz_o==1'b0)? 1'b0	:	1'b1;
assign	lock_reset = (~fpga_update_lock_reset_n_48s2 &&	fpga_update_lock_reset_n_48s3	)? 1'b1	:	1'b0;

//assign	fpga_update_en = (spi_reg[119:112]==`FPGA_UPDATE_UNLOCK_CODE)? 1'b1	:	1'b0;

assign clock2_pll_lock = clock2_pll_lock_status;
assign clock3_pll_lock = clock3_pll_lock_status;


//FPGA Internal	Reset	Block
//fpga_internal_reset	i_reset(main_48mhz_clk_r_i,	vcc3_aux_pgood_i,	fpga_rstn);


CDCE62005_2x clk2
             (.FPGA_48MHz(main_48mhz_clk_r_i)			,.FPGA_rst(fpga_rstn)					,.Vccpg(set_clk2)
              ,.PLL_Lock(clock2_pll_lock_status)		,.start()					,.busy(spi2_busy)
              ,.ready(spi2_ready)									,.iClock_div(clock2_baudi)				 ,.Write_data(clock2_datai)
              ,.Read_data(clock2_datao)						,.CLOCK2_SSPCS_o(clock2_sspcs1_o)	,.CLOCK2_SSPCK_o(clock2_sspck_o)
              ,.CLOCK2_SSPSI_o(clock2_sspsi_o)			,.CLOCK2_SSPSO_i(clock2_sspso_i) );


CDCE62005_3x clk3
             (.FPGA_48MHz(main_48mhz_clk_r_i)			,.FPGA_rst(fpga_rstn)				 ,.Vccpg(set_clk3)
              ,.PLL_Lock3(clock3_pll_lock_status)	,.start()						 ,.busy(spi3_busy)
              ,.ready(spi3_ready)									,.iClock_div(clock3_baudi)				 ,.Write_data(clock3_datai)
              ,.Read_data(clock3_datao)						,.CLOCK3_SSPCS_o(clock3_sspcs1_o)	,.CLOCK3_SSPCK_o(clock3_sspck_o)
              ,.CLOCK3_SSPSI_o(clock3_sspsi_o)			,.CLOCK3_SSPSO_i(clock3_sspso_i) );

//////////////////////////////////////////////////////////////////////
// DSP SPI interface
//////////////////////////////////////////////////////////////////////
debouncer	debouncer1(main_48mhz_clk_r_i,fpga_rstn,full_reset_i,full_reset);
debouncer	debouncer2(main_48mhz_clk_r_i,fpga_rstn,warm_reset_i,warm_reset);
debouncer	debouncer3(main_48mhz_clk_r_i,fpga_rstn,cold_reset_i,cold_reset);


always@(posedge	main_48mhz_clk_r_i or	negedge	fpga_rstn) begin
        if (~fpga_rstn) begin
                clock2_sspcs1_d1 <=	1'b1;
                clock3_sspcs1_d1 <=	1'b1;
                fullreset_n_48s1 <=	1'b1;
                fullreset_n_48s2 <=	1'b1;
                warmreset_n_48s1 <=	1'b1;
                warmreset_n_48s2 <=	1'b1;
                coldreset_n_48s1 <=	1'b1;
                coldreset_n_48s2 <=	1'b1;
                dsp_resetstat_n_48s1 <=	1'b1;
                dsp_resetstat_n_48s2 <=	1'b1;
                fpga_rst_n_48s1	<= 1'b1;
                fpga_rst_n_48s2	<= 1'b1;
                por_req_n	<= 1'b1;
                resetfull_req_n	<= 1'b1;
                hardreset_req_n	<= 1'b1;
                softreset_req_n	<= 1'b1;
                fpga_update_lock_reset_n_48s1	<= 1'b1;
                fpga_update_lock_reset_n_48s2	<= 1'b1;
                fpga_update_lock_reset_n_48s3	<= 1'b1;
            end
        else begin
                clock2_sspcs1_d1 <=	clock2_sspcs1_o;
                clock3_sspcs1_d1 <=	clock3_sspcs1_o;
                fullreset_n_48s1 <=	full_reset;
                fullreset_n_48s2 <=	fullreset_n_48s1;
                warmreset_n_48s1 <=	warm_reset;
                warmreset_n_48s2 <=	warmreset_n_48s1;
                coldreset_n_48s1 <=	cold_reset;
                coldreset_n_48s2 <=	coldreset_n_48s1;
                dsp_resetstat_n_48s1 <=	dsp_resetstat_ni;
                dsp_resetstat_n_48s2 <=	dsp_resetstat_n_48s1;
//                fpga_rst_n_48s1	<= fpga_rst_n_i;
//                fpga_rst_n_48s2	<= fpga_rst_n_48s1;
                por_req_n	<= 1'b1;
                resetfull_req_n	<= fullreset_n_48s2;	
                hardreset_req_n	<= 1'b1; 
                softreset_req_n	<= warmreset_n_48s2;		//20110218
                fpga_update_lock_reset_n_48s1	<= fpga_update_lock_reset_n;
                fpga_update_lock_reset_n_48s2	<= fpga_update_lock_reset_n_48s1;
                fpga_update_lock_reset_n_48s3	<= fpga_update_lock_reset_n_48s2;
            end
    end


reset_req_s	reset_req_ctrl(main_48mhz_clk_r_i, fpga_rstn,	por_req_n, resetfull_req_n,	hardreset_req_n,
                           softreset_req_n, dsp_porz_o, dsp_resetfullz_o, dsp_resetz_o, dsp_resetstat_n_48s2, por_out_n,
                           resetfull_out_n, reset_out_n);

////////////// DSP Timer 0
always@(posedge	main_48mhz_clk_r_i or	negedge	fpga_rstn) begin
    if (~fpga_rstn)
        half_clk <=	1'h0;
    else
        half_clk	<= ~half_clk;
end

//////////////// DSP Boot	Configuration
always@(posedge	main_48mhz_clk_r_i or	negedge	fpga_rstn) begin
    if (~fpga_rstn) begin
        dsp_gpio_en	<= 1'b1;
        dsp_gpio_ctrl_delay	<= 32'h0;
        dsp_timi0_o	<= pciessen_i;
    end
    else if	((~dsp_porz_o	&& ~dsp_resetstat_nd[1]) ||	(~dsp_resetfullz_o &&	~dsp_resetstat_nd[1])) begin
        dsp_gpio_en	<= 1'b1;
        dsp_gpio_ctrl_delay	<= 32'h0;
        dsp_timi0_o	<= pciessen_i;

    end
    else if	(~dsp_resetstat_nd[1]	&& dsp_gpio_en ) begin
        dsp_gpio_en	<= 1'b1;
        dsp_gpio_ctrl_delay	<= 32'h0;
        dsp_timi0_o	<= pciessen_i;
    end
    else if	(dsp_gpio_ctrl_delay >=	`DSP_GPIO_Output_delay_time) begin		//1ms
        dsp_gpio_en	<= 1'b0;
        dsp_timi0_o	<= half_clk;
    end
    else begin
        dsp_gpio_en	<= 1'b1;
        dsp_gpio_ctrl_delay	<= dsp_gpio_ctrl_delay + 1;
        dsp_timi0_o	<= pciessen_i;
    end
end

/////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// EVM Power Control & Sequence
//////////////////////////////////////////////////////////////////////

always@(posedge	main_48mhz_clk_r_i or	negedge	fpga_rstn) begin
    if (~fpga_rstn)
        all_power	<= 1'b0;
    else
        all_power	<= vcc5_pgood_d[1] & vcc2p5_pgood_d[1] & ucd9222_pg1_d[1]	&	ucd9222_pg2_d[1] & pgucd9222_d[1]	&	vcc1p8_pgood_d[1]	&	vcc1p5_pgood_d[1]	&	vcc0p75_pgood_d[1];
end

always@(posedge	main_48mhz_clk_r_i or	negedge	fpga_rstn) begin
    if (~fpga_rstn)
        fail_power_c <=	8'h0;
    else if	((PW_SM==4'ha) &&	(~all_power))
        fail_power_c <=	fail_power_c + 1'b1;
    else
        fail_power_c	<= 8'h0;
end

/*******************************/
/****		POWER	ON SEQUENCE	 *****/
/*******************************/

always@(posedge	main_48mhz_clk_r_i or	negedge	fpga_rstn) begin
        if (~fpga_rstn) begin
                PW_SM	<= SM_IDLE;
                ucd9222_ena1_o <=	1'b0;
                ucd9222_ena2_o <=	1'b0;
                ucd9222_rst_no <=	1'b1;
                vcc1p5_en_o	<= 1'b0;
                vcc1p8_en1_o <=	1'b0;
                vcc0p75_en_o <=	1'b0;
                vcc2p5_en_o	<= 1'b0;
                vcc_5v_en_o	<= 1'b1;
                refclk2_pd_no	<= 1'b0;
                refclk3_pd_no	<= 1'b0;
                PW_c <=	24'h0;
                power_fail <=	1'b0;
                vcc5_pgood_d	 <=	2'b00;
                vcc2p5_pgood_d <=	2'b00;
                vcc1p8_pgood_d <=	2'b00;
                vcc1p5_pgood_d <=	2'b00;
                vcc0p75_pgood_d<=	2'b00;
                ucd9222_pg1_d	 <=	2'b00;
                ucd9222_pg2_d	 <=	2'b00;
                pgucd9222_d		 <=	2'b00;
                clock2_pll_lock_d	<= 2'b00;
                clock3_pll_lock_d	<= 2'b00;
                dsp_resetstat_nd	<= 2'b00;
                set_clk2_c <=	24'h0;
                set_clk3_c <=	24'h0;
                set_clk2 <=	1'b0;
                set_clk3 <=	1'b0;
                latch_pgs	<= 8'h0;

                //        fpga_ics557_oe_o	 <=	1'b0;
                //        fpga_ics557_pd_no	 <=	1'b0;

                //        vid_oe_no					 <=	1'b1;
            end
        else begin
                vcc5_pgood_d			<= {vcc5_pgood_d[0],vcc5_pgood_i};
                vcc2p5_pgood_d		<= {vcc2p5_pgood_d[0],vcc2p5_pgood_i};
                vcc1p8_pgood_d		<= {vcc1p8_pgood_d[0],vcc1p8_pgood_i};
                vcc1p5_pgood_d		<= {vcc1p5_pgood_d[0],vcc1p5_pgood_i};
                vcc0p75_pgood_d		<= {vcc0p75_pgood_d[0],vcc0p75_pgood_i};
                ucd9222_pg1_d			<= {ucd9222_pg1_d[0],ucd9222_pg1_i};
                ucd9222_pg2_d			<= {ucd9222_pg2_d[0],ucd9222_pg2_i};
                pgucd9222_d				<= {pgucd9222_d[0],pgucd9222_i};
                clock2_pll_lock_d	<= {clock2_pll_lock_d[0],clock2_pll_lock};
                clock3_pll_lock_d	<= {clock3_pll_lock_d[0],clock3_pll_lock};
                dsp_resetstat_nd	<= {dsp_resetstat_nd[0],dsp_resetstat_ni};

                case (PW_SM)

                    SM_IDLE: begin
                        if	(PW_c	>= `PowerOn_idle_time) //11ms
                        begin
                            PW_SM			<= SM_V2P5;
                            PW_c				<= 24'h0;
                            power_fail	<= 1'b0;
                        end
                        else begin
                            PW_SM					<=SM_IDLE;
                            ucd9222_ena1_o	<= 1'b0;
                            ucd9222_ena2_o	<= 1'b0;
                            ucd9222_rst_no	<= 1'b1;
                            vcc1p5_en_o		<= 1'b0;
                            vcc1p8_en1_o		<= 1'b0;
                            vcc0p75_en_o		<= 1'b0;
                            vcc2p5_en_o		<= 1'b0;
                            vcc_5v_en_o		<= 1'b1;
                            refclk2_pd_no	<= 1'b0;
                            refclk3_pd_no	<= 1'b0;
                            PW_c						<= PW_c	+	1;
                            power_fail			<= 1'b0;
                            latch_pgs	<= 8'h0;

                            //fpga_ics557_oe_o		<= 1'b0;
                            //fpga_ics557_pd_no	<= 1'b0;
                            //vid_oe_no					<= 1'b1;
                        end
                    end

                    SM_V2P5: begin
                        vcc2p5_en_o <=	1'b1;//2.5v and 5v enable
                        vcc_5v_en_o <=	1'b1;
                        ucd9222_rst_no	<= 1'b1;

                        if(vcc5_pgood_d[1]	&	vcc2p5_pgood_d[1]) begin//10ms
                            if(PW_c >=	`PowerOn_v2p5_time) begin
                                PW_SM <=	SM_PMBS;
                                PW_c	 <=	24'h0;
                            end
                            else begin
                                PW_SM <=	SM_V2P5;
                                PW_c	 <=	PW_c + 1;
                            end
                            power_fail	<= 1'b0;
                        end
                        else begin
                            PW_SM <=	SM_V2P5;
                        end
                    end

                    SM_PMBS: begin
                        ucd9222_ena1_o	<= 1'b1;
                        //ucd9222_ena2_o	<= 1'b1;

                        if(PW_c >=	`PowerOn_pmbs_time) begin
                            PW_SM	<= SM_PMB2;
                            PW_c	<= 24'h0;
                        end
                        else begin
                            PW_SM	<= SM_PMBS;
                            PW_c	<= PW_c	+	1;
                        end            
                        power_fail	<= 1'b0;
                    end

                    SM_PMB2: begin
                        ucd9222_ena2_o	<= 1'b1;

                        if(ucd9222_pg1_d[1] & ucd9222_pg2_d[1]	&	pgucd9222_d[1]) begin
                            if(PW_c >=	`PowerOn_pmb2_time) begin
                                PW_SM <=	SM_V1P8;
                                PW_c	 <=	24'h0;
                                set_clk2_c	<= 24'h0;
                                set_clk3_c	<= 24'h0;
                            end
                            else begin
                                PW_SM <=	SM_PMB2;
                                PW_c	 <=	PW_c + 1;
                            end
                            power_fail	<= 1'b0;
                        end
                        else begin
                                PW_SM	<= SM_PMB2;
                            end
                        end

                    SM_V1P8: begin
                        vcc1p8_en1_o	<= 1'b1;
                        if(vcc1p8_pgood_d[1]) begin//10ms
                            refclk2_pd_no	<= 1'b1;

                            if(PW_c	>= `PowerOn_v1p8_time) begin
                                refclk3_pd_no <=	1'b1;
                                set_clk2	<= 1'b1;

                                if(set_clk2_c	>= `Set_Clk2_time) begin
                                    if(clock2_pll_lock_d[1]) begin
                                        set_clk3	<= 1'b1;

                                        if(set_clk3_c	>= `Set_Clk3_time) begin
                                            PW_SM	<= SM_V1P5;
                                            PW_c	<= 24'h0;
                                            set_clk2_c <=	24'h0;
                                            set_clk3_c <=	24'h0;
                                        end
                                        else begin
                                            PW_SM	<= SM_V1P8;
                                            set_clk3_c <=	set_clk3_c +1;
                                        end
                                    end
                                    else begin
                                        PW_SM <=	SM_V1P8;
                                    end
                                end
                                else begin
                                        PW_SM <=	SM_V1P8;
                                        set_clk2_c	<= set_clk2_c	+1;
                                    end
                                end
                            else begin
                                PW_SM	<= SM_V1P8;
                                PW_c	<= PW_c	+	1;
                            end

                            power_fail <=	1'b0;
                        end
                        else begin
                            PW_SM	<= SM_V1P8;
                        end
                        end

                    SM_V1P5: begin
                        vcc1p5_en_o <=	1'b1;
                        if(vcc1p5_pgood_d[1]) begin//10ms
                            if(PW_c	>= `PowerOn_v1p5_time) begin
                                //fpga_ics557_pd_no	<= 1'b1;
                                //vid_oe_no					<= 1'b0;
                                PW_SM <=	SM_V075;
                                PW_c	 <=	24'h0;
                            end
                            else begin
                                PW_SM <=	SM_V1P5;
                                PW_c	 <=	PW_c + 1;
                            end
                            power_fail <=	1'b0;
                        end
                        else begin
                            PW_SM	<= SM_V1P5;
                        end
                    end

                    SM_V075: begin
                        vcc0p75_en_o	<= 1'b1;
                        if(vcc0p75_pgood_d[1]) begin//10ms
                            if(PW_c	>= `PowerOn_v075_time) begin
                                PW_SM <=	SM_CLKG;
                                PW_c	 <=	24'h0;
                            end
                            else begin
                                PW_SM <=	SM_V075;
                                PW_c	 <=	PW_c + 1;
                            end
                            power_fail <=	1'b0;
                        end
                        else begin
                            PW_SM	<= SM_V075;
                        end
                    end

                    SM_CLKG: begin
                        if(clock2_pll_lock_d[1] & clock3_pll_lock_d[1]) begin//10ms
                            //fpga_ics557_oe_o	 <=	1'b1;
                            if(PW_c	>= `PowerOn_clkg_time) begin
                                PW_SM <=	SM_PORWR;
                                PW_c	 <=	24'h0;
                            end
                            else begin
                                PW_SM <=	SM_CLKG;
                                PW_c	 <=	PW_c + 1;
                            end
                            power_fail <=	1'b0;
                        end
                        else begin
                            PW_SM			 <=	SM_CLKG;
                        end
                    end

                    SM_PORWR: begin		 //	POR	deasserted
                        power_fail	<= 1'b0;
                        if(PW_c >=	`PowerOn_wait_time) begin
                            PW_SM <=	SM_WAIT;
                            PW_c	 <=	24'h0;
                        end
                        else begin
                            PW_SM <=	SM_PORWR;
                            PW_c	 <=	PW_c + 1;
                        end
                    end

                    SM_WAIT: begin
                        if(dsp_resetstat_nd[1]) begin//10ms
                            if(PW_c	>= `PowerOn_wait_time) begin
                                PW_SM <=	SM_PWOK;
                                PW_c	 <=	24'h0;
                            end
                            else begin
                                PW_SM <=	SM_WAIT;
                                PW_c	 <=	PW_c + 1;
                            end
                            power_fail <=	1'b0;
                        end
                        else begin
                            PW_SM	<= SM_WAIT;
                        end
                    end

                    SM_PWOK: begin
                        power_fail	<= 1'b0;
                        PW_c				<= 24'h0;

                        if(fail_power_c > `PowerOk_fail_time)
                            PW_SM <=	SM_POFF1;
                        else
                            PW_SM <=	SM_PWOK;
                    end

                    SM_POFF1: begin
                        power_fail <=	1'b1;
                        latch_pgs[3:0] <=	{vcc0p75_pgood_d[1], pgucd9222_d[1], ucd9222_pg2_d[1], ucd9222_pg1_d[1]};	 //20110128
                        latch_pgs[7:4] <=	{vcc5_pgood_d[1],	vcc2p5_pgood_d[1], vcc1p8_pgood_d[1],	vcc1p5_pgood_d[1]};	 //20110128
                        if(PW_c	>= `PowerOff_delay_time) begin
                            PW_SM <=	SM_POFF2;
                            PW_c	 <=	24'h0;
                        end
                        else begin
                            PW_SM <=	SM_POFF1;
                            PW_c	 <=	PW_c + 1;
                        end
                    end

                    SM_POFF2: begin
                        power_fail		 <=	1'b1;
                        PW_SM					 <=	SM_POFF2;
`ifdef Pre_Test

`else
                        ucd9222_ena1_o <=	1'b0;
                        ucd9222_ena2_o <=	1'b0;
                        ucd9222_rst_no <=	1'b1;
                        vcc1p5_en_o		 <=	1'b0;
                        vcc1p8_en1_o	 <=	1'b0;
                        vcc0p75_en_o	 <=	1'b0;
                        vcc2p5_en_o		 <=	1'b0;
                        //vcc_5v_en_o		 <=	1'b0;//troy20110411
                        refclk2_pd_no	 <=	1'b0;
                        refclk3_pd_no	 <=	1'b0;
`endif

                        PW_c					 <=	24'b0;
                    end

                    SM_FAIL: begin
                        PW_SM <=	SM_FAIL;
                    end

                    default: begin
                        PW_SM <=	SM_IDLE;
                    end

                endcase

            end
    end

endmodule

