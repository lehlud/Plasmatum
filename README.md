---
author: Ludwig Lehnert
---

# Plasmatum
A minimal functional programming language.


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
)
```
### while (standalone)
```
condition ? (
    ... code if true
) : (
    ... code if false
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
) : (
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
