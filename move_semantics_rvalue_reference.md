# Move Semantics and Rvalue Reference in C++

Move semantics and rvalue reference are two powerful yet confusing features added in C++11. In this article, I will explain them from a programmers perspective:
1. What problem it tries to address. 
2. 

## Problem: Unnecessary Copy of Objects

One of C++'s advantages is it can produce fast programs. However, one problem has existed to slow down C++ programs C++11: unnecessary copy of objects.
Take a look at following example:

```C++
#include <vector>
using namespace std;

vector<int> createArray(int n){
  vector<int> ret;
  for(int i = 0; i < n; i++){
    ret.push_back(i * i);
  }
  return ret;
}

int main(){
  vector<int> vec_a = createArray(5);
}
```

Before C++11, the above code will generate terrible performance due to copy of array ret. There can be up to 2 copies, one is generated when return value is created in
function createArray, the second is when we assign the return value to `vec_a` in main function. The first copy can be avoid if compiler applies "return value optimization".
The second is unavoidable because a copy constructor of vector is called which will allocate memory space for vec_a and copy values from temproary values returned by function createArray. 

The real problem is we don't want to create temprorary objects and copy from it. To avoid it, there are serveral ways C++ programmers can avoid it like returning a pointer to vector or pass in the return value as a reference. Both can save performance issue but are not natural forms of programming. 

Move semantic is introduced to address this, avoid copying when assigning a temprorary value that is about to disappear to another value. Before we introduce move semantic, we have to introduce some concepts to help you understand better - lvalue, rvalues and rvalue references.

## Lvalues and Rvalues

Lvalue and rvalue are **value categories**. In C++, each expression has 2 independent properties: value type and value category. Value category defines some basic rules compiler must follow when creating, assigning, copying objects when evaluating the expression. The actual definition of lvalue and rvalue are shockingly completely. In this article, I will give you a simplified view that is enough for most cases. 

An **lvalue** represents an object that occupies some identifiable location in memory.
An **rvalue** is defined by exclusion, by saying an expression is an rvalue if it's not lvalue.
Let look at some basic examples:
```C++
int a;
a = 4;
```
Above is a legal assignment experssion in C++. `a` is an lvalue because it has an identifiable location (using name `a`). An assignment operator expects an lvalue as its left operand, here we store value `4` to the memory location `a` locates. Following are invalid experssions:

```C++
3 = 5;
(a * 2) = 3;
```
It's almost too obvious that the above statments make no sense. Why? The experssion on the left sides `3` and `(a * 2)` don't have identifiable memory locations (which by definition makes them rvalues not lvalues). Therefore they are illegal statements. In fact, rvalues are temprorary results of expressions, 'temprorary' means you can't locate them after this statement. Yes, they evaperates.
In fact, if you put them in a compiler (GCC), you will get an error: 

```
main.cpp:12:9: error: lvalue required as left operand of assignment
     3 = 5;
         ^
main.cpp:13:15: error: lvalue required as left operand of assignment
     (a * 2) = 3;
               ^
```
Make sense, right? As we said, Assignments operator requires lvalues as left operand.


For the discussion of this article, we want to perticularly point out that reference type in C++ are lvalues and can appear on the left side of assignment operator.
```C++
int a = 1;
int& b = a; // b is a reference type and is a alias of a 
b = 2;
```

## Rvalue Reference 
Prior to C++11, only one type of reference exits in C++: reference or lvalue reference (post C++11). Reference type give us an easy way to refer to the object without copying it. Like in the first example, we can pass in the return value as a reference type. Because of lvalue reference, we can deal lvalue without copying. But what about rvalues? They can only be assigned to non-modifiable lvalue references. For example:

```C++
int& a = 4; // Error, can assign rvalue to a lvalue reference type
const int& a = 4; // Correct, can assign rvalue to a non-modifiable lvalue reference type
```

Rvalue reference is introduced to address this issue. It can only be assigned from a rvalue. 

```C++
// Example: Rvalue Reference
int main(){
  
}
```

However, these are not the common use case of rvalue reference, they are 


## Move Semantics




