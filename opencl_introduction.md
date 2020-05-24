# OpenCL Introduction

## OpenCL Introduction
OpenCL is a **low-level** language for **heterogeneous** computing. There are 2 key words here:
* **low level**: Programmer has the control (and have to) on details on things like a memory allocation, memory transfer compared to
high level languages where compiler/runtime can do it for you.
* **heterogeneous**: It supports multiple devices, today OpenCL is running on CPU/GPU/FPGAs.

The OpenCL language is based on C (C99), which gives C/C++ programmers have a small learning curve. 

## What is OpenCL good at
OpenCL is mostly used in **GPGPU computing** which are **computationaly intensive** and **data parallel** computing.
**GPUGPU computing**: It stands for general purpose GPU computing, even though OpenCL is a open standard and is not restricted to certain device
type, it's mostly used on GPUs, this article's examples will focus on GPU.
**Computationally intensity**: More math operations than memory operations proportionally.
**Data Paralllism**: Same independent operations on lots of data. Examples include applying the same filter on every small part of the picture. 

## Key OpenCL Concepts

### Global and Local Dimensions
* **global dimensions**: a 1D/2D/3D paralelelism defined for each kernel execution.
* **work-item**: a thread executed for every point in the global dimensions.

| Example  | Global Dimension                  | Work Items |
|----------|-----------------------------------|------------|
| 1k audio | 1024 (1D global dimension)        | 1024       |
| HD Video | 1920 x 1080 (2D global dimension) | 2M         |




