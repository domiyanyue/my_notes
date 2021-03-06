# Move Semantics and Rvalue References in C++

Move semantics and rvalue reference are two advanced and confusing features added in C++11. In this article, I will explain what they are and why they are needed. Let's start with the main problem they are trying to tackle (yes, together).

## Problem: Unnecessary Copy of Objects

C++'s major advantage compared to other programming languages is *it's fast*. However, there had been one problem to slow down C++ programs before C++11: unnecessary copy of objects.
Take a look at the following example, which create and return an integer array:

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
  vector<int> vec_a(createArray(5));
}
```

Before C++11, the above code will perform terribly due to the copying of array `ret`. There can be up to 2 copies:
1. One is generated when the return value is created in function `createArray` (C++ standard requires creating a temporary object to hold a function's return value).
2. The second is when we pass the return value to `vec_a` in main function to `std::vector`'s constructor. 

While first copy can be avoided if the compiler applies "return value optimization" (and most compiler do), the second is unavoidable because a copy constructor of `std::vector` is called which will allocate memory space for vec_a and copy values from temporary values returned by function `createArray`. 

To fix the problem of the second copy, we need to code in a way that compiler won't create temporary objects and copy from it. There are several ways C++ programmers often uses like returning a pointer to vector or pass in the return value as a reference. Both can save performance issues but are not ideas. Using pointers requires developers to explicitly manage memory which makes programs prone to memory leaks. Using references requires the user to put return value in the argument list, which is not a natural form of programming and can hurt the code's readability.

Move semantic is introduced to address this issue - avoid copying when assigning a temporary value that is about to disappear. Before we explain move semantic, we have to introduce some concepts to help you understand - lvalue, rvalues, reference, and rvalue references.

## Lvalues and Rvalues

Lvalue and rvalue are **value categories**. In C++, each expression has 2 independent properties: value type and value category. Value category defines some basic rules compiler must follow when creating, assigning, copying objects when evaluating the expression. The actual definition of lvalue and rvalue are shockingly complicated. In this article, I will give you a simplified view that is enough to understand most cases. 

- An **lvalue** represents an object that occupies some identifiable location in memory.
- An **rvalue** is defined by exclusion, by saying an expression is an rvalue if it's not an lvalue.

Here are some basic examples:

```C++
int x;
x = 5;
```

Above is a legal assignment expression in C++. `x` is an lvalue because it has an identifiable location (using name `x`). An assignment operator `=` expects an lvalue as its left operand, here we store value `4` to the memory location `x` located. Following are invalid expressions:

```C++
3 = 5;
(a * 2) = 3;
```

It's almost too obvious that the above code makes no sense. Why? The expression on the left sides `3` and `(a * 2)` don't have identifiable memory locations (which by definition makes them rvalues). Therefore they are illegal statements. Rvalues are temporary results of expressions, 'temporary' means you can't locate(or use) them after this statement. If you compile them, you will get errors: 

```
main.cpp:12:9: error: lvalue required as left operand of assignment
     3 = 5;
         ^
main.cpp:13:15: error: lvalue required as left operand of assignment
     (a * 2) = 3;
               ^
```

Make sense, right? As we said, the assignment operator requires lvalue as the left operand.

### Lvalue to Rvalue Conversion
In the following example, `+` takes two rvalues as arguments and returns an rvalue.

```C++
int a = 1;
int b = 2;
int c = a + b;
```

We know both `a` and `b` are lvalues. In the third line, they undergo an implicit *lvalue-to-rvalue* conversion. All lvalues that aren't arrays, functions or of incomplete types can be converted to rvalues. However, rvalues can't be converted to lvalues.  

### References
A reference ("lvalue reference" since C++11) is a type of C++ variable that can act as an alias to another value. An lvalue reference is created using a single ampersand. In the next example, `refv` is a reference of type int and is an alias to `v`. Changing the value of `refv` is equivalent to modifying `v`.

```C++
int v = 3;
int& refv = v;
refv = 5;
```

It's worth pointing out that *reference type in C++ are lvalues* and can appear on the left side of the assignment operator.
```C++
int a = 1;
int& b = a; // b is a reference type and is an alias of a 
b = 2;
```

## Rvalue References 
Prior to C++11, only one type of reference exits in C++: reference or lvalue reference (name post C++11). Reference type gives us an easy way to refer to the object without copying it. In the first example, we can pass in the return value as a reference type like:

```C++
#include <vector>
using namespace std;

void createArray(int n, vector<int>& vec){
  for(int i = 0; i < n; i++){
    ret.push_back(i * i);
  }
}

