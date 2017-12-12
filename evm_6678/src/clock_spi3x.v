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
////  File name	: clock_spi3x.v	                                  ////
////  Description                                                 ////
////      		: TMX320C6678 EVM FPGA Core MODULE                ////
////  Created Date                                                ////
////			: 2010/12/09									  ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) Advantech Co., Ltd.  All Rights Reserved       ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
module CDCE62005_3x( FPGA_48MHz     ,FPGA_rst       ,Vccpg          ,PLL_Lock3     ,start          
                   ,busy           ,ready          ,iClock_div      ,Write_data    ,Read_data
                   ,CLOCK3_SSPCS_o ,CLOCK3_SSPCK_o ,CLOCK3_SSPSI_o ,CLOCK3_SSPSO_i );
  input         FPGA_48MHz;
  input         FPGA_rst;
  input         Vccpg;
  output        PLL_Lock3;
  reg           PLL_Lock3;
  input         start;//0, trigger transaction
  output        busy;//1, busy
  output        ready;
  input  [7:0]  iClock_div;
  wire   [7:0]  Clock_div;
  reg    [7:0]  Clock_cnt   = 8'h00;
  input  [31:0] Write_data;
  output [31:0] Read_data;
  reg    [31:0] Read_data = 32'h0;
///////////////////////////
  output        CLOCK3_SSPCS_o;
  reg           CLOCK3_SSPCS_o = 1'b1;
  output        CLOCK3_SSPCK_o;
  reg           CLOCK3_SSPCK_o  = 1'b0;
  reg           dCLOCK3_SSPCK_o  = 1'b0;
  output        CLOCK3_SSPSI_o;
//  reg           CLOCK2_SSPSI_o;
  input         CLOCK3_SSPSO_i;
//////////////////////////
  reg           Vccpg_record;
  reg    [5:0]  Bit_cnt     = 6'h00;
  reg    [7:0]  M_rec_add   = 8'h00;
  reg    [7:0]  Pulse_width = 8'h00;
  reg           R_W_ind;//1, write
  reg    [1:0]  start_record;
  reg           ini_done;
  reg    [7:0]  ini_T7;
  reg    [3:0]  ini_cnt;
  wire   [31:0] Write_data_buff;

wire wait_count_done;
reg  count_en;
reg [16:0] clk_gen_wait_time;

