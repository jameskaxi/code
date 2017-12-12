//
//`include "../clock_spi2x.v"
//
//module spi_tb;
//
//reg FPGA_48MHz;   
//reg FPGA_rst;     
//reg Vccpg;        
//wire PLL_Lock;     
//reg start;        
//wire busy;         
//wire ready;        
//reg [7:0] iClock_div;   
//reg [31:0] Write_data;   
//wire [31:0] Read_data;    
//wire CLOCK2_SSPCS_o;
//wire CLOCK2_SSPCK_o;
//wire CLOCK2_SSPSI_o;
//wire CLOCK2_SSPSO_i;
//
//	reg [31:0] cnt;
//
//CDCE62005_2x    uut (FPGA_48MHz,
//                  FPGA_rst,
//                  Vccpg,
//                  PLL_Lock,
//                  start,
//                  busy,
//                  ready,
//                  iClock_div,
//                  Write_data,
//                  Read_data,
//                  CLOCK2_SSPCS_o,
//                  CLOCK2_SSPCK_o,
//                  CLOCK2_SSPSI_o,
//                  CLOCK2_SSPSO_i );
//                  
//    initial begin
//    	  FPGA_48MHz = 1'b0;
//    	  FPGA_rst   = 1'b0;
//    		Vccpg = 1'b0;
//    		start = 1'b0;
//    		iClock_div = 8'd2;
//    		Write_data = 32'h12345678;
////    		CLOCK2_SSPCS_o = 1'b1;
////    		CLOCK2_SSPCK_o = 1'b0;
////    		CLOCK2_SSPSI_o = 1'b0;
//    		cnt = 32'd0;
//    		//#100
//    		//FPGA_rst   = 1'b1;
//  	end
//  	
//  	always #100 FPGA_48MHz = ~FPGA_48MHz;
//  	
//  always @(posedge FPGA_48MHz)
//  if(cnt == 100)
//    FPGA_rst <= 1'b1;
//  
//  	always @(posedge FPGA_48MHz)
//  	begin
//  		  cnt = cnt + 1'b1;
//  		  if(cnt >= 32'd20000)
//  		  		Vccpg = 1'b1;
//  		  else if(cnt >= 32'd40000) begin
//  		  	cnt = 0;
//            
//          end
//  	end
//  	
//endmodule
//
