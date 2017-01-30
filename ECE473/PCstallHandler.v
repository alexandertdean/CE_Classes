module PCstallHandler (
	input wire [4:0] aluFunc,
	input wire [4:0] rd_decode,
	input wire [31:0] instruction_fetch,
	
	output reg pausePC);
	
	initial begin
		pausePC <= 0;
	end
	
	always begin
		pausePC <= 0;
		if (aluFunc == 24) begin
			if (rd_decode == instruction_fetch[25:21] || rd_decode == instruction_fetch[20:16]) begin
				pausePC <= 1;
			end
		end
	end
endmodule
