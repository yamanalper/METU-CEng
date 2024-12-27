`timescale 1ns / 1ps

module ac_flip_flop(
    input A,
    input C,
    input clk,
    output reg Q = 1
);

  always@(posedge clk) begin 
    case({A,C})
        2'b00: Q <= 1;   
        2'b01: Q <= ~Q; 
        2'b10: Q <= Q; 
        2'b11: Q <= 1; 
      endcase
    end  
endmodule

module ic1406(
    input A0,
    input A1,
    input A2,
    input clk,
    output Q0,
    output Q1,
    output Z
);
  xor(a,A0,A1);
  nor(b,a,~A2);
  and(c,A0,A1);
  ac_flip_flop gate1(b,c,clk,Q0);
  nor(k,~A0,~A1);
  and(l,~k,~A2);
  ac_flip_flop gate2(c,l,clk,Q1);
  xor(Z,Q0,Q1);
  
endmodule

