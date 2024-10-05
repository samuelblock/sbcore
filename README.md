# sbcore
Sbcore is an easy-to-use boilerplate header that I use in all of my C projects. It aims to modernize the C language via its preprocessor and provide users with much-needed quality of life features.

`!!WARNING!!` It is important to note that all variables containing only dollar signs ($) are reserved for sbcore. Using them in your codebase may cause compilation errors or worse: uncaught logical errors.

## Configuration
Sbcore can be configured by defining macros before including the header. Below is a list of every configurat
ion option in no particular order:

| Macro                     | Function                                           |
|---------------------------|----------------------------------------------------|
| SBCORE\_NO\_INTEGERS      | Removes all `u*_t` and `i*_t` types                |
| SBCORE\_NO\_FLOATS        | Removes all `f*_t` types                           |
| SBCORE\_NO\_BOOLEAN       | Removes the `bool_t` type and associated constants |
| SBCORE\_NO\_STRING        | Removes the `str_t` type and all `str_*` functions |
| SBCORE\_NO\_CONTROL\_FLOW | Removes the `loop`, `until`, and `defer` keywords  |
| SBCORE\_IMPLEMENT\_STRING | Implements all `str_*` functions                   |
| SBCORE\_DONT\_OVERLOAD    | Removes function overloading on argument count     |
