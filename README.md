# GlyphoInterpreter

Interpreter for the [Glypho](https://github.com/TryItOnline/glypho?tab=readme-ov-file)
esoteric programming language with some extra features:
 - integer numbers of arbitrary lengths
 - any number base from 2 to 35 (digits for bases after 10 are the letters from
   the english alphabet) 

### Building & Testing

The included `Makefile` will generate the executable called `glypho-interpreter`
in the project root by running:

```sh
make build
```

The project requires a compiler that supports C++ 17 standard. There are no extra
dependencies. 

The included tests can be run with:

```sh
make test
```

Each test has a Glypho source file (`.gsh`) according to the
[instruction set](https://github.com/TryItOnline/glypho?tab=readme-ov-file#instruction-set)
and a file where instructions are randomly encoded int symbol strings of length
four (`.gly`).

### Using the program

The interpreter expects at least one argument, the path of the `.gly` file, and
optionally a base for the numbers (default is decimal):

```sh
    ./glypho-interpreter gly_file_path [base] 
```
