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
<iframe width="800px" height="400px" src="https://godbolt.org/e#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:1,endLineNumber:6,positionColumn:1,positionLineNumber:6,selectionStartColumn:1,selectionStartLineNumber:6,startColumn:1,startLineNumber:6),source:'template+%3Ctypename+Number%3E%0ANumber+add(Number+a,+Number+b)+%7B+%0A++++return+a+%2B+b%3B+%0A%7D%0A%0Aint+main()+%7B+%0A++++return+add(2,+-1)%3B+%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',m:61.491301312840996,n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:gsnapshot,compilerName:'',compilerOutShown:'0',execArgs:'',execStdin:'',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'--std%3Dc%2B%2B20',overrides:!(),runtimeTools:!(),source:1,stdinPanelShown:'1',wrap:'1'),l:'5',n:'0',o:'Executor+x86-64+gcc+(trunk)+(C%2B%2B,+Editor+%231)',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>
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

<iframe width="800px" height="400px" src="https://godbolt.org/e?hideEditorToolbars=true#g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:'//+types+needed+to+create+a+function+which+can+extract+the+return+type+of+a+member+function%0Atemplate+%3Ctypename%3E%0Astruct+Result+%7B%7D%3B%0A%0Atemplate+%3Ctypename+R,+typename...+Args%3E%0Astruct+Result%3CR(Args...)%3E%7B%0A++++using+r_type+%3D+R%3B%0A%7D%3B%0A%0Atemplate+%3Ctypename+M,+typename+T%3E%0Aauto+extractResultType(M+T::*+pm)+-%3E+Result%3CM%3E::r_type%3B+%0A%0A//+custom+is_same%0Atemplate+%3Ctypename+T,+typename+U%3E%0Astruct+is_same+%7B+static+inline+constexpr+bool+value+%3D+false%3B+%7D%3B%0A%0Atemplate+%3Ctypename+U%3E%0Astruct+is_same%3CU,+U%3E+%7B+static+inline+constexpr+bool+value+%3D+true%3B+%7D%3B%0A%0A//+example+structs+used+to+query+return+types%0Astruct+X%7B%0A++++int+add(int+a,+int+b)+%7Breturn+a%2Bb%3B%7D%3B%0A%7D%3B%0Astruct+Y%7B%0A++++double+add(double+a)+%7Breturn+1.%2Ba%3B%7D%3B%0A%7D%3B%0A%0Aint+main()+%7B%0A++++static_assert(is_same%3Cdecltype(extractResultType(%26X::add)),+int+++%3E::value)%3B%0A++++static_assert(is_same%3Cdecltype(extractResultType(%26Y::add)),+double%3E::value)%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:99.70254126058308,l:'4',m:61.491301312840996,n:'0',o:'',s:0,t:'0')),version:4"></iframe>

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
<iframe width="800px" height="400px" src="https://www.compiler-explorer.com/e?hideEditorToolbars=true#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:1,endLineNumber:4,positionColumn:1,positionLineNumber:4,selectionStartColumn:1,selectionStartLineNumber:4,startColumn:1,startLineNumber:4),source:'%23include+%3Ciostream%3E%0A%0Avoid+print(...)+%7B+std::cout+%3C%3C+%22ellipses.%5Cn%22%3B+%7D%0A%0Avoid+print(int)+%7B+std::cout+%3C%3C+%22integer.%5Cn%22%3B+%7D%0A%0Aint+main()+%7B%0A++++print(1)%3B%0A++++print(%22Hello%22)%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:g132,compilerName:'',compilerOutShown:'0',execArgs:'',execStdin:'',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'',source:1,stdinPanelShown:'1',wrap:'1'),l:'5',n:'0',o:'Executor+x86-64+gcc+13.2+(C%2B%2B,+Editor+%231)',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

### Background Knowledge: SFINAE

