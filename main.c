//Directives du preprocesseur
#include <stdio.h>
#include <stdlib.h>
#define N 100
//variables globales
char nomfi[32],nomfo[32];
int M[N][N];
int n;
float enth_total;
int vect_nb_frag[4];
//Declaration des fonctions
int trouver_dim_n(char nomfi[]);
void lire_matrice_fichier(char nomfi[],int M[][N], int n);
void determiner_code_frag(int M[][N],int vect_code_frag[], int n);
void determiner_enthalpie_frag(int vect_code_frag[], float vect_enth_frag[], int n);
float calcul_enthalpie_hydrocarbure(float vect_enth_frag[], int n);
void compter_nb_fragements(int vect_nb_frag[],int vect_code_frag[], int n);
void ecrire_resultat_fichier(char nomfo[] ,float enth_total, int vect_nb_frag[]);
//Programme principal
int main(void){
    printf("Donner le nom de fichier input :");
    scanf("%s",nomfi);
    n=trouver_dim_n(nomfi);
    int vect_code_frag[n];
    float vect_enth_frag[n];
    lire_matrice_fichier(nomfi,M,n);
    determiner_code_frag(M,vect_code_frag,n);
    determiner_enthalpie_frag(vect_code_frag,vect_enth_frag,n);
    enth_total=calcul_enthalpie_hydrocarbure(vect_enth_frag,n);
    compter_nb_fragements(vect_nb_frag,vect_code_frag,n);
    printf("Donner le nom de fichier output :");
    scanf("%s",nomfo);
    ecrire_resultat_fichier(nomfo,enth_total,vect_nb_frag);
    return 0;
}
//Definitions des fonctions
int trouver_dim_n(char nomfi[]){
    FILE *fich=fopen(nomfi, "r");
    if (fich == NULL) {
            printf("Ce nom de fichier n'existe pas fichier n'existe pas\n");
            exit (1);
    }
    int n=1;
    char c;
    for (c=getc(fich);c!=EOF;c=getc(fich))
        if (c=='\n') {
          n=n+1;
        }

        fclose(fich);
        return n;
}
void lire_matrice_fichier(char nomfi[],int M[][N], int n){
        FILE *fich=fopen(nomfi, "r");
        if (fich==NULL) exit (1);
        int i,j;

        for (i=0;i<n;i++){
            for (j=0;j<n;j++){
            fscanf(fich,"%d",&M[i][j]);
            }
        }
        fclose(fich);
}
void determiner_code_frag(int M[][N],int vect_code_frag[], int n){
    int f0=0,f1=0;
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            if (M[i][j]==0) {
                f0=f0+1;
            }
            else {
                f1=f1+1;
            }
        }
        vect_code_frag[i]=f0*1+f1*5;
        f0=0,f1=0;
    }
}
void determiner_enthalpie_frag(int vect_code_frag[], float vect_enth_frag[], int n){
    int i;
    for (i=0;i<n;i++){
        switch (vect_code_frag[i]) {
        case 13 : {vect_enth_frag[i]=-10.2; break;}
        case 17 : {vect_enth_frag[i]=-4.93; break;}
        case 21 : {vect_enth_frag[i]=-1.9; break;}
        case 25 : {vect_enth_frag[i]=-0.5; break;}
        }
    }
}
float calcul_enthalpie_hydrocarbure(float vect_enth_frag[], int n){
    int i;
    float enth_total=0;
    for (i=0;i<n;i++){
    enth_total=vect_enth_frag[i]+enth_total;
    }
    return enth_total;
}
void compter_nb_fragements(int vect_nb_frag[],int vect_code_frag[], int n){
    int i;
    for (i=0;i<4;i++){
        vect_nb_frag[i]=0;
    }
    for (i=0;i<n;i++){
        switch (vect_code_frag[i]) {
        case 13 : {vect_nb_frag[0]=vect_nb_frag[0]+1; break;}
        case 17 : {vect_nb_frag[1]=vect_nb_frag[1]+1; break;}
        case 21 : {vect_nb_frag[2]=vect_nb_frag[2]+1; break;}
        case 25 : {vect_nb_frag[3]=vect_nb_frag[3]+1; break;}
        }
    }
}
void ecrire_resultat_fichier(char nomfo[] ,float enth_total, int vect_nb_frag[]){
    FILE *fich = fopen(nomfo,"w");
    if (fich == NULL) exit (1);
    fprintf(fich,"fragement \t code \t enthalpie \t présence\n");
    fprintf(fich,"C-(H)3C \t 13 \t -10.2 \t\t %d fois \n",vect_nb_frag[0]);
    fprintf(fich,"C-(H)2C2 \t 17 \t -4.93 \t\t %d fois \n",vect_nb_frag[1]);
    fprintf(fich,"C-(H)C3 \t 21 \t -1.9 \t\t %d fois \n",vect_nb_frag[2]);
    fprintf(fich,"C-(C4) \t\t 25 \t -0.5 \t\t %d fois \n",vect_nb_frag[3]);
    fprintf(fich,"\nl'enthalpie de formation de l'alcane est %.2f", enth_total);
    fclose(fich);
}
