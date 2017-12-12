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
////  File name	: reset_req.v			                          ////
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


module reset_req_s (
           clk,				// Clock input for the timing delay counter
           rst_n,				// Reset input to this block
           por_req_n,  		// POR Request
           resetfull_req_n,	// Resetfull Request
           hreset_req_n,	// Hard Reset Request
           sreset_req_n,	// Soft Reset Request
           cs_por_n,			// Current Status of PORZ signal
           cs_resetfull_n,     // Current Status of ResetfullZ signal
           cs_reset_n,			// Current Status of ResetZ signal
           cs_resetstat_n,		// Current Status of ResetstatZ signal
           por_out_n,			// Output to PORz
           resetfull_out_n,	// Output to ResetfullZ
           reset_out_n			// Output to ResetZ
       );

input	clk;
input	rst_n;
input	por_req_n;
input	resetfull_req_n;
input	hreset_req_n;
input  	sreset_req_n;
input 	cs_por_n;
input	cs_resetfull_n;
input 	cs_reset_n;
input 	cs_resetstat_n;
output	por_out_n;
output	resetfull_out_n;
output	reset_out_n;

reg		por_out_n;
reg		resetfull_out_n;
reg		reset_out_n;


reg		[23:0]	delay_timing_counter;  		//counter for clock divider

reg		[3:0]	reset_st;		//Reset Mechanism State-Machine
parameter idle = 4'h0;
parameter por_assert = 4'h1;
parameter por_delay = 4'h2;
parameter por_release = 4'h3;
parameter resetfull_check = 4'h4;
parameter resetfull_assert = 4'h5;
parameter resetfull_delay = 4'h6;
parameter resetfull_release = 4'h7;
parameter hreset_check = 4'h8;
parameter hreset_assert = 4'h9;
parameter hreset_delay = 4'ha;
parameter hreset_release = 4'hb;
parameter sreset_check = 4'hc;
parameter sreset_assert = 4'hd;
parameter sreset_delay = 4'he;
parameter sreset_release = 4'hf;

