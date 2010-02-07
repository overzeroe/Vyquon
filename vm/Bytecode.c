#include "Vyquon.h"

/* Create a new bytecode object */
Bytecode* CreateBytecode(){
    Bytecode* bytecode = VyMalloc(sizeof(Bytecode));

    /* Initial size of 1024 instructions was arbitrary; experiments should be done to find better value perhaps */
    bytecode->size = 1024;
    bytecode->used = 0;
    bytecode->instructions = VyMalloc(sizeof(Instruction) * bytecode->size);
    return bytecode;
}

/* Increase size of bytecode */
void ExpandBytecode(Bytecode* bytecode){
    /* Multiplier size of 2 was chosen arbitrarily, experiments should be done to find better value perhaps */
    bytecode->size *= 2;

    /* Multiply size by a constant and reallocate that much */
    bytecode->instructions = VyRealloc(bytecode->instructions, sizeof(Instruction) * bytecode->size);
}

/* Add another instruction to the bytecode */
void EmitInstruction(Bytecode* bytecode, Instruction instr){
    int index = bytecode->used;

    /* If we don't have enough room, allocate more */
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
inline Instruction Bind(){
    Instruction instr = {opcode: INSTR_BIND, data: None()};
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
    Instruction instr = {opcode: INSTR_CALL, data: WrapObj((void*) num_args, TypeNone)};
    return instr;
}

/* Functions used for compiling to bytecode */
Bytecode* CompileExpr(Bytecode*, VyObj);
VyObj CompileFunctionObj(VyObj arg_list, VyObj statement_list);

/* External interface shouldn't be recursive. CompileExpr* is recursive, Compile isn't. */
Bytecode* Compile(VyObj compileObj){
    Bytecode* bytecode = CreateBytecode();
    return CompileExpr(bytecode, compileObj);
}

/* Main recursive function */
Bytecode* CompileExpr(Bytecode* bytecode, VyObj expr){
    /* How we compile depends on what we're compiling */
    VyObj obj = expr;

/* Convenience macro */
#define INSTR(x) EmitInstruction(bytecode, x)

    /* Self-evaluating forms are just PUSH'd onto the stack */
    if(IsType(obj, TypeString) || IsType(obj, TypeFloat) || IsType(obj, TypeInt))
        INSTR(Push(expr));

    /* Symbols are evaluate as variables */
    if(IsType(obj, TypeSymbol)){
        INSTR(Value(expr));
    }

    /* Lists are either special forms or function calls */
    if(IsType(obj, TypeCons)){
        VyCons* cons = (VyCons*) Obj(obj);

        bool special_form = false;

        /* All special forms have a symbol as the car */
        if(IsType(Car(obj), TypeSymbol)){
            VySymbol* symbol = (VySymbol*)Obj(Car(obj));

            /* Quoted forms are also just PUSH'd onto the stack */
            if(SymbolEq(symbol, "quote")){
                special_form = true;
                
                /* ... is the cadr of (quote ...); */
                VyObj quoted = Car(Cdr(obj));
                INSTR(Push(quoted));
            } 
            
            /* Set the variable specified the by 1st expression to the value of the 2nd expression */
            else if(SymbolEq(symbol, "setvar")){
                special_form = true;
                
                /* Name is the first expression, but it doesn't have to be a symbol; 
                   Name could be computed with a function call or something, so compile the expression */
                VyObj name = ListGet(cons, 1);

                /* The name is a symbol which is pushed onto the stack */
                CompileExpr(bytecode, name);

                /* Compile the second expression so that the thing that should be bound to the name is on the stack */
                CompileExpr(bytecode, ListGet(cons, 2));

                /* The bind instruction will pop the name and value off the stack and bind them into darkness */
                INSTR(Bind());
            } 
            
            /* fn is the renamed lambda */
            else if(SymbolEq(symbol, "fn")){
                special_form = true;

                VyObj arg_list = ListGet(cons, 1);
                VyObj statements = Cdr(Cdr(obj));

                /* The function is created at compile time and pushed onto the stack */
                VyObj function_obj = CompileFunctionObj(arg_list, statements);
                INSTR(Push(function_obj));

                /* The FUNC instruction will set the creation scope of the function */
                INSTR(Func());
            } 
            
            /* An if form of the form (if condition then-clause [else-clause]) */
            else if(SymbolEq(symbol, "if")){
                special_form = true;

                /* The bytecode for the if form looks like this:
                 * EVAL <condition>
                 * IFNJMP <to ELSE-CLAUSE>
                 * <then-clause>
                 * JMP <to END>
                 * ELSE-CLAUSE <is here>
                 * END <is here>
                 * 
                 * We do not know where to jump to at the time that the JMPs are being emitted, so we will
                 * hack around that by emitting them and filling in the jump points later.
                 */

                /* Not all ifs must have an else clause */
                bool has_else_clause = (ListLen(obj) == 4);

                /* Push condition evaluation onto stack */
                bytecode = CompileExpr(bytecode, ListGet(cons, 1));
               
                /* Generate placeholder instructions - we can't actually issue the real ones because we don't know
                   how far we need to jump. Thus we will issue fake instructions, and modify them later. */
                Instruction if_jmp = {opcode: INSTR_IFNJMP, data: None()};
                EmitInstruction(bytecode, if_jmp);

                /* Get the pointer to the instruction we just issued so we can change it later */
                Instruction* if_jmp_placeholder = &bytecode->instructions[bytecode->used - 1];

                /* Now compile the then-clause */
                bytecode = CompileExpr(bytecode, ListGet(cons, 2));

                /* Repeat what we did previously with the placeholder instruction */
                Instruction jmp = {opcode: INSTR_JMP, data: None()};
                EmitInstruction(bytecode, jmp);
                Instruction* jmp_placeholder = &bytecode->instructions[bytecode->used - 1];

                /* Record where the else-clause starts */
                int else_index = bytecode->used;

                /* If it has an else, use it, otherwise, make 'nil the else */
                if(has_else_clause){
                    bytecode = CompileExpr(bytecode, ListGet(cons, 3));
                } 
                
                /* We have to return something if we don't have an else clause and it's nil, so let's make our else clause just nil */
                else {
                    INSTR(Push(Nil()));
                }
                
                /* Substitute in values for the placeholders */
                int end_index = bytecode->used;
                jmp_placeholder->data = WrapObj((void*) end_index, TypeNone);
                if_jmp_placeholder->data = WrapObj((void*)else_index, TypeNone);
            } 
            
            /* A while form of the form (while condition [statements]*) */
            else if(SymbolEq(symbol, "while")){
                special_form = true;

                /* The bytecode for a while loop looks like this:
                 * START <is here>
                 * EVAL <condition>
                 * IFNJMP <to END>
                 * LOOP BODY <is here>
                 * JMP <to START>
                 * END <is here>
                 *
                 * Again, we don't know where we're jumping to when we emit the IFNJMP because we jump forward to not-yet-emitted instructions,
                 * so we will just emit the IFNJMP anyway and fill in the jump point later.
                 */

                /* Where the loop starts */
                int start_index = bytecode->used;

                /* Compile the condition */
                bytecode = CompileExpr(bytecode, ListGet(cons, 1));

                /* Emit incomplete instruction and record where it is */
                Instruction if_jmp = {opcode: INSTR_IFNJMP, data: None()};
                EmitInstruction(bytecode, if_jmp);
                Instruction* if_jmp_placeholder = &bytecode->instructions[bytecode->used - 1];

                /* Compile loop body */
                int i = 0, len = ListLen(obj) - 1;
                for(i = 0; i < len; i++){
                    bytecode = CompileExpr(bytecode, ListGet(cons, i + 1));

                    /* We do not want a pileup of values on the stack, so pop them off */
                    /* Sadly this means that we can't make the last statement be returned, so while's return nil */
                    INSTR(Pop());
                }

                /* Jmp back to start */
                Instruction jmp = {opcode: INSTR_JMP, data: WrapObj((void*) start_index, TypeNone)};
                EmitInstruction(bytecode, jmp);

                /* This is where the loop should exit to, update the if_jmp instr */
                int end_index = bytecode->used;
                if_jmp_placeholder->data = WrapObj((void*) end_index, TypeNone);

                /* The loop has to return something, and it will be nil. */
                INSTR(Push(Nil()));

            }

        }

        /* Evaluate and call function if it's not a special form */
        if(!special_form) {
            int num_args = ListLen(obj) - 1;

            /* Evaluate all the arguments in reverse order (so they're popped off the stack in the right order) */
            int cur_arg = num_args;
            for(cur_arg = num_args; cur_arg > 0; cur_arg--){
                bytecode = CompileExpr(bytecode, ListGet(cons, cur_arg));
            }

            /* Evaluate the function - it could be a lambda, doesn't have to be a symbol */
            bytecode = CompileExpr(bytecode, ListGet(cons, 0));

            /* Emit the CALL instruction */
            INSTR(Call(num_args));
        }

    }



    return bytecode;
}

/* Create a function to be pushed onto the stack */
VyObj CompileFunctionObj(VyObj arg_list, VyObj statement_list){
    /* Functions have their own bytecode */
    Bytecode* func_bytecode = CreateBytecode();
    VyCons* statement_cons = (VyCons*) Obj(statement_list);

    int statement_ind = 0;
    int len = ListLen(statement_list);
    while(statement_ind < len){
        /* Emit bytecode for each statement */
        VyObj statement = ListGet(statement_cons, statement_ind);
        func_bytecode = CompileExpr(func_bytecode, statement);

        statement_ind++;

        /* Only leave the last statement result on the stack as the return value, the rest we don't want */
        if(statement_ind != len){
            EmitInstruction(func_bytecode, Pop());
        }
    }

    /* Create the actual object */
    VyFunction* func = CreateFunction(ParseArgList(arg_list), func_bytecode);
    return WrapObj(func, TypeFunction);
}
