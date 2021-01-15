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

## data structures
### declaration
```clojure
ddef id (attr1, attr2, attr3)
```
### instances
```clojure
id from (attr1=a, attr2=b)
```

## branches
### if (standalone)
```
condition
? ... code if true
```
```
condition
? ... code if true
: ... code if false
```
### if else
```
condition
? ... code if true
: ... code if false
```
### while
```
while condition
? ... code if true
```
```
while condition
? ... code if true
: ... code if false

... breaking required!
```

### for
```
for i -> value (
    ... code here
)
```
```
for i -> 29 (
    ... code runs 30 times
)
```
