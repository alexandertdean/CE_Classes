module execute_memory (
	input wire [31:0] aluResult_execute,
	input wire [4:0] rd_execute,
	input wire regWrite_execute,
	input wire memRead_execute,
	input wire memWrite_execute,
	input wire [31:0] data_out_2_execute,
	
	input wire pause,
	input wire clock,
	
	output reg [31:0] aluResult_memory,
	output reg [4:0] rd_memory,
	output reg regWrite_memory,
	output reg memRead_memory,
	output reg memWrite_memory,
	output reg [31:0] data_out_2_memory);
	
	always @(posedge clock) begin
		if (pause) begin
			aluResult_memory <= 0;
			rd_memory <= 0;
			regWrite_memory <= regWrite_memory;
			memRead_memory <= 0;
			memWrite_memory <= 0;
			data_out_2_memory <= data_out_2_memory;
		end
		else begin
			aluResult_memory <= aluResult_execute;
			rd_memory <= rd_execute;
			regWrite_memory <= regWrite_execute;
			memRead_memory <= memRead_execute;
			memWrite_memory <= memWrite_execute;
			data_out_2_memory <= data_out_2_execute;
		end
	end
endmodule

	