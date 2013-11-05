module pulser(
input sig,
input pclk,
output edges);

reg [5:0] syncer;

always@(posedge pclk)
begin
/*  syncer <= (syncer<<1) + sig; */

    syncer[0] <= sig;
    syncer[1] <= syncer[0];
    syncer[2] <= syncer[1];
    syncer[3] <= syncer[2];
    syncer[4] <= syncer[3];
    syncer[5] <= syncer[4];
end
/* create a pulse 3 clocks high on the rising and falling edges of sig */
wire [2:0] temp = (syncer[2:0] & ~syncer[5:3]) | (~syncer[2:0] & syncer[5:3]);
assign edges = temp[0];
endmodule