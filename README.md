<p align="center">
  <img src="assets/logo.svg" width="600">
</p>

## 📃 Description

_Rebind_ is a simple, tiny C++20 reflection library for aggregates and enums.  
This library is fully compiler agnostic as it figures out how to demangle given types at compile time.

## 📦 Installation

* Using [CPM](https://github.com/cpm-cmake/CPM.cmake)
  ```cmake
  CPMFindPackage(
    NAME           rebind
    VERSION        3.0.0
    GIT_REPOSITORY "https://github.com/Curve/rebind"
  )
  ```

* Using FetchContent
  ```cmake
  include(FetchContent)

  FetchContent_Declare(rebind GIT_REPOSITORY "https://github.com/Curve/rebind" GIT_TAG v3.0.0)
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
template <auto T>
static constexpr auto arity = /*...*/;
```

> **Returns**: Count of members of `T`

```cpp
template <typename T>
    requires std::is_aggregate_v<T>
constexpr auto to_tuple(T &value);
```

> **Returns**: A tuple of references to all members of the given `value`

```cpp
template <std::size_t I, typename T>
    requires std::is_aggregate_v<T>
constexpr auto& get(T &value);
```

> **Returns**: A reference to the nth member of `value`

---

```cpp
template <typename T, std::size_t I>
static constexpr auto inspect = /*...*/;
```

> **Returns**: Metadata on the nth member of `T`  
> **Contains**: The members `name`, `type`

```cpp
template <auto T>
    requires std::is_member_pointer_v<decltype(T)>
static constexpr auto member_name = /*...*/;
```

> **Returns**: The name of the given member pointer `T`  

```cpp
template <typename T>
static constexpr auto member_names = /*...*/;
```

> **Returns**: An array containing the names of all members of `T`  

```cpp
template <typename T, typename Fn>
    requires std::is_aggregate_v<T>
constexpr auto visit(T &value, Fn &&callback);
```

> **Brief**: Calls `callback` with all members of `value`  
> **Note**: The passed `callback` receives two arguments, the first being a reference to the member and the second being the metadata on the member _(as returned by `rebind::inspect`)_

---

```cpp
template <auto T>
static constexpr auto enum_name = /*...*/;
```

> **Returns**: The name of the given enum value  
> **Note**: To get the name of an enum-type use `rebind::type_name` instead

```cpp
template <typename T>
static constexpr auto enum_fields = /*...*/;
```

> **Returns**: All possible entires of the given enum `T`  
> **Contains**: The enum `value` and `name`

```cpp
template <typename T>
    requires std::is_enum_v<T>
constexpr std::optional<std::string_view> find_enum_name(T value);
```

> **Returns**: The enum name corresponding to the given value

```cpp
template <typename T>
    requires std::is_enum_v<T>
constexpr std::optional<T> find_enum_value(std::string_view name)
```

> **Returns**: The enum value corresponding to the given name
