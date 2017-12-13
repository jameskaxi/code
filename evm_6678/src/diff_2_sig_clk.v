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
////  File name	: diff_2_sig_clk.v		                          ////
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

	
module diff_2_sig_clk( FPGA_48MHz        ,FPGA_rst 
                      ,S_TDM_CLKA        ,S_TDM_CLKB        ,S_TDM_CLKC        ,S_TDM_CLKD
                      ,DSP_TSIP0_CLKA0_o ,DSP_TSIP0_CLKB0_o ,DSP_TSIP1_CLKA1_o ,DSP_TSIP1_CLKB1_o
                      ,DSP_TSIP0_FSA0_o  ,DSP_TSIP0_FSB0_o  ,DSP_TSIP1_FSA1_o  ,DSP_TSIP1_FSB1_o
                      );
 input  FPGA_48MHz;
 input  FPGA_rst;

 input  S_TDM_CLKA;
 input  S_TDM_CLKB;
 input  S_TDM_CLKC;
 input  S_TDM_CLKD;

 output DSP_TSIP0_CLKA0_o;
 output DSP_TSIP0_CLKB0_o;
 output DSP_TSIP1_CLKA1_o;
 output DSP_TSIP1_CLKB1_o;
 
 output DSP_TSIP0_FSA0_o;
 output DSP_TSIP0_FSB0_o;
 output DSP_TSIP1_FSA1_o;
 output DSP_TSIP1_FSB1_o;
 
 reg 	DSP_TSIP0_CLKA0_o;
 reg 	DSP_TSIP0_CLKB0_o;
 reg 	DSP_TSIP1_CLKA1_o;
 reg 	DSP_TSIP1_CLKB1_o;
 
 reg 	DSP_TSIP0_FSA0_o;
 reg 	DSP_TSIP0_FSB0_o;
 reg 	DSP_TSIP1_FSA1_o;
 reg 	DSP_TSIP1_FSB1_o;
 
 reg [2:0] A_filter;
 reg [5:0] A_record;
 reg       A_fail;
 reg [2:0] B_filter;
 reg [5:0] B_record;
 reg       B_fail;
 reg [2:0] C_filter;
 reg [5:0] C_record;
 reg       C_fail;
 reg [2:0] D_filter;
 reg [5:0] D_record;
 reg       D_fail;
 
 reg clk_ab_sel;
 reg sync_a_nclk_ab_sel;
 reg sync_b_nclk_ab_sel;
 
 reg mux_clk_ab_out;
 
 reg clk_cd_sel;
 reg sync_c_nclk_cd_sel;
 reg sync_d_nclk_cd_sel;
 
 reg mux_clk_cd_out;
 
 `ifdef TDM_FAILOVER
 always@(mux_clk_ab_out or mux_clk_cd_out) begin
	DSP_TSIP0_CLKA0_o <= mux_clk_ab_out;
	DSP_TSIP0_CLKB0_o <= mux_clk_ab_out;
	DSP_TSIP1_CLKA1_o <= mux_clk_ab_out;
	DSP_TSIP1_CLKB1_o <= mux_clk_ab_out;
 
	DSP_TSIP0_FSA0_o  <= mux_clk_cd_out;
	DSP_TSIP0_FSB0_o  <= mux_clk_cd_out;
	DSP_TSIP1_FSA1_o  <= mux_clk_cd_out;
	DSP_TSIP1_FSB1_o  <= mux_clk_cd_out;
end	
`else
always@(S_TDM_CLKA or S_TDM_CLKC) begin
	DSP_TSIP0_CLKA0_o <= S_TDM_CLKA;
	DSP_TSIP0_CLKB0_o <= S_TDM_CLKA;
	DSP_TSIP1_CLKA1_o <= S_TDM_CLKA;
	DSP_TSIP1_CLKB1_o <= S_TDM_CLKA;
 
	DSP_TSIP0_FSA0_o  <= S_TDM_CLKC;
	DSP_TSIP0_FSB0_o  <= S_TDM_CLKC;
	DSP_TSIP1_FSA1_o  <= S_TDM_CLKC;
	DSP_TSIP1_FSB1_o  <= S_TDM_CLKC;
