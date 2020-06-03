# SYCL Device Memory Model

In this tutorial, we will talk about basic concepts first on device memory, including.... After that we will introduce more
concepts and their corresponding representation in SYCL.

## Global Dimension
A global dimension is a 1D/2D/3D parrellism defined for each kernel execution, it defines how the whole problem is paritioned and mapped to work-item. A work-item (thread) is execute in every point in the global dimension. 

Let's take a few example:

**Example 1.** Processing 1024 audio pieces, each work-item deal with 1 audio piece. In this case the global dimension is 1024 and number of work items is 1024. Each t

**Example 2.** Processing HD videos of size 1920 x 1080. Depends on the nature of problem and underlying hardware feature, you may come up with different ways to partition the problem. Let's consider 3 cases in which you will use different global dimensions. 
  1. Say you want to process each pixel with one work-item, in this case, the global dimension is a 2-dimension value of 1920 x 1080, and total work-item number is `2M ~= 1920 x 1080`.  
  
  2. If your compute task is performed per line, your compute task proces each line (1920 pixels) with one work-item. You only need 1080 work-items.
  
  3. If your tasks is filter-like and each work-item deal with a 4 x 4 block. The global dimension is 480 (1920 / 4) x 270 (1080 / 4) and number of work-items is `129,600 = 480 x 270`.

At this point, hope the concept is clear to you. Global dimension refects how the problem input is divided in a 1D/2D/3D way, a work-item deal with each unit of of the whole problem space. How to partition the problem is not a easy job which requires a lot of consideration. We will not cover that in this tutorial. 

Now we have dispatched the problem to each work-items and problem is solved, right? No yet. Turns out for more complicated problems, there is one important part we haven't consider, that is synchronization between work-items. In hardware design, the compute units which work-items works on can only be synchronized with compute units nearby with low cost. The reflect such hehaviour in software model, we need to introduce the concept of local dimension. 

## Local Dimension


