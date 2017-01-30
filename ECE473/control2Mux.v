module control2Mux (
	input wire a,
	input wire b,
	input wire switch,
	
	output reg out);
	
	always begin
		if (switch) out = b;
		else out = a;
	end
endmodule
