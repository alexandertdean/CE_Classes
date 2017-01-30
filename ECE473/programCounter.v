module programCounter (
	input wire [31:0] PCoverride,
	input wire override,
	input wire reset,
	input wire pause,
	input wire clock,
	
	output reg [31:0] instruction_address);
	
	initial begin 
		instruction_address = 0;
	end
	
	always @(posedge clock) begin
		if (reset) begin
			instruction_address = -4;
		end
		else if (pause) begin
			instruction_address = instruction_address;
		end
		else if (override) begin
			instruction_address = PCoverride;
		end
		else begin
			instruction_address = instruction_address + 4;
		end
	end
endmodule
