#include "Vyquon.h"

Bytecode* CreateBytecode(){
    Bytecode* bytecode = VyMalloc(sizeof(Bytecode));
    bytecode->size = 1024;
    bytecode->used = 0;
    bytecode->instructions = VyMalloc(sizeof(Instruction) * bytecode->size);
    return bytecode;
}

void ExpandBytecode(Bytecode* bytecode){
    bytecode->size *= 2;
    bytecode->instructions = VyRealloc(bytecode->instructions, sizeof(Instruction) * bytecode->size);
}

void EmitInstruction(Bytecode* bytecode, Instruction instr){
    int index = bytecode->used;
    bytecode->used++;
    if(bytecode->used > bytecode->size)
        ExpandBytecode(bytecode);

    bytecode->instructions[index] = instr;
}

/* Create instructions */
inline Instruction Push(VyObj data){
    Instruction instr = {opcode: INSTR_PUSH, data: data};
    return instr;
}
inline Instruction Pop(){
    Instruction instr = {opcode: INSTR_POP, data: None()};
    return instr;
}
inline Instruction Bind(VyObj name){
    Instruction instr = {opcode: INSTR_BIND, data: name};
    return instr;
}
inline Instruction Value(VyObj name){
    Instruction instr = {opcode: INSTR_VALUE, data: name};
    return instr;
}
inline Instruction Func(){
    Instruction instr = {opcode: INSTR_FUNC, data: None()};
    return instr;
}
inline Instruction Call(int num_args){
    Instruction instr = {opcode: INSTR_CALL, data: WrapObj((void*) num_args, OBJ_NONE)};
    return instr;
}

Bytecode* CompileExpr(Bytecode*, VyObj);
Bytecode* Compile(VyObj compileObj){
    Bytecode* bytecode = CreateBytecode();
    return CompileExpr(bytecode, compileObj);
}

VyObj CompileFunctionObj(VyObj arg_list, VyObj statement_list);
Bytecode* CompileExpr(Bytecode* bytecode, VyObj expr){
    ObjType type = Type(expr);

#define INSTR(x) EmitInstruction(bytecode, x)

    /* Self-evaluating forms are just PUSH'd onto the stack */
    if(type == OBJ_STR || type == OBJ_NUM)
        INSTR(Push(expr));

    if(type == OBJ_CONS){
        /* Quoted forms are also just PUSH'd onto the stack */
        VyCons* cons = (VyCons*) Obj(expr);
        if(Type(cons->car) == OBJ_SYM){
            VySymbol* symbol = (VySymbol*)Obj(cons->car);
            VyObj cdr = cons->cdr;

            if(SymbolEq(symbol, "quote")){
                VyObj quoted = ((VyCons*) Obj(cdr))->car;
                INSTR(Push(quoted));
            } else if(SymbolEq(symbol, "setvar")){
                VyObj name = ListGet(cons, 1);
                CompileExpr(bytecode, ListGet(cons, 2));
                INSTR(Bind(name));
            } else if(SymbolEq(symbol, "fn")){
                VyObj arg_list = ListGet(cons, 1);
                VyObj statements = Cdr((VyCons*) Obj(Cdr(cons)));

                VyObj function_obj = CompileFunctionObj(arg_list, statements);
                INSTR(Push(function_obj));
                INSTR(Func());
            } else if(SymbolEq(symbol, "if")){
                bool has_else_clause = (ListLen(cons) == 4);

                /* Push condition evaluation onto stack */
                bytecode = CompileExpr(bytecode, ListGet(cons, 1));
                Instruction if_jmp = {opcode: INSTR_IFNJMP, data: None()};
                Instruction jmp = {opcode: INSTR_JMP, data: None()};
                EmitInstruction(bytecode, if_jmp);
                Instruction* if_jmp_placeholder = &bytecode->instructions[bytecode->used - 1];
                bytecode = CompileExpr(bytecode, ListGet(cons, 2));

                EmitInstruction(bytecode, jmp);
                Instruction* jmp_placeholder = &bytecode->instructions[bytecode->used - 1];
                int else_index = bytecode->used;

                /* If it has an else, use it, otherwise, make 'nil the else */
                if(has_else_clause){
                    bytecode = CompileExpr(bytecode, ListGet(cons, 3));
                } else {
                    INSTR(Push(Nil()));
                }
                
                /* Substitute in values for the placeholders */
                int end_index = bytecode->used;
                jmp_placeholder->data = WrapObj((void*) end_index, OBJ_NONE);
                if_jmp_placeholder->data = WrapObj((void*)else_index, OBJ_NONE);
            } else if(SymbolEq(symbol, "while")){
                int start_index = bytecode->used;

                /* Compile the condition */
                bytecode = CompileExpr(bytecode, ListGet(cons, 1));

                Instruction if_jmp = {opcode: INSTR_IFNJMP, data: None()};
                EmitInstruction(bytecode, if_jmp);
                Instruction* if_jmp_placeholder = &bytecode->instructions[bytecode->used - 1];

                /* Compile loop body */
                int i = 0, len = ListLen(cons) - 1;
                for(i = 0; i < len; i++){
                    bytecode = CompileExpr(bytecode, ListGet(cons, i + 1));
                    INSTR(Pop());
                }

                /* Jmp back to start */
                Instruction jmp = {opcode: INSTR_JMP, data: WrapObj((void*) start_index, OBJ_NONE)};
                EmitInstruction(bytecode, jmp);

                /* This is where the loop should exit to, update the if_jmp instr */
                int end_index = bytecode->used;
                if_jmp_placeholder->data = WrapObj((void*) end_index, OBJ_NONE);

                /* The loop has to return something, and it will be nil. */
                INSTR(Push(Nil()));

            }

            /* Evaluate and call function */
            else {
                int num_args = ListLen(cons) - 1;
                int cur_arg = num_args;
                for(cur_arg = num_args; cur_arg > 0; cur_arg--){
                    bytecode = CompileExpr(bytecode, ListGet(cons, cur_arg));
                }
                bytecode = CompileExpr(bytecode, ListGet(cons, 0));
                INSTR(Call(num_args));
            }

        }

    }


    /* Evaluate as a variable */
    if(type == OBJ_SYM){
        INSTR(Value(expr));
    }

    return bytecode;
}

/* Create a function to be pushed onto the stack */
VyObj CompileFunctionObj(VyObj arg_list, VyObj statement_list){
    Bytecode* func_bytecode = CreateBytecode();
    VyCons* statement_cons = (VyCons*) Obj(statement_list);

    int statement_ind = 0;
    int len = ListLen(statement_cons);
    while(statement_ind < len){
        VyObj statement = ListGet(statement_cons, statement_ind);
        func_bytecode = CompileExpr(func_bytecode, statement);

        statement_ind++;
        if(statement_ind != len){
            EmitInstruction(func_bytecode, Pop());
        }
    }

    VyFunction* func = CreateFunction(ParseArgList(arg_list), func_bytecode);
    return WrapObj(func, OBJ_FUNC);
}
