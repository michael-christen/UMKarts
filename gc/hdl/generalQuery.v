// Expect 100MHz clock, does not deal with response
// need to used inout keyword

//note lengthVal should be 1 greater than cmdLength
module generalQuery(clk100mhz, reset, data, send, command, cmdLength);
input wire clk100mhz;
input wire reset;
input [24:0] command;
input [12:0] cmdLength;
output data;
output send;

reg [12:0] count;
reg data;
reg send;
reg init;
reg [12:0] lengthVal;

wire clk;
//Why count?
reg [9:0]reset_count;
reg cmdBit;

clockdiv(clk100mhz, clk);

always @(posedge clk100mhz) begin
	if (reset && ~init) begin
		reset_count <= 150;
	end 
	else if (reset_count == 0)
		reset_count <= 0;
	else
		reset_count <= reset_count - 1;
end

always @(posedge clk) begin
	if (reset_count > 0) begin
		count <= 0;
		lengthVal = cmdLength + 1;
	end
	else
		count <= count + 1;
	if(send) begin
		if(lengthVal > 0 && count%4==0) begin
			lengthVal <= lengthVal - 1;
			cmdBit <= command[lengthVal-1];
		end
		//done sending
		if(lengthVal == 0) begin
			//init <= 0;
			data <= 1'bz;
			send <= 1'b0;
		end
		//normal case
		else begin
			if(cmdBit) begin
				if(count % 4 == 0) begin
					data <= 0;
				end
				else begin
					data <= 1'bz;
				end
			end
			else begin
				if(count % 4 == 3) begin
					data <= 1'bz;
				end
				else begin
					data <= 0;
				end
			end
		end
	end
end

endmodule


