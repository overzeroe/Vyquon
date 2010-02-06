#ifndef VM_H
#define VM_H

VyObj Eval(VyObj);

typedef struct _Instruction Instruction;
struct _Instruction {
    int opcode;
    VyObj data;
};

typedef struct _Bytecode Bytecode;
struct _Bytecode {
    int size;
    int used;
    Instruction* instructions;
};

Bytecode* Compile(VyObj);
VyObj EvalBytecode(Bytecode*);

/* VM Stack manipulation */
void StackPush(VyObj);
VyObj StackPop();
VyObj StackPeek();


/* Instructions for the stack-based VM */
#define INSTR_POP   10
#define INSTR_PUSH  20
#define INSTR_CALL  30
#define INSTR_IFNJMP 40
#define INSTR_JMP   50
#define INSTR_VALUE 60
#define INSTR_BIND  70
#define INSTR_FUNC  80

#endif /* VM_H */
