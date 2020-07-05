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