_Substition Failure Is Not An Error_.
A function template is allowed to cause a compilation error during type substitution.
This will only lead to the funtion being neglected in overload resolution (and the compilation error is silenced).

<iframe width="800px" height="400px" src="https://www.compiler-explorer.com/e?hideEditorToolbars=true#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGE6a4AMngMmAByPgBGmMQgZtIADqgKhE4MHt6%2B/qRJKY4CwaERLNGx8baY9vkMQgRMxAQZPn5SFVVptfUEheFRMXHSCnUNTVmtQ109xaUDAJS2qF7EyOwc5gDMIcjeWADUJutuTkPEmKwH2CYaAIIbWzuY%2B4cEAJ4JmAD6BMRMhAoXuwA9IDdowmJF6B88FQvldbjdgbs3AhMMgANYKXb8Yi7V7vTEEBBMAi7IkAN0eTF2oSGmHQ%2BwAbBoJgQvP8mbs2CUYnCCJgWAkDHyDm48WC2LsACoXOEnLwOUlMBQfFlsj5c0r7ADsVhuCWIeDJxPYcN2Ztx/MFxpFYuYErkpFxb3FjwOABFdlhtmKIHIQCBVQpZjKbubdiy8MhdmgGLTVPrdpFUJ4LUNfQAqWbaiy7U6s4gMXHELyYA45kxat1w03mvkCoWl57Ou2Ny6h80RqMxuMJpMpvlpgB0w6zFZzeaWhaoYiUZe1VZucISXghkZANbNnejAh7OL7tF2Ru8rvWHoHBBF0vW2AgDC8tFoCW%2BwfWutulbL1Zudatwqb7xbKUQ2uEJaBCR5uz5eM92TA8iWVQN1X5UoPjJJ4PXglU6lZZUNR5Q4r2wf0jxLT9FwREFsFUVhBUeKgvAYBw0l2NkQmAXYhAAMQASTCa5sFxVBQWYCFHhIT08AUcF6ETJU6V2ARcRRXZ9UwJRGLEqhw2wtlOWQnlv0tBsbWbVhHkI3lTIlIZ0H9MFRKhGEL0OTDELw4hUMvC5HTJVA8HQYD9RCAgsOJNkIElUcdQ3bTbJQRYSRFEV9jMMxJWdT1UDUxUKV2KlAz07liEHbSwoUEAUrMJ43GSyV/QKpLDl2MA1gAVjcBgWrI98F3ha4f2M/8XSA69LIAszYrskTIWhWFDjAMBXJ03D9I8skvOvHy/IC0a9QNQRQpwiKorfMMbP9NAvESw5kvMNKMvQLLMQYVASXJSlSpwwrSkHEx2oYO7uorXq4URKiaJktkmGAUsbjlBV0veAB1QgEAAWVW7NPsjbdYyghNgs%2B3T3V2SQzDnYHuvhklEcwFHCQSjGiuzSnXy/EDBE5X4GAgE6YqCg7Awi516fR1ax2Bl9y3bM0BZCoXadFxnxeiyspf2GXc0wfNCw0IGPxuDh5loThWt4PwOC0UhUE4GrLGscNFmWV0zHWHhSAITQjfmNEQFajR9E4SRza963OF4cqA89y2jdIOBYCQTBVFRK6SHISh6mABRlEMSohAQVAAHcLfdtABToYk0hz0JaHzouLatsuEjofpgC4dYzFIJuW%2BIMIzPDrvUHL%2BhiAAeSuuvi9DpPUWuYgs4HmfkFqfALd4fhBBEMR2FaDf5CUNRQ90Lh9EMYxrGsfQ8EicrIHmVAnzScqOAjp2Vj0GyQmrvOC6n7heELj8BInAeDG1NiHGOYcOBURTkQHEqgAAcDIAC0DJJC7GAMgKM7dBxVQgHbKwlhHS4EIOJDYXBZi8GjloWYPs/YBxNhwYOpAG68Bti/WwIAo5e1oYHDgZgIFW3YVQnh8wKTEBSM4SQQA%3D"></iframe>

