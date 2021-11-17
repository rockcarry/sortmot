#ifndef __KMMATCH_H__
#define __KMMATCH_H__

void* km_init (int n);
void  km_free (void *ctx);
int*  km_match(void *ctx, int *weight);

#endif
