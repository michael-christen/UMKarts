//////////////////////////////////////////////////////////////////////
// Created by Actel SmartDesign Mon Dec 09 22:28:11 2013
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
    .UART_1_RXD({1{1'b0}}),
    .SPI_0_DI({1{1'b0}}),
    .VAREF0({1{1'b0}}),
    .LED_RECV_IN({1{1'b0}}),
    .GPIO_4_IN({1{1'b0}}),
    .GPIO_2_IN({1{1'b0}}),
    .UART_1_CTS_N({1{1'b0}}),

    // Outputs
    .UART_0_TXD( ),
    .PWM1( ),
    .LMOTOR( ),
    .RMOTOR( ),
    .LSERVO( ),
    .RSERVO( ),
    .UART_1_TXD( ),
    .TX( ),
    .SPI_0_DO( ),
    .SPEAKER_DAC( ),
    .LED_OUT( ),
    .GPIO_3_OUT( ),
    .GPIO_5_OUT( ),
    .UART_1_RTS_N( ),

    // Inouts
    .SPI_0_CLK( ),
    .SPI_0_SS( ),
    .controller_data( ),
    .I2C_0_SCL( ),
    .I2C_0_SDA( )

);

endmodule

