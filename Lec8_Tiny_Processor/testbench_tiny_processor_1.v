module testbench_tiny_processor_1(
    );
    reg clk, reset_p;
    wire [3:0]gpio_out;

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

    fourbit_tiny_processor processor_inst(
        .clock( clk),
        .reset_p( reset_p),
        .gpio_in(4'b0),
        .gpio_out(gpio_out)
        );
endmodule
