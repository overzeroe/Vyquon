#include "Vyquon.h"

/* Eval front-end: compile to bytecode, then evaluate the bytecode */
VyObj Eval(VyObj sexp){
    Bytecode* bytecode = Compile(sexp);
    VyObj result = EvalBytecode(bytecode);
    return result;
}

/* Instructions */
void PushInstr(VyObj);
void PopInstr();
void BindInstr();
void ValueInstr(VyObj);
void FuncInstr();
void CallInstr(int);
int IfJmpInstr(VyObj);

/* Loop over instructions and evaluate them */
VyObj EvalBytecode(Bytecode* bytecode){
    /* The way jumps work is they set the nextInstr ptr to something, and then on the start
     * of the next loop, we actually jump there, and reset the ptr to nothing. When it's 
     * negative, we don't care, when it's positive, we jump. */
    int nextInstr = -1;

    int i;
    for(i = 0; i < bytecode->used; i++){

        /* Execute jumps from the previous instruction */
        if(nextInstr >= 0){
            i = nextInstr;
            nextInstr = -1;
        }

        /* Current instruction */
        Instruction instr = bytecode->instructions[i];

        /* We can't declare variables inside switch statements. This one is for the CALL opcode */
        int args;

        /* Just call the appropriate function for each opcode */
        switch(instr.opcode){
            case INSTR_PUSH:
                /* Data: what we are pushing */
                PushInstr(instr.data);
                break;
            case INSTR_POP:
                PopInstr();
                break;
            case INSTR_BIND:
                BindInstr();
                break;
            case INSTR_VALUE:
                /* Data: the variable name */
                ValueInstr(instr.data);
                break;
            case INSTR_FUNC:
                FuncInstr();
                break;
            case INSTR_CALL:
                /* Data: how many args are being passed */
                args = (int)Obj(instr.data);
                CallInstr(args);
                break;

            /* The jump instructions modify the jump ptr; we actually do the jump next iteration around. */
            case INSTR_JMP:
                nextInstr = (int)Obj(instr.data);       
                break;
            case INSTR_IFNJMP:
                nextInstr = IfJmpInstr(instr.data);

            /* If it's another opcode type, we're confused as hell. */
            default:    
                fprintf(stderr, "Unknown opcode type, I'm confused.\n");
                exit(0);
        }
    }

    /* Return whatever was last on the stack */
    return StackPop();
}

/* Just call the stack Pop and Push functions */
void PushInstr(VyObj obj){
    StackPush(obj);
}
void PopInstr(){
    StackPop();
}

void BindInstr(){
    VyObj name_obj = StackPop();
    VyObj val = StackPeek();
    VySymbol* name = (VySymbol*) Obj(name_obj);
    VariableBind(name, val);
}
void ValueInstr(VyObj obj){
    VySymbol* name = (VySymbol*) Obj(obj);
    VyObj val = VariableValue(name);
    StackPush(val);
}
void FuncInstr(){
    VyObj func_obj = StackPeek();
    VyFunction* func = (VyFunction*) Obj(func_obj);
    func->live_scope = CreateScope(CurrentScope());
}
int IfJmpInstr(VyObj data){
    int ifFalse = (int) Obj(data);
    VyObj condition_value = StackPop();
    if(IsTrue(condition_value))
        return -1;
    else
        return ifFalse;
}

void CallInstr(int num_args){
    VyFunction* func = (VyFunction*) Obj(StackPop());

    VyObj arguments[num_args];
    int i;
    for(i = num_args - 1; i >= 0; i--){
        arguments[i] = StackPop();
    }

    if(!func->native){
        Scope* caller_scope = CurrentScope();
        EnterScope(func->live_scope);

        BindArguments(func->arguments, arguments, num_args);

        VyObj return_val = EvalBytecode(func->code.bytecode);
        ClearScope(func->live_scope);
        EnterScope(caller_scope);
        StackPush(return_val);
    } else {
        VyObj return_val = func->code.native(&arguments[0], num_args);
        StackPush(return_val);
    }
}
