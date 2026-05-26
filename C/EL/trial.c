#include<stdio.h>
#include<stdbool.h>
#include "helper.c"

// FUNCTIONS


bool is_won(const int arr[][7], int i, int j, int p);
void move(int arr[][7], int top[], int *p, int *stop);
void display(const int arr[][7]);
void printc(int p);



// MAIN
/**************** ********************************************************************************/

int main(){
    int arr[6][7] = {0};
    int top[7] = {5,5,5,5,5,5,5};
    int player = 1, stop = 0;
    while(!stop){
        display(arr);
        move(arr, top, &player, &stop);
    }
    return 0;
}

/************************************************************************************************/




/******************************************************/


void printc(int p){
    if(p==1){
        printf("\033[1;31m\xDB\xDB\033[0m");
    }
    else if(p==2){
        printf("\033[1;33m\xDB\xDB\033[0m");
    }
    else{
        printf("\xDB\xDB");
    }
}

/******************************************************/


bool is_won(const int arr[][7], int i, int j, int p){
    if(check_right(arr, i, j, 1, p) == p){
        return true;
    }
    return false;
}

/*****************************************************/
void move(int arr[][7], int top[], int *p, int *stop){
    int m = 0;
    do{
        printf("\nPlayer %d's move : ", *p);
        scanf("%d", &m);
        fflush(stdin);
        if((m>7 || m<1)){
            if((top[m-1] > 5 || top[m-1]<0)){
                m = 0;
                printf("\ninvalid move !! \n");
                continue;
            }
        }
        arr[top[m-1]][m-1] = *p;
        top[m-1]--;

    }while(m<1 || m>7);

    if(is_won(arr,top[m-1], m-1, *p)){
        *stop = 1;
        printf("\n\n\n\n PLAYER %d has won \n", *p);

    }


    *p = 3-*p;
}

/******************************************************/


void display(const int arr[][7]){
    printf("\n\n\n ********* CONNECT 4 **********\n\n");
    for(int i =0; i<6; i++){
        printf("\n   ");
        for(int j =0; j<7; j++){
            printc(arr[i][j]);
            printf("   ");
        }
        printf("\n");
    }
}