SFINAE is used in multiple places in that example:
1. The member function `has_status_member::test(U*)` fails to compile if `U::status` does not exist.
2. The functions `print_value` contain an `std::enable_if_t` in their return type.

`std::enable_if_t` is a functionality to conditionally compile a construct leveraging SFINAE for pre-C++20.

### Implementation

<iframe width="800px" height="400px" src="https://godbolt.org/e?hideEditorToolbars=true#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:14,endLineNumber:21,positionColumn:14,positionLineNumber:21,selectionStartColumn:14,selectionStartLineNumber:21,startColumn:14,startLineNumber:21),source:'%23include+%3Carray%3E%0A%23include+%3Ccstddef%3E%0A%23include+%3Cutility%3E%0A%0Atemplate+%3Ctypename+T%3E%0Astruct+is_container+%7B%0Aprivate:%0A++++template+%3Ctypename%3E%0A++++static+constexpr+std::byte+test(...)%3B%0A%0A++++template+%3Ctypename+U%3E%0A++++static+constexpr+std::size_t+test(typename+U::iterator*)%3B%0A%0Apublic:%0A++++static+constexpr+bool+value+%3D+(sizeof(size_t)+%3D%3D+sizeof(test%3CT%3E(nullptr)))%3B%0A%7D%3B%0A%0Aint+main()+%7B%0A++++static_assert(false+%3D%3D+is_container%3Cint+++++++++++++++%3E::value)%3B%0A++++static_assert(true++%3D%3D+is_container%3Cstd::array%3Cint,+1%3E%3E::value)%3B%0A++++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',m:61.491301312840996,n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:g132,compilerName:'',compilerOutShown:'0',execArgs:'',execStdin:'',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'',source:1,stdinPanelShown:'1',wrap:'1'),l:'5',n:'0',o:'Executor+x86-64+gcc+13.2+(C%2B%2B,+Editor+%231)',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

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

<iframe width="800px" height="400px" src="https://godbolt.org/e?hideEditorToolbars=true#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:1,endLineNumber:16,positionColumn:1,positionLineNumber:16,selectionStartColumn:1,selectionStartLineNumber:16,startColumn:1,startLineNumber:16),source:'%23include+%3Carray%3E%0A%0Atemplate+%3Ctypename+T%3E%0Astruct+is_container+%7B%0Aprivate:%0A++++template+%3Ctypename%3E%0A++++static+constexpr+std::byte+test(...)%3B%0A%0A++++template+%3Ctypename+U%3E%0A++++static+constexpr+std::size_t+test(typename+U::iterator*)%3B%0A%0Apublic:%0A++++static+constexpr+bool+value+%3D+(sizeof(std::size_t)+%3D%3D+sizeof(test%3CT%3E(nullptr)))%3B%0A%7D%3B%0A%0Atemplate+%3Ctypename+T%3E%0Aconsteval+int+sum(T+t)+%7B%0A++++if+constexpr+(is_container%3CT%3E::value)+%7B%0A++++++++int+ret+%3D+0%3B%0A++++++++for+(auto%26%26+e+:+t)%0A++++++++++++ret+%2B%3D+e%3B%0A++++++++return+ret%3B%0A++++%7D%0A++++else+%0A++++++++return+t%3B%0A%7D%0A%0Aint+main()+%7B%0A++++static_assert(42+%3D%3D+sum(42))%3B%0A++++static_assert(10+%3D%3D+sum(std::array%7B1,+2,+3,+4%7D))%3B%0A++++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',m:61.491301312840996,n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:clang_trunk,compilerName:'',compilerOutShown:'0',execArgs:'',execStdin:'',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'--std%3Dc%2B%2B20',overrides:!(),runtimeTools:!(),source:1,stdinPanelShown:'1',wrap:'1'),l:'5',n:'0',o:'Executor+x86-64+clang+(trunk)+(C%2B%2B,+Editor+%231)',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>


