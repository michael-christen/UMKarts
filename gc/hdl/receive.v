module gc_receive(clk, data, send, response, t, data1, data2);

input wire clk;
input wire data;
input wire send;
output reg [63:0]response;

wire start_count;
output reg data1, data2;
//This keeps track of intermediate signals
reg [63:0]next_response;
reg [7:0] next_response_count;
reg [7:0] count;

output wire t;

assign t = count[0];
assign start_count = ~send && ~data1 && data2;

always @(posedge clk) begin
	//Clock the input
	data1 <= data;
	data2 <= data1;
	//Begin
	if (count == 0 && start_count == 1) begin
		count <= 1;
		//What?
		next_response_count <= next_response_count;
		response <= response;
	end 
	else if (count == 200) begin
		count <= 0;
		//shift everything over
		//not sure if this is the best way to shift
		next_response[63] <= data;
		next_response[62:0] <= next_response[63:1];
		//finishing receive, and putting in response
		if (next_response_count >= 62) begin
			next_response_count <= 0;
			response[63] <= data;
			response[62:0] <= next_response[63:1];
		end 
		else begin
			next_response_count <= next_response_count + 1;
			response <= response;
		end
	end 
	else if (count == 0) begin
		count <= count;
		if (!send)
			next_response_count <= next_response_count;
		else
			next_response_count <= 0;
		response <= response;
	end 
	else begin
		count <= count + 1;
		//Why?
		next_response_count <= next_response_count;
		response <= response;
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
output reg init
); 
 
assign PSLVERR = 0; 
assign PREADY = 1; 
 
//As the only thing we read is the switch, we can constantly drive it! 
always @(posedge PCLK) //register control for APB3 reads 
begin 
	if (PADDR[2])
		PRDATA <= response[63:32];
	else
		PRDATA <= response[31:0];

	if (PENABLE && PWRITE && PSEL)
		init <= 1;
	else
		init <= 0;
end 
 
endmodule 
