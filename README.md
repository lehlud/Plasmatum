# Plasmatum
A minimal functional programming language.

## Under the hood
Plasmatum works by using a stack machine. Here's an example with pseudo stack machine code:
```
FUNC_START      1
-- some c++ implementation
FUNC_END
DEF_GLOBAL      print

LOAD_CONST      40
LOAD_CONST      2
ADD

LOAD_GLOBAL     print
CALL
```

output:
```
42
```


## language specification
### functions
```plsm
-- quadratic function
f(x) = x ** 2;

-- quadratic function with a higher slope
g(x) = 2 * f(x);
```

### variables
```
-- default variable definition (global)
i = 1;

-- integer constant
k := 42;
```


