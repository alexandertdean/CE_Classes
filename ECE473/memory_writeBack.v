module memory_writeBack (
	input wire [31:0] aluResult_memory,
	input wire [4:0] rd_memory,
	input wire regWrite_memory,
	
	input wire pause,
	input wire clock,
	
	output reg [31:0] aluResult_writeBack,
	output reg [4:0] rd_writeBack,
	output reg regWrite_writeBack);
	
	initial begin
		regWrite_writeBack = 0;
	end
	
	always @(posedge clock) begin
		if (pause) begin
			aluResult_writeBack <= aluResult_writeBack;
			rd_writeBack <= rd_writeBack;
			regWrite_writeBack <= regWrite_writeBack;
		end
		else begin
			aluResult_writeBack <= aluResult_memory;
			rd_writeBack <= rd_memory;
			regWrite_writeBack <= regWrite_memory;
		end
	end
endmodule
