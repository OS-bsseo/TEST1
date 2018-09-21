#include <stdio.h>
#include <conio.h>

#define SEARCH_COUNT    5
int value[2][6]={0,};

int verify(int n)
{
    int i,j;
    
    // 중복된 숫자 제거
    for(i=0;i<5;i++){
        for(j=i+1;j<6;j++){
            if(value[1][i] == value[1][j]){
                value[0][j] = 0;
                value[1][j] = 0;
            }
        }
    }
    
    for(i=0;i<6;i++){
        if(value[1][i] == n){
            value[0][i]++;
            return 0;
        }
    }
    
    for(i=0;i<6;i++){
        if(value[1][i] == 0){
            value[0][i] = 1;
            value[1][i] = n;
            return 1;
        }
    }
    
    return 2;
}

int main(void)
{

    
    int i,j;
    int n,lower,t;

    srand((unsigned int)time(NULL));
    
    printf("init program");
    
    while(1){
        n = rand()%45+1;
        
        switch(verify(n)){
        case 0:     // 같은 숫자가 있다면 +1 증가
        case 1:     // 배열이 비어있다면 배열에 숫자 추가
        break;
        case 2:
            // 같은 숫자가 없다면 2 이하 중에 하나를 새로운 숫자로 대체한다.
            for(i=0;i<5;i++){
                for(j=i+1;j<6;j++){
                    if(value[0][i] >= value[0][j]){
                        t = value[0][i+1];
                        value[0][i+1] = value[0][i];
                        value[0][i] = t;

                        t = value[1][i+1];
                        value[1][i+1] = value[1][i];
                        value[1][i] = t;
                    }
                }
            }

            // 2이하의 마지막 위치를 찾는다.
            lower = 0;
            for(i=0;i<6;i++){
                if(value[0][i] <= 2) lower = i;
            }
            if(lower != 0) i = rand()%lower;
            else            i = 0;
            
            value[0][i] = 1;
            value[1][i] = n;
            /*

            // 중간 출력
            printf("Search....%d\r\n",lower);
            for(i=0;i<6;i++){
                printf("%d\t",value[0][i]);
            }
            printf("\r\n");
            for(i=0;i<6;i++){
                printf("%d\t",value[1][i]);
            }
            printf("\r\n");
            getch();
            */
        break;
        }
/*
        // 배열에 0이 있다면 저장
        if(verify(n) == 1) continue;
        
        // 같은 숫자가 있는지 확인한다.
        if(verify(n) == 0){
            
            
            // 2이하의 마지막 위치를 찾는다.
            lower = 0;
            for(i=0;i<6;i++){
                if(value[0][i] <= 2) lower = i;
            }
            i = rand()%lower;
            value[1][i] = 1;
            value[1][i] = n;
        }
*/
        // 같은 숫자를 SEARCH_COUNT 번씩 찾았는지 확인한다.
        for(i=0;i<6;i++){
            if(value[0][i] < SEARCH_COUNT) break;
        }
        if(i == 6) break;


    }
    
    printf("Search complete\r\n");
    for(i=0;i<6;i++){
        printf("%d\t",value[0][i]);
    }
    printf("\r\n");
    for(i=0;i<6;i++){
        printf("%d\t",value[1][i]);
    }
    printf("\r\n");
    getch();
    return 0;
}
