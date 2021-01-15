# Plasmatum
A minimal functional programming language.

# Default values
## empty pointer
```js
null
```
## number
### definition per regular expression
```bison
([0-9]+)?"."?[0-9]+
```
### examples
```js
10
12.631
.0834
```
## expressions
### logical expressions
#### value equality
```js
a == b
```
#### instance equality
```js
a === b
```
#### greater than
```js
a > b
```
#### greater than or equal
```js
a >= b
```
#### smaller than
```js
a < b
```
#### greater than or equal
```js
a >= b
```

### connecting logical expressions
#### and
```python
expr1 and expr2
```
#### or
```python
expr1 or expr2
```
#### xor
```python
expr1 xor expr2
```


## variables
### declaration (normal)
```clojure
def id = value
```
### declaration (with data type)
```clojure
def (data type) id = value
```
### assignment
```dart
set id = value
```
### deletion
```dart
undef id
```
### constant values
```clojure
def id := value
```

## functions
### declaration (single line)
```clojure
fdef id on [a, b, c] -> result
```
### declaration (multi line)
```clojure
fdef id on [a, b, c] -> (
    ... code here
) -> result
```
### function calls
```clojure
id on (a, b, c)
```

## branches
### if (standalone)
```
condition ? (
    ... code if true
)
```
### if else
```
condition ? (
    ... code if true
) : (
    ... code if false
)
```
### while (standalone)
```
condition ? (
    ... code if true
)
```
### while-else
```
while condition ? (
    ... code if true
) : (
    ... code if false (once)
)
```
```
while condition ? (
    ... code if true
) :: (
    ... code if false (once, but also when condition gets false)
)
```
### for
```php
for i -> value (
    ... code here
)
```
```php
def x = 29
def y = x + 1
for i -> x (
    ... code runs y times
)
```

## data structures
### declaration
```clojure
ddef id (attr1, attr2, attr3)
```
### instances
```clojure
id from (attr1=a, attr2=b)
```
#### getting attributes of instances
```
attr1 of id
```

## console output
```php
echo value
```
