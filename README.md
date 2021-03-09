# Plasmatum
A minimal functional programming language.

### comments
```
... this is a single line comment
../ this is a delimited comment /..
```

### data types
```
bool
char
number
```

### empty pointer
```
null
```
### number
Numbers are only stored as doubles in memory. This should make computing easier on 64-bit machines without any real drawbacks.
#### definition per regular expression
```
"-"?[0-9]*"."?[0-9]+
```
#### examples
```
10
12.631
.0834
```
### expressions
#### logical expressions
##### true-false
```
true
false
```
##### value equality
```
a == b
```
##### greater than
```
a > b
```
##### greater than or equal
```
a >= b
```
##### smaller than
```
a < b
```
##### smaller than or equal
```
a <= b
```

#### connecting logical expressions
##### not
```
not expr
```
##### and
```
expr1 and expr2
```
##### or
```
expr1 or expr2
```
##### xor
```
expr1 xor expr2
```

### variables
#### declaration/assignment (normal)
```
def id = value
```
#### deletion
```
undef id
```

### functions
#### declaration (single line)
```
fdef id on [a, b, c] -> result
```
#### declaration (multi line)
```
fdef id on [a, b, c] -> (
    ... code here
) -> result
```
#### function calls
```
id on (a, b, c)
```

### branches
#### if (standalone)
```
condition ? (
    ... code if true
)
```
#### if else
```
condition ? (
    ... code if true
) : (
    ... code if false
)
```
#### if else-if else
```
cond1 ? (
    ... code if true
) : cond2 ? (
    ... code if cond1 is false and cond2 is true
) : (
    ... code if cond1 and cond2 are false
)
```
#### while loop
```
while condition ? (
    ... code if true
)
```

#### for loop
```
for i -> max (
    ... code here
)
```
```
for i=default -> max (
    ... code here
)
```
##### examples
```
for id -> 41 (
    ... code runs 42 times
)

for id=1 -> 42 (
    ... code runs 42 times
)

for id=1 -> 84 (
    set id = id + 1
    ... code runs 42 times
)
```

### data structures
#### declaration
```
ddef id (attr1, attr2, attr3)
ddef id (attr1, (data type) attr2, attr3)
```
#### instances
```
id from (attr1=a, attr2=b)
```
##### getting attributes of instances
```
attrName of id
```

### console output
```
echo expr                   ... this produces a new line
stdout expr                 ... this doesn't
```

### file output
```
echo expr > filename        ... write to file
echo expr >> filename       ... append to file

stdout expr > filename      ... write to file
stdout expt >> filename     ... append to file
```
