#include "Vyquon.h"

/* Our stack is just an expanding array */
VyObj* stack = NULL;

/* Initial stack size of 100 is arbitrary - adjustment might be good */
int stack_size = 100;
int stack_index = 0;

/* Create the stack */
void InitStack(){
    stack = VyMalloc(sizeof(VyObj) * stack_size);
    stack_index = 0;
}

/* Expand the stack - multiply size by 2 */
void ExpandStack(){
    stack_size *= 2;
    stack = VyRealloc(stack, sizeof(VyObj) * stack_size);
}

/* Add something to the top of the stack */
void StackPush(VyObj obj){
    /* Expand or create the stack if needed */
    if(!stack)
        InitStack();
    else if(stack_index == stack_size)
        ExpandStack();


    stack[stack_index] = obj;
    stack_index++;
}

/* Take off the top of the stack, return it */
VyObj StackPop(){
    stack_index--;
    VyObj val = stack[stack_index];

    /* Clear that object, so if bugs come up they're more obvious */
    stack[stack_index] = None();

    return val;
}

/* Return whatever is on top of the stack */
VyObj StackPeek(){
    return stack[stack_index - 1];
}
