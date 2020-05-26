# C++ feature SYCL developper should know : Functor and Lambda 

SYCL is built on top of C++11. New features in C++11 such as functor and lambda expression are widely used in
SYCL to perform daily tasks such as creating a kernel. Every SYCL developer should know it before getting started.

## Functor
In software development, sometimes you want to pass in a function as a parameter to another function like
defining your own sorting algorithm. C has function pointer to solve this but it suffers from serveral limitations.
C++ improved in by adding **function object (aka functor)**.

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
In main function, the usage of my_functor looks just like a function which is how operator() works. In this example,
it seems unnecessary to use functor since it's such a simple function call. One use case of functor is to add state to
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
In this example, we add attribute base to myFunctor. In each object, the value of base is different, thus each represent
a independent functor. 

A more complicated and real word example to use functor is when we want to customize a comparison function
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
We can contruct functor when pass in function without declare it:
```C++
  // Simplify Code:
  // MyCompare comparitor;
  // std::sort(vecStudent.begin(), vecStudent.end(), comparitor);
  // To:
  std::sort(vecStudent.begin(), vecStudent.end(), MyCompare{});
```

In SYCL, we need to use functor to wrap around device code.

## lambda expression
Lambda expression is a syntactic short-cut for functor, you can replace functor with lambda
expression. With lambda expression, usually you can write less code. Think about the previous
student example, in order to do comparison, we added some boilerplate code. Using lambda expression
can simply:
```C++
# using functor
struct MyCompare { 
   bool operator()(const Student &a, const Student &b) {
       return a.x < b.x;
   }
};
...
  MyCompare comparitor;
  std::sort(vecStudent.begin(), vecStudent.end(), comparitor);
```
to:
```C++
# using lambda
   auto comparitor = [](const Student &a, const Student &b) { return a.x < b.y; };
   std::sort(vecStudent.begin(), vecStudent.end(), comparitor);
```
or:
```C++
    std::sort(vecStudent.begin(), vecStudent.end(), [](const Student &a, const Student &b) { return a.x < b.y; });
```
Now let's look into the details of lambda expression.
The basic syntax for lambda expression looks like:
```
[ captures ] (parameters) -> returnTypesDeclaration { lambdaStatements; }
```
*\[capture\]*: The capture clause, also known as the lambda introducer, specifies which outside variables are available
for the lambda function and whether they should be captured by value (copying) or by reference. You will always be able
to identify the start of a lambda expression with the presence of the capture clause. An empty capture clause [] means
capture nothing, in which case the lambda expression body doesn't access variables in the enclosing scope. 

*parameters*: This is the optional parameters list, also known as the lambda declarator. You can omit the parameters
list if you want a function that takes zero arguments.

*returnTypesDeclaration*: This is the return type. Most of the time, compilers can deduce the return type of the lambda
expression when you have zero or one return statement. However, if it makes it easier to understand the code, you can
specify the return type. 

*{ lambdaStatements; }*: This is the lambda body. The statements within the lambda body can access the captured variables
and the parameters.

Of the 4 parts, parameter, returnTypeDeclaration and lambdaStatement are very similar to to a normal C++ function's corresponding
part. Let's look at some examples:

* Example 1 : Compile deduce return type
```C++
   auto comparitor = [](const Student &a, const Student &b) { return a.x < b.x; };
```
One detail we notice is the type of a lamdba expression is auto. In fact it's a compile-time generated type, as user we
always put auto when declare it. Here the lambda expression captures nothing and take 2 parameters, the return type is
automatically deduced by the compile based on expression inside lambda body. If we want to be more explict on return type, 
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
variables. In example (3). y and z are captured by reference and x is by value.

* Example 3: capture example using this
*this* keyworkd can be used in capture clause to reprent by-refernce copy of the current object. Check following example, the lambda 
expression set the value of object's attribute x to 2.
```C++
struct S {
    int x;
    void f() {
        auto func = [this](){x = 2;};
        func();
    }
};
```
C++ lambda a new feature in C++11 and has been constantly involving in C++14/17/20. Here we only introduced the basics.



