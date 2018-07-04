`timescale 1ns / 1ps

module fir_1(
    input clk,
    input reset_p,
    input [15:0]x,
    input act,
    output [15:0]y,
    output ready
    );
    
    parameter L1 = 3'b000, L2 = 3'b001, L3 = 3'b010, Endloop = 3'b011, L4 = 3'b100;
    
    reg [2:0]state;
    reg [4:0]i;
    reg [31:0]acc;
    reg [15:0]shift_reg[0:10];
    reg [15:0]y;
    reg ready;
    
    integer j;
    
    always@( posedge clk or posedge reset_p)begin
        if( reset_p == 1'b1)begin
            state <= 3'b0;
            i <= 5'b0;
            acc <= 32'b0;            
            y <= 16'b0;
            ready <= 1'b1;
            // Following statements are concluded by 'for' loop
            // shift_reg[0] <= 16'b0;
            // shift_reg[1] <= 16'b0;
            // ...
            // shift_reg[10] <= 16'b0;
            for(j = 0; j < 11; j = j + 1) begin
                shift_reg[j] <= 16'b0;
            end
        end else begin
            case( state)
            L1: begin
                if( act == 1'b1)begin
                    state <= L2;
                    ready <= 1'b0;
                end else begin
                    ready <= 1'b1;
                end
                acc <= 32'b0;
                i   <= 5'd10;
            end
            L2: begin
                if( i == 5'b11111)begin // i = -1 in signed decimal
                    state <= L4;
                end else begin
                    state <= L3;
                end
            end
            L3: begin
                if( i == 5'd0) begin
                    acc <= acc + (x * 16'b1111111101111000);
                    shift_reg[0] <= x;
                end else begin
                    case( i)
                        5'd10: acc <= acc + (shift_reg[10] * 16'b1111111101111000);//-4.120289718403869e-03
                        5'd9:  acc <= acc + (shift_reg[ 9] * 16'b1111111001110011);//-1.208600321298122e-02
                        5'd8:  acc <= acc + (shift_reg[ 8] * 16'b1111111110101001);//-2.650603053411641e-03
                        5'd7:  acc <= acc + (shift_reg[ 7] * 16'b0000101110111100);// 9.166631627169690e-02
                        5'd6:  acc <= acc + (shift_reg[ 6] * 16'b0010000010010010);// 2.544318483405623e-01
                        5'd5:  acc <= acc + (shift_reg[ 5] * 16'b0010101110000110);// 3.400000000000001e-01
                        5'd4:  acc <= acc + (shift_reg[ 4] * 16'b0010000010010010);// follwings are symmetric for previous weights
                        5'd3:  acc <= acc + (shift_reg[ 3] * 16'b0000101110111100);
                        5'd2:  acc <= acc + (shift_reg[ 2] * 16'b1111111110101001);
                        5'd1:  acc <= acc + (shift_reg[ 1] * 16'b1111111001110011);
                        5'd0:  acc <= acc + (shift_reg[ 0] * 16'b1111111101111000);                   
                        default: acc <= acc;
                    endcase

                    case( i)
                    5'd10: shift_reg[10] <= shift_reg[9];
                    5'd9:  shift_reg[ 9] <= shift_reg[8];
                    5'd8:  shift_reg[ 8] <= shift_reg[7];
                    5'd7:  shift_reg[ 7] <= shift_reg[6];
                    5'd6:  shift_reg[ 6] <= shift_reg[5];
                    5'd5:  shift_reg[ 5] <= shift_reg[4];
                    5'd4:  shift_reg[ 4] <= shift_reg[3];
                    5'd3:  shift_reg[ 3] <= shift_reg[2];
                    5'd2:  shift_reg[ 2] <= shift_reg[1];
                    5'd1:  shift_reg[ 1] <= shift_reg[0];
                    default:shift_reg[0] <= 16'b0;
                    endcase
                end
                state <= Endloop;
            end
            Endloop: begin
                i <= i - 1;
                state <= L2;
            end
            L4: begin
                state <= L1;
                y <= acc[31:16];
            end
            default: begin
                state <= 3'b000;
            end
            endcase
        end
