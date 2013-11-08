module gc_state(clk, start_init, controller_init, wavebird_id, wavebird_id_ready, wavebird_id_send, wavebird_id_sent, button_data_ready);
input wire clk;
input wire start_init;
input wire [23:0] wavebird_id;
input wire wavebird_id_ready;
input wire wavebird_id_sent;
input wire button_data_ready;
output reg controller_init;
output reg wavebird_id_send;

reg [31:0]last_response;

always @(posedge clk) begin
	if (start_init) begin
		controller_init = 1;
		wavebird_id_send = 0;
	end

	if (last_response >= 100000000) begin
		controller_init = 1;
		wavebird_id_send = 0;
	end
	
	// Restart the init sequence if we haven't gotten a response in 1 sec.
	if (button_data_ready || wavebird_id_ready) begin
		last_response <= 0;
	end else begin
		last_response <= last_response + 1;
	end
	
	if (controller_init && wavebird_id_ready) begin
		// If we get 0x09 at the beginning, we have a wired controller, no further init necessary
		if (wavebird_id[23:16] == 8'h09) begin
			controller_init = 0;
		end
		// If we got 0xA8 at the beginning, the wavebird isn't ready to pair yet
		else if (wavebird_id[23:16] == 8'hA8) begin
		
		end
		
		// Anything else, we need to send the wavebird init command.
		else begin
			wavebird_id_send = 1;
		end
	end

	if (wavebird_id_sent) begin
		controller_init = 0;
	end
end

endmodule


module send_query(clk100mhz, data, send, controller_init, wavebird_id_send, wavebird_id, wavebird_id_sent);
input wire clk100mhz;
input wire controller_init;
input wire wavebird_id_send;
input wire [23:0]wavebird_id;
output data;
output send;
output reg wavebird_id_sent;

reg [6:0] c_count;
reg [12:0] count;
reg data;
reg send;

wire clk;

task send0; begin
	if (count % 4 == 3) begin
		data <= 1'bz;
	end else begin
		data <= 0;
	end
end endtask

task send1; begin
	if (count % 4 == 0) begin
		data <= 0;
	end else begin
		data <= 1'bz;
	end
end endtask

always @(posedge clk100mhz) begin
	if (c_count >= 100) begin
		count <= count + 1;
		c_count <= 0;
	end else begin
		count <= count;
		c_count <= c_count + 1;
	end

	wavebird_id_sent = 0;
	
	if (controller_init && wavebird_id_send && count / 4 <= 24) begin
		if (count / 4 < 24 || count % 4 == 0) send <= 1; else send <= 0;
		if (count / 4 == 3 || count / 4 == 4 || count / 4 == 5 || count / 4 == 6 || (count / 4 >= 8 && count / 4 < 24 && wavebird_id[23-(count/4)]) || count / 4 == 24)
			send1();
		else
			send0();

		if (count / 4 == 24 && count % 4 == 3 && c_count == 99)
			wavebird_id_sent = 1; 
	end else if (controller_init && count / 4 <= 8) begin
		// Send 0x00 to check controller status
		if (count / 4 < 8 || count % 4 == 0) send <= 1; else send <= 0;
		if (count / 4 == 8)
			send1();
		else
			send0();
	end else if (~controller_init && count / 4 <= 24) begin
		// Send 0x400302 to poll button status
		if (count / 4 < 24 || count % 4 == 0) send <= 1; else send <= 0;
		if (count / 4 == 1 || count / 4 == 14 || count / 4 == 15 || count / 4 == 22 || count / 4 == 24)
			send1();
		else
			send0();
	end else begin
		send <= 1'b0;
		data <= 1'bz;
	end
/*
	if ((wavebird_init && wavebird_id_ready && (count / 4 == 1 || count / 4 == 4 || count / 4 == 5 || count / 4 == 6 || count / 4 == 8 || count / 4 == 9 || count / 4 == 10 || count / 4 == 12 || count / 4 == 16 || count / 4 == 17 || count / 4 == 19 || count / 4 == 21 || count / 4 == 22 || count / 4 == 24)) ||
		(~wavebird_init && ) ||
		(wavebird_init && !wavebird_id_ready && count / 4 == 8)) begin
		send <= 1'b1;
		if (count % 4 == 0) begin
			data <= 0;
		end else begin
			data <= 1'bz;
		end
	end else if (count / 4 < 9 || ((~wavebird_init || wavebird_id_ready) && count / 4 < 25)) begin
		
	end else begin
		data <= 1'bz;
		send <= 1'b0;
	end*/
end

endmodule
