// LED_PULSE.v
module LED_PULSE(
	input wire INV_RESET,
	input wire PCLK,
	input wire LED_ON,
	output wire LED
);
	wire RESET = !INV_RESET;
	clk_div pulse(RESET | ~LED_ON, PCLK, LED);
	// Pulse at 56 kHz
	defparam pulse.period = 32'd1786;
	 

endmodule