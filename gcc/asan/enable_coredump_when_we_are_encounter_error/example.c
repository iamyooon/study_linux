#include <stdio.h>

int main(int argc, char** argv)
{
        //char buf[32]={0,};
        int int_buf[32]={0,1,2,3,4,5,6,7,8,9,10,};
        int int_buf2[32]={0,1,2,3,4,5,6,7,8,9,10,};
        int i;
        if (argc > 2){
                int_buf[32] = 2222;
                for(i=0;i<33; i++)
                        printf("buf[%d]=%d\n",i,int_buf[i]);
        } else{
                int_buf[32] = 1111;
                for(i=0;i<33; i++)
                        printf("buf[%d]=%d\n",i,int_buf2[i]);
        }

        return 0;
}
