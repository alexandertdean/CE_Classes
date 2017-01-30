module instructionMemory(
	input clock,
	input [31:0] address,
	
	output reg [31:0] instruction);
	
	reg [31:0] instruction_memory [255:0];
	
	wire [31:0] memOut;

	initial begin
		$readmemh("instmem.mips", instruction_memory);
	end
	
	assign #1 memOut = instruction_memory[address / 4];
	
	always @(negedge clock) begin
		instruction <= instruction_memory[address / 4];
	end
endmodule