always@(posedge clk or negedge rst_n) begin
    if (~rst_n) begin
        reset_st <= idle;
        por_out_n <= 1'b1;
        resetfull_out_n <= 1'b1;
        reset_out_n <= 1'b1;
        delay_timing_counter <= 24'b0;
    end
    else begin
        case (reset_st)

            idle: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b1;
                delay_timing_counter <= 24'b0;

                if (~por_req_n)
                    reset_st <= por_assert;
                else if (~resetfull_req_n)
                    reset_st <= resetfull_check;
                else if (~hreset_req_n)
                    reset_st <= hreset_check;
                else if (~sreset_req_n)
                    reset_st <= sreset_check;
                else
                    reset_st <= idle;
            end

            ////////////////////  POR   ///////////////////////////////
            por_assert: begin
                por_out_n <= 1'b0;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b1;
                delay_timing_counter <= 24'b0;

                if (por_req_n)
                    reset_st <= por_delay;
                else
                    reset_st <= por_assert;
            end

            por_delay: begin
                por_out_n <= 1'b0;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b1;

                if (~cs_por_n & cs_resetfull_n & cs_reset_n & ~cs_resetstat_n)
                    if(delay_timing_counter >= `POR_assert_delay_time) begin
                        reset_st <= por_release;
                        delay_timing_counter <= 24'b0;
                    end
                    else begin
                        reset_st <= por_delay;
                        delay_timing_counter <= delay_timing_counter + 1;
                    end
                else begin
                    reset_st <= por_delay;
                    delay_timing_counter <= 24'b0;
                end
            end

            por_release: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b1;

                if (cs_por_n & cs_resetfull_n & cs_reset_n & cs_resetstat_n)
                    if(delay_timing_counter >= `POR_release_delay_time) begin
                        reset_st <= idle;
                        delay_timing_counter <= 24'b0;
                    end
                    else begin
                        reset_st <= por_release;
                        delay_timing_counter <= delay_timing_counter + 1;
                    end
                else begin
                    reset_st <= por_release;
                    delay_timing_counter <= 24'b0;
                end
            end

            ////////////////////  Full-Reset   ///////////////////////////////
            resetfull_check: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b1;

                if (~por_req_n)
                    reset_st <= por_assert;
                else if (cs_por_n & cs_resetfull_n & cs_reset_n)
                    if(delay_timing_counter >= `Resetfull_check_delay_time) begin
                        reset_st <= resetfull_assert;
                        delay_timing_counter <= 24'b0;
                    end
                    else begin
                        reset_st <= resetfull_check;
                        delay_timing_counter <= delay_timing_counter + 1;
                    end
                else begin
                    reset_st <= resetfull_check;
                    delay_timing_counter <= 24'h0;
                end
            end


            resetfull_assert: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b0;
                reset_out_n <= 1'b1;
                delay_timing_counter <= 24'b0;

                if (resetfull_req_n)
                    reset_st <= resetfull_delay;
                else
                    reset_st <= resetfull_assert;
            end

            resetfull_delay: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b0;
                reset_out_n <= 1'b1;

                if (cs_por_n & ~cs_resetfull_n & cs_reset_n & ~cs_resetstat_n)
                    if(delay_timing_counter >= `Resetfull_assert_delay_time) begin
                        reset_st <= resetfull_release;
                        delay_timing_counter <= 24'b0;
                    end
                    else begin
                        reset_st <= resetfull_delay;
                        delay_timing_counter <= delay_timing_counter + 1;
                    end
                else begin
                    reset_st <= resetfull_delay;
                    delay_timing_counter <= 24'b0;
                end
            end

            resetfull_release: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b1;

                if (cs_por_n & cs_resetfull_n & cs_reset_n & cs_resetstat_n)
                    if(delay_timing_counter >= `Resetfull_release_delay_time) begin
                        reset_st <= idle;
                        delay_timing_counter <= 24'b0;
                    end
                    else begin
                        reset_st <= resetfull_release;
                        delay_timing_counter <= delay_timing_counter + 1;
                    end
                else begin
                    reset_st <= resetfull_release;
                    delay_timing_counter <= 24'b0;
                end
            end

            ////////////////////  Hard-Reset   ///////////////////////////////
            hreset_check: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b1;

                if (~por_req_n)
                    reset_st <= por_assert;
                else if (~resetfull_req_n)
                    reset_st <= resetfull_check;
                else if (cs_por_n & cs_resetfull_n & cs_reset_n)
                    if(delay_timing_counter >= `Hardreset_check_delay_time) begin
                        reset_st <= hreset_assert;
                        delay_timing_counter <= 24'b0;
                    end
                    else begin
                        reset_st <= hreset_check;
                        delay_timing_counter <= delay_timing_counter + 1;
                    end
                else begin
                    reset_st <= hreset_check;
                    delay_timing_counter <= 24'h0;
                end
            end


            hreset_assert: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b0;
                delay_timing_counter <= 24'b0;

                if (hreset_req_n)
                    reset_st <= hreset_delay;
                else
                    reset_st <= hreset_assert;
            end

            hreset_delay: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b0;

                if (cs_por_n & cs_resetfull_n & ~cs_reset_n & ~cs_resetstat_n)
                    if(delay_timing_counter >= `Hardreset_assert_delay_time) begin
                        reset_st <= hreset_release;
                        delay_timing_counter <= 24'b0;
                    end
                    else begin
                        reset_st <= hreset_delay;
                        delay_timing_counter <= delay_timing_counter + 1;
                    end
                else begin
                    reset_st <= hreset_delay;
                    delay_timing_counter <= 24'b0;
                end
            end

            hreset_release: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b1;

                if (cs_por_n & cs_resetfull_n & cs_reset_n & cs_resetstat_n)
                    if(delay_timing_counter >= `Hardreset_release_delay_time) begin
                        reset_st <= idle;
                        delay_timing_counter <= 24'b0;
                    end
                    else begin
                        reset_st <= hreset_release;
                        delay_timing_counter <= delay_timing_counter + 1;
                    end
                else begin
                    reset_st <= hreset_release;
                    delay_timing_counter <= 24'b0;
                end
            end

            ////////////////////  Soft-Reset   ///////////////////////////////
            sreset_check: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b1;

                if (~por_req_n)
                    reset_st <= por_assert;
                else if (~resetfull_req_n)
                    reset_st <= resetfull_check;
                else if (~hreset_req_n)
                    reset_st <= hreset_check;
                else if (cs_por_n & cs_resetfull_n & cs_reset_n)
                    if(delay_timing_counter >= `Softreset_check_delay_time) begin
                        reset_st <= sreset_assert;
                        delay_timing_counter <= 24'b0;
                    end
                    else begin
                        reset_st <= sreset_check;
                        delay_timing_counter <= delay_timing_counter + 1;
                    end
                else begin
                    reset_st <= sreset_check;
                    delay_timing_counter <= 24'h0;
                end
            end


            sreset_assert: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b0;
                delay_timing_counter <= 24'b0;

                if (sreset_req_n)
                    reset_st <= sreset_delay;
                else
                    reset_st <= sreset_assert;
            end

            sreset_delay: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b0;

                if (cs_por_n & cs_resetfull_n & ~cs_reset_n & ~cs_resetstat_n)
                    if(delay_timing_counter >= `Softreset_assert_delay_time) begin
                        reset_st <= sreset_release;
                        delay_timing_counter <= 24'b0;
                    end
                    else begin
                        reset_st <= sreset_delay;
                        delay_timing_counter <= delay_timing_counter + 1;
                    end
                else begin
                    reset_st <= sreset_delay;
                    delay_timing_counter <= 24'b0;
                end
            end

            sreset_release: begin
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b1;

                if (cs_por_n & cs_resetfull_n & cs_reset_n & cs_resetstat_n)
                    if(delay_timing_counter >= `Softreset_release_delay_time) begin
                        reset_st <= idle;
                        delay_timing_counter <= 24'b0;
                    end
                    else begin
                        reset_st <= sreset_release;
                        delay_timing_counter <= delay_timing_counter + 1;
                    end
                else begin
                    reset_st <= sreset_release;
                    delay_timing_counter <= 24'b0;
                end
            end

            default : begin
                reset_st <= idle;
                por_out_n <= 1'b1;
                resetfull_out_n <= 1'b1;
                reset_out_n <= 1'b1;
                delay_timing_counter <= 24'b0;
            end

        endcase
    end
end

endmodule

