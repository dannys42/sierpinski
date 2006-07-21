#ifndef SIERP_POINT_LIST_PRV_H
#define SIERP_POINT_LIST_PRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sierp/sierp.h"

struct SIERP_POINT_LIST {
    int maxlen;
    SIERP_POINT *point;
    int head;
    int tail;
};

#ifdef __cplusplus
}
#endif
#endif /* SIERP_POINT_LIST_PRV_H */