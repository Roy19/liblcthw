#include <stats.h>

Stats *Stats_recreate(double sum,double sumsq,unsigned long n,double min,
                double max){
        Stats *st = (Stats *)malloc(sizeof(Stats));
        check_mem(st);

        st->sum = sum;
        st->sumsq = sumsq;
        st->n = n;
        st->min = min;
        st->max = max;

        return st;
error:
        return NULL;
}
Stats *Stats_create(){
        return Stats_recreate(0.0,0.0,0L,0.0,0.0);
}
void Stats_sample(Stats *st,double s){
        check(st != NULL,"Invalid Stats pointer given to sample().");

        st->sum += s;
        st->sumsq += (s*s);
        if(st->n == 0){
                st->min = s;
                st->max = s;
        }else{
                if(st->min > s){
                        st->min = s;
                }
                if(st->max < s){
                        st->max = s;
                }
        }
        st->n++;
error:  //fallthrough
        return;
}
void Stats_dump(Stats *st){
        check(st != NULL,"Invalid Stats pointer given to dump().");
        
        fprintf(stderr,"sum: %f, sumsq: %f, n: %ld, min: %f, max: %f, mean: %f, stddev: %f",
                st->sum,st->sumsq,st->n,st->min,st->max,Stats_mean(st),Stats_stddev(st));
error:  //fallthrough
        return;
}