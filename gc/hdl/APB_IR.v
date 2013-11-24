// APB_IR.v

module APB_IR(
	input PCLK,
	input PRESERN,
	input PSEL,
	input PENABLE,
	output wire PREADY,
	output wire PSLVERR,
	input PWRITE,
	input [31:0] PADDR,
	input wire [31:0] PWDATA,
	output reg [31:0] PRDATA,

	// IO Ports
	input wire [7:0] IR_DATA,
	input wire BUF_FULL,

	output wire [7:0] MSG,
	output wire ENQUEUE
);

	assign MSG = PWDATA[7:0];
	
	assign PSLVERR = 1'b0;

	// Always ready (so we don't block)
	// If we are currently sending, we just ignore this write
	assign PREADY = 1'b1;

	wire data_write = PSEL & PWRITE & PENABLE;
	assign ENQUEUE = data_write & ~BUF_FULL & PRESERN;

	wire data_read  = PSEL & ~PWRITE & PENABLE;


	// Deal with reads
	always @(posedge PCLK) begin
		PRDATA[31:9] <= 0;
		PRDATA[7:0] <= IR_DATA;
		PRDATA[8] <= BUF_FULL;
	end

endmodule