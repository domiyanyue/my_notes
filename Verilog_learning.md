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