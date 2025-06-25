`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/24/2025 08:57:53 AM
// Design Name: 
// Module Name: Top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module top(
    input  wire [3:0] btn,   // BTN0-BTN3
    input  wire [1:0] sw,    // SW0-SW1
    output reg  [3:0] led    // LED0-LED3
);

// Simple combinational mapping:
//  - If SW0=1 (up), each button maps to the same-indexed LED.
//  - If SW0=0 (down), the mapping is reversed.
always @(*) begin
    if (sw[0]) begin
        led = btn;
    end else begin
        led[3] = btn[0];
        led[2] = btn[1];
        led[1] = btn[2];
        led[0] = btn[3];
    end
end

endmodule