int main(){
  vector<int> vec_a;
  createArray(5,vec_a);
}
```

Because of lvalue references, we can efficiently use lvalue by generating an alias without copying. But what about rvalues? They can only be assigned to non-modifiable lvalue references. For example:

```C++
int& a = 4; // Error, can not assign rvalue to a lvalue reference type
const int& a = 4; // Correct, can assign rvalue to a non-modifiable lvalue reference type
```

Rvalue reference is introduced to address this issue. It can only be assigned from an rvalue. An rvalue reference is created using a double ampersand.

```C++
int x = 5;
int &lref = x; // lvalue reference initilized with l-value x
int &&rref = 5; // rvalue reference initilized with rvalue 5
```

*Please notice rvalue reference type is an lvalue as we can see it can appear on the left side of assignment operator*. 

Rvalue reference can be used just like lvalue reference. It expands the lifetime of a temporary variable and also makes it possible to be modified:

```C++
int &&rref = 5;
std::cout << rref << endl;
rref = 6;
std::cout << rref << endl;
```

During the initialization of `rref`, a temporary object is created from literal `5`, the rvalue reference `rref` is referencing the temporary object (not literal). In the next assignment, we change the value of the temporary object through rvalue reference. 

However, these are not the common use of manners of rvalue reference. Rvalue references are more often used as function parameters to create *move constructor* and *move assignment operator*. 

## Move Semantics
In the first example, we identified the unnecessary copy problem. With rvalue reference, we can use move semantics to solve this.
Essentially, we want to pass the parameter(return value of the `createArray`) to `std::vector`'s constructor without copying it. Class constructor is a special case of function call. Let's first look at the general case where rvalue reference is used as a normal function parameter. 

### Rvalue Reference as Function Parameter
In the following example, we declare a function that *takes an argument by rvalue reference*.

```C++
void func(vector<int>&& vec){
    for(int i = 0; i < vec.size(); i++){
        vec[i] = 1;
    }
    cout << "\n";
}

```

This is an optimization where the parameter `vec` is not copied. `vec` is just an alias of some other rvalue variable. Another way to describe is the parameter's ownership is transferred through calling `func`. The value of argument `vec` after calling this function is unspecified because the ownership has been transferred. Let's see what happens if we call this function:

```C++
    func(vector<int>{1,2,3}); // works, the parameter is a temporary rvalue
    vector<int> v{4,5,6}; 
    func(v); // error: can't bind an lvalue to rvalue reference type
```

This first line is legal since we pass in rvalue created on the fly. The third line is illegal as compiler complained:

```
 error: cannot bind ‘std::vector’ lvalue to ‘std::vector&&’
     func(v);
           ^
```

The compiler refused to cast the lvalue to rvalue implicitly. Why? Because passing by rvalue reference indicating the value is not copied but moved and the value after function call is undefined. By making it an error can help programmers avoid making serious mistakes (treating moving as copying). What if we *do want to* change the ownership of the parameter and don't care about the value after function call? C++ provides `std::move` to cast an object to rvalue-reference, enabling moving from it. The following code is legal: 

```
    vector<int> v{4,5,6}; 
    func(std::move(v)); 
```

Now we can bind both an rvalue or lvalue (using explicit cast `std::move`) to an rvalue reference parameter. We will see next how to write a move constructor.

### Move Constructor
As we mentioned in the beginning, rvalue references and move semantics are proposed to solve this problem: reduce unnecessary copying. In that specific case, we need to create a constructor that performs move instead of copy. Imagine we have a class that encapsulates a pointer style array, before C++11, we would write it as:

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

It includes a default constructor and copy constructor (for simplicity, I didn't include copy assignment operator). Notice the copy constructor performs a lot of work: allocating memory and copying elements. We can add a move constructor to improve efficiency when copying is not necessary:

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

The move constructor is much cheaper than the copy constructor! It simply steals (takes the ownership of) the pointer from the object it moved from. Notice for object members (`string m_name`), we pass in `std::move(array.m_name)` to trigger the move constructor for `std::string` instead of copy constructor. `array.name` is lvalue (rvalue reference is lvalue) and needs to be cast explicitly to utilize move constructor as we emphasized. Now if you write code like:

```C++

MyArray createNewArray(){
   MyArray ret;
   ...
   return ret;
}

MyArray array(createNewArray()); // or MyArray array = createNewArray() if assignment operator is also implemented
```

The move constructor (or move assignment operator) will be called automatically since return value MyArray is a rvalue and can be cast to rvalue reference directly.

The good news here is move semantics (move constructor and move assignment operator) has been added to the standard library. This means you can efficiently deal with vector, map, string and other standard library objects when dealing with construction or value assignments. In the example mentioned at the beginning of this article, with C++11, the second copy of vector would be avoided because line `vector<int> vec_a = createArray(5);` triggered the move assignment operator instead of copy thanks to move semantics support in the standard library. Efficiency is improved without a single code change! 

## Summary
In this article, we took a long-short at understanding move semantics and rvalue reference in C++:

1. The problem to solve is how to **transfer the ownership of objects/pointers/variables efficiently without unnecessary copy**.
2. **Lvalue** and **rvalue** are **value categories** defining how compiler views assignment, copy, construction, and parameter passing. Lvalue is an object that has an identifiable name in memory. Rvalue is defined by exclusion, usually, it's a temporary object. 
3. **Rvalue references** let us declare a reference type to temporary objects (rvalues). We can use `std::move` to explicitly cast lvalue to rvalue reference. 
4. **Move semantics** are introduced when we pass in a parameter by rvalue reference where no creation of new objects (copying) happens.
5. We implemented a move constructor in a simple class.

## Reference
[1] Understanding lvalues and rvalues in C and C++
[2] Value Categories: Lvalues and Rvalues (C++)
[3] Rvalue References and Move Semantics in C++11
[4] C++ rvalue references and move semantics for beginners
