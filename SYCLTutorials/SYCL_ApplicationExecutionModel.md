# SYCL Application Execution Model

The execution of SYCL program include two parts: **SYCL kernel functions** and **SYCL application** (execute on host).
SYCL application that execute on the host is governed by the *SYCL Application Execution Model* which is what We focus on in this article. 

## What is SYCL Application Execution Model

As the host part of a heterogeous computing model, the following must be clearly defined in the execution model:
1. How hetergoneous devices are represented?  
2. How to specify a device where certain kernel will be running on? 
3. What's the order of execution?
We will address them one by one.

## Representaion of Hetergoneous devices
Three concepts are introduced for this problem:

**Platfrom**: A platform is a given hardware vendor's runtime and devices. 


**Context**


**Devices**




## Resources managed by SYCL Application


If you are familiar with OpenCL, this part shouldn't be new to you. 

* **Platforms**:

* **Contexts**:

* **Devices**:

* **Kernels**:

* **Program Objects**:

* **Command Queue**:


## SYCL Command Groups and Execution Order

## Summary
