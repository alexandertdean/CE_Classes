module data2mux (
	input wire [31:0] a,
	input wire [31:0] b,
	input wire switch,
	
	output reg [31:0] out);
	
	always begin
		if (switch) out = b;
		else out = a;
	end
endmodule
