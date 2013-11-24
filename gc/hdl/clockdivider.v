// clockdivider.v

module clk_div(reset, clk, clkOut);
	input clk, reset;
	output clkOut;
	reg clkOut;

	parameter period = 32'd60000;
	parameter half = period / 2;

	reg [31:0] counter;

	always @(posedge clk) begin
		if (reset) begin
			counter <= 32'd0;
		end else if (counter == period - 1) begin
			counter <= 32'd0;
		end else begin
			counter <= counter + 32'd1;
		end

		clkOut <= (counter < half) & ~reset;
	end
endmodule