end	
`endif
 
 always@(posedge FPGA_48MHz or negedge FPGA_rst) begin
   if(~FPGA_rst) begin
       A_filter  <= 3'h0;
       A_record  <= 6'h15;
       A_fail    <= 1'b0;//1, fail
       B_filter  <= 3'h0;
       B_record  <= 6'h15;
       B_fail    <= 1'b0;
   end
   
   else begin
       A_filter <= {A_filter[1],A_filter[0],S_TDM_CLKA};
       B_filter <= {B_filter[1],B_filter[0],S_TDM_CLKB};
       A_record <= {A_record[4:0],A_filter[2]};
       B_record <= {B_record[4:0],B_filter[2]};
       if(A_record[3:0]==4'h0 || A_record[3:0]==4'hF) A_fail <= 1'b1;//Detect if 16.384Mh exist
       else                                 A_fail <= 1'b0;
       if(B_record[3:0]==4'h0 || B_record[3:0]==4'hF) B_fail <= 1'b1;
       else                                 B_fail <= 1'b0;
   end
 end
 
 ///////////////////////////////////// Glitch-Free Control for TDM_CLK_A & B  ///////////////////////
 always@(posedge FPGA_48MHz or negedge FPGA_rst) begin
   if(~FPGA_rst) 
       clk_ab_sel  <= 1'b0; 
   else if (A_fail) 
       clk_ab_sel  <= 1'b1;
   else if (B_fail)
       clk_ab_sel  <= 1'b0;
   else
       clk_ab_sel  <= 1'b0;
 end
 
 always@(negedge S_TDM_CLKA or negedge FPGA_rst) begin
   if(~FPGA_rst) 
       sync_a_nclk_ab_sel  <= 1'b0; 
   else 
       sync_a_nclk_ab_sel  <= clk_ab_sel;
 end

 always@(negedge S_TDM_CLKB or negedge FPGA_rst) begin
   if(~FPGA_rst) 
       sync_b_nclk_ab_sel  <= 1'b0; 
   else 
       sync_b_nclk_ab_sel  <= clk_ab_sel;
 end
//////////////////////////////////////////// Clock A & B Mux //////////////////////////////

 always@(FPGA_rst or A_fail or B_fail or sync_a_nclk_ab_sel or sync_b_nclk_ab_sel or 
           S_TDM_CLKA or S_TDM_CLKB) begin
   if(FPGA_rst==1'b0)
       mux_clk_ab_out <= S_TDM_CLKA ;
   else if(A_fail==1'b1 && B_fail==1'b1)
       mux_clk_ab_out <= 1'b0 ;
   else if(A_fail==1'b1 && sync_b_nclk_ab_sel==1'b1)
       mux_clk_ab_out <= S_TDM_CLKB ;
   else if(A_fail==1'b0 && sync_a_nclk_ab_sel==1'b0)
       mux_clk_ab_out <= S_TDM_CLKA ;
end	   
 
///////////////////////////////////////////////////////////////////////////////////////////
 always@(posedge FPGA_48MHz or negedge FPGA_rst) begin
   if(~FPGA_rst) begin
       C_filter  <= 3'h0;
       C_record  <= 6'h15;
       C_fail    <= 1'b0;//1, fail
       D_filter  <= 3'h0;
       D_record  <= 6'h15;
       D_fail    <= 1'b0;
   end
   
   else begin
       C_filter <= {C_filter[1],C_filter[0],S_TDM_CLKC};
       D_filter <= {D_filter[1],D_filter[0],S_TDM_CLKD};
       C_record <= {C_record[4:0],C_filter[2]};
       D_record <= {D_record[4:0],D_filter[2]};
       if(C_record[3:0]==4'h0 || C_record[3:0]==4'hF) C_fail <= 1'b1;//Detect if 16.384Mh exist
       else                                 C_fail <= 1'b0;
       if(D_record[3:0]==4'h0 || D_record[3:0]==4'hF) D_fail <= 1'b1;
       else                                 D_fail <= 1'b0;
   end
 end
 
 ///////////////////////////////////// Glitch-Free Control for TDM_CLK_C & D  ///////////////////////
 always@(posedge FPGA_48MHz or negedge FPGA_rst) begin
   if(~FPGA_rst) 
       clk_cd_sel  <= 1'b0; 
   else if (C_fail) 
       clk_cd_sel  <= 1'b1;
   else if (D_fail)
       clk_cd_sel  <= 1'b0;
   else
       clk_cd_sel  <= 1'b0;
 end
 
 always@(negedge S_TDM_CLKC or negedge FPGA_rst) begin
   if(~FPGA_rst) 
       sync_c_nclk_cd_sel  <= 1'b0; 
   else 
       sync_c_nclk_cd_sel  <= clk_cd_sel;
 end

 always@(negedge S_TDM_CLKD or negedge FPGA_rst) begin
   if(~FPGA_rst) 
       sync_d_nclk_cd_sel  <= 1'b0; 
   else 
       sync_d_nclk_cd_sel  <= clk_cd_sel;
 end
//////////////////////////////////////////// Clock A & B Mux //////////////////////////////

 always@(FPGA_rst or C_fail or D_fail or sync_c_nclk_cd_sel or sync_d_nclk_cd_sel or 
           S_TDM_CLKC or S_TDM_CLKD) begin
   if(FPGA_rst==1'b0)
       mux_clk_cd_out <= S_TDM_CLKC ;
   else if(C_fail==1'b1 && D_fail==1'b1)
       mux_clk_cd_out <= 1'b0 ;
   else if(C_fail==1'b1 && sync_d_nclk_cd_sel==1'b1)
       mux_clk_cd_out <= S_TDM_CLKD ;
   else if(C_fail==1'b0 && sync_c_nclk_cd_sel==1'b0)
       mux_clk_cd_out <= S_TDM_CLKC ;
end	   

endmodule