---
title: 'Talk: Modern Template Metaprogramming'
categories:
- C++ Talks
tags:
- c++
- CppCon
- template metaprogramming
- SFINAE
- TMP
---

Talk: _CppCon 2014: Walter E. Brown "Modern Template Metaprogramming: A Compendium_

Resources:
- videos on YouTube: [part 1](https://youtu.be/Am2is2QCvxY), [part 2](https://youtu.be/a0FliKwcwXE)
- [slides](https://github.com/CppCon/CppCon2014/blob/7b15ec44ac01de0ff3e65a7194b84aca2d4e2366/Presentations/Modern%20Template%20Metaprogramming%20-%20A%20Compendium/Modern%20Template%20Metaprogramming%20-%20A%20Compendium%20-%20Walter%20E%20Brown%20-%20CppCon%202014.pdf)
- [TMP on wikipedia](https://en.wikipedia.org/wiki/Template_metaprogramming)
- [TMP on wikibooks](https://en.wikibooks.org/wiki/C++_Programming/Templates/Template_Meta-Programming)

A well-rounded overview about the topic of _Template Metaprogramming (TMP)_.
It is from 2014 and therefore, out-dated at some points, but the general ideas are either still important or needed to understand legacy code!

I can only recommend the videos - they are quite entertaining and truly worth watching!

The following sections will be mostly a write-up of the talk and slides with minimal extra information from my side.

## What is TMP?

Code which manipulates other code at compilation time.
TMP uses template instantiation to drive compile-time evaluation.

Best explained via a simple example computing the absolute number at compile-time:

```cpp
#include <limits>  // numeric_limits

template <int N>
struct abs {
  static_assert(N !=
                std::numeric_limits<int>::min());  // UB for two's complement
  static constexpr int value = (N >= 0) ? N : -N;
};

int main() {
  constexpr int n = abs<5>::value;
  static_assert(5 == n);
  static_assert(5 == abs<-5>::value);
  static_assert(0 == abs<0>::value);
}
```

Comparing TMP's metafunctions with usual functions:
- _Arguments_ are supplied via template arguments (e.g. `<5>` instead of `(5)`).
- To _call_ a metafunction one accesses the published value (e.g. `::value` which is a convention - more on that later).


## Comparison to `constexpr`

`constexpr` functions can do compile-time computation since C++11.
TMP is the "old-school" way.

Advantage of `constexpr` functions:
- Readability: `constexpr` functions look just as usual functions and programmers are more accustomed to them.

Advantage of TMP:
- Memoization: Metafunctions are only evaluated once and their values are cached (templates are instantiated once).
  `constexpr` functions might be instantiated multiple times.
  Standard example is computing Fibonacci numbers.
- Returning types: It is trivial returning types in TMP.
  `constexpr` functions might need to make use of `decltype()`, `declval()`.

## Compile-time recursion

Compile-time recursion (and loops) can be implemented by using metafunctions and specializations.

Example: computing the greatest common divisor
```cpp
template <unsigned M, unsigned N>
struct gcd {
  static constexpr unsigned value = gcd<N, M % N>::value;
};

template <unsigned M>
struct gcd<M, 0> {
  static_assert(M != 0);  // gcd(0, 0) is undefined, so disallow
  static constexpr unsigned value = M;
};

int main() {
  static_assert(4 == gcd<8, 12>::value);
  static_assert(7 == gcd<7, 7>::value);
}
```

## Taking types as arguments

Metafunctions can take types as arguments.

As an example we can implement a custom `rank` function:
```cpp
#include <cstddef>  // size_t

// primary template handles scalar types
template <class T>
struct rank {
  static constexpr size_t value = 0u;
};

// partial specialization handles array types
template <class T, size_t N>
struct rank<T[N]> {
  static constexpr size_t value = 1u + rank<T>::value;
};

int main() {
  static_assert(0 == rank<int>::value);
  static_assert(3 == rank<int[10][20][30]>::value);
}
```

(Available in the the standard via [`std::rank`](https://en.cppreference.com/w/cpp/types/rank).)

## Returning a type

Upto now we showed only metafunctions returning values.
They can also be used to return types.

As an example we can implement a metafunction removing a type's `const`-qualification (e.g. returning `int` for `const int` as input):
```cpp
#include <type_traits>  // is_same_v

// primary template handles not const-qualified
template <class T>
struct remove_const {
  using type = T;
};

// partial specialization recognizes const-qualified types
template <class T>
struct remove_const<T const> {
  using type = T;
};

int main() {
  static_assert(std::is_same_v<int, remove_const<int>::type>);
  static_assert(std::is_same_v<int, remove_const<int const>::type>);
}
```

(Available in the the standard via [`std::remove_const`](https://en.cppreference.com/w/cpp/types/remove_cv).)

## C++11 library conventions

### `::type` member
A metafunction returning a type should save its result in a `::type` member.
(See the above given examples.)

This convention can be enforced by making use of a helper `type_is`
```cpp
template <class T>
struct type_is {
  using type = T;
};

template <class T>
struct remove_const : public type_is<T> {};

template <class T>
struct remove_const<T const> : public type_is<T> {};
```

(Available in the the standard via [`std::type_identity`](https://en.cppreference.com/w/cpp/types/type_identity).)

### `_t` call alias

There should be a call alias with the `_t` subfix.
Example for the above given `remove_const`:

```cpp
template <class T>
using remove_const_t = remove_const<T>::type;

int main() {
  static_assert(std::is_same_v<int, remove_const_t<int>>);
  static_assert(std::is_same_v<int, remove_const_t<int const>>);
}
```

### `::value` member

A metafunction returning a type should save its result in a `::value` member.

Example:
```cpp
template <class T, T v>
struct integral_constant {
  static constexpr T value = v;
};

int main() {
  static_assert(7 == integral_constant<int, 7>::value);
  static_assert('z' == integral_constant<char, 'z'>::value);
}
```

### `_v` call alias

In C++14 [variable templates](https://en.cppreference.com/w/cpp/language/variable_template) were introduced.
Since then, value returning metafunctions usually have a helper function accessing the `::value` member.

Example:
```cpp
template <class T, T v>
inline constexpr T integral_constant_v = integral_constant<T, v>::value;

int main() {
  static_assert(7 == integral_constant_v<int, 7>);
  static_assert('z' == integral_constant_v<char, 'z'>);
}
```

## Compile-time if-else

Conditional branching is possible via the following construct

```cpp
#include <type_traits>  // is_same_v

// primary template: assumes true precondition p
template <bool p, class T, class>
struct IF : public type_is<T> {};

// partial specialization: recognizes a false value
template <class T, class F>
struct IF<false, T, F> : public type_is<F> {};

template <bool p, class T, class F>
using IF_t = IF<p, T, F>::type;

int main() {
  static_assert(std::is_same_v<int, IF_t<true, int, char>>);
  static_assert(std::is_same_v<char, IF_t<false, int, char>>);
}
```

(Available in the standard via [`std::conditional_t`](https://en.cppreference.com/w/cpp/types/conditional).)

## SFINAE

See [previous post](https://paulxicao.github.io/c++%20talks/2024/03/12/talk-from-templates-to-concepts.html#background-knowledge-sfinae).

Take special note of [`std::enable_if`](https://en.cppreference.com/w/cpp/types/enable_if).

## Dispatch to most efficient implementation (e.g. tag types)

A generic metafunction can dispatch do different implementations depending on the type.

As an example we can look at a `distance` function taking in two iterators.
A simple dispatching on a single boolean type trait is possible as in
```cpp
#include <iterator> // iterator_traits

// specific implementation assuming random access iterators (giving O(1) runtime
// cost)
template <class Iter>
auto distance(Iter b, Iter e, true_type) {
  return e - b;
}

// implementation for non-random access iterators (giving O(n) runtime cost)
template <class Iter>
auto distance(Iter b, Iter e, false_type) {
  ~~~
}

template <class Iter>
auto distance(Iter b, Iter e) {
  return distance(b, e, is_random_access_iter_t<Iter>{});
}
```

More fine-grained control via tag types (e.g. iterator tags):
```cpp
#include <iterator>

template <class Iter>
auto distance(Iter b, Iter e, std::random_access_iterator_tag);

template <class Iter>
auto distance(Iter b, Iter e, std::input_iterator_tag);

template <class Iter>
inline auto distance(Iter b, Iter e) {
  return distance(b, e, std::iterator_traits<Iter>::iterator_category{});
}
```

## Parameter pack

Variadic templates can be useful in many occasions.
We will look into implementing a `is_one_of` metafunction which checks if the first template argument is in one of the following.
This is a generalization of `is_same`.

```cpp
#include <type_traits>  // true/false_type

// primary template: is T the same as one of the types P0toN... ?
template <class T, class... P0toN>
struct is_one_of; // declare interface only

// base #1: specialization recognizes empty list of types
template <class T>
struct is_one_of<T> : public std::false_type {};

// base #2: specialization recognizes match at head of list of types
template <class T, class... P1toN>
struct is_one_of<T, T, P1toN...> : public std::true_type {};

// specialization recognizes mismatch at head of list of types
template <class T, class P0, class... P1toN>
struct is_one_of<T, P0, P1toN...> : public is_one_of<T, P1toN...> {}; // inspect list's tail

template <class T, class... P0toN>
inline constexpr bool is_one_of_v = is_one_of<T, P0toN...>::value;

int main() {
  static_assert(not is_one_of_v<int>);

  static_assert(not is_one_of_v<int, float>);
  static_assert(is_one_of_v<int, int>);

  static_assert(not is_one_of_v<int, bool, float>);
  static_assert(is_one_of_v<int, int, float>);
  static_assert(is_one_of_v<int, float, int>);
  static_assert(is_one_of_v<int, int, int>);
}
```

## Unevaluated operands

The arguments to `decltype` and others (e.g. `sizeof`, `alignof`, `typeid`, `noexcept`) are not even evaluated at compile-time.
There is no code generation and thus, only declarations need to be available (e.g. functions without implementations).

For an example we can look at a testing of copy-assignability

```cpp
#include <type_traits>  // true/false_type
#include <utility>      // declval

template <class T>
struct is_copy_assignable {
 private:
  // helper alias for the result type of a valid copy assignment
  template <class U>
  using copy_assign_t = decltype(std::declval<U&>() = std::declval<U const&>());

  // SFINAE may apply
  template <class U, class = copy_assign_t<U>>
  static std::true_type try_assign(U&&);

  // catch-all overload
  static std::false_type try_assign(...);

 public:
  using type = decltype(try_assign(std::declval<T>()));
};

template <class T>
inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::type::value;

int main() {
  static_assert(is_copy_assignable_v<int>);
  static_assert(not is_copy_assignable_v<const int>);
  static_assert(not is_copy_assignable_v<int[1]>);
}
```

There is quite a lot happening in this example and we should dissect this to some point
- `copy_assign_t`: This alias tries to assign an object of type `U const&` to an object `U&`.
  Especially note that this is allowed to fail (SFINAE)!
  The result type of that assignment is retrieved via `decltype`.
- `try_assign`: There are two possible overloads.
  If `copy_assign_t<U>` is valid then the overload with `true_type` as return type is chosen.
  Otherwise an overload with `false_type` as return type.
- `type`: The `try_assign` function is called for an object of type `T` and the return type extracted via `decltype`.
- Especially note that the `try_assign` functions have no implementations as they are only used in unevaluated contexts via `decltype`.

## `void_t`

Super simple implementation
```cpp
template <class ...>
using void_t = void;
```

Useful for checking of well-formedness of expressions, i.e. if argument to `void_t` is not well-formed then the template will not be considered in template overload resolution (SFINAE).

Example: Check if a given type `T` has a type member `::type`
```cpp
#include <type_traits>  // true/false_type, void_t

// primary template
template <class T, class = void>
struct has_type_member : public std::false_type {};

// specialization
template <class T>
struct has_type_member<T, std::void_t<typename T::type>>
    : public std::true_type {};

template <class T>
inline constexpr bool has_type_member_v = has_type_member<T>::type::value;

int main() {
  static_assert(has_type_member_v<std::true_type>);
  static_assert(not has_type_member_v<int>);
}
```

To understand this we need to track how an instantiation like `has_type_member_v<int>` is treated.
At first note that the second argument is defaulted to `void`, i.e. we actually call `has_type_member_v<int, void>`.
There is the primary template (`false_type`), but a more specialized version (`true_type`) is available iff `std::void_t<typename T::type>` is well-formed.
