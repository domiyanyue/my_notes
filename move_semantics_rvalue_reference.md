# Move Semantics and Rvalue Reference in C++

Move semantic and rvalue references are two powerful yet confusing features added in C++11. In this article, I am trying to explain them to C++ programmers who are new to this concept. 

## Problem: Unnecessary Copy of Objects

One of C++'s advantages is it can produce fast programs. However, one problem has existed to slow down C++ programs before C++11:  unnecessary copying of objects.
Take a look at the following example:

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

Before C++11, the above code will generate terrible performance due to the copying of array `ret`. There can be up to 2 copies, one is generated when the return value is created in
function createArray, the second is when we assign the return value to `vec_a` in main function. The first copy can be avoided if the compiler applies "return value optimization".
The second is unavoidable because a copy constructor of vector is called which will allocate memory space for vec_a and copy values from temproary values returned by function createArray. 

The root problem is we don't want to create temprorary objects and copy from it. To avoid it, there are serveral ways C++ programmers can avoid it like returning a pointer to vector or pass in the return value as a reference. Both can save performance issue but are not natural forms of programming. 

Move semantic is introduced to address this, avoid copying when assigning a temprorary value that is about to disappear to another value. Before we introduce move semantic, we have to introduce some concepts to help you understand better - lvalue, rvalues and rvalue references.

## Lvalues and Rvalues

Lvalue and rvalue are **value categories**. In C++, each expression has 2 independent properties: value type and value category. Value category defines some basic rules compiler must follow when creating, assigning, copying objects when evaluating the expression. The actual definition of lvalue and rvalue are shockingly complicated. In this article, I will give you a simplified view that is enough to explain most cases. 

An **lvalue** represents an object that occupies some identifiable location in memory.
An **rvalue** is defined by exclusion, by saying an expression is an rvalue if it's not lvalue.
Let's look at some basic examples:
```C++
int a;
a = 4;
```
Above is a legal assignment experssion in C++. `a` is an lvalue because it has an identifiable location (using name `a`). An assignment operator expects an lvalue as its left operand, here we store value `4` to the memory location `a` locates. Following are invalid experssions:

```C++
3 = 5;
(a * 2) = 3;
```
It's almost too obvious that the above statments make no sense. Why? The experssion on the left sides `3` and `(a * 2)` don't have identifiable memory locations (which by definition makes them rvalues according to definition). Therefore they are illegal statements. In fact, rvalues are temprorary results of expressions, 'temprorary' means you can't locate them after this statement. 
In fact, if you put them in a compiler (GCC), you will get an error: 

```
main.cpp:12:9: error: lvalue required as left operand of assignment
     3 = 5;
         ^
main.cpp:13:15: error: lvalue required as left operand of assignment
     (a * 2) = 3;
               ^
```
Make sense, right? Because assignments operator requires lvalue as left operand.

### Reference Type is Lvalue
It's worth pointing out that *reference type in C++ are lvalues* and can appear on the left side of assignment operator.
```C++
int a = 1;
int& b = a; // b is a reference type and is a alias of a 
b = 2;
```
On typical example of assigning to reference type is the overloading of brackets operator `[]` in some classes like `std::map` to perform index accessing. 

```C++
std::map<int, string> lookup;
lookup[2] = "hello";
```
The overloaded `[]` operator for class `std::map` returns a reference type to `string` which is lvalue. This makes index accessing access possible with assignment operator.

### Lvalue to Rvalue Conversion
In the following example, `+` takes two rvalues as arguments and returns an rvalue.
```C++
int a = 1;
int b = 2;
int c = a + b;
```
We know both `a` and `b` are lvalues. In the third line, they undergo an implicit *lvalue-to-rvalue* conversion. All lvalues that aren't arrays, functions or of incomplete types can be convered to rvalues. However, rvalues can't be converted to lvalues. 

## Rvalue Reference 
Prior to C++11, only one type of reference exits in C++: reference or lvalue reference (name post C++11). Reference type give us an easy way to refer to the object without copying it. Like in the first example, we can pass in the return value as a reference type. Because of lvalue reference, we can deal with lvalue without copying. But what about rvalues? They can only be assigned to non-modifiable lvalue references. For example:

```C++
int& a = 4; // Error, can assign rvalue to a lvalue reference type
const int& a = 4; // Correct, can assign rvalue to a non-modifiable lvalue reference type
```

Rvalue reference is introduced to address this issue. It can only be assigned from a rvalue. An rvalue reference is created using a double ampersand while an lvalue reference is created using a sinlge ampersand.

```C++
int x = 5;
int &lref = x; // lvalue reference initilized with l-value x
int &&rref = 5; // rvalue reference initilized with rvalue 5
```

*Please notice rvalue reference type is an lvalue as we can see it can appear on the left side of assignment operator*. 

Rvalue reference can be used just like lvalue reference. It expands the life time of a temprorary variable and also make it possible to be modified:
```C++
int &&rref = 5;
std::cout << rref << endl;
rref = 6;
std::cout << rref << endl;
```
During initilization of rref, a temprorary object is created from literal `5`, the rvalue reference `rref` is referencing the temprorary object (not literal). In the next assignment, we change the value of temprorary object through rvalue reference. 

