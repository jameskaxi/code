///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017 Xilinx, Inc.
// All Rights Reserved
///////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor     : Xilinx
// \   \   \/     Version    : 14.7
//  \   \         Application: Xilinx CORE Generator
//  /   /         Filename   : vio.veo
// /___/   /\     Timestamp  : Fri Nov 17 14:41:41 中国标准时间 2017
// \   \  /  \
//  \___\/\___\
//
// Design Name: ISE Instantiation template
///////////////////////////////////////////////////////////////////////////////

// The following must be inserted into your Verilog file for this
// core to be instantiated. Change the instance name and port connections
// (in parentheses) to your own signal names.

//----------- Begin Cut here for INSTANTIATION Template ---// INST_TAG
vio YourInstanceName (
    .CONTROL(CONTROL), // INOUT BUS [35:0]
    .CLK(CLK), // IN
    .SYNC_IN(SYNC_IN), // IN BUS [7:0]
    .SYNC_OUT(SYNC_OUT) // OUT BUS [7:0]
);

// INST_TAG_END ------ End INSTANTIATION Template ---------

