module negedgeDelay (
	input wire in,
	input wire clock,
	
	output reg out);
	
	always @(negedge clock) begin
		out <= in;
	end
endmodule
