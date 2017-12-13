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
////  File name	: dsp_spi.v		                                  ////
////  Description                                                 ////
////      		: TMX320C6678 EVM FPGA Core MODULE                ////
////  Created Date                                                ////
////			: 2010/12/09									  ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) Advantech Co., Ltd.  All Rights Reserved       ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
module dsp_spi (
	cs,rw,addr,data_w,data_r,rdy,
	DSP_SSEL1,DSP_SCK1,DSP_MISO1,DSP_MOSI1
);

input DSP_SSEL1;
input DSP_SCK1;
input DSP_MOSI1;
output DSP_MISO1;

reg [4:0] spi_c;

/*******************************/  
/**** Other		   *****/
/*******************************/

output		cs;
output		rw;
output	[7:0]	addr;
output	[7:0]	data_w;
input	[7:0]	data_r;
output		rdy;

wire		cs;

assign cs = DSP_SSEL1;

reg		rw;
reg	[7:0]	addr;
reg	[7:0]	data_w = 8'h0;
reg		DSP_MISO1;
reg		rdy;

always@(negedge DSP_SCK1 or posedge DSP_SSEL1)		
begin
	if (DSP_SSEL1) begin
		spi_c <= 5'b0;
		rw <= 1'b0;
		addr <= 8'h0;
	//	data_w <= 8'h0;
		rdy <= 1'b0;
	end
	else begin		
		case (spi_c)
		
			5'h0: begin
				spi_c <= spi_c + 1;
				rw <= DSP_MOSI1;	
				addr <= 8'h0;
				data_w <= 8'h0;	
				rdy <= 1'b0;	
			end

			5'h1: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr[6] <= DSP_MOSI1;
				data_w <= 8'h0;		
				rdy <= 1'b0;	
			end

			5'h2: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr[5] <= DSP_MOSI1;
				data_w <= 8'h0;		
				rdy <= 1'b0;	
			end

			5'h3: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr[4] <= DSP_MOSI1;
				data_w <= 8'h0;	
				rdy <= 1'b0;		
			end

			5'h4: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr[3] <= DSP_MOSI1;
				data_w <= 8'h0;	
				rdy <= 1'b0;		
			end

			5'h5: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr[2] <= DSP_MOSI1;
				data_w <= 8'h0;	
				rdy <= 1'b0;		
			end

			5'h6: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr[1] <= DSP_MOSI1;
				data_w <= 8'h0;	
				rdy <= 1'b0;		
			end	

			5'h7: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr[0] <= DSP_MOSI1;
				data_w <= 8'h0;	
				if (~rw) rdy <= 1'b0;
				else rdy <= 1'b1;					
			end	

			5'h8: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr <= addr;
				if (~rw) data_w[7] <= DSP_MOSI1;				
				rdy <= 1'b0;
			end

			5'h9: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr <= addr;
				if (~rw) data_w[6] <= DSP_MOSI1;				
				rdy <= 1'b0;
			end

			5'ha: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr <= addr;
				if (~rw) data_w[5] <= DSP_MOSI1;				
				rdy <= 1'b0;
			end

			5'hb: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr <= addr;
				if (~rw) data_w[4] <= DSP_MOSI1;				
				rdy <= 1'b0;
			end
			
			5'hc: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr <= addr;
				if (~rw) data_w[3] <= DSP_MOSI1;				
				rdy <= 1'b0;
			end		

			5'hd: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr <= addr;
				if (~rw) data_w[2] <= DSP_MOSI1;				
				rdy <= 1'b0;
			end	

			5'he: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr <= addr;
				if (~rw) data_w[1] <= DSP_MOSI1;				
				rdy <= 1'b0;
			end	

			5'hf: begin
				spi_c <= spi_c + 1;
				rw <= rw;
				addr <= addr;
				if (~rw) begin
					data_w[0] <= DSP_MOSI1;				
					rdy <= 1'b1;
				end
				else begin
					rdy <= 1'b0;
				end			
			end	

			5'h10: begin
				spi_c <= 5'b0;
				rw <= 1'b0;
				addr <= 8'h0;
				data_w <= 8'h0;
				rdy <= 1'b0;		
			end														
		endcase		
	end
end

always@(posedge DSP_SCK1 or posedge DSP_SSEL1)		
begin
	if (DSP_SSEL1) DSP_MISO1 <= 1'b0;
	else begin		
		case (spi_c)		
			5'h0: DSP_MISO1 <= 1'b0;	
			5'h1: DSP_MISO1 <= 1'b0;
			5'h2: DSP_MISO1 <= 1'b0;
			5'h3: DSP_MISO1 <= 1'b0;
			5'h4: DSP_MISO1 <= 1'b0;
			5'h5: DSP_MISO1 <= 1'b0;
			5'h6: DSP_MISO1 <= 1'b0;
			5'h7: DSP_MISO1 <= 1'b0;
			5'h8: if (rw) DSP_MISO1 <= data_r[7];
			5'h9: if (rw) DSP_MISO1 <= data_r[6];
			5'ha: if (rw) DSP_MISO1 <= data_r[5];
			5'hb: if (rw) DSP_MISO1 <= data_r[4];
			5'hc: if (rw) DSP_MISO1 <= data_r[3];
			5'hd: if (rw) DSP_MISO1 <= data_r[2];
			5'he: if (rw) DSP_MISO1 <= data_r[1];
			5'hf: if (rw) DSP_MISO1 <= data_r[0];
			5'h10: begin
				if ((data_w==8'h00) && (~rw)) DSP_MISO1 <= 1'b1;
				else DSP_MISO1 <= 1'b0;																
			end
		endcase		
	end
end

endmodule 

