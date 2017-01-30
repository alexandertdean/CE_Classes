module controller (
	input wire [31:0] instruction,
	input wire reset,
	input wire pause,
	input wire clock,
	
	output reg [5:0] op,
	output reg [4:0] rs,
	output reg [4:0] rt,
	output reg [4:0] rd,
	output reg [4:0] shamt,
	output reg [5:0] funct,
	output reg [4:0] aluFunc,
	output reg [15:0] immediate,
	output reg [25:0] address,
	output reg regWrite,
	output reg memRead,
	output reg memWrite);
	
	initial begin
		op = 0;
		rs = 0;
		rt = 0;
		rd = 0;
		shamt = 0;
		funct = 0;
		aluFunc = 0;
		regWrite = 0;
		memRead = 0;							
		memWrite = 0;
		immediate = 0;
		address = 0;
	end
	
	always @(negedge clock) begin
		if (reset) begin
			op = 0;
			rs = 0;
			rt = 0;
			rd = 0;
			shamt = 0;
			funct = 0;
			aluFunc = 0;
			regWrite = 0;
			memRead = 0;
			memWrite = 0;
			immediate = 0;
			address = 0;
		end
		else if (pause) begin
			op = 0;
			rs = rs;
			rt = rt;
			rd = 0;
			shamt = shamt;
			funct = funct;
			aluFunc = 0;
			regWrite = 0;
			memRead = 0;
			memWrite = 0;
		end
		else if (instruction == 0) begin
			aluFunc <= 0;
			regWrite <= 0;
			memWrite <= 0;
			memRead <= 0;
			rd <= 0;
			op <= 0;
		end
		else if (instruction[31:26] == 0) begin
			case (instruction[5:0])
		/* R-Type instructions */
				6'b100000 : 			//ADD
					begin
						aluFunc <= 1;
						regWrite <= 1;
						memWrite <= 0;
						rd <= instruction[15:11];
						memRead <= 0;
					end
				6'b100001	:			//ADDU
					begin
						aluFunc <= 2;
						regWrite <= 1;
						memWrite <= 0;
						rd <= instruction[15:11];
						memRead <= 0;
					end
				6'b100010	:			//SUB
					begin
						aluFunc <= 3;
						regWrite <= 1;
						memWrite <= 0;
						rd <= instruction[15:11];
						memRead <= 0;
					end
				6'b100011	:			//SUBU
					begin
						aluFunc <= 4;
						regWrite <= 1;
						memWrite <= 0;
						rd <= instruction[15:11];
						memRead <= 0;
					end
				6'b100100	:			//AND
					begin
						aluFunc <= 5;
						regWrite <= 1;
						memWrite <= 0;
						rd <= instruction[15:11];
						memRead <= 0;
					end
				6'b100101	:			//OR
					begin
						aluFunc <= 6;
						regWrite <= 1;
						memWrite <= 0;
						rd <= instruction[15:11];
						memRead <= 0;
					end
				6'b100111	:			//NOR
					begin
						aluFunc <= 7;
						regWrite <= 1;
						memWrite <= 0;
						rd <= instruction[15:11];
						memRead <= 0;
					end
				6'b101010	:			//SLT
					begin
						aluFunc <= 8;
						regWrite <= 1;
						memWrite <= 0;
						rd <= instruction[15:11];
						memRead <= 0;
					end
				6'b000000	:			//SLL
					begin
						aluFunc <= 9;
						regWrite <= 1;
						memWrite <= 0;
						rd <= instruction[15:11];
						memRead <= 0;
					end
				6'b000010	:			//SRL
					begin
						aluFunc <= 10;
						regWrite <= 1;
						memWrite <= 0;
						rd <= instruction[15:11];
						memRead <= 0;
					end
				6'b000011	:			//SRA
					begin
						aluFunc <= 11;
						regWrite <= 1;
						memWrite <= 0;
						rd <= instruction[15:11];
						memRead <= 0;
					end
				6'b001000	:			//JR
					begin
						aluFunc <= 12;
						regWrite <= 0;
						memWrite <= 0;
						rd <= instruction[15:11];
						memRead <= 0;
					end
				6'b011000	:			//MULT
					begin
						aluFunc <= 13;
						regWrite <= 0;
						memWrite <= 0;
						memRead <= 0;
					end
				6'b010010	:			//MFLO
					begin
						aluFunc <= 14;
						regWrite <= 1;
						memWrite <= 0;
						rd <= instruction[15:11];
						memRead <= 0;
					end	
			endcase
		end else case (instruction[31:26])
			/* I-Type instructions */
			6'b001100		:				//ANDI
				begin
					aluFunc <= 15;
					regWrite <= 1;
					memWrite <= 0;
					rd <= instruction[20:16];
					memRead <= 0;
				end
			6'b001101		:				//ORI
				begin 
					aluFunc <= 16;
					regWrite <= 1;
					memWrite <= 0;
					rd <= instruction[20:16];
					memRead <= 0;
				end
			6'b001010		:			 	//SLTI
				begin
					aluFunc <= 17;
					regWrite <= 1;
					memWrite <= 0;
					rd <= instruction[20:16];
					memRead <= 0;
				end
			6'b001000		:				//ADDI
				begin
					aluFunc <= 18;
					regWrite <= 1;
					memWrite <= 0;
					rd <= instruction[20:16];
					memRead <= 0;
				end
			6'b001001		:				//ADDIU
				begin
					aluFunc <= 19;
					regWrite <= 1;
					memWrite <= 0;
					rd <= instruction[20:16];
					memRead <= 0;
				end
			6'b000100		:				//BEQ
				begin
					aluFunc <= 20;
					regWrite <= 0;
					memWrite <= 0;
					memRead <= 0;
				end
			6'b000101		:				//BNE
				begin
					aluFunc <= 21;
					regWrite <= 0;
					memWrite <= 0;
					memRead <= 0;
				end
			6'b000111		:				//BGTZ
				begin
					aluFunc <= 22;
					regWrite <= 0;
					memWrite <= 0;
					memRead <= 0;
				end
			6'b000001		:				//BGEZ
				begin
					aluFunc <= 23;
					regWrite <= 0;
					memWrite <= 0;
					memRead <= 0;
				end
			6'b100011		:				//LW
				begin
					aluFunc <= 24;
					regWrite <= 1;
					memWrite <= 0;
					rd <= instruction[20:16];
					memRead <= 1;
				end
			6'b101011		:				//SW
				begin
					aluFunc <= 25;
					regWrite <= 0;
					memWrite <= 1;
					memRead <= 0;
				end
			6'b001111		:				//LUI
				begin
					aluFunc <= 26;
					regWrite <= 1;
					memWrite <= 0;
					rd <= instruction[20:16];
					memRead <= 0;
				end
			/* J-Type instructions */
			6'b000010		:				//J
				begin
					aluFunc <= 27;
					regWrite <= 0;
					memWrite <= 0;
					memRead <= 0;
				end
			6'b000011		:				//JAL
				begin
					aluFunc <= 28;
					regWrite <= 1;
					memWrite <= 0;
					memRead <= 0;
					rd <= 31;
				end
		endcase		
	
	op <= instruction[31:26];
	rs <= instruction[25:21];
	rt <= instruction[20:16];
	shamt <= instruction[10:6];
	funct <= instruction[5:0];
	immediate <= instruction[15:0];
	address <= instruction[25:0];
	end
endmodule

						