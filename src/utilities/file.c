#include <tice.h>  // os functions
#include "file.h"

void saveVar (float var) {

    real_t var_n = os_FloatToReal(var);  // convert var to real_t
    ti_SetVar(TI_REAL_TYPE, location, &var_n);  // save the real_t to variable
}

float readVar (void) {
    void * var_n;  // create empty pointer to store real_t to

    ti_RclVar(TI_REAL_TYPE, location, &var_n);  // save real_t to pointer
    return os_RealToFloat(var_n);  // return real_t converted to float
}