parameter clk_gen_wait_2m_time=17'h186a0;
//parameter clk_gen_wait_2m_time=17'h0010;
parameter clk_gen_wait_100u_time=17'h1388;  
  
  assign Write_data_buff = ini_done ? ( (ini_cnt==4'h0) ? 32'he9400020 :    //100MHz MUX from PRI_IN //20110115  //e9840320   
                                       ((ini_cnt==4'h1) ? 32'he9800301 :    //312.5MHz  //e9800301                                 
                                       ((ini_cnt==4'h2) ? 32'he9800302 :    //312.5MHz  //e9800302                                
                                       ((ini_cnt==4'h3) ? 32'he9400003 :    //100MHz MUX from PRI_IN   //e9400003                 
                                       ((ini_cnt==4'h4) ? 32'h69860314 :                                                   
                                       ((ini_cnt==4'h5) ? 32'h107c0bf5 :  //32'h107c0bf5                                   
                                       ((ini_cnt==4'h6) ? 32'h84be19a6 :                                                                          
                                       ((ini_cnt==4'h7) ? 32'hfd0037f7 :                                                  
                                       ((ini_cnt==4'h8) ? 32'h80be19a6 : //write encal_mode to 1  write encal to 0 
                                       ((ini_cnt==4'h9) ? 32'h84be19a6 : //write encal to 0 followed by a 1  wait 100usec
                                       ((ini_cnt==4'ha) ? 32'h20008D98 : //write sync to 0
                                       ((ini_cnt==4'hb) ? 32'h20009D98 : //                 followed by a 1
                                       ((ini_cnt==4'hc) ? 32'h0000008e : 32'h0))))))))))))) : Write_data ;
                                                                      



                         
  assign CLOCK3_SSPSI_o = (!CLOCK3_SSPCS_o && R_W_ind) ? (Bit_cnt==6'h20 ? Write_data_buff[6'h1f] : Write_data_buff[Bit_cnt]) : 1'b0;
  assign busy = (~FPGA_rst || ~CLOCK3_SSPCS_o || M_rec_add[3:0]==4'he) ? 1'b1 : 1'b0;
  assign ready = 0;
  assign Clock_div[7:0] = (iClock_div[7:0]==8'h0)? 8'h1 : iClock_div[7:0];

  always@(posedge FPGA_48MHz or negedge FPGA_rst) begin//control CS
    if(~FPGA_rst) begin
        Vccpg_record   <= 1'b0;                   
        CLOCK3_SSPCS_o <= 1'b1;                   
        ini_T7         <= 8'h00;                
        ini_cnt        <= 4'h0;                   
        start_record   <= 2'h0;                   
        R_W_ind        <= 1'b1;                   
        M_rec_add      <= 8'h00;                  
        Pulse_width    <= 8'h00;                  
        ini_done       <= 1'b0;                   
        count_en       <= 1'b0;                   
        clk_gen_wait_time <=17'd0;                
                    
    end
    
    else begin
      Vccpg_record <=  Vccpg;
      start_record <= {start_record[0],start};
      if(!Vccpg_record && Vccpg) begin
          CLOCK3_SSPCS_o <= 1'b0;//trigger from power-on
          ini_done       <= 1'b1;
          ini_T7         <= 8'h00;
          ini_cnt        <= 8'h00;
      end
  
      else if(CLOCK3_SSPCS_o && ini_done) begin
        if (ini_T7!=Clock_div)                                         
          ini_T7 <= ini_T7 + 1'b1;                                     
                                                                        
        if(ini_T7==8'h0 && ini_cnt==4'hc && PLL_Lock3) ini_done <= 1'b0;
      
        else if(ini_T7==8'h0 && ini_cnt!=4'hc) ini_cnt <= ini_cnt + 1'b1;
       
        else if(ini_T7==Clock_div) begin
        
          if (ini_cnt==4'd7 & !wait_count_done) begin
            count_en<=1'b1;
            clk_gen_wait_time<=clk_gen_wait_2m_time;
          end
          else if (ini_cnt==4'd9 & !wait_count_done) begin
            count_en<=1'b1;
            clk_gen_wait_time<=clk_gen_wait_100u_time;
          end          
          else begin
            ini_T7         <= 8'h0;
            M_rec_add      <= Write_data_buff[7:0];
            CLOCK3_SSPCS_o <= 1'b0;//load default 
            count_en       <= 1'b0;            
          end
          
        end
      end
    
      if(!start_record[1] && start_record[0]) begin
         CLOCK3_SSPCS_o <= 1'b0;//trigger from DSP
         M_rec_add      <= Write_data_buff[7:0];
      end

      else if(Bit_cnt==6'h20  && Clock_cnt==Clock_div && CLOCK3_SSPCK_o) begin
        CLOCK3_SSPCS_o <= 1'b1;//T6 setup time
        R_W_ind        <= 1'b1;
      end
    
      if(M_rec_add[3:0]==4'he && CLOCK3_SSPCS_o && Pulse_width!= Clock_div) begin
         R_W_ind     <= 1'b0;//Read
         Pulse_width <= Pulse_width + 1'b1;   
      end
    
      else if(Pulse_width==Clock_div && !R_W_ind && CLOCK3_SSPCS_o) begin//trigger by read command
         CLOCK3_SSPCS_o <= 1'b0;
         Pulse_width    <= 8'h00;
         M_rec_add[3:0] <= 4'h0;
      end
    end 
  end

  always@(posedge FPGA_48MHz or negedge FPGA_rst) begin//control CLOCK1_SSPCK_o
    if(~FPGA_rst) begin
        Clock_cnt      <= 8'h00;
        CLOCK3_SSPCK_o <= 1'b0;
    end

    else begin//original not modify
      Clock_cnt <= Clock_cnt + 1'b1;
      if(Clock_cnt == Clock_div) begin
         CLOCK3_SSPCK_o  <= (~CLOCK3_SSPCS_o) & (~CLOCK3_SSPCK_o);
         Clock_cnt       <= 8'h0;
      end

      if(!start_record[1] && start_record[0]) begin
          CLOCK3_SSPCK_o <= 1'b0;
        Clock_cnt      <= Clock_div - 1'b1;//T1 setup time
      end
    end
  end

  always@(posedge FPGA_48MHz or negedge FPGA_rst) begin//control CLOCK1_SSPCK_o
    if(~FPGA_rst) begin
		dCLOCK3_SSPCK_o <= 1'b0;	
    end
    else begin
		dCLOCK3_SSPCK_o <= CLOCK3_SSPCK_o;

    end
  end
  
  always@(posedge FPGA_48MHz or posedge CLOCK3_SSPCS_o) begin
      if(CLOCK3_SSPCS_o) Bit_cnt <= 6'h00;
      else if(~dCLOCK3_SSPCK_o && CLOCK3_SSPCK_o) Bit_cnt <= Bit_cnt + 1;
  end

  always@(posedge CLOCK3_SSPCK_o or negedge FPGA_rst) begin//read command
    if(~FPGA_rst) begin
        Read_data <= 32'h0;
        PLL_Lock3  <= 1'b0;
    end

    else if(!R_W_ind) begin
      Read_data[Bit_cnt] <= CLOCK3_SSPSO_i;
      if(M_rec_add[7:4]==4'h8 && Bit_cnt==6'h0c) PLL_Lock3 <= CLOCK3_SSPSO_i;
    end
  end
  


reg		[16:0]	clk_c;  //counter for clock divider
reg           wait_count_done_cng;

always@(posedge FPGA_48MHz or negedge FPGA_rst)		
begin
  if (~FPGA_rst) begin 
    clk_c <= 17'h0;
    wait_count_done_cng <=1'b0;
  end	
  else if (clk_c >=clk_gen_wait_time & count_en) begin
    clk_c <= 17'h0;
    wait_count_done_cng <=~wait_count_done_cng;	  
  end	
  else if (count_en==1'b1 & clk_c <clk_gen_wait_time) begin 
    clk_c <= clk_c + 1;
  end	
end  
  
reg  wait_count_done_cng_1t; 
always@(posedge FPGA_48MHz or negedge FPGA_rst)		
begin
  if (~FPGA_rst)
    wait_count_done_cng_1t <= 1'b0;
  else
    wait_count_done_cng_1t <= wait_count_done_cng;	  
end    
  
assign wait_count_done =  wait_count_done_cng_1t ^ wait_count_done_cng; 
endmodule