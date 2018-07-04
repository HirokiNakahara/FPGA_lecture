`timescale 1ns / 1ps

module testbench_fir_1(
    );
    
    reg clk, reset_p, act;
    reg [15:0]x;
    reg [2:0]idx;
    reg state;
    wire [15:0]y;
    wire ready;
    
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
            act <= 1'b0;
            x <= 16'b0;
            idx <= 3'b0;
            state <= 1'b0;
        end else begin
            if( state == 1'b0)begin
                if( ready == 1'b1 && idx < 3'b100)begin
                    state <= 1'b1;
                    act <= 1'b1;
                    idx <= idx + 1;
                    case( idx)
                    3'b000:  x <= 16'd100;
                    3'b001:  x <= 16'd200;
                    3'b010:  x <= 16'd300;
                    3'b011:  x <= 16'd400;
                    default: x <= 16'd0;                    
                    endcase
                end
            end else begin
                act <= 1'b0;
                if( ready == 1'b1)begin
                    state <= 1'b0;
                end
            end
        end
    end
    
    fir_1 fir_1_inst(
        .clk( clk),
        .reset_p( reset_p),
        .x( x),
        .act( act),
        .y( y),
        .ready( ready)
        );    
endmodule
