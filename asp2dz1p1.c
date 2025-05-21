#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int binomniKoeficijent(int n, int k) {
    if (k==0 || k==n) {
        return 1;
    } else if (k>n) {
        return 0;
    } else {
        return binomniKoeficijent(n - 1, k - 1) + binomniKoeficijent(n - 1, k);
    }
}

int** formiranjeTrougla(int n){
    n = n+1;
    int** matrica = malloc(n*sizeof(int*));
    for(int i=0;i<n;i++){
        matrica[i]=malloc((i+1)* sizeof(int));
        for(int j=0;j<=i;j++){
            if(j==0 || j==i){
                matrica[i][j]=1;
            }else{
                if(i%2==0 && j%2==1){
                    int a=(i/2)-1;
                    int b=(j-1)/2;
                    matrica[i][j]=matrica[i-1][j-1]+matrica[i-1][j]- binomniKoeficijent(a,b);
                }
                else{
                    matrica[i][j]=matrica[i-1][j-1]+matrica[i-1][j];
                }
            }
        }
    }
    for(int i=0; i<n; i++) {
        for(int j=0; j<=i; j++) {
            printf("%d ", matrica[i][j]);
        }
        printf("\n");
    }
    return matrica;
}


int brojPretrazivanjaBinarno(int red[],int n,int kljuc){
    int brojac=0;
    int low=0;
    int high=(n-1)/2;
    while(low<=high){
        brojac++;
        int mid=(low+high)/2;
        printf("low: %d\n", low);
        printf("high: %d\n", high);
        printf("mid: %d\n", mid);
        printf("Trenutni niz:");
//        for(int s=low;s<=high;s++){
//            printf("%d ", red[s]);
//        }
        printf("\n");
        if(kljuc==red[mid]){
            printf("Kljuc je pronadjen!\n");
            return brojac;
        }
        else if(kljuc<red[mid]){
            high=mid-1;
        }else{
            low=mid+1;
        }

    }
    printf("Kljuc nije pronadjen!\n");
    return brojac;
}

int interpolacionoBrojPretrazivanja(int red[], int n, int kljuc){
    int brojac=0;
    int low=0;
    int high=(n-1)/2;
    while(low<=high){
        if(kljuc<red[low] || kljuc>red[high]){
            brojac=1;
            return brojac;

        }

        if (low == high) {
            if (kljuc == red[high]) {
                printf("Kljuc je pronadjen!\n");
                return brojac;
            } else {
                printf("Kljuc nije pronadjen!\n");
                return brojac;
            }
        }

        brojac++;
        int mid=low+(high-low)*(kljuc-red[low])/(red[high]-red[low]);
        printf("low: %d\n", low);
        printf("high: %d\n", high);
        printf("mid: %d\n", mid);
        printf("Trenutni niz:");
        for(int s=low;s<=high;s++){
            printf("%d ", red[s]);
        }
        printf("\n");
        if(kljuc==red[mid]){
            printf("Kljuc je pronadjen!\n");
            return brojac;
        }else if(kljuc<red[mid]){
            high=mid-1;
        }else{
            low=mid+1;
        }

    }
    printf("Kljuc nije pronadjen!\n");
    return brojac;
}

int main() {
    int **trougao=NULL;
    int k;
    int p;
    while(1){
        int sum1;
        int sum2;
        int m;
        printf("Odaberite jednu opciju:\n"
               "1.Kreiranje trougla\n"
               "2.Pretraga trougla\n"
               "3.Poredjenje performansi\n"
               "4.Prekidanje programa\n");
        scanf("%d", &m);
        if(m==1){
            int n;
            printf("Unesite red trougla: ");
            scanf("%d", &n);
            trougao = formiranjeTrougla(n);

        }
        else if(m==2){
            printf("Imajuci u vidu da indeksiranje krece od 0, unesite redni broj reda u kome zelite da se vrsi pretraga: ");
            scanf("%d", &k);
            printf("\n");

            printf("Unesite broj kljuceva koje zelite da pretrazite:");
            scanf("%d", &p);
            printf("\n");
            printf("Unesite niz kljuceva koje zelite da pretrazite:\n");

            int niz[p];
            for(int i=0;i<p;i++){
                printf("%d. kljuc je: ", i+1);
                scanf("%d",&niz[i]);
            }
            sum1=0;
            sum2=0;
            for(int i=0;i<p;i++){
                printf("Binarna pretraga za kljuc %d je:\n", niz[i]);
                int brojac= brojPretrazivanjaBinarno(trougao[k],k+1,niz[i]);
                sum1+=brojac;
                printf("Broj koraka pretrage je %d.\n\n", brojac);
                printf("Interpolaciona pretraga za kljuc %d je:\n", niz[i]);
                int cnt= interpolacionoBrojPretrazivanja(trougao[k],k+1,niz[i]);
                sum2+=cnt;
                printf("Broj koraka pretrage je %d.\n", cnt);
            }

        }
        else if(m==3){
            double binPerformansa;
            double interPerformansa;
            binPerformansa=(double)sum1/p;
            interPerformansa=(double)sum2/p;
            printf("Performanse binarne pretrage: %.2f\n",binPerformansa );
            printf("Performanse interpolacione pretrage: %.2f\n", interPerformansa);

        }
        else if(m==4){
            break;
        } else{
            printf("Unijeli ste nevalidnu vrijednost, unesite ponovo!");
        }
    }
    return 0;
}

