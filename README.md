# Plasmatum
A minimal functional programming language.

## Under the hood
Plasmatum works by using a stack machine. Here's an example with pseudo stack machine code:
```
FUNC_START      1
-- some c++ implementation
FUNC_FINISH
DEF_GLOBAL      print

-- the '2' specifies the
-- argument count (argc)
FUNC_START      2
ADD
RETURN
FUNC_END
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

-- a function that converts anything to a float
u(x) = x as Float;

-- because Plasmatum does not throw exceptions
-- converting anything to a Float that cannot
-- be converted to a Float creates an Undefined
-- value

```

### variables
```
-- default variable definition (global)
i = 1;

-- integer constant
k := 42;
```


