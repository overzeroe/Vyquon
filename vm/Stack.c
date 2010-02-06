#include "Vyquon.h"

VyObj* stack = NULL;
int stack_size = 100;
int stack_index = 0;

void InitStack(){
    stack = VyMalloc(sizeof(VyObj) * stack_size);
    stack_index = 0;
}
void ExpandStack(){
    stack_size *= 2;
    stack = VyRealloc(stack, sizeof(VyObj) * stack_size);
}

void StackPush(VyObj obj){
    if(!stack)
        InitStack();
    else if(stack_index == stack_size)
        ExpandStack();


    stack[stack_index] = obj;
    stack_index++;
}

VyObj StackPop(){
    stack_index--;
    VyObj val = stack[stack_index];
    stack[stack_index] = None();
    return val;
}

VyObj StackPeek(){
    return stack[stack_index - 1];
}
