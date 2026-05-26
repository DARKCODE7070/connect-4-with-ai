int check_right(int arr[][7], int i, int j , int c, int p){
    if(c<4){
        printf("arr= %d,i=%d , j=%d, c = %d ,p=%d", arr[i][j], i, j,c,p);
        if(arr[i][j] == p){
            if(j+1<7){
                return check_right(arr, i, j+1, c+1, p);
                
            }
            else
                return 0;
        }
        else
            return 0;
    }

    else if(c == 4){
        return p;
    }
}