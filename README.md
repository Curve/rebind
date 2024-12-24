<p align="center">
  <img src="assets/logo.svg" width="600">
</p>

## 📃 Description

_Rebind_ is a simple, tiny C++23 reflection library for aggregates and enums.  
This library is fully compiler agnostic as it figures out how to demangle given types at compile time.

## 📦 Installation

* Using [CPM](https://github.com/cpm-cmake/CPM.cmake)
  ```cmake
  CPMFindPackage(
    NAME           rebind
    VERSION        5.1.0
    GIT_REPOSITORY "https://github.com/Curve/rebind"
  )
  ```

* Using FetchContent
  ```cmake
  include(FetchContent)

  FetchContent_Declare(rebind GIT_REPOSITORY "https://github.com/Curve/rebind" GIT_TAG v5.0.0)
  FetchContent_MakeAvailable(rebind)

  target_link_libraries(<target> cr::rebind)
  ```

## 📖 Examples

https://github.com/Curve/rebind/blob/d8b79fa7a8fe0fd155f0f7c2f896052649210b26/tests/name.test.cpp#L9-L11
https://github.com/Curve/rebind/blob/d8b79fa7a8fe0fd155f0f7c2f896052649210b26/tests/enum.test.cpp#L22-L33
https://github.com/Curve/rebind/blob/d8b79fa7a8fe0fd155f0f7c2f896052649210b26/tests/member.test.cpp#L37-L45

> 🧪 For more examples see [tests](tests/)

## 📋 Documentation

```cpp
template <typename T>
static constexpr auto type_name = /*...*/;
```

> **Returns**: Human readable name of type `T`

```cpp
template <auto T>
static constexpr auto nttp_name = /*...*/;
```

> **Returns**: Human readable name of the given nttp `T`

---

```cpp
template <typename T>
static constexpr auto arity = /*...*/;
```

> **Returns**: Count of members of `T`

```cpp
template <typename T>
constexpr auto to_tuple(T &value);
```

> **Returns**: A tuple of references to all members of the given `value`

---

```cpp
template <typename T>
static constexpr auto members = /*...*/;
```

> **Returns**: A `rebind::member` object for each member present in the aggregate  
> **Contains**: The `type`, `name` and `index`

```cpp
template <auto T>
static constexpr auto member_name = /*...*/;
```

> **Returns**: The member name of the given member pointer

---

```cpp
template <auto T>
static constexpr auto enum_name = /*...*/;
```

> **Returns**: The name of the given enum value  
> **Note**: To get the name of an enum-type use `rebind::type_name` instead

```cpp
template <typename T>
static constexpr auto enum_values = /*...*/;
```

> **Returns**: All values of the given enum `T` (specialize `rebind::search_min` / `rebind::search_max` to tweak the search range)  
