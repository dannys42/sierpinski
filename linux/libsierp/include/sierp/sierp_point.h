#ifndef SIERP_POINT_H
#define SIERP_POINT_H

#ifdef __cplusplus
extern "C" {
#endif

struct SIERP_POINT {
    double x;
    double y;
    int iter;
};
typedef struct SIERP_POINT SIERP_POINT;

#ifdef __cplusplus
}
#endif

#endif /* SIERP_POINT_H */
