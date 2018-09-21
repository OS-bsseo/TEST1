#include <stdio.h>
#include <conio.h>

#define SEARCH_COUNT    5
int value[2][6]={0,};

int verify(int n)
{
    int i,j;
    
    // �ߺ��� ���� ����
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
        case 0:     // ���� ���ڰ� �ִٸ� +1 ����
        case 1:     // �迭�� ����ִٸ� �迭�� ���� �߰�
        break;
        case 2:
            // ���� ���ڰ� ���ٸ� 2 ���� �߿� �ϳ��� ���ο� ���ڷ� ��ü�Ѵ�.
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

            // 2������ ������ ��ġ�� ã�´�.
            lower = 0;
            for(i=0;i<6;i++){
                if(value[0][i] <= 2) lower = i;
            }
            if(lower != 0) i = rand()%lower;
            else            i = 0;
            
            value[0][i] = 1;
            value[1][i] = n;
            /*

            // �߰� ���
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
        // �迭�� 0�� �ִٸ� ����
        if(verify(n) == 1) continue;
        
        // ���� ���ڰ� �ִ��� Ȯ���Ѵ�.
        if(verify(n) == 0){
            
            
            // 2������ ������ ��ġ�� ã�´�.
            lower = 0;
            for(i=0;i<6;i++){
                if(value[0][i] <= 2) lower = i;
            }
            i = rand()%lower;
            value[1][i] = 1;
            value[1][i] = n;
        }
*/
        // ���� ���ڸ� SEARCH_COUNT ���� ã�Ҵ��� Ȯ���Ѵ�.
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
