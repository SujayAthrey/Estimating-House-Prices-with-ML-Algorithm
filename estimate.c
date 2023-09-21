#include <stdio.h>
#include <stdlib.h>
#include <string.h> 


double** transposeMtrx(double** matrix, int r, int c);
double** multMtrx(double** matrix, double ** matrix2, int r, int c, int r2, int c2);
double** multMtrx2(double** matrix, double ** matrix2, int r, int c, int r2, int c2);
double** inverseMtrx(double** matrix, int r);

double** transposeMtrx(double** matrix, int r, int c){
    double** transposeMtrx =(double**)malloc(c*sizeof(double*));
    for (int i = 0; i < c; i++)
        transposeMtrx[i] = (double*)malloc(r*sizeof(double));

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            transposeMtrx[j][i] = matrix[i][j];
        }
    }
    return transposeMtrx;
}

double** multMtrx(double** matrix, double** matrix2, int r, int c, int r2, int c2){
    
    double** product =(double**)malloc(r*sizeof(double*));
    for(int i = 0; i < r; i++)
        product[i] =(double*)malloc(c2*sizeof(double));
    
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c2; j++) {
            product[i][j] = 0; 
            for(int k = 0; k < c; k++){
                product[i][j] += matrix[i][k] * matrix2[k][j];
            }
        }
    }
    return product;
}

double** multMtrx2(double** matrix, double ** matrix2, int r, int c, int r2, int c2){
    
    double** price=(double**)malloc(r*sizeof(double*));
        for (int i = 0; i < r; i++)
            price[i]=(double*) malloc(sizeof(double));
                                        
    for(int i = 0; i < r; i++){
        for(int j = 0; j < r2 ;j++){
            if(j == 0)
                price[i][0] = matrix2[j][0];
            else
                price[i][0] += (matrix2[j][0] * matrix[i][j-1]);
        }
    }
    return price;
}

double** inverseMtrx(double** matrix, int r){
    
    int dimensionsNum = r;
    double pivotNum;
    
    double** augIdentity = (double**)malloc(r*sizeof(double*));
    for (int i = 0; i < r; i++){
        augIdentity[i] = (double*)malloc(r*2*sizeof(double));
    }

    double** inverse = (double **) malloc (r*sizeof(double*));
    for (int i = 0; i < r; i++){
        inverse[i] = (double*)malloc(r*sizeof(double));
    }

    for(int i = 0; i < r; i++){
        for(int j = 0; j < 2*r; j++){
            if(j == dimensionsNum)
                augIdentity[i][j] = 1;
            else if(j > (r-1))
                augIdentity[i][j] = 0;
            else
                augIdentity[i][j] = matrix[i][j];
        }
        dimensionsNum++;
    }    

    for(int i = 0; i < r; i++){
        for(int j = 0; j < r; j++){   
            if(i > j)
                continue;
            if(i == j){ 
                   pivotNum = augIdentity[i][j]; 
                   for(int k = 0; k < 2*r; k++){
                       augIdentity[i][k] /= pivotNum;
                   }
            }
            else if(i!=j){
                for(int k = j; k < r; k++){   
                    pivotNum = augIdentity[k][i];
                    for(int l = 0; l < 2*r; l++){
                        augIdentity[k][l] -= pivotNum * augIdentity[j-1][l];
                    }
                }
            }
        }
    }

    for(int i = r-2; i >= 0; i--){
        for(int j = r-1; augIdentity[i][j] != 1; j--){
            for(int k = i; k >= 0; k--){
                pivotNum = augIdentity[i][j];
                for(int l = 0; l < r*2; l++){
                    augIdentity[k][l] -= pivotNum * augIdentity[j][l];
                }
            }
        }
    }

    for(int i = 0; i < r; i++){
        for(int j = 0; j < r; j++){
            inverse[i][j] = augIdentity[i][j+r];
        }
    }
    
    return inverse; 
}

int main(int argc, char *argv[]){

    int r, c, r2, c2;
    double fileNum;
    char nameOfFile[10];

    FILE *train = fopen(argv[1], "r"); 
    fscanf(train, "%s\n", nameOfFile);
    fscanf(train, "%d\n", &c);
    fscanf(train, "%d\n", &r);

    FILE *data = fopen(argv[2], "r"); 
    fscanf(data, "%s\n", nameOfFile);
    fscanf(data, "%d\n", &c2);
    fscanf(data, "%d\n", &r2);

    double** X = (double**)malloc(r*sizeof(double*));
        for(int i = 0; i < r; i++)
           X[i] = (double*)malloc((c+1)*sizeof(double));
    
    double** Y = (double**)malloc(r*sizeof(double*));
        for(int i = 0; i < r; i++)
           Y[i] = (double*)malloc(sizeof(double));

    double** input = (double**)malloc(r2*sizeof(double*));
        for(int i = 0; i < r2; i++)
            input[i] = (double*)malloc(c2*sizeof(double));
    
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c+2; j++){
            if(j == 0){     
                X[i][j] = 1;
                    if(i < r2){
                        fscanf(data, "%lf ", &fileNum);
                        input[i][j] = fileNum;
                    }
            }
            else if(j == c+1){
                fscanf(train, "%lf\n", &fileNum);
                Y[i][0] = fileNum;
            }else{
                fscanf(train, "%lf ", &fileNum);
                X[i][j] = fileNum;
                if(i < r2 && j == c2-1){ 
                    fscanf(data, "%lf\n", &fileNum);
                    input[i][j] = fileNum;                            
                }
                else if(i < r2 && j < c2){ 
                    fscanf(data, "%lf ", &fileNum);
                    input[i][j] = fileNum;
                }
            }
        }
    }
    
    double** XT = transposeMtrx(X, r, c+1);
    double** XTX = multMtrx(XT, X, c+1, r, r, c+1);
    double** XTXInverse = inverseMtrx(XTX, c+1);
    double** XTXInverseXT = multMtrx(XTXInverse, XT, c+1, c+1, c+1, r);
    double** WEIGHT = multMtrx(XTXInverseXT, Y, c+1, r, r, 1);
    double** PRICE = multMtrx2(input, WEIGHT, r2, c2, c+1, 1);
    
    for(int i = 0; i < r2; i++){
            printf("%0.0lf\n", PRICE[i][0]);   
    }

    return 0;
}



