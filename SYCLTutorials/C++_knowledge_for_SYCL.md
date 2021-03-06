# C++ features every SYCL developer should know: Functor and Lambda Expression

In software development, sometimes you want to pass in a function as a parameter to another function like
defining your own comparison function for sorting algorithm. C uses function pointers to solve this but it suffers from several limitations. C++ (in C++11) improved it by adding **function objects (aka functors)** and **Lambda expression**. They are used in
SYCL to perform daily tasks such as creating a kernel. Every SYCL developer should know it before getting started.

## Functor
**Essentially, functor is an instance of a class/struct which overloads operator().**  This way, the object(instance)
can be used like a function call. Check the following example where we define a class MyFunctor and create an object my_functor
as a functor.

```C++
#include <iostream>

class myFunctor
{
    public:
        myFunctor () {}
        int operator() (int x, int y) { return x + y; }
};

int main(){
    myFunctor my_functor;
    std::cout << my_functor(1,2);
    return 0;
}
```
In the `main` function, the usage of my_functor looks just like a function which is basically how operator() works. In this example,
it seems unnecessary to use functor since it's such a naive function call. One use case of functor is to add state to
the class to make each functor behaves differently:

```C++
#include <iostream>

class myFunctor
{   
    private:
        int base = 0;
    public:
        myFunctor (int x) : base(x) {}
        int operator() (int x, int y) { return x + y + base; }
};

int main(){
    myFunctor my_functor_1(2);
    myFunctor my_functor_2(10);
    std::cout << my_functor_1(1,2) << std::endl;
    std::cout << my_functor_2(1,2) << std::endl;
    return 0;
}
```
In this example, we add an attribute base to myFunctor. Each functor created based on the class is different due to
their internal state, in this case, the value of `base`.

A more complicated and real-world example is to use functor when we want to customize a comparison function
in std::sort. In the following example, we need a customized function to compare 2 Student struct according to
attribute age. We define and pass in a functor `comparitor` to std::sort as it requires. 

```C++
#include <vector>
#include <iostream>
#include <algorithm>

struct Student {
  int age;
  Student(int x): age(x) {}
};

struct MyCompare { 
   bool operator()(const Student &a, const Student &b) {
       return a.age < b.age;
   }
};

int main(){
  std::vector<Student> vecStudent{{1}, {3}, {2}};
  MyCompare comparitor;
  std::sort(vecStudent.begin(), vecStudent.end(), comparitor);
  std::cout << vecStudent[1].age;
  return 0;
}
```
We can construct functor when pass in function without declare it:
```C++
  // Simplify Code:
  // MyCompare comparitor;
  // std::sort(vecStudent.begin(), vecStudent.end(), comparitor);
  // To:
  std::sort(vecStudent.begin(), vecStudent.end(), MyCompare{});
```

However, to create a new class and overload its operator doesn't seem like an elegant way to create a temporary  function. 
That's why the lambda expression is proposed.

## lambda Expression
Lambda expression is a syntactic short-cut for functor. **The return value of a lambda expression is a functor**.
Let's revisit the sorting code above. Most of the code we add including definition of the class, declaration of overload
operator() function and instance creating are boilerplate code. With lambda expression, we can simplify the code a lot:
```C++
// using lambda expression
   auto comparitor = [](const Student &a, const Student &b) { return a.x < b.y; };
   std::sort(vecStudent.begin(), vecStudent.end(), comparitor);
```
or 
```C++
    std::sort(vecStudent.begin(), vecStudent.end(), [](const Student &a, const Student &b) { return a.x < b.y; });
```

The basic syntax for lambda expression is:
```
[ captures ] (parameters) -> returnTypesDeclaration { lambdaStatements; }
```
**\[capture\]**: The capture clause, also known as the lambda introducer, specifies which outside variables are available
for the lambda function and whether they should be captured by value (copying) or by reference. You will always start a lambda expression with the presence of the capture clause. 

**parameters**: This is the optional parameters list, also known as the lambda declarator. You can omit the parameters
list if you want a function that takes zero arguments.

**returnTypesDeclaration**: This is the return type. Most of the time, compilers can deduce the return type of the lambda
expression when you have zero or one return statement. 

**{ lambdaStatements; }**: This is the lambda body. The statements within the lambda body can access the captured variables
and the parameters.

Let's look at some examples:

* Example 1 : Compile deduce return type
```C++
   auto comparitor = [](const Student &a, const Student &b) { return a.x < b.x; };
```
One detail we notice is the type of a lambda expression is `auto`. In fact it's a compile-time generated type, as developer we
always put `auto` when declare it. Here the lambda expression captures nothing and takes 2 parameters. The return type is
automatically deduced by the compiler based on expressions inside the lambda body. If we want to be more explicit on return type, 
we can write it as:
```C++
    auto comparitor = [](const Student &a, const Student &b) -> bool { return a.x < b.x; };
```

The capture clause here is `[]` which means it does not capture any variable.

* Example 2 : basic capture example 
Following example showed basic syntax of capture. 
```C++
    int x = 1;
    int y = 2;
    int z = 3;
    auto func1 = [&](){};    // (1), default capture by reference
    auto func2 = [=](){};    // (2), default capture by copy
    auto func3 = [&,x](){};  // (3), default capture by reference, except x by copy
    auto func4 = [=,&x](){}; // (4), default capture by copy, excep x by reference
```
As we can see, we can specify the default capture mode using & (by reference) or =(by value) and specific capture for certain
variables. In example (3). y and z are captured by reference and x is by value. In example (4), y and z are captured by value and x
is by reference.  
The mechanism of capture clause simplifies the scenario where we want to pass an argument that is in current scope.

* Example 3: Use lambda expression in STL library
We already see how lambda can be used in `std::sort`. In the C++ STL library, a lot of utility functions provide an API for functors, In
the following example, we will use `std::count_if` to count the number of elements in the array that is larger than `x`.
```C++
#include <vector>
#include <iostream>
#include <algorithm>

int count_larger_x(std::vector<int>& vec, int x) {
    return std::count_if(vec.begin(), vec.end(), [=](int v){return v > x;});
}

int main(){
  std::vector<int> vec{1,2,3,4,5,6,7,8};
  std::cout << count_larger_x(vec, 4);
  return 0;
}
```
As you can see, lambda expression makes the code shorter and more elegant especially when the functor is only used once.
In comparison, a class defined functor is more useful when it aims to be reused and configured multiple times.

## Summary
We learn the basics of functor and lambda expression in C++:  
1. Functor is an instance of class which overloads operator(). It can be used like a function.   
2. Lambda expression returns a functor. It removes boilerplate code when we just want to call a function once.   
3. Compared to normal functions, lambda expression has a capture clause, which provides a flexible way to interact
with variables in the scope.  
