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

### Global Dimensions
* **global dimensions**: a 1D/2D/3D paralelelism defined for each kernel execution.
* **work-item**: a thread executed for every point in the global dimensions.

| Example            | Global Dimension | Work Items |
|--------------------|------------------|------------|
| 1k audio           | 1024 (1D)        | 1024       |
| HD Video           | 1920 x 1080 (2D) | 2M         |
| HD per line        | 1080 (1D)        | 1080       |
| HD per 8 x 8 block | 240 x 135 (2D)   | 32K        |

### Local Dimensions
The global dimensions are broken down evenly into **local work-groups**
If global dimensions is 100 x 512
Possible glocal dimensions are 10 x 8, 100 x 1, 50 x 2, 2 x 128
  
Each **work-group** is logically exectued together on one compute unit.  
Synchronization is only allowed between **work-items** in the same **work-group**.

## OpenCL Kernels
**Kernel** is the code that is executed in parallel. Think of it as the "inner loop" of the program.  
Here is a comparison of computation in C versus OpenCL:  
C:  

```C
void calSin(float* data) {
  for(int id = 0; id < 1024; id ++) {
    data[id] = sin(data[id]);
  }
}
```
OpenCL:  
```C
void kernel calSin(global float *data) {
  int id = get_global_id(0);
  data[id] = sin(data[id]);
}
```

### Kernel Lannguages
Basic C99 + 
* vectors
* rounding and conversions
* intrinsic functions (math)

#### Utility functions
* **get_global_id(dim)** : get current work-item's ID in a particular dimension
* **get_work_dim()**
* **get_global_size(dim)**
* **get_local_size(), get_local_id(), get_num_groups(), get_group_id()**

## OpenCL Runtime Architecture











