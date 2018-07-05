`timescale 1ns / 1ps

module testbench_fir_pipe_1(
    );

    reg clk, reset_p;
    reg [15:0]x;
    reg [1:0]idx;
    wire [15:0]y;
    
    initial begin
        clk = 0;
        reset_p = 0;
        
        #50
        reset_p = 1;
        
        #100
        reset_p = 0;
    end
    
    initial begin
        forever begin
            #10 clk = ~clk; 
        end
    end
    
    always@( posedge clk or posedge reset_p)begin
        if( reset_p == 1'b1)begin
            x <= 16'b0;
            idx <= 2'b0;
        end else begin
            idx <= idx + 1;
            case( idx)
            2'b00:  x <= 16'd100;
            2'b01:  x <= 16'd200;
            2'b10:  x <= 16'd300;
            default: x <= 16'd400;                    
            endcase
        end
    end
    
    fir_pipe_1 fir_pipe_inst_1(
        .clk( clk),
        .reset_p( reset_p),
        .x( x),
        .y( y)
        );    
        
endmodule
