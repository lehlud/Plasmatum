# Plasmatum
A minimal functional programming language.

### data types
```
byte
bool
number
```

### empty pointer
```js
null
```
### number
#### definition per regular expression
```bison
([0-9]+)?"."?[0-9]+
```
#### examples
```js
10
12.631
.0834
```
### expressions
#### logical expressions
##### true-false
```js
true
false
```
##### value equality
```js
a == b
```
##### instance equality
```js
a === b
```
##### greater than
```js
a > b
```
##### greater than or equal
```js
a >= b
```
##### smaller than
```js
a < b
```
##### smaller than or equal
```js
a <= b
```

#### connecting logical expressions
##### not
```python
not expr
```
##### and
```python
expr1 and expr2
```
##### or
```python
expr1 or expr2
```
##### xor
```python
expr1 xor expr2
```

### comments
```
... this is a single-line comment
../ this is a delimited comment /..
```
### variables
#### declaration (normal)
```clojure
def id = value
```
#### declaration (with data type)
```clojure
def (data type) id = value
```
#### assignment
```dart
set id = value
```
#### deletion
```dart
undef id
```
#### constant values
```clojure
def id := value
```

### functions
#### declaration (single line)
```clojure
fdef id on [a, b, c] -> result
```
#### declaration (multi line)
```clojure
fdef id on [a, b, c] -> (
    ... code here
) -> result
```
#### function calls
```js
id on (a, b, c)
```

### branches
#### if (standalone)
```js
condition ? (
    ... code if true
)
```
#### if else
```js
condition ? (
    ... code if true
) : (
    ... code if false
)
```
#### if else-if else
```js
cond1 ? (
    ... code if true
) : cond2 ? (
    ... code if cond1 is false and cond2 is true
) : (
    ... code if cond1 and cond2 are false
)

... just two if-else statements merged together
```
#### while (standalone)
```js
while condition ? (
    ... code if true
)
```
#### while-else
```js
while condition ? (
    ... code if true
) : (
    ... code if false (once)
)
```
##### examples
```js
while condition ? (
    ... code if true
) :: (
    ... code if false (once, but also when condition gets false)
)
```
```js
cond1 ? (
    ... code if true
) : while cond2 ? (
    ... code if false (once)
)
```
#### for
```js
for i -> value (
    ... code here
)
```
```js
for i=default -> value (
    ... code here
)
```
```js
for i -> value (
    ... code here
)
```
##### examples
```js
for id -> 29 (
    ... code runs 30 times
)
```
```js
for id=1 -> 30 (
    ... code runs 30 times
)
```

### data structures
#### declaration
```js
ddef id (attr1, attr2, attr3)
```
```js
ddef id (attr1, (data type) attr2, attr3)
```
#### instances
```js
id from (attr1=a, attr2=b)
```
##### getting attributes of instances
```js
attrName of id
```

### console output
```php
echo value
```