However, these are not the common use manners of rvalue reference. Rvalue references are more often used as function parameters to create *move constructor* and *move assignment operator*. 

## Move Semantics
In the fisrt example, we identified the unnecessary copy problem. With rvalue reference, we can use move semantics to solve this. Before we build a class that utilize move semantics, let's look at the more generic case where rvalue reference is used as function parameter. 

### Rvalue Reference as Function Parameter
```C++
void func(vector<int>&& vec){
    for(int i = 0; i < vec.size(); i++){
        vec[i] = 1;
    }
    cout << "\n";
}
```
Consider the above function definition. It *takes an argument by ralue reference*, this is optimization for the case where data are intended to be "stolen" from the parameter instead of copying from it. Another way to express this behaviour is the paramemter's ownership is transfered to `func`. It also implies that the value of `vec` after calling this function is unspecified because the ownership has been transfered. Let's see what happens if we call this function:
```C++
    func(vector<int>{1,2,3}); // works, the parameter is a temprory rvalue
    vector<int> v{4,5,6}; 
    func(v); // error: can't bind an lvalue to rvalue reference type
```
This first line is legal since we pass in rvalue created on the fly. The third line is illegal as compiler complained:
```
 error: cannot bind ‘std::vector’ lvalue to ‘std::vector&&’
     func(v);
           ^
```
The compiler refused to cast the lvalue to rvalue implictily. Why? Because passing by rvalue reference indicating the value is not copied but moved and the value after function call is undefined. By making it an error can help programmers making serious mistakes (treaing move as copy). What if we *do want to* change the ownership of the parameter and don't care about the value after function call? C++ provides `std::move` to cast an object to rvalue-reference, enabling moving from it. The following code is legal: 
```
    vector<int> v{4,5,6}; 
    func(std::move(v)); 
```
Now we can bind both an rvalue or lvalue (using `std::move`) to a rvalue reference parameter. We will see next how to write a move constructor besides a copy constructor.

### Move Constructor
As we said, the concept of rvalue reference and move semantics are proposed to solve this problem: creating an constructor that performs move instead of copy. Imagine we have a class that encapsulate an pointer style array, before C++11, we would write it as:
```C++
class MyArray{
public:
    MyArray(int n){}
    
    // copy constructor
    MyArray(const MyArray& array):
        m_vals(new int[array.m_size]),
        m_size(array.m_size),
        m_name(array.m_name) {
        for(int i = 0; i < m_size; i++){
            m_vals[i] = array.m_vals[i];
        }
    }
    
    ~MyArray(){
        delete [] m_vals;
    }

public:
    int* m_vals = nullptr;
    int m_size = 0;
    string m_name = "";
};
```
It includes a default constructor and copy constructor (the simplicity, I didn't include copy assignment operator). Notice the copy constructor performs a lot of work: memory allocation and copying elements. We can add a move constructor to improve the efficiency under certain cases:

```C++
class MyArray{
public:
    MyArray(int n){}
    
    // copy constructor
    MyArray(const MyArray& array):
        m_vals(new int[array.m_size]),
        m_size(array.m_size),
        m_name(array.m_name) {
        for(int i = 0; i < m_size; i++){
            m_vals[i] = array.m_vals[i];
        }
    }

    // move constructor
    MyArray(MyArray&& array):
        m_vals(array.m_vals),
        m_size(array.m_size),
        m_name(std::move(array.m_name)){
        array.m_vals = nullptr;
        array.m_size = 0;
    }
    
    ~MyArray(){
        delete [] m_vals;
    }

public:
    int* m_vals = nullptr;
    int m_size = 0;
    string m_name = "";
};
```

The move constructor is much cheaper than the copy constructor! It simply steals (takes the ownership of) the pointer from the objet it moved from. Notice for object members (`string m_name`), we pass in `std::move(array.m_name)` to trigger the move constructor for `std::string` instead of copy constructor. Remeber `array.name` is lvalue (rvalue reference is lvalue) and needs to be cast explicitly to utilize move constructor. 

Another good thing here is move semantics (move constructor and move assignment operator) has been added to standard library. This means you can efficiently deal with vector, map, string and other standard library objects when dealing with construction or value assignments. If we look back at the example mentioned in the beginning of this article. The second copy of vector can be avoided because line `vector<int> vec_a = createArray(5);` triggered the move assignment operator instead of copy thanks to move semantics support in standard library. 

## Summary
In this article, we took a long short at understanding move semantics in C++:

1. The general problem we want to solve is how to **transfer the ownership of obects/pointers/variables efficiently without unnecessary copy**.
2. We went through a brief introduction of the concept of value category (lvalue and rvalue) to help understand how compiler views assignment, copy, construction
and parameter passing. This also help us understand the missing part of the language - a reference type to rvalue. 
3. Rvalue reference was introduced to let us declare a reference type to a temprorary objects. We can use `std::move` to explicitly cast a lvaue to rvalue reference type. 
4. Move semantic is introduced when we pass in a parameter by rvalue reference. No copy happens when passing parameters. We also implimented move constructor in a simple customized class type. 

