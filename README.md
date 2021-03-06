Compilers and Language Design: My Compiler

For this class, my semester-long project was to build my own compiler. Similar to C, it is created for a langauge that is safe, statically, and explicitly typed. 

The main components of this project are:

1) The Scanner:

The scanner scans the input file using Flex to return tokens that feed into the parser. It uses regexes to match these tokens and defines the syntax of the language. For example, the keyword 'for' maps to TOKEN_FOR. 

2) The Parser:

The parser parses the tokens obtained from the scanner using Bison to construct the Abstract Syntax Tree (AST). Here is where we define what makes up an EXPR, a STMT, a FUNCTION, etc. The AST is constructed in such a way that the program executes in the proper order.

3) The Type-Checker:

The type-checker confirms that the operations performed by the AST are all type-safe, otherwise it returns helpful error messages to aid in debugging. For example, in a FOR LOOP, the second EXPR must evaluate to a BOOLEAN.

4) The Assembly Generator:

The assembly generator runs once we know our input code is safe and follows the language definition in our grammar. The assembly generator creates the x86 code that the machine processes and stores it in an executable file. The executable can then be run with the output as expected.