## Main Example: `are_all_integral` Concept

### Background Knowledge: Variadic Templates and Fold Expressions

A [_Variadic Template_](https://en.cppreference.com/w/cpp/language/parameter_pack) is a template accepting a variable number of arguments (i.e. `number >= 0`), called a _parameter pack_.

[_Fold expressions_](https://en.cppreference.com/w/cpp/language/fold) reduce a parameter pack over a given operator.
See the examples in the previous link.

### Implementation

The following `are_all_integral` concept checks for each template argument if `std::is_integral` holds and combines those results via the `std::conjunction` which performs a fold expression (see comments in code).

<iframe width="800px" height="400px" src="https://godbolt.org/e?hideEditorToolbars=true#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:'%23include+%3Ctype_traits%3E+//+conjunction,+is_integral%0A%09%09%0Atemplate+%3Ctypename...+T%3E%0Astruct+are_all_integral+%0A++++:+public+%0A++++++++std::conjunction%3C+++++++++++++++//+performs+ANDs+of+all+given+arguments.+Result+saved+in+value+member%0A++++++++++++std::is_integral%3CT%3E...++++++//+unpacks+to:+std::is_integral%3CT0%3E,+std::is_integral%3CT1%3E,+...%0A++++++++%3E+%0A++++%7B%7D%3B%0A%0A//+short-hand+notation%0Atemplate+%3Ctypename...+T%3E%0Aconstexpr+bool+are_all_integral_v+%3D+are_all_integral%3CT...%3E::value%3B%0A%0Aint+main()+%7B%0A++++static_assert(+are_all_integral_v%3Cint+++++%3E)%3B%0A++++static_assert(+are_all_integral_v%3Cint,+int%3E)%3B%0A%0A++++static_assert(!!are_all_integral_v%3Cfloat+++++%3E)%3B%0A++++static_assert(!!are_all_integral_v%3Cint,+float%3E)%3B%0A%0A++++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',m:61.491301312840996,n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:clang_trunk,compilerName:'',compilerOutShown:'0',execArgs:'',execStdin:'',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'--std%3Dc%2B%2B20',overrides:!(),runtimeTools:!(),source:1,stdinPanelShown:'1',wrap:'1'),l:'5',n:'0',o:'Executor+x86-64+clang+(trunk)+(C%2B%2B,+Editor+%231)',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

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

<iframe width="800px" height="400px" src="https://godbolt.org/e?hideEditorToolbars=true#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGIMwDMpK4AMngMmAByPgBGmMQgAKzSAA6oCoRODB7evv5BaRmOAmER0SxxCcm2mPbFDEIETMQEOT5%2BgTV1WY3NBKVRsfFJ0gpNLW15nWN9A%2BWVIwCUtqhexMjsHOYB4cjeWADUJgFuhPFMRMTH2Ae3twD09wdxwOGkB67vEaoEJhoAgttdvtMEcTgQAJ4pTAAfQIxCYhAU1weTyoYiUcKhmHeADdUHh0HD3uECJhgAjaDC0AwZoISQpqQJcfFHDF6DDcQzOQSiVyDgA6IV/QEAoEMPZeQ7HNzNBEQ64i8WS6UnMbEcLARViwLAqWgmVeRy0QgKgI3O6og4pRFXHU7CUgsFuFkOEja0X/R4HZAITDIADWChABzG6BAIBe4QgbkWBzwCgOAHdarQALT8YhsdAisksFIGMnOyHQ5hsA5uD1eDJGA4AFQs5PCYIAIgcsHsS5gIGGI1GGD2COGQB3aLixDLK%2BaIItZ8cLEcxQDvb7/UGQ72QK4Y3GE8nUxmSNnc5h84WDeDsWXQVPsCLq5r69gGOhW%2B3/bQu4Ph9vN6Px7Qk7XDOc4BFYS5ek8q6BsGzxNgwByGK%2BTR0PGZIIpciaiAhcQ%2Bqg%2BbNJgr64ngTAHGAYDHC2J5nhcF5uF214HBYACS7yMawoLYCxVY1sA9aRKgBDYAAjl4YhvqOX5/h%2BAEyqxwFxhRVGhkOEb/hOJzcYpizzkqy5QX6MEbmpIDfAQED9ix6G7omKa0OmmbHgCeYFnRxZXpxzE8ea958fWLESpJH7SaZ5nfupsmaW4CnTrOulgfpkE%2BkZ66IQhZwYSQPqGHB77EJgVDxIwGw5i5p5uUWMoceW1nxLxj51i2xVUMFnbYhAABUMl7HJJx1VccUJeBnrelg6JeJ%2B8ZnqejBNPUNGVfRNU3uxnnlip%2BKEh66peA48aMjSKERMQBwmcO6K0JiXZHAA7FYt3UYlEGueeHmll5t4irt%2B0JkygiIidMoipalpuO8IOg7cm5bUSvwnJDUOWg28FAeaEMAkjoN1s%2BOYnLeGP/FjyOBaYJwo68DBo3e6OI8TdaCcJYnRRT4TU%2B8OMvtT1wc81BVUDKrNU/jiq%2BZjSMepakt3OdEakuSlL/XS8OVuLUMxKgniE1jm5/TSLItHg7KwriguM6J4mAeTjaU%2BzT5cyLvno88mtWzcSF01jDBCapw5/bDnKC3zhWCzbbOOzTkfpWVRPE9Dpl/VgP1shypvW6jEcoh7avE7rh2oCkEJK/Ce0pybocZ24X3mtHntI3nMJJyXDhG6ngu49zNfZ7HccN2ghfF8Qpet%2BX5Md5nOdS2LPdgjcJj3fPT0jSK3oKAgJAEGmCBIQc3vzVki1vdV603jt%2B/IHhtJkqoKSnRrngHf9x3xJyb567IgP1RHEYAV4mB6RBUkBwWCAxnHdEaloZiOGQDCJgCglAtAgI/I6n9iCBxOL7CMcomBmlOPSLgKJbjXGGnTKBeAYFwIQRZZBH9wgvzTm4TBIB1SPixsQgBM8yEUPgayJB78AZ0LQQwphrpLgylJIQoh5oSEQUgefWBPDEEUX4c/IRMp4yCDjhLaRHC5EXHIQoqhEBlH5wESddBjDNw2jwINPBBASSCBROw56npLQFQIGsBCGgOGLw4MsWgnBEi8D8BwLQpBUCcErJYawoZVjrANIEHgpACCaD8csAMSQND6E4JIXgLAQAADYuACkkAATi4AEDQFSuBmAABySA0KU0pBTSAhLCREjgvBgxZJSaEvxpA4CwCQJgVQ/ojQkHIJQZowAFDKEMLUIQ68kwhKSWgfMdB9ECDmREWgizUDLNSaQNZKQ6DDD2IYYAcIh4MADEc/CJz6DEEiJxTgvBjmnOIAAeSNHsg5fTgijOQP8YgMzXkAv9I0fAITeD8EECIMQ7ApAyEEIoFQ6h/m6C4PoC5KBrDWH0EbYMkBlgF3qMGDgaY0xhioqYaJlgzDtINhqJO8BljXMcGwOsrsiUzhWGsDYegwzhG2QspZKzeBJgRCkTgPB/GBOCYcjp2BAXjNOqoWpBS0wFMkD6AwtYIAlxuXGGMeLLDvFwIQbK2wuCLF4L0rQs5SB%2BiYFgBIvKMmJCyQEjguTWmKrBd05JqTHXerMAq/5HS7XBuWAbDIzhJBAA%3D"></iframe>

Note, that every test refers to one requirement listed in the Container Definition section.

## Concepts To The Resue

### Background: Concepts

_Concepts_ are a functionality to restrict possible arguments to a template.
The [`<concepts>` library](https://en.cppreference.com/w/cpp/concepts) introduces already general concepts, e.g. `std::same_as, std::integral`.

A simple example usage:
<iframe width="800px" height="400px" src="https://godbolt.org/e?hideEditorToolbars=true#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:5,endLineNumber:8,positionColumn:5,positionLineNumber:8,selectionStartColumn:5,selectionStartLineNumber:8,startColumn:5,startLineNumber:8),source:'%23include+%3Cconcepts%3E%0A%0Aconstexpr+bool+myComparison(std::integral+auto+a,+std::integral+auto+b)+%7B+return+a+%3D%3D+b%3B+%7D%0A%0Aint+main+()+%7B%0A++++static_assert(myComparison(2,+2))%3B%0A++++//+static_assert(myComparison(1.0,+1.0))%3B+//+fails+to+compile%0A++++%0A++++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',m:61.491301312840996,n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:clang_trunk,compilerName:'',compilerOutShown:'0',execArgs:'',execStdin:'',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'--std%3Dc%2B%2B20',overrides:!(),runtimeTools:!(),source:1,stdinPanelShown:'1',wrap:'1'),l:'5',n:'0',o:'Executor+x86-64+clang+(trunk)+(C%2B%2B,+Editor+%231)',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

There are multiple ways to define a concept yourself.
If possible one should make use of concepts already available in the C++ standard (e.g. `std::convertible_to`).
Requirements can also be combined via boolean expressions (e.g. `and`, `not`).
See this example:
<iframe width="800px" height="400px" src="https://godbolt.org/e?hideEditorToolbars=true#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:24,endLineNumber:5,positionColumn:24,positionLineNumber:5,selectionStartColumn:5,selectionStartLineNumber:5,startColumn:5,startLineNumber:5),source:'%23include+%3Cconcepts%3E%0A%0Atemplate+%3Ctypename+T%3E%0Aconcept+convertible_to_int_not_double+%3D+%0A++++std::convertible_to%3CT,+int%3E+and+%0A++++(not+std::same_as%3CT,+double%3E)%3B%0A%0Aint+main+()+%7B%0A++++convertible_to_int_not_double+auto+a+%3D+1%3B%0A++++//+convertible_to_int_not_double+auto+b+%3D+1.0%3B++//+fails+to+compile:+double%0A++++//+convertible_to_int+auto+c+%3D+%22hello%22%3B+++++++++//+fails+to+compile:+char*%0A%0A++++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',m:61.491301312840996,n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:clang_trunk,compilerName:'',compilerOutShown:'0',execArgs:'',execStdin:'',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'--std%3Dc%2B%2B20',overrides:!(),runtimeTools:!(),source:1,stdinPanelShown:'1',wrap:'1'),l:'5',n:'0',o:'Executor+x86-64+clang+(trunk)+(C%2B%2B,+Editor+%231)',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

Furthermore, one can employ `require` statements to specify concepts.
The keyword is quite overloaded to work in multiple ways.
Some examples:
<iframe width="800px" height="400px" src="https://godbolt.org/e?hideEditorToolbars=true#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:56,endLineNumber:16,positionColumn:56,positionLineNumber:16,selectionStartColumn:56,selectionStartLineNumber:16,startColumn:56,startLineNumber:16),source:'%23include+%3Cconcepts%3E%0A%0Atemplate+%3Ctypename+T%3E+requires+(sizeof(T)+%3E+sizeof(int))%0Astruct+larger_than_int+:+public+std::true_type+%7B%7D%3B%0A%0Atemplate+%3Ctypename+BI,+typename+EI%3E%0Aconcept+neq_on+%3D+requires(BI+bi,+EI+ei)%7B%0A++++%7Bbi+!!%3D+ei%7D+-%3E+std::convertible_to%3Cbool%3E%3B%0A%7D%3B%0A%0Aint+main+()+%7B%0A++++static_assert(larger_than_int%3Clong%3E%7B%7D)%3B%0A++++//+static_assert(larger_than_int%3Cshort%3E%7B%7D)%3B+//+fails+to+compile%0A%0A++++neq_on%3Cbool%3E+auto+a+%3D+true%3B%0A++++//+neq_on%3Cchar*%3E+auto+b+%3D+true%3B+//+fails+to+compile++++%0A%0A++++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',m:61.491301312840996,n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:clang_trunk,compilerName:'',compilerOutShown:'0',execArgs:'',execStdin:'',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'--std%3Dc%2B%2B20',overrides:!(),runtimeTools:!(),source:1,stdinPanelShown:'1',wrap:'1'),l:'5',n:'0',o:'Executor+x86-64+clang+(trunk)+(C%2B%2B,+Editor+%231)',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

To restrict function templates we can use concepts in multiple ways as well:
<iframe width="800px" height="400px" src="https://godbolt.org/e?hideEditorToolbars=true#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:2,endLineNumber:34,positionColumn:2,positionLineNumber:34,selectionStartColumn:2,selectionStartLineNumber:34,startColumn:2,startLineNumber:34),source:'%23include+%3Cconcepts%3E%0A%0Atemplate+%3Ctypename+BI,+typename+EI%3E%0Aconcept+neq_on+%3D+requires(BI+bi,+EI+ei)+%7B%0A++%7B+bi+!!%3D+ei+%7D+-%3E+std::convertible_to%3Cbool%3E%3B%0A%7D%3B%0A%0Atemplate+%3Ctypename+EI,+neq_on%3CEI%3E+BI%3E+constexpr+bool+fun(BI+bi,+EI+ei)+%7B%0A++return+true%3B%0A%7D%0A%0Atemplate+%3Ctypename+BI,+typename+EI%3E%0A++requires+neq_on%3CBI,+EI%3E%0Aconstexpr+bool+fun_2(BI+bi,+EI+ei)+%7B%0A++return+true%3B%0A%7D%0A%0Atemplate+%3Ctypename+BI,+typename+EI%3E%0Aconstexpr+bool+fun_3(BI+bi,+EI+ei)%0A++requires+neq_on%3CBI,+EI%3E%0A%7B%0A++return+true%3B%0A%7D%0A%0Aconstexpr+bool+fun_4(auto+bi,+neq_on%3Cdecltype(bi)%3E+auto+ei)+%7B+return+true%3B+%7D%0A%0Aint+main()+%7B%0A++static_assert(fun(0,+0))%3B%0A++static_assert(fun_2(0,+0))%3B%0A++static_assert(fun_3(0,+0))%3B%0A++static_assert(fun_4(0,+0))%3B%0A%0A++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',m:61.491301312840996,n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:clang_trunk,compilerName:'',compilerOutShown:'0',execArgs:'',execStdin:'',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'--std%3Dc%2B%2B20',overrides:!(),runtimeTools:!(),source:1,stdinPanelShown:'1',wrap:'1'),l:'5',n:'0',o:'Executor+x86-64+clang+(trunk)+(C%2B%2B,+Editor+%231)',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

### Third Attempt: Concepts For Each Requirement

The following implementation uses seperate concepts for each requirement.
This results in more maintainable/readable code as well as more descriptive error messages.

<iframe width="800px" height="400px" src="https://godbolt.org/e#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGEgKykrgAyeAyYAHI%2BAEaYxCAAzADspAAOqAqETgwe3r4BaRlZAqHhUSyx8cm2mPaOAkIETMQEuT5%2BXIF2mA7Zjc0EpZExcYkpCk0tbfmdtpODYcMVo8kAlLaoXsTI7BzmCWHI3lgA1CYJbmgMO6kECufYJhoAgvuHx5hnF4RxTETEDyerxeb2uHy%2BbmaxCYAE9ASCzAcwV5Tuc3AA3Hr/eHA54ETAsVIGfEQggw1KMVifNw4q43AgnBioAgAfUwAEcvGIWbFgGE2Qx0F8ACInYgcrx4cUKCBuE7IVZnJJWF4nJUWE4TdAgEC8sIQBUnMBgc6irU61wGxUmJKigC0D01BG1IAUVJZTHuF2iqE8gISKtetvOgaB%2BMJxM%2BaLJFOYbBONISjxedMwtxOCE9PMwfIYHsFAqFprFEqlmBlcsNNsDavNupz%2BoVIaBtedFsFVubIODAaBYYJRL%2BUYuMcp8cTyeeqfTh3FbEETGi9GzuZFJc5ZYr8utyrOqqdLvCqgIEDreoYVtWIb3QeFXdx4cHJOj5LH1NpAnpJyw4qocUYyB4Iuy7niyTKsuiqB4EWCSiuKG7SrK27qi266StKjLMgeOpumwHpem4WBHKOEAAFRng2F4KusJyQdB/qhj2oYvI%2Bkakq%2BcbvkmQLTgyoGGOghYsmgqQwsJAgTMQXi9EumD5oJWCSdJjiyWu8HoeWSFVsqqHqZu2EoKgoniQwSkyfQaJEbQJEUbml4MbppYYXWIliVcZkqRZFxWTZbYgJaarUQ5%2B56c5fmKQQUnmZgaJqnF349NZr6nn5572dxvYhU55YGRFUWeTFFzxWqPnJXWAXbleGWMXemUPgObEvrGVIJh%2B1xpgyHgLosxBrpmCgrvyAmFmiE7FeNE2TRNAk3lNc0nLOBKME0smDQwo2OvNW3TYKqHbcVP6YH%2B4rXEBq2geBLJ0egG1Jvt40CXt91qvxBauOJxnuZFyl4Ktw15T9sm3Y8dVAmEDIsEw%2Bo7jWCayFD4S9UwXhECcTBrnWUKwtWXCkCcZg2rVsNdU0PVoyjqAnNEGN%2BZiDgkDjeME0xfb7gA9GzcPdYj5Oo8ga5cNeHMnFQUO0AoJx86ghJ0DF7OcyTCNxLzlMwaK5hmIuphmATAYnMLot0BLUsyxZIJZQQWwMCcGj3oTHDrLQnD%2BLwfgcFopCoJwNKWNYmqbNsUaIjwpAEJoDvrAA1iA/gaPonCSLwLAgAAbFwAB0kgAJxcAkGi51wZgABySBoWdZynpBux7XscLwCggHHYfuw7pBwLASCYKoPQU2QFAQM0wAKMohi1EICCoAA7m7IdoKbfzZCP4S0OPU/V7wc%2BpLL8RHIYwAst9DCR6Qm/bxEVKcBv0tb/QxAAPIo6v0/h0E3fIM8xBD5fr89I0%2BBu7wfgggRBiHYFIGQghFAqHUC3UguhcYGCMCgaw1h9C/QbpAdYRl6imU4HaO0WpTTaysJYMwNdMTEGINBcs8B1hSUEHgNgAAVX04taEbC2DsPQWowhLzHhPZ%2B3BeCT2hKkTgPBHbO1di/Wu2A369xOKoIuKc7Qp0kPKRBwATgQEPpHRUsoUGWDxrgQgJAzjB1WLwZuWhVjrAQJgJgP5KBRxjnHJ2HBE5Vxkd/BuTdw62PjhwMw0jYG1ysf49YFDMjOEkEAA%3D%3D%3D"></iframe>

