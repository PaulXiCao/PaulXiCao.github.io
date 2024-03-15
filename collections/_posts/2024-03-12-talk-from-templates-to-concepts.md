---
title: 'Talk: From Templates to Concepts'
categories:
- C++ Talks
tags:
- c++
- CppNow
- generic programming
- template
- concept
- trait
- SFINAE
- ellipses
- variadic template
- parameter pack
- fold expression
- concept
---

Talk: [CppNow23: From Templates to Concepts](https://youtu.be/x6_o-jz_Q-8), [slides](https://github.com/boostcon/cppnow_presentations_2023/blob/a7823c7a598db50cfcfb481a251d364c0ed1fbfd/cppnow_slides/From_Templates_to_Concepts.pdf)

A good talk about how [_Generic Programming_](https://en.wikipedia.org/wiki/Generic_programming) evolved from using templates to concepts.

# This Post

This post will be similar to a transcript of the talk.
I will show the most important examples, and give further background information.
The talk is rather detailed which is why this post is quite long.

# Generic Programming Example (vs template metaprogramming)

Generic Programming is about writing algorithms for which the actual data types will only be specified later on.
Quick example:
```cpp
template <typename Number>
Number add(Number a, Number b) {
    return a + b;
}

int main() {
    return add(2, -1);
}
```
Here the generic algorithm `add()` is defined without knowing the type it is used for later, e.g. it is instantiated for `int`s in `main()`.

This should not be confused with [_Template Metaprogramming (TMP)_](https://en.wikipedia.org/wiki/Template_metaprogramming) which uses templates to do compile-time computations.

# Talk Overview

The overarching theme is to show a short history of generic programming.
Starting from pre-C++11 where it was almost unreadable for the average programmer.
Employing C++20 features (e.g. concepts) this has evolved to something readable for programmers of almost any experience.

The talk is very practically in the sense that he works with many examples and very few formal definitions.
The main example is about an entity (e.g. a `concept` later) that can specify if its argument is a standard container or not, i.e `is_container`.

# Example 1: Custom `is_same`

We implement the functionality to test at compile-time if two types are the same.

```cpp
// types needed to create a function which can extract the return type of a
// member function
template <typename> struct Result {};

template <typename R, typename... Args> struct Result<R(Args...)> {
  using r_type = R;
};

template <typename M, typename T>
auto extractResultType(M T::*pm) -> Result<M>::r_type;

// custom is_same
template <typename T, typename U> struct is_same {
  static inline constexpr bool value = false;
};

template <typename U> struct is_same<U, U> {
  static inline constexpr bool value = true;
};

// example structs used to query return types
struct X {
  int add(int a, int b) { return a + b; };
};
struct Y {
  double add(double a) { return 1. + a; };
};

int main() {
  static_assert(is_same<decltype(extractResultType(&X::add)), int>::value);
  static_assert(is_same<decltype(extractResultType(&Y::add)), double>::value);
}
```

The point here is that this looks complicated and there is functionality from the C++ standard library which makes this simpler.
Especially that it will get easier with each newer release (e.g. C++11 < C++17 < C++20).

# The traits Library

The above custom implemented `is_same` is already availabe within the [`<type_traits>`](https://en.cppreference.com/w/cpp/header/type_traits) header as [`std::is_same`](https://en.cppreference.com/w/cpp/types/is_same).
The key takeaway is to always check the standard for pre-implemented features to be used.

# The `is_container` Example

This is the main example of the talk which will be expanded as we go along.
Its goal is to identify containers during compilation time, e.g.
```C++
template <typename T>
struct is_container { static const bool value = ???; };
```

## First Attempt: Naive Check for Iterator

### Background Knowledge: Ellipses

A function containing the ellipses `...` can match any number of arguments (variadic template) and is inferior in overload resolution.
Example:
```cpp
#include <string_view>

constexpr std::string_view print(...) { return "print(...) called"; }
constexpr std::string_view print(int) { return "print(int) called"; }

int main() {
  static_assert("print(int) called" == print(1));
  static_assert("print(...) called" == print("some example string"));
}
```

### Background Knowledge: SFINAE

_Substition Failure Is Not An Error_.
A function template is allowed to cause a compilation error during type substitution.
This will only lead to the funtion being neglected in overload resolution (and the compilation error is silenced).

```cpp
#include <type_traits> // enable_if_t

#include <string_view>

// Checks for types that have a nested `status` member
template <typename T> struct has_status_member {
private:
  template <typename U, typename = decltype(U::status)>
  static constexpr bool test(U *) { return true; }

  template <typename> static constexpr bool test(...) { return false; }

public:
  static constexpr bool value = test<T>(nullptr);
};

template <typename T>
inline constexpr bool has_status_member_v = has_status_member<T>::value;

// Example function using SFINAE to enable or disable based on the presence of
// status member
template <typename T>
constexpr typename std::enable_if_t<has_status_member_v<T>, std::string_view>
print_status() { return "status"; }

template <typename T>
constexpr typename std::enable_if_t<!has_status_member_v<T>, std::string_view>
print_status() { return "no status"; }

// Example usage
struct TypeWithMember { int status; };
struct TypeWithoutMember {};

int main() {
  static_assert("status"    == print_status<TypeWithMember   >());
  static_assert("no status" == print_status<TypeWithoutMember>());
  return 0;
}
```

SFINAE is used in multiple places in that example:
1. The member function `has_status_member::test(U*)` fails to compile if `U::status` does not exist.
2. The functions `print_status` contain an `std::enable_if_t` in their return type.

`std::enable_if_t` is a functionality to conditionally compile a construct leveraging SFINAE for pre-C++20.

### Implementation

```cpp
#include <cstddef> // byte, size_t

#include <array>

template <typename T> struct is_container {
private:
  template <typename> static constexpr std::byte test(...);

  template <typename U>
  static constexpr std::size_t test(typename U::iterator *);

public:
  static constexpr bool value = (sizeof(size_t) == sizeof(test<T>(nullptr)));
};

int main() {
  static_assert(!is_container<int               >::value);
  static_assert( is_container<std::array<int, 1>>::value);
  return 0;
}
```

This implements the `is_container` functionality by only checking if the given type has a member `iterator` type.
It is quite feature dense and needs some more explanation:
- The `value` member specifies if the template argument is a container (according to our iterator test).
- The `test` functions check for an `iterator` member by accepting it as an argument (i.e. `typename U::iterator*`).
    The check's answer can be retrieved via the functions' return type (i.e. `sizeof() == sizeof()`).
    As we make only use of their return type we do not need to implement their bodies.
- The `test` function accepts a pointer (i.e. `typename U::iterator*`) as this can be initialized by the general `nullptr`.
    If it were not a pointer then we could not generally instantiate it (e.g. which constructor to call?).
- SFINAE is explicitly used as `test(typename U::iterator*)` might not compile/be available for arbitrary `U`.

We can use this as follows

```cpp
template <typename T> consteval int sum(T t) {
  if constexpr (is_container<T>::value) {
    int ret = 0;
    for (auto &&e : t)
      ret += e;
    return ret;
  } else
    return t;
}

int main() {
  static_assert(42 == sum(42));
  static_assert(10 == sum(std::array{1, 2, 3, 4}));
  return 0;
}
```

## Main Example: `are_all_integral` Concept

### Background Knowledge: Variadic Templates and Fold Expressions

A [_Variadic Template_](https://en.cppreference.com/w/cpp/language/parameter_pack) is a template accepting a variable number of arguments (i.e. `number >= 0`), called a _parameter pack_.

[_Fold expressions_](https://en.cppreference.com/w/cpp/language/fold) reduce a parameter pack over a given operator.
See the examples in the previous link.

### Implementation

The following `are_all_integral` concept checks for each template argument if `std::is_integral` holds and combines those results via the `std::conjunction` which performs a fold expression (see comments in code).

```cpp
#include <type_traits> // conjunction, is_integral

template <typename... T>
struct are_all_integral
    : public std::conjunction<   // performs ANDs of all given arguments. Result
                                 // saved in value member.
          std::is_integral<T>... // unpacks to: std::is_integral<T0>,
                                 // std::is_integral<T1>, ...
          > {};

// short-hand notation
template <typename... T>
constexpr bool are_all_integral_v = are_all_integral<T...>::value;

int main() {
  static_assert(are_all_integral_v<int>);
  static_assert(are_all_integral_v<int, int>);

  static_assert(!are_all_integral_v<float>);
  static_assert(!are_all_integral_v<int, float>);

  return 0;
}
```

## Container Definition

A C++ container `C` must implement the following interface:
- `std::begin(C&)` must return a begin iterator
- `std::end(C&)` must return a tail iterator
- begin and tail iterators are comparable via `!=`
- `std::next()` can be used on a begin iterator
- a begin iterator can be dereferencable via `*` and return a non-void type
- begin and tail iterators are copy con- and destructible

The following examples will check for all those requirements.
Expect longer code samples ...

## Second Attempt: Performing all checks at once

### Background Knowdledge: `std::void_t`

To employ SFINAE we previously made use of `std::enable_if_t<b, T>`.
If the boolean condition `b` evaluated `false` then a compilation error is signaled and the template function is removed from the overload set.

`std::void_t` works in a similar fashion.
It accepts a parameter pack and if any argument is ill-formed then it is itself ill-formed.

### Implementation

An implementation which should mostly proof that templates can be quite scarry for the average programmer.
This is *hard to maintain, hard to debug, and error messages are just plain crazy*.

```cpp
#include <iterator> // begin, end, next
#include <type_traits> // false_type, void_t, integral_constant, is_convertible_v, is_void_v, ...

#include <array>
#include <string>
#include <utility> // pair
#include <vector>

// checks: std::begin(C) is well-formed
template <typename C> using TBegin = decltype(std::begin(std::declval<C>()));

// checks: std::end(C) is well-formed
template <typename C> using TEnd = decltype(std::end(std::declval<C>()));

// checks: begin and tail iterators can be compared via !=
template <typename BI, typename EI>
using TNotEqual = decltype(std::declval<BI>() != std::declval<EI>());

// checks: std::next(beginIter) is well-formed
template <typename BI> using TInc = decltype(std::next(std::declval<BI>()));

// checks: an iterator can be dereferenced
template <typename Iter> using TDeref = decltype(*std::declval<Iter>());

// default implementation
template <typename C, typename = void> struct is_container : std::false_type {};

template <typename C>
struct is_container<
        C,
        std::void_t<
            TBegin<C>,
            TEnd<C>,
            TInc<TBegin<C>>,
            TNotEqual<TBegin<C>, TEnd<C>>,
            TDeref<TBegin<C>>
        >
    >
    : std::integral_constant<
        bool,
        std::is_convertible_v<TNotEqual<TBegin<C>, TEnd<C>>, bool> and
            (not std::is_void_v<TDeref<TBegin<C>>>)                and
            std::is_destructible_v<TBegin<C>>                      and
            std::is_copy_constructible_v<TBegin<C>>                and
            std::is_destructible_v<TEnd<C>>                        and
            std::is_copy_constructible_v<TEnd<C>
        >
    > {};

// short-hand notation
template <typename C>
static constexpr bool is_container_v = is_container<C>::value;

int main() {
  static_assert(is_container_v<std::array<int,1>>);
  static_assert(is_container_v<std::string      >);
  static_assert(is_container_v<std::vector<int> >);

  static_assert(!is_container_v<int                >);
  static_assert(!is_container_v<std::pair<int, int>>);

  return 0;
}
```

Note, that every test refers to one requirement listed in the Container Definition section.

## Concepts To The Resue

### Background: Concepts

_Concepts_ are a functionality to restrict possible arguments to a template.
The [`<concepts>` library](https://en.cppreference.com/w/cpp/concepts) introduces already general concepts, e.g. `std::same_as, std::integral`.

A simple example usage:
```cpp
#include <concepts>

constexpr bool myComparison(std::integral auto a, std::integral auto b) {
  return a == b;
}

int main() {
  static_assert(myComparison(2, 2));
  // static_assert(myComparison(1.0, 1.0)); // fails to compile

  return 0;
}
```

There are multiple ways to define a concept yourself.
If possible one should make use of concepts already available in the C++ standard (e.g. `std::convertible_to`).
Requirements can also be combined via boolean expressions (e.g. `and`, `not`).
See this example:
```cpp
#include <concepts>

template <typename T>
concept convertible_to_int_not_double =
    std::convertible_to<T, int> and (not std::same_as<T, double>);

int main() {
  convertible_to_int_not_double auto a = 1;
  // convertible_to_int_not_double auto b = 1.0;  // fails to compile: double
  // convertible_to_int auto c = "hello";         // fails to compile: char*

  return 0;
}
```

Furthermore, one can employ `require` statements to specify concepts.
The keyword is quite overloaded to work in multiple ways.
Some examples:
```cpp
#include <concepts>

template <typename T>
  requires(sizeof(T) > sizeof(int))
struct larger_than_int : public std::true_type {};

template <typename BI, typename EI>
concept neq_on = requires(BI bi, EI ei) {
  { bi != ei } -> std::convertible_to<bool>;
};

int main() {
  static_assert(larger_than_int<long>{});
  // static_assert(larger_than_int<short>{}); // fails to compile

  neq_on<bool> auto a = true;
  // neq_on<char*> auto b = true; // fails to compile

  return 0;
}
```

To restrict function templates we can use concepts in multiple ways as well:
```cpp
#include <concepts>

template <typename BI, typename EI>
concept neq_on = requires(BI bi, EI ei) {
  { bi != ei } -> std::convertible_to<bool>;
};

template <typename EI, neq_on<EI> BI> constexpr bool fun_1(BI bi, EI ei) {
  return true;
}

template <typename BI, typename EI>
  requires neq_on<BI, EI>
constexpr bool fun_2(BI bi, EI ei) {
  return true;
}

template <typename BI, typename EI>
constexpr bool fun_3(BI bi, EI ei)
  requires neq_on<BI, EI>
{
  return true;
}

constexpr bool fun_4(auto bi, neq_on<decltype(bi)> auto ei) { return true; }

int main() {
  static_assert(fun_1(0, 0));
  static_assert(fun_2(0, 0));
  static_assert(fun_3(0, 0));
  static_assert(fun_4(0, 0));

  return 0;
}
```

### Third Attempt: Concepts For Each Requirement

The following implementation uses seperate concepts for each requirement.
This results in more maintainable/readable code as well as more descriptive error messages.

```cpp
#include <concepts>
#include <iterator>

#include <array>
#include <vector>

template <typename C>
concept not_equal_begin_end = requires(C c) {
  { std::begin(c) != std::end(c) } -> std::same_as<bool>;
};

template <typename C>
concept has_begin_and_end = requires(C c) {
  std::begin(c);
  std::end(c);
};

template <typename C>
concept incrementable_begin = requires(C c) {
  std::next(std::begin(c));
};

template <typename C>
concept dereferenciable_begin_not_void = requires(C c) {
  requires not std::same_as<decltype(*std::begin(c)), void>;
};

template <typename C>
concept begin_and_end_copy_constructible_and_destructible = requires(C c) {
  requires std::copy_constructible<decltype(std::begin(c))>;
  requires std::copy_constructible<decltype(std::end(  c))>;
  requires std::destructible<      decltype(std::begin(c))>;
  requires std::destructible<      decltype(std::end(  c))>;
};

template <typename C>
concept Container = has_begin_and_end<C>                                 and
                    incrementable_begin<C>                               and
                    dereferenciable_begin_not_void<C>                    and
                    begin_and_end_copy_constructible_and_destructible<C>;

int main() {
  Container auto a = std::array{1, 2};
  Container auto b = std::vector{1, 2};

  // Container auto c = 1; // fails to compile
  // Container auto d = "abc"; // fails to compile

  return 0;
}
```
