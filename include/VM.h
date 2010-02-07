#ifndef VM_H
#define VM_H

/* This file defines the interface to the Vyquon virtual machine.
 * At the moment, the virtual machine is a double pass interpreter -
 * it converts the parsed syntax tree into a bytecode, then loops
 * over the bytecode instructions and executes each one. This should
 * be faster than just a single pass interpreter, but we could make it
 * even faster by compiling at runtime to assembly (JIT). Also, we could
 * increase number of bytecode instruction types to decrease overhead for
 * each one. 
 */

/* Evaluate a syntax tree (internally, it compiles and then evaluates) */
VyObj Eval(VyObj);

/* A single instruction for the bytecode. Contains the opcode (type of
 * instruction) as well as associated data. Though the associated data is
 * stored as a VyObj, this is a lie, as sometimes we just hide an integer or
 * void* inside that and get it out. 
 */
typedef struct _Instruction {
    int opcode;
    union {
        VyObj obj;
        int num;
    } data;
} Instruction;

/* The bytecode for the interpreter. It is a dynamically allocated
 * growing array of instructions. It contains a pointer to the first
 * instruction, the current size of the instruction buffer, and how
 * many instructions are in the buffer. It is expanded dynamically
 * as instructions are added. Use the EmitInstruction() function to
 * add an instruction onto the end of a bytecode.
 */
typedef struct _Bytecode {
    int size;
    int used;
    Instruction* instructions;
} Bytecode;

/* Compile a parse tree (a list) to bytecode */
Bytecode* Compile(VyObj);

/* Evaluate the bytecode to produce an object */
VyObj EvalBytecode(Bytecode*);

/* The current virtual machine is stack based, meaning that it 
 * uses a stack to manipulate it's data. This is not like a real CPU;
 * those, while they do have stacks, also have a number of registers.
 * Register based virtual machines can speed up execution, but for now
 * this is a stack-based VM. These functions push an element on to the stack,
 * take the top one off and return it, and just return the top one without
 * taking it off the stack (respectively) 
 */
void StackPush(VyObj);
VyObj StackPop();
VyObj StackPeek();


/* Instructions for the stack-based VM */
#define INSTR_POP    10      /* Pop a value off the stack (associated data: none) */
#define INSTR_PUSH   20      /* Push a value onto the stack (associated data: what value to push onto the stack) */
#define INSTR_CALL   30      /* Call a function (associated data: # of arguments passed) */
#define INSTR_IFNJMP 40      /* Conditional jump (jump if top value on stack is false). (associated data: which instruction to jump to) */
#define INSTR_JMP    50      /* Unconditional jump. (associated data: which instruction to jump to) */
#define INSTR_VALUE  60      /* Find what the symbol on the top of the stack is bound to. (associated data: none) */
#define INSTR_BIND   70      /* Bind the symbol on the top of the stack to the value below the symbol (associated data: none) */
#define INSTR_FUNC   80      /* Create a function from the function object on the stack. (associated data: none) */

#endif /* VM_H */
