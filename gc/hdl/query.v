// Expect 100MHz clock, does not deal with response
// need to used inout keyword

module send_query(clk100mhz, reset, data, send);
input wire clk100mhz;
input wire reset;
output data;
output send;

reg [12:0] count;
reg data;
reg send;
reg init;

wire clk;
reg [9:0]reset_count;

clockdiv(clk100mhz, clk);

always @(posedge clk100mhz) begin
	if (reset && ~init) begin
		reset_count <= 150;
	end else if (reset_count == 0)
		reset_count <= 0;
	else
		reset_count <= reset_count - 1;
end

always @(posedge clk) begin
	if (reset_count > 0)
		count <= 1;
	else
		count <= count + 1;

	if (init || reset_count > 0) begin
		send <= 1'b1;
		if (count / 4 == 1 || count / 4 == 4 || count / 4 == 5 || count / 4 == 6 || count / 4 == 8 || count / 4 == 9 || count / 4 == 10 || count / 4 == 11 || count / 4 == 16 || count / 4 == 17 || count / 4 == 19 || count / 4 == 21 || count / 4 == 22 || count / 4 == 24) begin
			if (count % 4 == 0) begin
				data <= 0;
			end else begin
				data <= 1'bz;
			end
		end else if (count / 4 < 24) begin
			if (count % 4 == 3) begin
				data <= 1'bz;
			end else begin
				data <= 0;
			end
		end else begin
			data <= 1'bz;
		end

		if (count / 4 == 26) begin
			init <= 0;
		end else begin
			init <= 1;
		end
	end else if (count / 4 == 1 || count / 4 == 14 || count / 4 == 15 || count / 4 == 22 || count / 4 == 24) begin
		init <= 0;
		send <= 1'b1;
		if (count % 4 == 0) begin
			data <= 0;
		end else begin
			data <= 1'bz;
		end
	end else if (count / 4 < 24) begin
		init <= 0;
		send <= 1'b1;
		if (count % 4 == 3) begin
			data <= 1'bz;
		end else begin
			data <= 0;
		end
	end else begin
		init <= 0;
		data <= 1'bz;
		send <= 1'b0;
	end
end

endmodule

// Divides clock by 100.
module clockdiv(clockin, clockout);
input clockin;
output reg clockout;

reg [6:0] ccount;

always @(posedge clockin) begin
	if (ccount < 100)
		ccount <= ccount + 1;
	else
		ccount <= 0;
	
	if (ccount > 50)
		clockout <= 1;
	else
		clockout <= 0;
end

endmodule
