//////////////////////////////////////////////////////////////////////
// Created by SmartDesign Wed Nov 06 12:52:38 2013
// Version: v11.0 11.0.0.23
//////////////////////////////////////////////////////////////////////

`timescale 1 ns/100 ps

// gc
module gc(
    // Inputs
    CAPTURE_SWITCH,
    MSS_RESET_N,
    UART_0_RXD,
    // Outputs
    LMOTOR,
    LSERVO,
    PWM1,
    RMOTOR,
    RSERVO,
    UART_0_TXD,
    data1,
    data2,
    send,
    start_count,
    // Inouts
    data
);

//--------------------------------------------------------------------
// Input
//--------------------------------------------------------------------
input  CAPTURE_SWITCH;
input  MSS_RESET_N;
input  UART_0_RXD;
//--------------------------------------------------------------------
// Output
//--------------------------------------------------------------------
output LMOTOR;
output LSERVO;
output PWM1;
output RMOTOR;
output RSERVO;
output UART_0_TXD;
output data1;
output data2;
output send;
output start_count;
//--------------------------------------------------------------------
// Inout
//--------------------------------------------------------------------
inout  data;
//--------------------------------------------------------------------
// Nets
//--------------------------------------------------------------------
wire          CAPTURE_SWITCH;
wire          CoreAPB3_0_APBmslave0_PENABLE;
wire   [31:0] CoreAPB3_0_APBmslave0_PRDATA;
wire          CoreAPB3_0_APBmslave0_PREADY;
wire          CoreAPB3_0_APBmslave0_PSELx;
wire          CoreAPB3_0_APBmslave0_PSLVERR;
wire   [31:0] CoreAPB3_0_APBmslave0_PWDATA;
wire          CoreAPB3_0_APBmslave0_PWRITE;
wire   [31:0] CoreAPB3_0_APBmslave1_PRDATA;
wire          CoreAPB3_0_APBmslave1_PREADY;
wire          CoreAPB3_0_APBmslave1_PSELx;
wire          CoreAPB3_0_APBmslave1_PSLVERR;
wire          data_net_0;
wire          data1_net_0;
wire          data2_net_0;
wire          gc_MSS_0_FAB_CLK;
wire          gc_MSS_0_M2F_RESET_N;
wire          gc_MSS_0_MSS_MASTER_APB_PENABLE;
wire   [31:0] gc_MSS_0_MSS_MASTER_APB_PRDATA;
wire          gc_MSS_0_MSS_MASTER_APB_PREADY;
wire          gc_MSS_0_MSS_MASTER_APB_PSELx;
wire          gc_MSS_0_MSS_MASTER_APB_PSLVERR;
wire   [31:0] gc_MSS_0_MSS_MASTER_APB_PWDATA;
wire          gc_MSS_0_MSS_MASTER_APB_PWRITE;
wire   [63:0] gc_receive_0_response;
wire          gc_response_apb_0_init;
wire          LMOTOR_net_0;
wire          LSERVO_net_0;
wire          MSS_RESET_N;
wire          PWM1_net_0;
wire          RMOTOR_net_0;
wire          RSERVO_net_0;
wire          send_net_0;
wire          start_count_net_0;
wire          UART_0_RXD;
wire          UART_0_TXD_1;
wire          UART_0_TXD_1_net_0;
wire          start_count_net_1;
wire          send_net_1;
wire          data1_net_1;
wire          data2_net_1;
wire          data_net_1;
wire          PWM1_net_1;
wire          LMOTOR_net_1;
wire          RMOTOR_net_1;
wire          LSERVO_net_1;
wire          RSERVO_net_1;
//--------------------------------------------------------------------
// TiedOff Nets
//--------------------------------------------------------------------
wire          GND_net;
wire          VCC_net;
wire   [31:0] IADDR_const_net_0;
wire   [31:0] PRDATAS2_const_net_0;
wire   [31:0] PRDATAS3_const_net_0;
wire   [31:0] PRDATAS4_const_net_0;
wire   [31:0] PRDATAS5_const_net_0;
wire   [31:0] PRDATAS6_const_net_0;
wire   [31:0] PRDATAS7_const_net_0;
wire   [31:0] PRDATAS8_const_net_0;
wire   [31:0] PRDATAS9_const_net_0;
wire   [31:0] PRDATAS10_const_net_0;
wire   [31:0] PRDATAS11_const_net_0;
wire   [31:0] PRDATAS12_const_net_0;
wire   [31:0] PRDATAS13_const_net_0;
wire   [31:0] PRDATAS14_const_net_0;
wire   [31:0] PRDATAS15_const_net_0;
wire   [31:0] PRDATAS16_const_net_0;
//--------------------------------------------------------------------
// Bus Interface Nets Declarations - Unequal Pin Widths
//--------------------------------------------------------------------
wire   [31:0] CoreAPB3_0_APBmslave0_PADDR;
wire   [7:0]  CoreAPB3_0_APBmslave0_PADDR_0_7to0;
wire   [7:0]  CoreAPB3_0_APBmslave0_PADDR_0;
wire   [31:20]gc_MSS_0_MSS_MASTER_APB_PADDR_0_31to20;
wire   [19:0] gc_MSS_0_MSS_MASTER_APB_PADDR_0_19to0;
wire   [31:0] gc_MSS_0_MSS_MASTER_APB_PADDR_0;
wire   [19:0] gc_MSS_0_MSS_MASTER_APB_PADDR;
//--------------------------------------------------------------------
// Constant assignments
//--------------------------------------------------------------------
assign GND_net               = 1'b0;
assign VCC_net               = 1'b1;
assign IADDR_const_net_0     = 32'h00000000;
assign PRDATAS2_const_net_0  = 32'h00000000;
assign PRDATAS3_const_net_0  = 32'h00000000;
assign PRDATAS4_const_net_0  = 32'h00000000;
assign PRDATAS5_const_net_0  = 32'h00000000;
assign PRDATAS6_const_net_0  = 32'h00000000;
assign PRDATAS7_const_net_0  = 32'h00000000;
assign PRDATAS8_const_net_0  = 32'h00000000;
assign PRDATAS9_const_net_0  = 32'h00000000;
assign PRDATAS10_const_net_0 = 32'h00000000;
assign PRDATAS11_const_net_0 = 32'h00000000;
assign PRDATAS12_const_net_0 = 32'h00000000;
assign PRDATAS13_const_net_0 = 32'h00000000;
assign PRDATAS14_const_net_0 = 32'h00000000;
assign PRDATAS15_const_net_0 = 32'h00000000;
assign PRDATAS16_const_net_0 = 32'h00000000;
//--------------------------------------------------------------------
// Top level output port assignments
//--------------------------------------------------------------------
assign UART_0_TXD_1_net_0 = UART_0_TXD_1;
assign UART_0_TXD         = UART_0_TXD_1_net_0;
assign start_count_net_1  = start_count_net_0;
assign start_count        = start_count_net_1;
assign send_net_1         = send_net_0;
assign send               = send_net_1;
assign data1_net_1        = data1_net_0;
assign data1              = data1_net_1;
assign data2_net_1        = data2_net_0;
assign data2              = data2_net_1;
assign data_net_1         = data_net_0;
assign data               = data_net_1;
assign PWM1_net_1         = PWM1_net_0;
assign PWM1               = PWM1_net_1;
assign LMOTOR_net_1       = LMOTOR_net_0;
assign LMOTOR             = LMOTOR_net_1;
assign RMOTOR_net_1       = RMOTOR_net_0;
assign RMOTOR             = RMOTOR_net_1;
assign LSERVO_net_1       = LSERVO_net_0;
assign LSERVO             = LSERVO_net_1;
assign RSERVO_net_1       = RSERVO_net_0;
assign RSERVO             = RSERVO_net_1;
//--------------------------------------------------------------------
// Bus Interface Nets Assignments - Unequal Pin Widths
//--------------------------------------------------------------------
assign CoreAPB3_0_APBmslave0_PADDR_0_7to0 = CoreAPB3_0_APBmslave0_PADDR[7:0];
assign CoreAPB3_0_APBmslave0_PADDR_0 = { CoreAPB3_0_APBmslave0_PADDR_0_7to0 };

assign gc_MSS_0_MSS_MASTER_APB_PADDR_0_31to20 = 12'h0;
assign gc_MSS_0_MSS_MASTER_APB_PADDR_0_19to0 = gc_MSS_0_MSS_MASTER_APB_PADDR[19:0];
assign gc_MSS_0_MSS_MASTER_APB_PADDR_0 = { gc_MSS_0_MSS_MASTER_APB_PADDR_0_31to20, gc_MSS_0_MSS_MASTER_APB_PADDR_0_19to0 };

//--------------------------------------------------------------------
// Component instances
//--------------------------------------------------------------------
//--------CoreAPB3   -   Actel:DirectCore:CoreAPB3:4.0.100
CoreAPB3 #( 
        .APB_DWIDTH      ( 32 ),
        .APBSLOT0ENABLE  ( 1 ),
        .APBSLOT1ENABLE  ( 1 ),
        .APBSLOT2ENABLE  ( 0 ),
        .APBSLOT3ENABLE  ( 0 ),
        .APBSLOT4ENABLE  ( 0 ),
        .APBSLOT5ENABLE  ( 0 ),
        .APBSLOT6ENABLE  ( 0 ),
        .APBSLOT7ENABLE  ( 0 ),
        .APBSLOT8ENABLE  ( 0 ),
        .APBSLOT9ENABLE  ( 0 ),
        .APBSLOT10ENABLE ( 0 ),
        .APBSLOT11ENABLE ( 0 ),
        .APBSLOT12ENABLE ( 0 ),
        .APBSLOT13ENABLE ( 0 ),
        .APBSLOT14ENABLE ( 0 ),
        .APBSLOT15ENABLE ( 0 ),
        .IADDR_OPTION    ( 0 ),
        .MADDR_BITS      ( 12 ),
        .SC_0            ( 0 ),
        .SC_1            ( 0 ),
        .SC_2            ( 0 ),
        .SC_3            ( 0 ),
        .SC_4            ( 0 ),
        .SC_5            ( 0 ),
        .SC_6            ( 0 ),
        .SC_7            ( 0 ),
        .SC_8            ( 0 ),
        .SC_9            ( 0 ),
        .SC_10           ( 0 ),
        .SC_11           ( 0 ),
        .SC_12           ( 0 ),
        .SC_13           ( 0 ),
        .SC_14           ( 0 ),
        .SC_15           ( 0 ),
        .UPR_NIBBLE_POSN ( 2 ) )
CoreAPB3_0(
        // Inputs
        .PRESETN    ( GND_net ), // tied to 1'b0 from definition
        .PCLK       ( GND_net ), // tied to 1'b0 from definition
        .PWRITE     ( gc_MSS_0_MSS_MASTER_APB_PWRITE ),
        .PENABLE    ( gc_MSS_0_MSS_MASTER_APB_PENABLE ),
        .PSEL       ( gc_MSS_0_MSS_MASTER_APB_PSELx ),
        .PREADYS0   ( CoreAPB3_0_APBmslave0_PREADY ),
        .PSLVERRS0  ( CoreAPB3_0_APBmslave0_PSLVERR ),
        .PREADYS1   ( CoreAPB3_0_APBmslave1_PREADY ),
        .PSLVERRS1  ( CoreAPB3_0_APBmslave1_PSLVERR ),
        .PREADYS2   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS2  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS3   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS3  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS4   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS4  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS5   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS5  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS6   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS6  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS7   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS7  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS8   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS8  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS9   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS9  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS10  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS10 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS11  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS11 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS12  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS12 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS13  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS13 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS14  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS14 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS15  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS15 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS16  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS16 ( GND_net ), // tied to 1'b0 from definition
        .PADDR      ( gc_MSS_0_MSS_MASTER_APB_PADDR_0 ),
        .PWDATA     ( gc_MSS_0_MSS_MASTER_APB_PWDATA ),
        .PRDATAS0   ( CoreAPB3_0_APBmslave0_PRDATA ),
        .PRDATAS1   ( CoreAPB3_0_APBmslave1_PRDATA ),
        .PRDATAS2   ( PRDATAS2_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS3   ( PRDATAS3_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS4   ( PRDATAS4_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS5   ( PRDATAS5_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS6   ( PRDATAS6_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS7   ( PRDATAS7_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS8   ( PRDATAS8_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS9   ( PRDATAS9_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS10  ( PRDATAS10_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS11  ( PRDATAS11_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS12  ( PRDATAS12_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS13  ( PRDATAS13_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS14  ( PRDATAS14_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS15  ( PRDATAS15_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS16  ( PRDATAS16_const_net_0 ), // tied to 32'h00000000 from definition
        .IADDR      ( IADDR_const_net_0 ), // tied to 32'h00000000 from definition
        // Outputs
        .PREADY     ( gc_MSS_0_MSS_MASTER_APB_PREADY ),
        .PSLVERR    ( gc_MSS_0_MSS_MASTER_APB_PSLVERR ),
        .PWRITES    ( CoreAPB3_0_APBmslave0_PWRITE ),
        .PENABLES   ( CoreAPB3_0_APBmslave0_PENABLE ),
        .PSELS0     ( CoreAPB3_0_APBmslave0_PSELx ),
        .PSELS1     ( CoreAPB3_0_APBmslave1_PSELx ),
        .PSELS2     (  ),
        .PSELS3     (  ),
        .PSELS4     (  ),
        .PSELS5     (  ),
        .PSELS6     (  ),
        .PSELS7     (  ),
        .PSELS8     (  ),
        .PSELS9     (  ),
        .PSELS10    (  ),
        .PSELS11    (  ),
        .PSELS12    (  ),
        .PSELS13    (  ),
        .PSELS14    (  ),
        .PSELS15    (  ),
        .PSELS16    (  ),
        .PRDATA     ( gc_MSS_0_MSS_MASTER_APB_PRDATA ),
        .PADDRS     ( CoreAPB3_0_APBmslave0_PADDR ),
        .PWDATAS    ( CoreAPB3_0_APBmslave0_PWDATA ) 
        );

//--------gc_MSS
gc_MSS gc_MSS_0(
        // Inputs
        .MSS_RESET_N ( MSS_RESET_N ),
        .MSSPREADY   ( gc_MSS_0_MSS_MASTER_APB_PREADY ),
        .MSSPSLVERR  ( gc_MSS_0_MSS_MASTER_APB_PSLVERR ),
        .UART_0_RXD  ( UART_0_RXD ),
        .MSSPRDATA   ( gc_MSS_0_MSS_MASTER_APB_PRDATA ),
        // Outputs
        .FAB_CLK     ( gc_MSS_0_FAB_CLK ),
        .M2F_RESET_N ( gc_MSS_0_M2F_RESET_N ),
        .MSSPSEL     ( gc_MSS_0_MSS_MASTER_APB_PSELx ),
        .MSSPENABLE  ( gc_MSS_0_MSS_MASTER_APB_PENABLE ),
        .MSSPWRITE   ( gc_MSS_0_MSS_MASTER_APB_PWRITE ),
        .UART_0_TXD  ( UART_0_TXD_1 ),
        .MSSPADDR    ( gc_MSS_0_MSS_MASTER_APB_PADDR ),
        .MSSPWDATA   ( gc_MSS_0_MSS_MASTER_APB_PWDATA ) 
        );

//--------gc_receive
gc_receive gc_receive_0(
        // Inputs
        .clk      ( gc_MSS_0_FAB_CLK ),
        .data     ( data_net_0 ),
        .send     ( send_net_0 ),
        // Outputs
        .t        ( start_count_net_0 ),
        .data1    ( data1_net_0 ),
        .data2    ( data2_net_0 ),
        .response ( gc_receive_0_response ) 
        );

//--------gc_response_apb
gc_response_apb gc_response_apb_0(
        // Inputs
        .PCLK     ( gc_MSS_0_FAB_CLK ),
        .PRESERN  ( gc_MSS_0_M2F_RESET_N ),
        .PSEL     ( CoreAPB3_0_APBmslave0_PSELx ),
        .PENABLE  ( CoreAPB3_0_APBmslave0_PENABLE ),
        .PWRITE   ( CoreAPB3_0_APBmslave0_PWRITE ),
        .PADDR    ( CoreAPB3_0_APBmslave0_PADDR ),
        .PWDATA   ( CoreAPB3_0_APBmslave0_PWDATA ),
        .response ( gc_receive_0_response ),
        // Outputs
        .PREADY   ( CoreAPB3_0_APBmslave0_PREADY ),
        .PSLVERR  ( CoreAPB3_0_APBmslave0_PSLVERR ),
        .init     ( gc_response_apb_0_init ),
        .PRDATA   ( CoreAPB3_0_APBmslave0_PRDATA ) 
        );

//--------motorWrapper
motorWrapper motorWrapper_0(
        // Inputs
        .PCLK           ( gc_MSS_0_FAB_CLK ),
        .PENABLE        ( CoreAPB3_0_APBmslave0_PENABLE ),
        .PSEL           ( CoreAPB3_0_APBmslave1_PSELx ),
        .PRESETN        ( gc_MSS_0_M2F_RESET_N ),
        .PWRITE         ( CoreAPB3_0_APBmslave0_PWRITE ),
        .CAPTURE_SWITCH ( CAPTURE_SWITCH ),
        .PADDR          ( CoreAPB3_0_APBmslave0_PADDR_0 ),
        .PWDATA         ( CoreAPB3_0_APBmslave0_PWDATA ),
        // Outputs
        .PREADY         ( CoreAPB3_0_APBmslave1_PREADY ),
        .PSLVERR        ( CoreAPB3_0_APBmslave1_PSLVERR ),
        .FABINT         (  ),
        .PWM1           ( PWM1_net_0 ),
        .PWM2           (  ),
        .LMOTOR         ( LMOTOR_net_0 ),
        .RMOTOR         ( RMOTOR_net_0 ),
        .LSERVO         ( LSERVO_net_0 ),
        .RSERVO         ( RSERVO_net_0 ),
        .PRDATA         ( CoreAPB3_0_APBmslave1_PRDATA ),
        .TPS            (  ) 
        );

//--------send_query
send_query send_query_0(
        // Inputs
        .clk100mhz ( gc_MSS_0_FAB_CLK ),
        .reset     ( gc_response_apb_0_init ),
        // Outputs
        .data      ( data_net_0 ),
        .send      ( send_net_0 ) 
        );


endmodule
