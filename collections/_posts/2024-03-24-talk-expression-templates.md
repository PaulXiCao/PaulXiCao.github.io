---
title: 'Talk: Expression templates'
categories:
- C++ Talks
tags:
- c++
- CppCon
- expression template
---

Talk: _Expression Templates for Efficient, Generic Finance Code - Bowie Owens - CppCon 2019_

Resources:
- [video on YouTube](https://youtu.be/4IUCBx5fIv0)
- [slides](https://github.com/CppCon/CppCon2019/blob/e84f0777fb0e17517852c4b669c73ed66f6a1a91/Presentations/expression_templatesfor_efficient_generic_finance_code/expression_templatesfor_efficient_generic_finance_code__bowie_owens__cppcon_2019.pdf)
- [blog post at modernescpp.com](https://www.modernescpp.com/index.php/expression-templates/)
- [Wikipedia](https://en.wikipedia.org/wiki/Expression_templates)

This talk introduces the idea of expression templates using C++17 features.

The goal of expression templates is to circumvent creating expensive intermediate temporary objects, especially in the context of mathematical operations.

The standard example is adding multiple vectors which will create temporaries for each single operation.

Expression templates on the other hand save references to the operators and only lazily evaluate when assignment is requested.
This will minimize unnecessary allocations and loops.
Even as optimizations (e.g. return value optimization) are applied.

## Standard example

The "hello world" kind of example is assigning the elementwise addition of multiple arrays to another one.
This will create multiple intermediate temporaries which might allocate memory.

See [this example](https://github.com/PaulXiCao/expressionTemplatesCppCon2019/blob/main/tests/standardExample.cpp).

## The solution: Expression templates

The code snippets shown in the presentation have been collected in this [repository](https://github.com/PaulXiCao/expressionTemplatesCppCon2019).

### The expression `expr`

The workhorse is the `expr` expression class which keeps track of the operation to apply and its arguments.
```cpp
struct expression {};

template <class callable, class... operands>
class expr : public expression {
 private:
  callable f_;
  std::tuple<operands const&...> args_;

 public:
  expr(callable f, operands const&... args) : args_(args...), f_(f) {}

  auto operator[](index const i) const {
    auto const call_at_index = [this, i](operands const&... a) {
      return f_(subscript(a, i)...);
    };
    return std::apply(call_at_index, args_);
  }
};
```

At first I want to mention that one should think of a simple `double add(double, double)` function for the `callable f_`, and `args_` to be two `std::array<double, N>`.

Thinks to note here:
- We introduce the `expression` base class for type traits later on.
- Variadic templates (e.g. `class... operands`) are used in stead of the older CRTP method which simplifies the code.
    We need this variability to work for unary (e.g. negation), binary (e.g. addition), .. operations.
- The `operator[]` is needed to recursively call down to lower expressions.
- Its implementation calls the callable with the appropriate arguments.
    The `apply` calls the `call_at_index` with the `args_` arguments which calls the callable `f_` for the index arguments `subscript(a, i)`.
- The `auto` return type is an important simplification to older standards.

### Indexing arguments

The `subscript` function differentiates between arrays, expressions, and others (i.e. scalars).

```cpp
template <class operand>
auto subscript(operand const& v, index const i) {
  if constexpr (is_array_or_expression<operand>)
    return v[i];
  else
    return v;
}
```

Where the `is_array_or_expression` concept simply checks for predefined traits, e.g. a `std::vector` is marked as an array.
Users can mark their custom types as well.
This is similar to whitelisting types.

### Example operation: Multiplication

Multiplication of arrays with arrays or scalars should return an `expr` and then iteratively multiplications with an `expr` should also yield an `expr`.

```cpp
template <class LHS, class RHS>
  requires(is_binary_op_ok<LHS, RHS>)
auto operator*(LHS const& lhs, RHS const& rhs) {
  return expr{[](auto const& l, auto const& r) { return l * r; }, lhs, rhs};
}
```

The concept `is_binary_op_ok` is used to whitelist types for which we want to allow multiplication with an `expr` result.

You might want to compare this call with the `expr::expr()` constructor definition.

### Custom type: `tridiagonal`

Types which want to make use of the `expr` class need to implement two member functions

```cpp
class tridiagonal {
  // ...

  template <class src_type>
  tridiagonal& operator=(src_type const& src) {
    for (size_t i = 0; i < v_.size(); ++i) v_[i] = src[i];
    return *this;
  }

  double operator[](index i) const { return v_[i]; }
};
```

Furthermore, we need to whitelist this type, e.g.

```cpp
constexpr bool is_binary_op_ok<tridiagonal, tridiagonal> = true;

struct is_array<tridiagonal> : public std::true_type {};
```

Usage on client side is quite clean

```cpp
int main() {
  const tridiagonal<double> x{1, 2, 3};
  const tridiagonal<double> y{1, 3, 5};

  tridiagonal<double> z(3u);
  z = x * y * x;

  return 0;
}
```
