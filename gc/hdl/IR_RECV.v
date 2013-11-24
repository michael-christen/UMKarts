// IR_RECV.v
module LED_RECV(input wire INV_RESET, input wire RECV_IN, input wire CLK, output reg [7:0] DATA, output wire INTERRUPT);
  // Assuming CLK == 100 MHz
  reg[2:0] SYNC_IN;
  reg[31:0] COUNTER;
  reg[3:0] SIZE;
  reg[7:0] TEMP_DATA;
  reg NEWDATA;
  reg ERROR;

  wire RESET = !INV_RESET;

  parameter SEVEN_HUNDRED_EIGHTY_MICROSECS = 32'd78000;
  parameter NINE_HUNDRED_MICROSECS = 32'd90000;
  parameter FOUR_HUNDRED_MICROSECS = 32'd40000;

  // Create different wires first
  wire FALLING_EDGE = SYNC_IN[0] & ~SYNC_IN[1];
  wire WIRE_ERROR = (FALLING_EDGE && COUNTER <= SEVEN_HUNDRED_EIGHTY_MICROSECS) || (COUNTER >= NINE_HUNDRED_MICROSECS);
  wire SAMPLE = (COUNTER == FOUR_HUNDRED_MICROSECS);
  assign INTERRUPT = (NEWDATA);

  // Deal with SYNC_IN reg
  always @(posedge CLK) begin
    if (RESET)
      SYNC_IN <= 3'd0;
    else begin
      SYNC_IN[2] <= RECV_IN;
      SYNC_IN[1:0] <= SYNC_IN[2:1];
    end
  end

  // Deal with ERROR
  // Stays 1 until Falling EDGE
  always @(posedge CLK) begin
    if (FALLING_EDGE)
      ERROR <= 1'b0;
    else if (RESET | WIRE_ERROR)
      ERROR <= 1'b1;     
  end

  // Deal with NEWDATA
  always @(posedge CLK) begin
    if (SIZE == 4'd8)
      NEWDATA <= 1'b1;
    else begin
      NEWDATA <= 1'b0;
    end
  end

  // Deal with counter
  always @(posedge CLK) begin
    if (RESET | FALLING_EDGE)
      COUNTER <= 32'd0;
    else
      COUNTER <= COUNTER + 32'd1;
  end

  // Deal with SIZE
  always @(posedge CLK) begin
    if (RESET | ERROR | NEWDATA)
      SIZE <= 4'd0;
    else if (SAMPLE)
      SIZE <= SIZE + 4'd1;
  end

  // Deal with TEMP_DATA
  always @(posedge CLK) begin
    if (RESET | ERROR)
      TEMP_DATA <= 8'd0;
    else if (SAMPLE) begin
      TEMP_DATA[7] <= ~SYNC_IN[0];
      TEMP_DATA[6:0] <= TEMP_DATA[7:1];
    end
  end

  // Deal with DATA
  always @(posedge CLK) begin
    if (RESET)
      DATA <= 8'd0;
    else if (NEWDATA)
      DATA <= TEMP_DATA;
  end
endmodule