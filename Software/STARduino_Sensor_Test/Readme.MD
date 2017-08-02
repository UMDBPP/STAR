STARduino Coding Guidelines
===========================

For this project, we've decided to have some coding guidelines to make writing code easier for multiple developers.  
For the most part, these will be unintrusive to your personal code, and will only affect how your code interacts with
other code in the project. That is not to say you can do whatever in your own code and have it be okay. I *strongly*
suggest trying to remain as internally consistent as possible.

I will break this document down into "rules", "guidelines", and "suggestions".  
**Rules** *must* be followed at all times in the code. These are things that will keep the codebase clean and usable
by everyone.  
**Guidelines** are things that *should* be followed, but can be broken when the alternative is significantly more
clumsy/difficult/infeasible.  
**Suggestions** are just things I find helpful, and do not need to be heeded.

This document will be updated if more guidelines are determined necessary.

Rules
-----

- All function prototypes (including those for class methods) must use the following convention: 
`void function_name_with_spaces(int arg1, int arg2);`
    - That is to say, function names shall use words/acronyms separated by spaces.
    - For clarity, return types need not be `void` and argument types need not be `int`.
- All functions (including class methods) must be clearly documented with block comments of the 
following form in their _definition_ (.cpp file):

        /*
         * Short description of function's purpose, up to three lines
         *
         * Inputs:
         * arg1: description of argument
         * arg2: description of argument
         *
         * Output:
         * var_name: description of output
         *
         * Return:
         * Meaning of returned value (optional)
         */
- All classes/structures must be named as follows: `class/struct SomeClass { ... };`, i.e. CapitalCamelCase.
- All classes/structures must be preceded with a block comment of the following form in their _declaration_ (.h file):

        /*
         * Description of class/struct purpose, up to three lines
         */
- All macros (`#define` statements) must be preceded with a single-line comment describing them.
    - Blocks of very similar macros may be grouped together, with a single comment preceding all of them.
- All macro names must be in all caps
- All macro names must contain, as a prefix, what system they are part of (i.e. `#define SERIAL_DEBUG` or `#define PIN_SYNC_PULSE`)
- All header files must be wrapped in include guards, i.e.

        #ifndef MY_HEADER
        #define MY_HEADER

        // Header file content

        #endif // MY_HEADER

Guidelines
----------

- Code should avoid use of floating point numbers (`float` and `double`) whenever possible.
    - Microcontroller doesn't have hardware float support, so these operations are very slow; we need the performance!
- Dynamic memory allocation (i.e. `new`, `delete`, `malloc()`, and `free()`) should be avoided whenever possible
    - Stack/heap collisions are very possible when you don't have an operating system to check for these things. Be careful.
- Function definitions should differentiate between function arguments, class members, and local variables
    - i.e. arguments are written as `int _arg1`, class members are `int classMember`, local variables are `int localvariable_`
        - function arguments are the inputs to the function; suggestion is prefixing these with an underscore
        - class members are variables accessible to the entire class you are working in (if you are using a class); suggestion
        for these is lowerCamelCase
        - local variables are variables that are only within the function you are currently writing; suggestion for these
        is entirely lowercase names followed by a trailing underscore
    - You may use your own convention, so long as it is internally consistent
- Class member variables should be private, and values set/read through setter/getter member functions
    - Setters and getters should typically be named as such: `int get_member()` or `void set_member(int _newVal)`
- Global variables should be avoided when possible.
    - If global variables *are* needed, naming suggestion is all caps with a trailing underscore: `int MY_GLOBAL_VAR_`
- Single-line loops and conditionals should be avoided for code readability
- Files should contain a single blank line at the end (this is just to keep git from complaining.)

Suggestions
-----------

- Maintain an internally consistent coding style (beyond what is stipulated).
    - It will make re-reading and debugging your own code *much* simpler!
- Define local variables at the top of a function definition.
- Write the key algorithms in your code such that they can be compiled and tested on a desktop machine.
    - This allows you to test at least small parts of the code without an Arduino/STARduino
