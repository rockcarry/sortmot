#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kmmatch.h"

typedef struct {
    int  N;
    int *cx, *cy;
    int *vx, *vy;
    int *match;
    int *weight;
} KMCONTEXT;

static int dfs(KMCONTEXT *ctx, int xidx)
{
    int  yidx;
    ctx->vx[xidx] = 1;
    for (yidx=0; yidx<ctx->N; yidx++) {
        if (ctx->vy[yidx] == 0 && ctx->cx[xidx] + ctx->cy[yidx] - ctx->weight[yidx * ctx->N + xidx] == 0) {
            ctx->vy[yidx] = 1;
            if (ctx->match[yidx] == -1 || dfs(ctx, ctx->match[yidx])) {
                ctx->match[yidx] = xidx;
                return 1;
            }
        }
    }
    return 0;
}

void *km_init(int n)
{
    KMCONTEXT *ctx = malloc(sizeof(KMCONTEXT) + sizeof(int) * (2 * n + 2 * n + n));
    if (!ctx) return NULL;
    ctx->N      = n;
    ctx->cx     = (int*)((char*)ctx + sizeof(KMCONTEXT));
    ctx->cy     = ctx->cx + n;
    ctx->vx     = ctx->cy + n;
    ctx->vy     = ctx->vx + n;
    ctx->match  = ctx->vy + n;
    return ctx;
}

void km_free(void *ctx) { free(ctx); }

int* km_match(void *ctx, int *weight)
{
    int xidx, yidx, i, j;
    KMCONTEXT *km = (KMCONTEXT*)ctx;
    if (!ctx) return NULL;

    km->weight = weight;
    memset(km->match, -1, km->N * sizeof(int));
    memset(km->cy   ,  0, km->N * sizeof(int));
    for (i=0; i<km->N; i++) {
        km->cx[i] = km->weight[i * km->N + 0];
        for (j=1; j<km->N; j++) {
            if (km->cx[i] < km->weight[i * km->N + j]) km->cx[i] = km->weight[i * km->N + j];
        }
    }

    for (i=0; i<km->N; i++) {
        while (1) {
            memset(km->vx,  0, km->N * sizeof(int));
            memset(km->vy,  0, km->N * sizeof(int));

            if (dfs(km, i)) break;

            int d = 0x7fffffff;
            for (xidx=0; xidx<km->N; xidx++) {
                if (km->vx[xidx]) {
                    for (yidx=0; yidx<km->N; yidx++) {
                        if (!km->vy[yidx]) {
                            if (d > km->cx[xidx] + km->cy[yidx] - km->weight[yidx * km->N + xidx]) {
                                d = km->cx[xidx] + km->cy[yidx] - km->weight[yidx * km->N + xidx];
                            }
                        }
                    }
                }
            }

            for (j=0; j<km->N; j++) {
                if (km->vx[j]) km->cx[j] -= d;
                if (km->vy[j]) km->cy[j] += d;
            }
        }
    }
    return km->match;
}

#ifdef _TEST_
int main(void)
{
    void *km = km_init(5);
    int weight[] = {
        1, 3, 4, 3, 5,
        4, 8, 9, 6, 3,
        4, 3, 2, 8, 9,
        1, 2, 4, 8, 1,
        8, 9, 3, 1, 8,
    };
    int *match, i;
    match = km_match(km, weight);
    if (match) {
        for (i=0; i<5; i++) {
            printf("%d ", match[i]);
        }
        printf("\n");
    }
    km_free(km);
    return 0;
}
#endif
