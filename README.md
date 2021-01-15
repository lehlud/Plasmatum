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
### true-false
```js
true
false
```
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
#### not
```python
not expr1
```
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
```js
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
while condition ? (
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
for i=0 -> value (
    ... code here
)
```
```php
for i -> 29 (
    ... code runs 30 times
)
```
```php
for i=1 -> 30 (
    ... code runs 30 times
)
```

## data structures
### declaration
```python
ddef id (attr1, attr2, attr3)
```
### instances
```python
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
