# Plasmatum
Plasmatum aims to be a kind of functional programming language. Fundamentally it is state-independent
and therefore does not allow any kind of global variables. Also, Plasmatum is a compiled language.

## Why
Haskell is already pretty neat. So why do you need yet another similar programming language?
Well, first of all: this language is not like Haskell. It aims to provide code that will be way
easier to read and to understand on a first look. Also, it aims to be simpler and will be using
easier to understand concepts, especially when it comes to data structures.

## Language specification
### Constants
```plsm
../ These are all constant values.
 .. They are defined using the
 .. 'define-as' syntax /..

../ quadratic function /..
define f as (x) => x ** 2;

../ steeper quadratic function
define k as 2;
define g as (x) => k * f(x);

../ Why the double arrow though?
 .. Well, the function not only points
 .. to the result, but the result follows
 .. from x. /..
```


