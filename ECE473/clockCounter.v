module clockCounter(
	input wire clock,
	input wire reset,
	
	output reg [15:0] count);
	
	always @(posedge clock) begin
		if (reset) count = 0;
		else begin
			count = count + 1;
		end
	end
endmodule
