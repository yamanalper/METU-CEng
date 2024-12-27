#include <stdio.h>

int main() {
    
    int N,M,i=0,k,l=0,t=0;
    int partyseat[26];
    float vparty[26] = {0},temp,temp2 = 0;
    scanf("%d %d",&N,&M);
    for (i=0;i<N;i++){
        scanf("%f",&vparty[i]);
    }
    for (i=0;i<N;i++){
        partyseat[i] = 0;
    }
    i = 0;
    while (i<M){
        for(k=0;k<N-1;k++){
            if((vparty[k] / (partyseat[k]+1))>=(vparty[k+1] / (partyseat[k+1]+1))){
                temp = vparty[k] / (partyseat[k]+1);
                if (temp>=temp2){
                    temp2=temp;
                    l=k;
                }
            }
            else {
                temp = vparty[k+1] / (partyseat[k+1]+1);
                if(temp>=temp2){
                    temp2=temp;
                    l = k+1;
                }
            }

            
        }
        partyseat[l]++;
        i++;
        temp2 = 0;
    }
    
        for(k=0;k<N;k++){
            for (i=0;i<N;i++){
                if(partyseat[i]>=partyseat[i+1]){
                    temp = partyseat[i];
                }
                else{
                    temp = partyseat[i+1];
                }
                if(temp>=temp2){
                    temp2 = temp;
                    t = i;
                }
            }
            if (partyseat[t] == 0){
                break;
            }
            for (l=1;l<N;l++){
                if (partyseat[t] == partyseat[t-1]){
                    t = t-1;
                }
                else if (partyseat[t] == partyseat[t-l]){
                    t = t-l;
                }
            }
            printf("%c: %d\n",(65+t),partyseat[t]);
            partyseat[t] = 0;
            t=0;
            temp2 = 0;
        }
    return 0;
}

