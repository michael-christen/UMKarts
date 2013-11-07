module gc_receive(clk, data, send, response, wavebird_id, wavebird_id_ready, controller_init);

input wire clk;
input wire data;
input wire send;
input wire controller_init;
output reg [63:0]response;
output reg [23:0]wavebird_id;
output reg wavebird_id_ready;

wire start_count;
reg data1, data2;
reg [63:0]next_response;
reg [7:0] next_response_count;
reg [7:0] count;


assign start_count = ~send && ~data1 && data2;

always @(posedge clk) begin
	if (controller_init)
		wavebird_id_ready = 0;

	data1 <= data;
	data2 <= data1;
	if (count == 0 && start_count == 1) begin
		count <= 1;
	end else if (count == 200) begin
		count <= 0;
		next_response[63] = data;
		next_response[62:0] = next_response[63:1];
		if (!controller_init) begin
			if (next_response_count >= 62) begin
				next_response_count <= 0;
				response[63] = next_response;
			end else begin
				next_response_count <= next_response_count + 1;
			end
		end else begin
			if (next_response_count >= 22) begin
				next_response_count <= 0;
				wavebird_id = next_response[63:40];
				wavebird_id_ready = 1;
			end else begin
				next_response_count <= next_response_count + 1;
			end
		end
	end else if (count == 0) begin
		if (send)
			next_response_count <= 0;
	end else begin
		count <= count + 1;
	end
end

endmodule

module gc_response_apb( 
 
/*** APB3 BUS INTERFACE ***/ 
input PCLK, // clock 
input PRESERN, // system reset 
input PSEL, // peripheral select 
input PENABLE, // distinguishes access phase 
output wire PREADY, // peripheral ready signal 
output wire PSLVERR, // error signal 
input PWRITE, // distinguishes read and write cycles 
input [31:0] PADDR, // I/O address 
input wire [31:0] PWDATA, // data from processor to I/O device (32 bits) 
output reg [31:0] PRDATA, // data to processor from I/O device (32-bits)  
input [63:0] response, // port to switch
output reg start_init,
input wire [23:0] x
); 
 
assign PSLVERR = 0; 
assign PREADY = 1; 
 
//As the only thing we read is the switch, we can constantly drive it! 
always @(posedge PCLK) //register control for APB3 reads 
begin 
	if (PADDR[3]) begin PRDATA[23:0] <= x; PRDATA[31:24] <= 0;end else
	if (PADDR[2])
		PRDATA <= response[63:32];
	else
		PRDATA <= response[31:0];

	if (PENABLE && PWRITE && PSEL)
		start_init <= 1;
	else
		start_init <= 0;
end 
 
endmodule 
