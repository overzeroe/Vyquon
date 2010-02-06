
# The compiler used and the command line arguments to it
COMPILER	= gcc
ARGS		= -Wall -I include/ -g -I/usr/local/include/gdsl
EXECUTABLE	= vyquon
CMDLINK		= ${COMPILER} -lm -L/usr/local/lib/ -lgdsl -o ${EXECUTABLE} ${ARGS}		# Link the .o files into an executable
CMD		= ${COMPILER} -c ${ARGS}				# Don't link, just compile to .o

ALLFILES 	= Lexer.o Mem.o Vyquon.o Parser.o Cons.o String.o Symbol.o Function.o Obj.o Eval.o Stack.o Bytecode.o Scope.o External.o Lib.o IO.o

# Top level rule, compile whole program
all: ${EXECUTABLE}

# Invoke the compiler with linking enabled 
${EXECUTABLE}: ${ALLFILES}
	cd bin; ${CMDLINK} ${ALLFILES}
	mv bin/${EXECUTABLE} .

# Compile all C files into object code
Vyquon.o: 
	${CMD} Vyquon.c -o bin/Vyquon.o

Mem.o: 
	${CMD} mem/Mem.c -o bin/Mem.o

Eval.o: 
	${CMD} vm/Eval.c -o bin/Eval.o
Stack.o: 
	${CMD} vm/Stack.c -o bin/Stack.o
Bytecode.o: 
	${CMD} vm/Bytecode.c -o bin/Bytecode.o
Scope.o: 
	${CMD} vm/Scope.c -o bin/Scope.o




Cons.o: 
	${CMD} types/Cons.c -o bin/Cons.o
String.o: 
	${CMD} types/String.c -o bin/String.o
Symbol.o: 
	${CMD} types/Symbol.c -o bin/Symbol.o
Function.o: 
	${CMD} types/Function.c -o bin/Function.o
Obj.o: 
	${CMD} types/Obj.c -o bin/Obj.o

External.o: 
	${CMD} external/External.c -o bin/External.o
Lib.o: 
	${CMD} external/Lib.c -o bin/Lib.o
IO.o: 
	${CMD} external/IO.c -o bin/IO.o


Lexer.o: 
	${CMD} parser/Lexer.c -o bin/Lexer.o
Parser.o: 
	${CMD} parser/Parser.c -o bin/Parser.o

# Clean out the project and delete all .o files
clean:
	rm -rf bin/*.o 
	rm ${EXECUTABLE}

