module motor(
			pclk,
			nreset,
			bus_write_en, 
			bus_read_en,
			bus_addr,
			bus_write_data, //data_in
			bus_read_data,
            fabint,
            pwm1,
            pwm2,
			leftMotor,
			rightMotor,
			leftServo,
			rightServo,
            switch //Switch/Capture input
            );
	
input pclk, nreset, bus_write_en, bus_read_en;
input [7:0] bus_addr;
input [31:0] bus_write_data;
input switch;
output reg [31:0] bus_read_data;
output reg fabint;
output reg pwm1;
output reg pwm2;
output wire leftMotor;
output wire rightMotor;
output wire leftServo;
output wire rightServo;

reg [31:0] compareReg;
reg [31:0] counterReg;
reg [31:0] controlReg;
reg [31:0] overflowReg;
reg [31:0] captureSyncReg;
reg [31:0] captureAsyncReg;
reg [31:0] directionReg;

reg overflowReset;       //Resets counterReg when new overflow value is written

wire timerEn;        //Timer Enable
wire interruptEn;    //Interrupt Enable
wire compareEn;      //Compare Enable
wire overflowEn;     //Overflow Enable
wire captureEn;
wire pwmEn;

assign timerEn      = controlReg[0];
assign interruptEn  = controlReg[1];
assign compareEn    = controlReg[2];
assign overflowEn   = controlReg[3];
assign pwmEn	    = controlReg[4];
assign captureEn    = controlReg[5];

//Assign motor stuff
assign leftMotor = directionReg[0];
assign rightMotor= directionReg[1];
assign leftServo = directionReg[2];
assign rightServo = directionReg[3];

reg [1:0] interrupt_status;
reg reset_interrupt;
reg timer_interrupt;

reg [2:0] switch_syncer;
reg capture_interrupt;
reg capture_status_sync;
reg capture_status_async;
reg reset_capture_sync;
reg reset_capture_async;

reg [31:0] nextCounter;

always@(posedge pclk)
if(~nreset)
  fabint   <= 1'b0;
else
  begin
    if(timer_interrupt || capture_interrupt)
      fabint   <= 1'b1;
    else
      fabint   <= 1'b0;
end

always@(posedge pclk)
if(~nreset)
  begin
    overflowReset   <= 1'b0;
    compareReg      <= 32'h00000000;
    overflowReg     <= 32'h00000000;
    controlReg      <= 32'h00000000;
    reset_interrupt     <= 1'b0;
    reset_capture_sync  <= 1'b0;
    reset_capture_async <= 1'b0;
  end
else begin
	if(bus_write_en) begin : WRITE
		case(bus_addr[4:2])
			3'b000: // Overflow Register
                begin 
                overflowReset   <= 1'b1;
				overflowReg     <= bus_write_data;
                end
            3'b001: // Timer Value, Read Only
                begin
                overflowReset <= 1'b0;
                end
            3'b010: // Timer Control
                begin 
                overflowReset   <= 1'b0;
                controlReg      <= bus_write_data;
                end
            3'b011: // Compare Register
                begin
                overflowReset   <= 1'b0;
                compareReg      <= bus_write_data;
                end
            3'b100: //Interrupt/Capture Status, Read Only
                begin
                overflowReset <= 1'b0;
                end
            3'b101: //CaptureSync, Read Only
                begin
                overflowReset <= 1'b0;
                end
            3'b110: //CaptureAsync, Read Only
                begin
                overflowReset <= 1'b0;
                end
			3'b111: //Set Motor direction
				begin
				directionReg <= bus_write_data;
				end
        endcase
    end
	else if(bus_read_en) begin : READ
        case(bus_addr[4:2])
	        3'b000: // Timer Overflow register
                begin 
		        bus_read_data       <= overflowReg;
                reset_interrupt     <= 1'b0;
                reset_capture_sync  <= 1'b0;
                reset_capture_async <= 1'b0;
				end
            3'b001: // Timer Value, Read Only
                begin 
                bus_read_data       <= counterReg;
                reset_interrupt     <= 1'b0;
                reset_capture_sync  <= 1'b0;
                reset_capture_async <= 1'b0;
				end
            3'b010: // Timer Control
                begin 
                bus_read_data       <= controlReg;
                reset_interrupt     <= 1'b0;
                reset_capture_sync  <= 1'b0;
                reset_capture_async <= 1'b0;
			    end
            3'b011: //Compare Register
                begin
                bus_read_data       <= compareReg;
                reset_interrupt     <= 1'b0;
                reset_capture_sync  <= 1'b0;
                reset_capture_async <= 1'b0;
                end
            3'b100: // Interrupt/Capture Status
                begin 
                bus_read_data[31:4] <= 28'd0;;
                bus_read_data[1:0]  <= interrupt_status;
                bus_read_data[2]    <= capture_status_sync;
                bus_read_data[3]    <= capture_status_async;
                reset_interrupt     <= 1'b1;
                reset_capture_sync  <= 1'b0;
                reset_capture_async <= 1'b0;
                end
            3'b101: //CaptureSync
                begin
                bus_read_data       <= captureSyncReg;
                reset_interrupt     <= 1'b0;
                reset_capture_sync  <= 1'b1;
                reset_capture_async <= 1'b0;
                end
            3'b110: //CaptureAsync
                begin
                bus_read_data       <= captureAsyncReg;
                reset_interrupt     <= 1'b0;
                reset_capture_sync  <= 1'b0;
                reset_capture_async <= 1'b1;
                end
			3'b111: //Get Motor direction
				begin
				bus_read_data <= directionReg;
				end
        endcase
     end
     else begin
        overflowReset       <= 1'b0;
        reset_interrupt     <= 1'b0;
        reset_capture_sync  <= 1'b0;
        reset_capture_async <= 1'b0;
     end
