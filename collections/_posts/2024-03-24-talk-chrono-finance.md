---
title: 'Talk: std::chrono in finance'
categories:
- C++ Talks
- Finance
tags:
- c++
- CppCon
- chrono
---

Talk: _Using std::chrono Calendar Dates for Finance in Cpp - Daniel Hanson - CppCon 2022_

Resources:
- [video on YouTube](https://www.youtube.com/watch?v=iVnZGqAvEEg)
- [slides](https://github.com/CppCon/CppCon2022/blob/1a54ee18dcfc80b19677a6cc5a1bc374fff70390/Presentations/StdChronoDates_CppCon2022_CORRECTED.pdf)
- [source code](https://github.com/QuantDevHacks/CppCon-2022-C-20-Dates-in-Finance)
- [std::chrono on cppreference](https://en.cppreference.com/w/cpp/chrono)

The introduction of std::chrono to the C++20 standard made date calculation readily available.

His talk starts by a gentle introduction to fixed-income bond pricing.
Then showing some example usage of std::chrono's workhorse `std::chrono::year_month_day`.
Its interface is not as simple as it could be (e.g. invalid dates are possible, adding months might create invalid dates, adding days is not supported).
Thus, he creates a simple wrapper which is available on his github (see above linked "source code").

(I would not directly recommend this video.
A shorter and more on-point introduction to std::chrono can be surely found on web.)
