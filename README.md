# Plasmatum
A minimal functional programming language.

## Under the hood
Plasmatum works by using a stack machine. Here's an example with pseudo stack machine code:
```
FUNC_START      1
---------------------------
-- some c++ implementation
-- to print out something
-- to the console
---------------------------
LOAD_CONST      Undefined
RETURN
FUNC_FINISH
DEF_GLOBAL      print

-- the '2' specifies the
-- argument count (argc)
FUNC_START      2
ADD
RETURN
FUNC_FINISH
DEF_GLOBAL      add

LOAD_CONST      40
LOAD_CONST      2
LOAD_GLOBAL     add
CALL

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

-- a function that converts anything to a 'Float'
u(x) = x as Float;

-- Because Plasmatum does not throw any errors,
-- everything that should be converted to a
-- 'Float' but cannot be converted to a 'Float'
-- becomes 'Undefined'.
```

### variables
```
-- default variable definition (global)
i = 1;

-- integer constant
k := 42;
```


