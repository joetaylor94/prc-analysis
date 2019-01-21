//
//  b.c
//  
//
//  Created by Joe Taylor on 20/01/2019.
//

#include <stdio.h>
#include <math.h>

#define N 2
#define LENPRC 1000
#define FIRINGS 10000

int main(){
    
    FILE* file1 = fopen("f1.txt", "r");
    FILE* file2 = fopen("f2.txt", "r");
    FILE* file3 = fopen("init_phi.txt", "r");
    FILE* file4 = fopen("period.txt", "r");
    
    int n,k,m,min;
    int prc_coord;
    int sim_firing;
    int cell_fired[N];
    double dummy;
    double phi[N];
    double period[N];
    double phi_dummy[N];
    double f2_remember[N];
    double time_to_fire[N];
    double prc_order_1[N][LENPRC];
    double prc_order_2[N][LENPRC];
    
    /* Read in 1st- and 2nd-order phase response curves for the (N-1) possible simultaneous inputs */
    for (n=0; n<N-1; n++)
    {
        for (k=0; k<LENPRC; k++)
        {
            fscanf(file1,"%lf %lf\n",&dummy,&prc_order_1[n][k]);
            fscanf(file2,"%lf %lf\n",&dummy,&prc_order_2[n][k]);
        }
    }
    
    /* Read in intrinsic period and initial phase of each neuron */
    for (n=0; n<N; n++)
    {
        fscanf(file4,"%lf\n",&period[n]);
        fscanf(file3,"%lf\n",&phi[n]);
    }
    
    /* Iterative map */
    for (m=0; m<FIRINGS; m++)
    {
        time_to_fire[0] = period[0]*(1-phi[0]);
        min = 0;
        
        /* Determine which neuron will fire next */
        sim_firing = 0;
        for(n=1; n<N; n++)
        {
            time_to_fire[n] = period[n]*(1.0 - phi[n]);
            if (time_to_fire[n] < time_to_fire[min]) min = n;
        }
        
        /* Count how many neurons fire simultaneously */
        for(n=0; n<N; n++)
        {
            cell_fired[n] = 0;
            if (fabs(time_to_fire[n] - time_to_fire[min]) < 0.000001)
            {
                sim_firing++;
                cell_fired[n] = 1;
                if (n == 1) printf("%f\n",phi[n]-phi[0]);
            }
        }
        
        /* Remember 2nd-order contributions for the firing after the next for all non-fired neurons */
        for (n=0; n<N; n++)
        {
            if (cell_fired[n]==0)
            {
                phi_dummy[n] = phi[n] + time_to_fire[min]/period[n];
                prc_coord = (phi_dummy[n]*LENPRC)/1;
                f2_remember[n] = f2_remember[n] + prc_order_2[sim_firing-1][prc_coord];
            }
            if (cell_fired[n]==1)
            {
                phi_dummy[n] = 0.0;
                prc_coord = (phi_dummy[n]*LENPRC)/1;
                if(sim_firing>1) f2_remember[n] = f2_remember[n] + prc_order_2[sim_firing-2][prc_coord];
            }
        }
        
        /* Update phase of fired neurons as a consequence of previous 2nd-order contributions, and add f1 contributions of any simultaneously firing neurons */
        for (n=0; n<N; n++)
        {
            if (cell_fired[n] == 1)
            {
                phi[n] = 0.0 - f2_remember[n];
                if (sim_firing > 1) phi[n] = phi[n] - prc_order_1[sim_firing-2][0];
                f2_remember[n] = 0;
            }
        }
        
        /* Update the phase of non-firing neurons as a result of f1 contrubitions of firing neurons */
        for (n=0; n<N; n++)
        {
            if (cell_fired[n] == 0)
            {
                phi_dummy[n] = phi[n] + time_to_fire[min]/period[n];
                prc_coord = (phi_dummy[n]*LENPRC)/1;
                phi[n] = phi_dummy[n] - prc_order_1[sim_firing-1][prc_coord];
            }
        }
//        printf("phi[0] = %f\n", time_to_fire[0]);
//        printf("phi[1] = %f\n", time_to_fire[1]);

        
    }
    
    return 0;
}
