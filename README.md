# sbcore.h - Samuel Block's Core Header
Sbcore is an easy-to-use boilerplate header that I use in all of my C projects. It aims to modernize the C language via its preprocessor and provide users with much-needed quality of life features. Users may choose to either include separate headers for each of sbcore's features or configure the single `sbcore.h` header with macros.

**WARNING:** It is important to note that all variable names containing only dollar signs ($) are reserved for sbcore. Using them in your codebase may cause compilation errors or unintended--and very hard to debug--behaviors.

## Integer Types
One of sbcore's most foundational features are its integer types. Sbcore provides users with more concise, Rust-like versions of many types found in `stdint.h` and similar standard headers. They are as follows:

|Type   |Standard Equivalent|Description                                   |
|-------|-------------------|----------------------------------------------|
|u8\_t  |uint8\_t           |An 8-bit, unsigned integer                    |
|i8\_t  |int8\_t            |An 8-bit, signed integer                      |
|u16\_t |uint16\_t          |A 16-bit, unsigned integer                    |
|i16\_t |int16\_t           |A 16-bit, signed integer                      |
|u32\_t |uint16\_t          |A 32-bit, unsigned integer                    |
|i32\_t |int16\_t           |A 32-bit, signed integer                      |
|u64\_t |uint16\_t          |A 64-bit, unsigned integer                    |
|i64\_t |int16\_t           |A 64-bit, signed integer                      |
|uptr\_t|uintptr\_t         |An unsigned integer of equal size to a pointer|
|iptr\_t|intptr\_t          |A signed integer of equal size to a pointer   |
|ulen\_t|size\_t            |An unsigned integer used for lengths and sizes|
|ilen\_t|ssize\_t           |A signed integer used for lengths and sizes   |

These types are referenced throughout the rest of sbcore, so some features may not compile if this one is not included. This feature can be disabled by defining the `SBCORE_NO_INTEGERS` macro before including the header.

## Floating-Point Types
Similarly, sbcore also provides alternate floating-point types. They are as follows:

|Type   |Description                                       |
|-------|--------------------------------------------------|
|f32\_t |A 32-bit, single-precision floating-point number  |
|f64\_t |A 64-bit, double-precision floating-point number  |
|fptr\_t|A floating-point number of equal size to a pointer|

Unlike the integers, however, equivalent floating-point types are not provided in the C standard library. So, the `SBCORE_F32_TYPE`, `SBCORE_F64_TYPE`, and `SBCORE_FPTR_TYPE` macros may be defined for manual configuration in case the default value(s) are incorrect on your system.

This feature can be disabled by defining the `SBCORE_NO_FLOATS` macro before including the header.

## Booleans
Sbcore provides its own definition of a boolean type in order to have one that aligns with its naming conventions. The `bool_t` type is 8 bits in size as has two constants associated with it: `TRUE` and `FALSE`, which are defined as boolean-casted number literals.

This feature can be disabled by defining the `SBCORE_NO_BOOLEAN` macro before including the header.

## Strings
What makes up the greatest portion of the `sbcore.h` header is its implementation of the `str_t` type. All sbcore strings may be passed to any function that accepts a regular, C-style string and operate without problem, so long as it is never reallocated. Despite this, however, they are much faster. That is because bscore strings are layed out as follows:
```
+-----------------+-------------------+-----------------------+------------------------+
| Length (ulen_t) | Capacity (ulen_t) | String data (char...) | Null terminator (char) |
+-----------------+-------------------+-----------------------+------------------------+
```
A string's "length" is how much of the allocated space is actually taken up by intentional data. A string's "capacity" is how much space has been allocated for that data, regardless of its length. This is done to allow for strings to grow in large steps, reducing the number of calls to `realloc` in loops and other repetitive scenarios. The pointer returned from `str_make` points to the first character of the string's data, not its header. This allows for zero-based indexing but can complicate memory management. After the string data, an extra byte is allocated to ensure there is always enough space for the null terminator.

After every call to any* sbcore string function, it is garunteed that `string[str_length(string)] == 0`, meaning it is always safe to pass as a regular, C-style string. To achieve this, however, any function that has the possibilty of reallocating the string returns its potentially new location.

```c
str_t str_make(char* c_string, ulen_t n)
```
Creates a string from some C-style string literal and length.

```c
str_t str_make_empty(ulen_t n)
```
Creates am empty string of some length.

```c
str_header(str_t string)
```
The base address of some string.

```c
str_length(string)
```
Some string's length field.

```c
str_capacity(string)
```
Some string's capacity field.

```c
str_free(string)
```
Calls `free` on some string's base pointer.

```c
str_t str_resize(str_t string, ulen_t n)
```
Reallocates some string and updates its capacity field. **Warning:** This function assumes `n` is greater than the string's length. If it isn't, users will manuall have to re-insert a null terminator and update its length field.

```c
str_fit(string)
```
Realocates some string such that it is just big enough to perfectly contain its data.

```c
str_t str_append_char(str_t string, char ch)
// OR
str_t str_append(str_t string, char ch)
```
Appends some character to some string, reallocating it as needed.

```c
str_t str_append_string(str_t string, char* appendage, ulen_t n)
// OR
str_t str_append(str_t string, char* appendage, ulen_t n)
```
Appends some number of characters from a C-style string to a sbcore string.

```c
str_concat(string, appendage)
```
Concatenates two strings.

```c
char str_pop(str_t string);
```
Pops a single character from the end of some string and returns it. Returns `0` if the string is empty.

By default, `sbcore.h` does not include the implementations of these functions. To do so, define the `SBCORE_IMPLEMENT_STRING` macro before including the header. This feature can be disabled by defining the `SBCORE_NO_STRING` macro before including the header. Function overloading can be disabled by defining the `SBCORE_DONT_OVERLOAD` macro before including the header.

## Control Flow Keywords
Sbcore provides three additional keywords that aid in controlling the flow of logic.

```c
loop { ... }
until(condition) { ... }
defer(statement_a, statement_b, ...) { ... }
```

The `loop` keyword loops its body indefinitely until it is broken. The `until` keyword loops its body until some condition is met (the opposite of a while loop). Finally, `defer` executes any number of comma-separated statements after its body has finished execution on the current thread.

