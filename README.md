# Plasmatum
A minimal functional programming language.

## Under the hood
Plasmatum works by using a stack machine. Here's an example with pseudo stack machine code:
```
-- add (x y) = x + y
FUNC_START      2
ADD
RETURN
FUNC_FINISH
DEF_GLOBAL      add

-- print(add(40 2))
LOAD_CONST      40
LOAD_CONST      2
LOAD_GLOBAL     add
CALL            2
LOAD_GLOBAL     print
CALL            1
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