end

always@*
	nextCounter = counterReg + 1;
  
always@(posedge pclk)
if(~nreset) begin
	counterReg          <= 32'h00000000;
    timer_interrupt     <= 1'b0;
    interrupt_status    <= 2'b00;
    pwm1                <= 1'b0;
end
else begin
    if(reset_interrupt)begin
        interrupt_status <= 2'b00;
        timer_interrupt <= 1'b0;
    end
    else begin
        if(overflowReset) begin
            counterReg <= 32'd0;
            timer_interrupt <= 1'b0;
        end
        else if(timerEn) begin
            if(counterReg == overflowReg) begin
                counterReg <= 32'd0;
                pwm1 <= 1'b0;
                if(interruptEn && overflowEn) begin
                    timer_interrupt <= 1'b1;
                    interrupt_status[0] <= 1'b1;
                end
                else
                    timer_interrupt <= 1'b0;
            end
            else begin
                if(counterReg == compareReg && interruptEn && compareEn) begin
                    timer_interrupt <= 1'b1;
                    interrupt_status[1] <= 1'b1;
                end
                else
                    timer_interrupt <= 1'b0;
                if(counterReg == compareReg && pwmEn)
                    pwm1 <= 1'b1;

                counterReg <= nextCounter;
            end
        end

    end
end

//Sync Switch Press
always@(posedge pclk or negedge switch)
if(~switch) begin
    switch_syncer[0] <= 1'b0;
    switch_syncer[1] <= 1'b0;
    switch_syncer[2] <= 1'b0;
end
else begin
    switch_syncer[0] <= 1'b1;
    switch_syncer[1] <= switch_syncer[0];
    switch_syncer[2] <= switch_syncer[1];
end

//captureSync Logic
always@(posedge pclk or posedge reset_capture_sync)
if(~nreset || reset_capture_sync) begin
    capture_interrupt  <= 1'b0;
    capture_status_sync     <= 1'b0;
    captureSyncReg          <= 32'h00000000;
end
else begin
    if(captureEn && switch_syncer[1] == 1'b1 && switch_syncer[2] == 1'b0) begin
        capture_interrupt <= 1'b1;
        capture_status_sync <= 1'b1;
        captureSyncReg <= counterReg;
    end
    else
        capture_interrupt <= 1'b0;
end

//captureAsycn Logic
always@(posedge switch or negedge nreset or posedge reset_capture_async)
if(~nreset || reset_capture_async) begin
    capture_status_async <= 1'b0;
    captureAsyncReg <= 32'h00000000;
end
else if(captureEn && switch && ~capture_status_async) begin
        capture_status_async <= 1'b1;
        captureAsyncReg <= counterReg;
end

always@* 
    pwm2 <= pwm1;


endmodule