module decode_execute (
	input wire [5:0] op_decode,
	input wire [4:0] rs_decode,
	input wire [4:0] rt_decode,
	input wire [4:0] rd_decode,
	input wire [4:0] shamt_decode,
	input wire [5:0] funct_decode,
	input wire [31:0] data_out_1_decode,
	input wire [31:0] data_out_2_decode,
	input wire [15:0] immediate_decode,
	input wire [25:0] address_decode,
	input wire [4:0] aluFunc_decode,
	input wire regWrite_decode,
	input wire memRead_decode,
	input wire memWrite_decode,
	
	input wire pause,
	input wire clock,
	input wire [31:0] PC_decode,
	input wire pausePC,
	
	output reg [5:0] op_execute,
	output reg [4:0] rs_execute,
	output reg [4:0] rt_execute,
	output reg [4:0] rd_execute,
	output reg [4:0] shamt_execute,
	output reg [5:0] funct_execute,
	output reg [31:0] data_out_1_execute,
	output reg [31:0] data_out_2_execute,
	output reg [15:0] immediate_execute,
	output reg [25:0] address_execute,
	output reg [4:0] aluFunc_execute,
	output reg regWrite_execute,
	output reg memRead_execute,
	output reg memWrite_execute,
	output reg [31:0] PC_execute);
	
	always @(posedge clock) begin
		if (pausePC) aluFunc_execute = 0;
		if (pause) begin
			op_execute <= 0;
			rs_execute <= 0;
			rt_execute <= 0;
			rd_execute <= 0;
			shamt_execute <= 0;
			funct_execute <= 0;
			data_out_1_execute <= 0;
			data_out_2_execute <= 0;
			immediate_execute <= 0;
			address_execute <= address_execute;
			aluFunc_execute <= 0;
			regWrite_execute <= 0;
			memRead_execute <= memRead_execute;
			memWrite_execute <= memWrite_execute;
			PC_execute <= PC_execute;
		end
		else begin
			op_execute <= op_decode;
			rs_execute <= rs_decode;
			rt_execute <= rt_decode;
			rd_execute <= rd_decode;
			shamt_execute <= shamt_decode;
			funct_execute <= funct_decode;
			data_out_1_execute <= data_out_1_decode;
			data_out_2_execute <= data_out_2_decode;
			immediate_execute <= immediate_decode;
			address_execute <= address_decode;
			aluFunc_execute <= aluFunc_decode;
			regWrite_execute <= regWrite_decode;
			memRead_execute <= memRead_decode;
			memWrite_execute <= memWrite_decode;
			PC_execute <= PC_decode;
		end
	end
endmodule
	
	
	