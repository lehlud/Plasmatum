# Plasmatum
A minimal functional programming language.


## language specification
### functions
```plsm
-- quadratic function
f(x) = x ** 2;

-- quadratic function with specified data type
g(x:Int) = x ** 2;
```

### variables
```
-- default variable definition (automatic data type)
i = 1;

-- integer constant
k := 42;

-- integer variable
v:Int = 1337;

-- undefined integer variable
u:Int = undefined;
```


