module dataMemory(
	input wire [31:0] address,
	input wire [31:0] dataIn,
	input wire [4:0] debug,	
	input wire memRead,
	input wire memWrite,

	input wire clock,
	output reg [31:0] dataOut,
	output wire [31:0] deubg_out);
	
	reg [31:0] memory [128:0];
	
	initial begin
		$readmemh("dataMem.mips", memory);
	end
	
	assign debug_out = memory[debug];
	
	always @(address, memWrite) begin
		dataOut <= memory[address[15:2]];
	end
	
	always @(negedge clock) begin
		if (memWrite) begin
			memory[address[15:2]] <= dataIn;
		end
	end
endmodule
