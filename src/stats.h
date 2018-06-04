#ifndef lcthw_stats_h_
#define lcthw_stats_h_

#include <stdlib.h>
#include <dbg.h>
#include <math.h>

typedef struct Stats{
        double sum;
        double sumsq;
        unsigned long n;
        double min;
        double max;
}Stats;

Stats *Stats_recreate(double sum,double sumsq,unsigned long n,
                double min,double max);
Stats *Stats_create();
void Stats_sample(Stats *st,double s);
void Stats_dump(Stats *st);

static inline double Stats_mean(Stats *st){
        check(st != NULL,"Invalid Stats pointer given to mean().");
        check(st->n != 0,"n cannot be 0");

        return (st->sum / st->n);
error:
        return 0.0;
}
static inline double Stats_stddev(Stats *st){
        check(st != NULL,"Invalid Stats pointer given to stddev().");
        check(st->n != 0,"n cannot be 0");

        return sqrt((st->sumsq - (st->sum*st->sum/st->n))/(st->n-1));
error:
        return 0.0;
}

#endif