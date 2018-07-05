`timescale 1ns / 1ps

module fir_pipe_1(
    input clk,
    input reset_p,
    input [15:0]x,
    output [15:0]y
    );
    
    wire [15:0]w_x[0:9];
    wire [31:0]w_acc[0:9];
    wire [31:0]tmp_y;
    
    FIR_stage state_0_inst( .clk( clk), .reset_p( reset_p), .x_in( x), .acc_in( 32'b0), 
        .coef( 16'b1111111101111000), .x_out( w_x[0]), .acc_out( w_acc[0]));

    FIR_stage state_1_inst( .clk( clk), .reset_p( reset_p), .x_in( w_x[0]), .acc_in( w_acc[0]), 
        .coef( 16'b1111111001110011), .x_out( w_x[1]), .acc_out( w_acc[1]));

    FIR_stage state_2_inst( .clk( clk), .reset_p( reset_p), .x_in( w_x[1]), .acc_in( w_acc[1]), 
        .coef( 16'b1111111110101001), .x_out( w_x[2]), .acc_out( w_acc[2]));

    FIR_stage state_3_inst( .clk( clk), .reset_p( reset_p), .x_in( w_x[2]), .acc_in( w_acc[2]), 
        .coef( 16'b0000101110111100), .x_out( w_x[3]), .acc_out( w_acc[3]));

    FIR_stage state_4_inst( .clk( clk), .reset_p( reset_p), .x_in( w_x[3]), .acc_in( w_acc[3]), 
        .coef( 16'b0010000010010010), .x_out( w_x[4]), .acc_out( w_acc[4]));

    FIR_stage state_5_inst( .clk( clk), .reset_p( reset_p), .x_in( w_x[4]), .acc_in( w_acc[4]), 
        .coef( 16'b0010101110000110), .x_out( w_x[5]), .acc_out( w_acc[5]));

    FIR_stage state_6_inst( .clk( clk), .reset_p( reset_p), .x_in( w_x[5]), .acc_in( w_acc[5]), 
        .coef( 16'b0010000010010010), .x_out( w_x[6]), .acc_out( w_acc[6]));

    FIR_stage state_7_inst( .clk( clk), .reset_p( reset_p), .x_in( w_x[6]), .acc_in( w_acc[6]), 
        .coef( 16'b0000101110111100), .x_out( w_x[7]), .acc_out( w_acc[7]));

    FIR_stage state_8_inst( .clk( clk), .reset_p( reset_p), .x_in( w_x[7]), .acc_in( w_acc[7]), 
        .coef( 16'b1111111110101001), .x_out( w_x[8]), .acc_out( w_acc[8]));

    FIR_stage state_9_inst( .clk( clk), .reset_p( reset_p), .x_in( w_x[8]), .acc_in( w_acc[8]), 
        .coef( 16'b1111111001110011), .x_out( w_x[9]), .acc_out( w_acc[9]));

    FIR_stage state_10_inst( .clk( clk), .reset_p( reset_p), .x_in( w_x[9]), .acc_in( w_acc[9]), 
        .coef( 16'b1111111101111000), .x_out( ), .acc_out( tmp_y));
    
    assign y = tmp_y[31:16];
    
endmodule

module FIR_stage(
    input clk,
    input reset_p,
    input [15:0]x_in,
    input [31:0]acc_in,
    input [15:0]coef,
    output [15:0]x_out,
    output [31:0]acc_out
);

    reg [15:0]shift_reg;
    reg [15:0]pipe_reg_in;
    reg [31:0]pipe_reg_acc;

    always@( posedge clk or posedge reset_p)begin
        if( reset_p == 1'b1)begin
            shift_reg <= 16'b0;
            pipe_reg_in <= 16'b0;
            pipe_reg_acc <= 16'b0;
        end else begin
            pipe_reg_in <= x_in;
            shift_reg <= pipe_reg_in;
            pipe_reg_acc <= acc_in;
        end
    end
    
    assign x_out = shift_reg;
    assign acc_out = pipe_reg_acc + (coef * shift_reg);
endmodule
