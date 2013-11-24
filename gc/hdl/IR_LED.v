// IR_LED.v

module IR_LED(
	input CLK,
	input INV_RESET,
	input [7:0] MSG,
	input wire MSG_VALID,
	input SEND,
	output wire DIVCLK,
	output wire LEDOUT,
	output wire IR_READY
);
	wire RESET = !INV_RESET;
	reg LATCHED_RESET;
	reg SLOW_RESET;

	parameter SEND_SIZE		 = 8'd8;

	parameter STATE_RESET    = 4'h0;
	parameter STATE_IDLE     = 4'h1;
	parameter STATE_LED_ON   = 4'h2;
	parameter STATE_SEND_0_1 = 4'h3;
	parameter STATE_SEND_1_1 = 4'h4;
	parameter STATE_LED_OFF  = 4'h5;
	parameter STATE_SEND_0_2 = 4'h6;
	parameter STATE_SEND_1_2 = 4'h7;
	parameter STATE_STALL_1  = 4'h8;
	parameter STATE_STALL_2  = 4'h9;
	parameter STATE_STALL_3  = 4'hA;
    parameter STATE_SYNC_IN  = 4'hB;
	
	reg [3:0] state;
	reg [3:0] next_state = STATE_IDLE;

	reg [7:0] size;
	reg [7:0] next_size = 8'd0;

	reg [7:0] msg;
	reg [7:0] next_msg = 8'd0;

	assign IR_READY = (state == STATE_IDLE);
	assign LEDOUT = ((state == STATE_LED_ON) || (state == STATE_SEND_1_1) || (state == STATE_SEND_1_2));

	clk_div div(RESET, CLK, DIVCLK);
	// Set period to 200 microsecs
	defparam div.period = 32'd20000;

	// Deal with latched reset
	always @(posedge CLK) begin
		if (RESET)
			LATCHED_RESET <= 1'b1;
		else if (SLOW_RESET)
			LATCHED_RESET <= 1'b0;
	end

	// Update state, size and msg
	always @(posedge DIVCLK) begin
		if (LATCHED_RESET) begin
			state <= STATE_RESET;
			size <= 8'd0;
			msg <=  8'd0;
			SLOW_RESET <= 1'b1;
		end else begin 
			state <= next_state;
			size <= next_size;
			msg <= next_msg;
			SLOW_RESET <= 1'b0;
		end
	end

	// Compute next state
	always @* begin
		case(state)
			// Reset always goes to IDLE
			STATE_RESET: begin
				next_state = STATE_IDLE;
			end

			// Stay IDLE until ready to send
			STATE_IDLE: begin
				if (SEND)
					next_state = STATE_SYNC_IN;
				else
					next_state = STATE_IDLE;
			end

			STATE_SYNC_IN: begin
				if (MSG_VALID)
					next_state = STATE_LED_ON;
				else
					next_state = STATE_SYNC_IN;
			end

			// If sending 1, goto send_1 else goto send 0
			STATE_LED_ON: begin
				if (msg[0] == 1'b1)
					next_state = STATE_SEND_1_1;
				else
					next_state = STATE_SEND_0_1;
			end

			// Keep LED on
			STATE_SEND_1_1: begin
				next_state = STATE_SEND_1_2;
			end

			// Keep LED On
			STATE_SEND_1_2: begin
				next_state = STATE_LED_OFF;
			end

			// Turn off LED
			STATE_SEND_0_1: begin
				next_state = STATE_SEND_0_2;
			end

			// Turn off LED
			STATE_SEND_0_2: begin
				next_state = STATE_LED_OFF;
			end

			// Turn off LED, goto IDLE if done
			STATE_LED_OFF: begin
				// There was an error. RESET
				if (size > SEND_SIZE)
					next_state = STATE_RESET;
				// Done, go back to IDLE
				else if (size == SEND_SIZE)
					next_state = STATE_STALL_1;
				else
					next_state = STATE_LED_ON;
			end

			STATE_STALL_1:
				next_state = STATE_STALL_2;

			STATE_STALL_2:
				next_state = STATE_STALL_3;

			STATE_STALL_3:
				next_state = STATE_IDLE;

			default: begin
				next_state = STATE_RESET;
			end
		endcase
	end

	// Compute next size
	always @* begin
		if (state == STATE_IDLE || state == STATE_RESET)
			next_size = 8'd0;
		else if (state == STATE_LED_ON)
			next_size = size + 8'd1;
		else
			next_size = size;
	end

	// Compute next data
	always @* begin
		if (state == STATE_RESET)
			next_msg = 8'b0;
		else if (state == STATE_SYNC_IN)
			next_msg = MSG;
		else if (state == STATE_LED_ON)
			next_msg = msg >> 1;
		else
			next_msg = msg;
	end

endmodule