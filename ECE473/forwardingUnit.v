module forwardingUnit (
	input wire [4:0] rs_execute,
	input wire [4:0] rt_execute,
	input wire [4:0] rd_memory,
	input wire [4:0] rd_writeBack,
	input wire regWrite_memory,
	input wire regWrite_writeBack,
	
	output reg [1:0] switch_rs,
	output reg [1:0] switch_rt);
	
	initial begin
		switch_rs = 0;
		switch_rt = 0;
	end
	
	always begin
		switch_rs = 0;
		switch_rt = 0;
		if (regWrite_writeBack) begin
			if (rd_writeBack == rs_execute) switch_rs = 2;
			if (rd_writeBack == rt_execute) switch_rt = 2;
		end
		if (regWrite_memory) begin
			if (rd_memory == rs_execute) switch_rs = 1;
			if (rd_memory == rt_execute) switch_rt = 1;
		end
	end
endmodule
