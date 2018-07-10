module topmodule_fourbit_tiny_processor(
    input sysclk,
    input [0:0]btn,
    output [3:0]led
    );

    reg [24:0]count; // 1sec ~ 8ns(125MHz) * 125,000,000
    
    always@( posedge sysclk or posedge btn[0])begin
        if( btn[0] == 1'b1) begin
            count <= 25'b0;
        end else begin
            count <= count + 1'b1;
        end    
    end
    
    fourbit_tiny_processor tiny_processor_inst(
        .clock( count[24]),
        .reset_p( btn[0]),
        .gpio_in( 4'b0000),
        .gpio_out( led)
    );    
    
endmodule
