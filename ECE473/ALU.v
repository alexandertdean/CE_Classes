module ALU (
	input wire [4:0] aluFunc,
	input wire [31:0] rs,
	input wire [31:0] rt,
	input wire [4:0] shamt,
	input wire [15:0] immediate,
	input wire [31:0] PC,
	
	output reg [31:0] result);
	
	reg [31:0]mulResult;

	initial begin
		result <= 0;
		mulResult <= 0;
	end
	
	always begin
		case (aluFunc)
			0 : result = 0;					//NOP
			1 : result = rs + rt;			//ADD
			2 : result = rs + rt;			//ADDU
			3 : result = rs - rt;			//SUB
			4 : result = rs - rt;			//SUBU
			5 : result = rs & rt;			//AND
			6 : result = rs | rt;			//OR
			7 : result = ~(rs | rt);		//NOR
			8 : 						
				begin								//SLT
					if (rs[31] == rt[31]) result = (rs < rt);
					else if (rs[31] != rt[31]) result = rs[31];
				end
			9 : result = (rt << shamt);	//SLL
			10 : result = (rt >> shamt);	//SRL
			11 : result = rt >>> shamt;	//SRA
			13 : mulResult = rs * rt;		//MULT
			14 : result = mulResult;		//MFLO
			15	: result = rs & immediate;	//ANDI
			16	: result = rs | immediate;	//ORI
			17 : 
				begin							//SLTI
					if (rs[31] == immediate[15]) result = (rs < immediate);
					else if (rs[31] != immediate[15]) result = rs[31];
				end
			18 : result = rs + immediate;	//ADDI
			19	: result = rs + immediate;	//ADDIU
			20 : result = (rs == rt);		//BEQ
			21 : result = (rs != rt);		//BNE
			22 : //BGTZ
				begin
					if (rs & 32'h80000000) result = 0;
					else result = (rs > 0);
				end
						
			23 :		//BGEZ
				begin
					if (rs & 32'h80000000) result = 0;
					else result = (rs >= 0);
				end
			24 : result = rs + immediate;	//LW (offset for load address)
			25 : result = rs + immediate;	//SW (offset for store address)
			26 : result = immediate << 16;//LUI
			28 : result = PC + 4;
		endcase
	end
endmodule
