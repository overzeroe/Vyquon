#include "Vyquon.h"

VyObj Eval(VyObj sexp){
    Bytecode* bytecode = Compile(sexp);
    VyObj result = EvalBytecode(bytecode);
    return result;
}

void PushInstr(VyObj);
void PopInstr();
void BindInstr(VyObj);
void ValueInstr(VyObj);
void FuncInstr();
void CallInstr(int);
int IfJmpInstr(VyObj);

VyObj EvalBytecode(Bytecode* bytecode){
    int i;
    int nextInstr = -1;
    for(i = 0; i < bytecode->used; i++){
        /* Execute jumps from the previous instruction */
        if(nextInstr >= 0){
            i = nextInstr;
            nextInstr = -1;
        }

        Instruction instr = bytecode->instructions[i];
        int args;

        switch(instr.opcode){
            case INSTR_PUSH:
                PushInstr(instr.data);
                break;
            case INSTR_POP:
                PopInstr();
                break;
            case INSTR_BIND:
                BindInstr(instr.data);
                break;
            case INSTR_VALUE:
                ValueInstr(instr.data);
                break;
            case INSTR_FUNC:
                FuncInstr();
                break;
            case INSTR_CALL:
                args = (int)Obj(instr.data);
                CallInstr(args);
                break;
            case INSTR_JMP:
                nextInstr = (int)Obj(instr.data);       
                break;
            case INSTR_IFNJMP:
                nextInstr = IfJmpInstr(instr.data);
            default:    
                break;
        }
    }

    return StackPop();
}

void PushInstr(VyObj obj){
    StackPush(obj);
}
void PopInstr(){
    StackPop();
}
void BindInstr(VyObj obj){
    VyObj val = StackPeek();
    VySymbol* name = (VySymbol*) Obj(obj);
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

    Scope* caller_scope = CurrentScope();
    EnterScope(func->live_scope);

    VyObj arguments[num_args];
    int i;
    for(i = num_args - 1; i >= 0; i--){
        arguments[i] = StackPop();
    }

    BindArguments(func->arguments, arguments, num_args);

    VyObj return_val = EvalBytecode(func->code);
    ClearScope(func->live_scope);
    EnterScope(caller_scope);
    StackPush(return_val);
}
