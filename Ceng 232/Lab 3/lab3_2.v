`timescale 1ns / 1ps 
module lab3_2(
    		input [4:0] money,
			input CLK, 
			input [1:0] selectedArea, //00:exit, 01:enter, 1x: idle 
    		input plugAvailability,
			output reg [5:0] moneyLeft,
			output reg [5:0] seatLeft,
			output reg seatUnavailable,
    		output reg insufficientFund,
    		output reg notExactFund,
    		output reg invalidPlugSeat,
    		output reg plugSeatUnavailable,
    		output reg seatReady
	);
	 

	
	initial begin
	  insufficientFund = 0;
      notExactFund = 0;
      invalidPlugSeat = 0;
      plugSeatUnavailable = 0;
      seatReady = 0;
 	  seatUnavailable = 0;
      moneyLeft = 0;
      seatLeft = 0;
	end
  	  reg [4:0] loud_seat = 15;
  	  reg [4:0] quiet_noplug = 15;
  	  reg [4:0] quiet_plug = 10;
  	  reg [4:0] ind = 20;
  	  reg [4:0] loud_left = 15;
  	  reg [4:0]	quiet_noplug_left = 15;
  	  reg [4:0]	quiet_plug_left = 10;
  	  reg [4:0]	ind_left = 20;
    always @(posedge CLK) begin
      if (selectedArea == 2'b00) begin
        
        insufficientFund = 0;
        notExactFund = 0;
        invalidPlugSeat = 0;
        plugSeatUnavailable = 0;
        seatReady = 0;
 	    seatUnavailable = 0;
        moneyLeft = money;
        seatLeft = 0;
        
        if (loud_left != 0) begin
          
          if (plugAvailability == 1) begin
     			invalidPlugSeat = 1;
	        	moneyLeft = money;
      			seatLeft = loud_left;
          end
      	  else begin
            if(money >= 10) begin
              moneyLeft = money - 10;
      		  seatReady = 1;
      		  loud_left = loud_left - 1;
      		  seatLeft = loud_left;
            end
            else begin
              insufficientFund = 1;
      		  moneyLeft = money;
      		  seatLeft = loud_left;
            end
         end
       end
       else begin
            seatUnavailable = 1;
       	    moneyLeft = money;
      	    seatLeft = loud_left;
       end
     end
      if(selectedArea == 2'b01) begin
        insufficientFund = 0;
		notExactFund = 0;
		invalidPlugSeat = 0;
		plugSeatUnavailable = 0;
		seatReady = 0;
		seatUnavailable = 0;
		moneyLeft = money;
		seatLeft = 0;
        if(plugAvailability == 1) begin
          if(quiet_plug_left != 0) begin
            if(money >= 20) begin
              moneyLeft = money - 20;
      		  seatReady = 1;
      		  quiet_plug_left = quiet_plug_left - 1;
      		  seatLeft = quiet_plug_left;
            end
            else begin
              insufficientFund = 1;
      		  moneyLeft = money;
      		  seatLeft = quiet_plug_left;
            end
          end
          else begin
            seatUnavailable = 1;
      		moneyLeft = money;
      		seatLeft = quiet_plug_left;
          end
       end
       else begin
          if(quiet_noplug_left != 0) begin
            if(money >= 20) begin
      			moneyLeft = money - 20;
      		  	seatReady = 1;
      		  	quiet_noplug_left = quiet_noplug_left - 1;
      		  	seatLeft = quiet_noplug_left;
            end
            else begin
              	insufficientFund = 1;
      		  	moneyLeft = money;
      		  	seatLeft = quiet_noplug_left;
            end
          end
          else begin
            	seatUnavailable = 1;
      		  	moneyLeft = money;
      		  	seatLeft = quiet_noplug_left;
          end
      end
     end
     if(selectedArea == 2'b11) begin
       	insufficientFund = 0;
        notExactFund = 0;
        invalidPlugSeat = 0;
        plugSeatUnavailable = 0;
        seatReady = 0;
 	    seatUnavailable = 0;
        moneyLeft = money;
        seatLeft = 0;
        if(ind_left != 0) begin
          if(money == 30) begin
            	moneyLeft = 0;
      			seatReady = 1;
      			ind_left = ind_left - 1;
      			seatLeft = ind_left;
          end
          else begin
            	notExactFund = 1;
      			moneyLeft = money;
      			seatLeft = ind_left;
          end
        end
        else begin
         		seatUnavailable = 1;
      			moneyLeft = money;
      			seatLeft = ind_left;
        end 
    end
       
       
    end
endmodule
