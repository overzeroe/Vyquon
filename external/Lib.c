#include "Vyquon.h"

/* All libraries will be external, added to the project at link-time */
extern void LoadIO();

/* Load the core library */
void LoadCoreLibrary(){
    LoadIO();
}
