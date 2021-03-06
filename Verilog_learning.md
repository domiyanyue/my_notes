# Learning Verilog

Verilog is a hardware description language.

## Module Declaration

```
module Module1
    ...
    `include "module2.v"
    ...
endmodule
```

## Code Structure

### Declaration Statements

In Verilog design unit, a **module** statement does not need to be declared; nor do subprograms, that is, a **task** or **function**.

### Concurrent Statement

### Sequential Statements

## Data Type
hardware does have two kinds of drivers. 
A driver is a data type which can drive a load. Basically, in a physical circuit, a driver would be anything that electrons can move through/into.
- Driver that can store a value (example: flip-flop).
- Driver that can not store value, but connects two points (example: wire).
	
The first type of driver is called a **reg** in Verilog (short for "register"). The second data type is called a **wire**. 

## Control Statements
**if, else, repeat, while, for, case** 

### if else
```
// begin and end act like curly braces in C/C++.
if (enable == 1'b1) begin
  data = 10; // Decimal assigned
  address = 16'hDEAD; // Hexadecimal
  wr_enable = 1'b1; // Binary  
  end else begin
  data = 32'b0;
  wr_enable = 1'b0;
  address = address + 1;  
end
```
If-else and case statements require all the cases to be covered for combinational logic.

## Variable Assignment
- Combinational elements can be modeled using assign and always statements.
- Sequential elements can be modeled using only always statement.
- There is a third block, which is used in test benches only: it is called Initial statement.

### Initial Block
An initial block, as the name suggests, is executed only once when simulation starts. This is useful in writing test benches. If we have multiple initial blocks, then all of them are executed at the beginning of simulation.
```
initial begin
    clk = 0;
    reset = 0;
    req_0 = 0;
    req_1 = 0;
end
 ```
 
 ### Always Blocks
 An always block executes always. An always block should have a sensitive list or a delay associated with it.
 	
The sensitive list is the one which tells the always block when to execute the block of code, as shown in the figure below. The @ symbol after reserved word ' always', indicates that the block will be triggered "at" the condition in parenthesis after symbol @.

One important note about always block: it can not drive wire data type, but can drive reg and integer data types.

```
always  @ (a or b or sel)
begin
  y = 0;
  if (sel == 0) begin
  y = a;
  end else begin
  y = b;
  end
end
```

```
always  @ (posedge clk )
if (reset == 0) begin
  y <= 0;
  end else if (sel == 0) begin
  y <= a;
  end else begin
  y <= b;
end
```

```
always  begin
  #5  clk = ~clk;
end
```
#5 in front of the statement delays its execution by 5 time units.
 

