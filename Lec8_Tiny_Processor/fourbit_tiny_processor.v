module fourbit_tiny_processor(
    input clock,
    input reset_p,
    input [3:0]gpio_in,
    output [3:0]gpio_out
    );
    
    reg [2:0]state;
    reg [3:0]PC;
    reg [3:0]INST, IMM;
    reg [3:0]wr_en;
    reg jump_en;
    reg [1:0]sel_idx;
    reg [3:0]sel_val;
    reg [3:0]REGA, REGB;
    reg [3:0]add_result;
    reg CF;
    reg [3:0]gpio_out;

    wire [4:0]tmp_add_result;
    
    always@( posedge clock or posedge reset_p)begin
        if( reset_p == 1'b1)begin
            state <= 3'b000;
            PC <= 4'b0;
            INST <= 4'b0;
            IMM <= 4'b0;
            wr_en <= 4'b0;
            jump_en <= 1'b0;
            sel_idx <= 2'b0;
            sel_val <= 4'b0;
            REGA <= 4'b0;
            REGB <= 4'b0;
            add_result <= 4'b0;
            CF <= 1'b0;
            gpio_out <= 4'b0;
        end else begin
            case( state)
            3'b000:begin // read ROM
                state <= 3'b001;
                {INST, IMM} <= ROM( PC);                
            end
            3'b001:begin // decode inst
                state <= 3'b010;
                {wr_en, jump_en, sel_idx} <= INST_DEC( INST, CF);
            end
            3'b010:begin // select reg
                state <= 3'b011;
                sel_val <= sel_idx == 2'b00 ? REGA :
                                    sel_idx == 2'b01 ? REGB :
                                    sel_idx == 2'b10 ? gpio_in :
                                    4'b0000;
            end
            3'b011:begin // execute
                state <= 3'b100;
                add_result <= tmp_add_result[3:0];
                CF <= tmp_add_result[4];
            end
            3'b100:begin // write back regs
                state <= 3'b000;
                if( wr_en[0] == 1'b1) REGA <= add_result;
                if( wr_en[1] == 1'b1) REGB <= add_result;
                if( wr_en[2] == 1'b1) gpio_out <= add_result;
                if( jump_en == 1'b1)
                    PC <= add_result;
                else
                    PC <= PC + 1'b1;
            end
            default:begin
                state <= 3'b000;
            end
            endcase
        end
    end
        
    function [7:0]ROM( input [3:0]address);
    begin
        case( address)
        /* LED Blinking */
        4'b0000: ROM = 8'h81;
        4'b0001: ROM = 8'h82;
        4'b0010: ROM = 8'h84;
        4'b0011: ROM = 8'h88;
        4'b0100: ROM = 8'h84;
        4'b0101: ROM = 8'h82;
        4'b0110: ROM = 8'hA0;
        4'b0111: ROM = 8'h00;
        4'b1000: ROM = 8'h00;
        4'b1001: ROM = 8'h00;
        4'b1010: ROM = 8'h00;
        4'b1011: ROM = 8'h00;
        4'b1100: ROM = 8'h00;
        4'b1101: ROM = 8'h00;
        4'b1110: ROM = 8'h00;
        4'b1111: ROM = 8'h00;
        /* Timer for 3min 
        4'b0000: ROM = 8'h87; // OUT 0111
        4'b0001: ROM = 8'h41; // ADD A,0001
        4'b0010: ROM = 8'hB1; // JNC 0001
        4'b0011: ROM = 8'h41; // ADD A,001
        4'b0100: ROM = 8'hB3; // JNC 0011
        4'b0101: ROM = 8'h86; // OUT 0110
        4'b0110: ROM = 8'h41; // ADD A,0001
        4'b0111: ROM = 8'hB6; // JNC 0110
        4'b1000: ROM = 8'h41; // ADD A,0001
        4'b1001: ROM = 8'hB8; // JNC 1000
        4'b1010: ROM = 8'h80; // OUT 0000
        4'b1011: ROM = 8'h84; // OUT 0100
        4'b1100: ROM = 8'h41; // ADD 0001
        4'b1101: ROM = 8'hBA; // JNC 1010
        4'b1110: ROM = 8'h88; // OUT 1000
        4'b1111: ROM = 8'hAF; // JMP 1111
        */
        endcase
    end
    endfunction
    
    function [6:0]INST_DEC( input [3:0]inst, input CF);
    begin
        casex( {inst, CF})
        5'b0000_x: INST_DEC = 7'b1001_0_11;
        5'b0001_x: INST_DEC = 7'b1010_0_11;
        5'b0010_x: INST_DEC = 7'b1001_0_01;
        5'b0011_x: INST_DEC = 7'b1010_0_00;
        5'b0100_x: INST_DEC = 7'b1001_0_00;
        5'b0101_x: INST_DEC = 7'b1001_0_01;
        5'b0110_x: INST_DEC = 7'b1001_0_10;
        5'b0111_x: INST_DEC = 7'b1010_0_10;
        5'b1000_x: INST_DEC = 7'b1100_0_11;
        5'b1001_x: INST_DEC = 7'b1100_0_01;
        5'b1010_x: INST_DEC = 7'b1000_1_11;
        5'b1011_0: INST_DEC = 7'b1000_1_11;
        5'b1011_1: INST_DEC = 7'b1000_0_11;
        default:   INST_DEC = 7'b0000_0_00;
        endcase
    end
endmodule
