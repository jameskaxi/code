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
////  File name	: fpga_internal_reset.v	                          ////
////  Description                                                 ////
////      		: TMX320C6678 EVM FPGA Core MODULE                ////
////  Created Date                                                ////
////			: 2010/12/10									  ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) Advantech Co., Ltd.  All Rights Reserved       ////
////                                                              ////
//////////////////////////////////////////////////////////////////////

	`include "params.v"


module fpga_internal_reset (
	clk,				// Clock input for the timing delay counter
	rst_n,				// Reset input to this block
    fpga_reset_n		// Output to FPGA internal Reset
);

input	clk;
input	rst_n;
output	fpga_reset_n;

reg		fpga_reset_n;


reg		[15:0]	fpga_reset_counter;  

reg		[1:0]	reset_st;		//Reset Mechanism State-Machine 
parameter idle = 4'h0; 
parameter reset_assert = 4'h1; 
parameter reset_release = 4'h2;


always@(posedge clk or negedge rst_n)		
begin
	if (~rst_n) begin
		reset_st <= idle;
		fpga_reset_n <= 1'b0;		 
		fpga_reset_counter <= 16'b0;
	end
	else begin  
	    case (reset_st)
		idle: begin		
		    fpga_reset_n <= 1'b0;		 
			fpga_reset_counter <= 16'b0;
		    if (rst_n) 
				reset_st <= reset_assert;
			else
				reset_st <= idle;
		end

		reset_assert: begin	
		    fpga_reset_n <= 1'b0;	
			if (fpga_reset_counter >= `FPGA_ireset_delay_time) begin
				reset_st <= reset_release;
				fpga_reset_counter <= 16'b0;
			end	
			else begin
				reset_st <= reset_assert;
				fpga_reset_counter <= fpga_reset_counter + 1;
			end	
		end
			
		reset_release: begin	
		    fpga_reset_n <= 1'b1;
			fpga_reset_counter <= 16'b0;
			reset_st <= reset_release;
		end
			
		default : begin
            reset_st <= idle;
			fpga_reset_n <= 1'b1;
			fpga_reset_counter <= 16'b0;
	    end                       
		endcase 	
	end
end	  
			
endmodule 

