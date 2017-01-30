module registerFile (
	input wire [4:0] read_address_1,
	input wire [4:0] read_address_2,
	input wire [4:0] write_address,
	input wire [31:0] write_data_in,
	input wire WriteEnable,
	input wire reset,
	input wire pause,
	input wire [4:0] read_address_debug,
	input wire clock,
	input wire clock_debug,
	
	output reg [31:0] data_out_1,
	output reg [31:0] data_out_2,
	output reg [31:0] data_out_debug);
	
	reg [31:0] register[31:0];
	integer i;
	
	initial begin
		for (i = 0; i < 32; i = i + 1) begin	
			register[i] = 0;
		end
		register[29] = 128;
	end
	
	always @(posedge clock) begin
		if (reset) begin
			for (i = 0; i < 32; i = i + 1) begin
				register[i] = 0;
			end
			register[29] = 0;
		end
		else if (WriteEnable) begin
			register[write_address] = write_data_in;
		end
	end
	always @(negedge clock) begin
		if (pause) begin
			data_out_1 <= 0;
			data_out_2 <= 0;
		end
		else begin
			data_out_1 <= register[read_address_1];
			data_out_2 <= register[read_address_2];
		end
	end
	always @(posedge clock_debug) begin
		data_out_debug <= register[read_address_debug];
	end
endmodule

	