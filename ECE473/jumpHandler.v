module jumpHandler (
	input wire [4:0] aluFunc,
	input wire [31:0] jrAddress,
	input wire [25:0] jtypeAddress,
	input wire [15:0] branchAddress,
	input wire branchConfirm,
	
	output reg [31:0] newPC,
	output reg overwritePC,
	output reg pause,
	output reg nextPause);
	
	
	initial begin
		overwritePC <= 0;
		pause <= 0;
	end
	
	always begin
		pause = 0;
		overwritePC <= 0;
		if (aluFunc == 12) begin	//JR
			newPC <= jrAddress;
			overwritePC <= 1;
			pause <= 1;
		end
		else if (aluFunc == 27 || aluFunc == 28) begin		//J + JAL
			newPC <= jtypeAddress;
			overwritePC <= 1;
			pause <= 1;
		end
		else if (aluFunc == 20 || aluFunc == 21 || aluFunc == 22 || aluFunc == 23) begin
			if (branchConfirm == 1) begin
				newPC <= branchAddress;
				overwritePC <= 1;
				pause <= 1;
			end
		end
	end

endmodule
