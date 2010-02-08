#include "Vyquon.h"

/* Predicates */
VyObj IsInt(VyObj* objs, int num){
    bool is_int = IsType(objs[0], TypeInt);
    return is_int ? TrueObj() : FalseObj();
}
VyObj IsFloat(VyObj* objs, int num){
    bool is_float = IsType(objs[0], TypeFloat);
    return is_float ? TrueObj() : FalseObj();
}

/* Arithmetic */
VyObj AddFun(VyObj* objs, int num){
    double sum = 0;
    bool integral = true;
    int i;
    for(i = 0; i < num; i++){
        if(IsType(objs[i], TypeInt))
            sum += UNWRAP(objs[i], VyInt)->val;
        if(IsType(objs[i], TypeFloat)) {
            sum += UNWRAP(objs[i], VyFloat)->val;
            integral = false;
        }
    }

    if(integral)
        return CreateInt((int) sum);
    else 
        return CreateFloat(sum);
}
VyObj SubFun(VyObj* objs, int num){
    /* (-) evaluates to 0 */
    if(num == 0){
        return CreateInt(0);
    }

    /* If only one argument, change the sign */
    if(num == 1){
         if(IsType(objs[0], TypeInt))
            UNWRAP(objs[0], VyInt)->val *= -1;
         if(IsType(objs[0], TypeFloat))
            UNWRAP(objs[0], VyFloat)->val *= -1;

         return objs[0];
    }

    /* If >1 argument, we have to start with the 1st argument and subtract from that  */
    double sum = 0;
    if(IsType(objs[0], TypeInt))
        sum = UNWRAP(objs[0], VyInt)->val;
    if(IsType(objs[0], TypeFloat))
        sum = UNWRAP(objs[0], VyFloat)->val;

    bool integral = IsType(objs[0], TypeInt);
    int i;
    for(i = 0; i < num; i++){
        if(IsType(objs[i], TypeInt))
            sum -= UNWRAP(objs[i], VyInt)->val;
        if(IsType(objs[i], TypeFloat)) {
            sum -= UNWRAP(objs[i], VyFloat)->val;
            integral = false;
        }
    }

    if(integral)
        return CreateInt((int) sum);
    else 
        return CreateFloat(sum);
}
VyObj MulFun(VyObj* objs, int num){
    double product = 1;
    bool integral = true;
    int i;
    for(i = 0; i < num; i++){
        if(IsType(objs[i], TypeInt))
            product += UNWRAP(objs[i], VyInt)->val;
        if(IsType(objs[i], TypeFloat)) {
            product += UNWRAP(objs[i], VyFloat)->val;
            integral = false;
        }
    }

    if(integral)
        return CreateInt((int) product);
    else 
        return CreateFloat(product);
}
VyObj DivFun(VyObj* objs, int num){
    float num_one; 
    if(IsType(objs[0], TypeInt))
        num_one = UNWRAP(objs[0], VyInt)->val;
    if(IsType(objs[0], TypeFloat))
        num_one = UNWRAP(objs[0], VyFloat)->val;

    float num_two; 
    if(IsType(objs[1], TypeInt))
        num_two = UNWRAP(objs[1], VyInt)->val;
    if(IsType(objs[1], TypeFloat))
        num_two = UNWRAP(objs[1], VyFloat)->val;

    return CreateFloat(num_one/num_two);
}

/* Initialize the IO functions and library */
void LoadMath(){
    /* Predicates */
    NewFunction("int?", "(x)", &IsInt);
    NewFunction("float?", "(x)", &IsFloat);

    /* Arithmetic */
    NewFunction("+", "(... vals)", &AddFun);
    NewFunction("-", "(... vals)", &SubFun);
    NewFunction("*", "(... vals)", &MulFun);
    NewFunction("/", "(a b)", &DivFun);
}
