#include <tice.h>
#include "file.h"

void saveVar (float var) {

    real_t var_n = os_FloatToReal(var);
    ti_SetVar(TI_REAL_TYPE, location, &var_n);
}

float readVar (void) {
    void * var_n;

    ti_RclVar(TI_REAL_TYPE, location, &var_n);
    return os_RealToFloat(var_n);
}
