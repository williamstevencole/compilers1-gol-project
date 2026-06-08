# Gol

A hand-written front end (lexer + recursive-descent parser) for **Gol**, a small
statically-typed teaching language with `int` / `bool` values, first-class
functions, pass-by-reference, and `print` / `println` statements. Built in
modern C++ (C++20) as a compilers course project.

The lexer is a deterministic finite automaton transcribed by hand from a Flex
specification (see `assets/Lexer.l` and the rendered diagram in
`assets/Lexer.svg`). The parser is a top-down recursive-descent recognizer that
follows the EBNF in `docs/gramatica.txt`.

---

## Project layout

```
.
├── assets/                 # Lexer DFA: Flex source, Graphviz, SVG
├── docs/
│   ├── gol-language-spec.pdf
│   └── gramatica.txt       # EBNF grammar consumed by the parser
└── src/
    ├── CMakeLists.txt
    ├── main.cpp            # CLI entry point
    ├── lexer/              # DFA-driven tokenizer
    │   ├── Lexer.hpp
    │   └── Lexer.cpp
    ├── parser/             # Recursive-descent parser
    │   ├── Parser.hpp
    │   └── Parser.cpp
    ├── utils/
    │   └── Tokens.hpp      # TokenID enum and Token struct
    └── example/
        ├── correct/        # 5 programs the parser must accept
        └── incorrect/      # 5 programs the parser must reject
```

---

## Build

Requires CMake ≥ 3.20 and a C++20 compiler (Clang, GCC, or MSVC).

```bash
cd src
cmake -S . -B build
cmake --build build --parallel
```

The executable is produced at `src/build/Gol`.

## Run

The binary takes a single positional argument: the path to a `.txt` source file.

```bash
./build/Gol example/correct/sample1_arithmetic.txt
# → code is valid :D

./build/Gol example/incorrect/bad1_missing_semicolons.txt
# → Syntax error: Expected {;} but got: var
```

On success the program prints `code is valid :D`. On a syntax error it prints
the offending token and the production that failed.

---

## The Gol language at a glance

```
var x int = 5;
var ready bool = true;

func add(a int, b int) int {
    return a + b;
}

func swap(ref a int, ref b int) {
    var tmp int = a;
    a = b;
    b = tmp;
}

func main() {
    result := add(x, 7);          // short variable declaration
    if result >= 10 && ready {
        println("big: ", result);
    } else {
        print("small");
    }
    swap(&x, &result);            // pass by reference
}
```

Highlights:

- Two value types: `int` and `bool`.
- Top-level `var` declarations and `func` definitions.
- Both long (`var x int = 5;`) and short (`x := 5;`) variable declarations.
- `if` / `else if` / `else` chains.
- Pass-by-value by default; `ref` parameters with `&arg` at the call site.
- Logical (`&& || !`), relational (`== != < > <= >=`), and arithmetic
  (`+ - * / %`) operators with the usual precedence.
- `print` / `println` accept any mix of expressions and string literals.
- Line (`//`) and block (`/* ... */`) comments.

The authoritative grammar is in [`docs/gramatica.txt`](docs/gramatica.txt); the
full language specification is in
[`docs/gol-language-spec.pdf`](docs/gol-language-spec.pdf).

---

## Example programs

Each file under `src/example/correct/` exercises a different feature and is
expected to parse cleanly:

| File                          | Exercises                                |
| ----------------------------- | ---------------------------------------- |
| `sample1_arithmetic.txt`      | arithmetic, value parameters, `return`   |
| `sample2_conditionals.txt`    | `if` / `else if` / `else`, short decl    |
| `sample3_booleans.txt`        | `bool`, `&&`, `||`, `!`, relational ops  |
| `sample4_refparams.txt`       | `ref` parameters and `&` at the call site|
| `sample5_recursion.txt`       | recursive functions (factorial, fib, gcd)|

Each file under `src/example/incorrect/` is malformed on purpose and must be
rejected:

| File                            | Intended error                          |
| ------------------------------- | --------------------------------------- |
| `bad1_missing_semicolons.txt`   | missing `;` after statements            |
| `bad2_unbalanced_braces.txt`    | unmatched `{` and `)`                   |
| `bad3_bad_vardecl.txt`          | malformed `var` declarations            |
| `bad4_broken_expressions.txt`   | broken expressions / bad operators      |
| `bad5_bad_func_and_if.txt`      | malformed `func` and `if` constructs    |

A quick smoke test:

```bash
for f in example/correct/*.txt; do
    echo "=== $f ==="
    ./build/Gol "$f"
    echo
done

for f in example/incorrect/*.txt; do
    echo "=== $f ==="
    ./build/Gol "$f"
    echo
done
```

