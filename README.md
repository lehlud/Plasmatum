# Plasmatum
A minimal functional programming language.

## Under the hood
Plasmatum works by using a stack machine. Here's an example with pseudo stack machine code:
```
-- add (x y) = x + y
FUNC_START      1
LOAD_ARG        0   -- 0
LOAD_CONST      1   -- 1
GT                  -- 2
JUMP_COND       6   -- 3    >>
LOAD_ARG        0   -- 4
RETURN              -- 5
LOAD_ARG        0   -- 6    <<
LOAD_CONST      1   -- 7
SUB                 -- 8
LOAD_GLOBAL     fib -- 9
CALL            1   -- 10
LOAD_ARG        0   -- 11
LOAD_CONST      2   -- 12
SUB                 -- 13
LOAD_GLOBAL     fib -- 14
CALL            1   -- 15
ADD                 -- 16
RETURN              -- 17
FUNC_FINISH
DEF_GLOBAL      fib

FUNC_START      1
LOAD_ARG        0   -- 0
LOAD_CONST      2   -- 1
MOD                 -- 2
LOAD_CONST      0   -- 3
NE                  -- 4
JUMP_COND       10  -- 5    >>
LOAD_ARG        0   -- 6
LOAD_CONST      2   -- 7
DIV                 -- 8
RETURN              -- 9
LOAD_ARG        0   -- 10   <<
LOAD_CONST      3   -- 11
MUL                 -- 12
LOAD_CONST      1   -- 13
ADD                 -- 14
RETURN              -- 15
FUNC_FINISH
DEF_GLOBAL      collatz_step

-- print(collatz_step(7))
LOAD_CONST      7
LOAD_GLOBAL     collatz_step
CALL            1
LOAD_GLOBAL     print
CALL            1

-- print(fib(25))
LOAD_CONST      25
LOAD_GLOBAL     fib
CALL            1
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


