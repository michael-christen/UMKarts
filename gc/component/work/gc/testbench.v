//////////////////////////////////////////////////////////////////////
// Created by Actel SmartDesign Wed Nov 06 23:11:11 2013
// Testbench Template
// This is a basic testbench that instantiates your design with basic 
// clock and reset pins connected.  If your design has special
// clock/reset or testbench driver requirements then you should 
// copy this file and modify it. 
//////////////////////////////////////////////////////////////////////

`timescale 1ns/100ps

module testbench;

parameter SYSCLK_PERIOD = 100; // 10MHz

reg SYSCLK;
reg NSYSRESET;

initial
begin
    SYSCLK = 1'b0;
    NSYSRESET = 1'b0;
end

//////////////////////////////////////////////////////////////////////
// Reset Pulse
//////////////////////////////////////////////////////////////////////
initial
begin
    #(SYSCLK_PERIOD * 10 )
        NSYSRESET = 1'b1;
end


//////////////////////////////////////////////////////////////////////
// 10MHz Clock Driver
//////////////////////////////////////////////////////////////////////
always @(SYSCLK)
    #(SYSCLK_PERIOD / 2.0) SYSCLK <= !SYSCLK;


//////////////////////////////////////////////////////////////////////
// Instantiate Unit Under Test:  gc
//////////////////////////////////////////////////////////////////////
gc gc_0 (
    // Inputs
    .MSS_RESET_N(NSYSRESET),
    .UART_0_RXD({1{1'b0}}),
    .CAPTURE_SWITCH(NSYSRESET),

    // Outputs
    .UART_0_TXD( ),
    .start_count( ),
    .data1( ),
    .data2( ),
    .PWM1( ),
    .LMOTOR( ),
    .RMOTOR( ),
    .LSERVO( ),
    .RSERVO( ),
    .data_0( ),
    .send_0( ),

    // Inouts
    .data( )

);

endmodule

