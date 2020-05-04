#include<stdio.h>

#include<string.h>
 
int cur = 0;
/**
 * 
 * |_长度_|png图片| ... -1|
 *    4    变长      N  
 * 
 * */
char data[1024*1024];
int len;
void package() {
    FILE *out = fopen("s.res","wb"); 
    for(int i = 1; i<= 10;i++) {
        char fname[128] = {"png/651-"};
        if(i <= 9) {
            fname[8]='0'+i;
            fname[9]='\0';
        }else if(i < 20) {
            fname[8]= '1';
            fname[9]='0'+i-10;
            fname[10]= '\0';
        }
        strcat(fname,".png");
        FILE *t = fopen(fname,"rb");
        int ii = 0;
        len = 0;
        char c = fgetc(t);
        while (!feof(t))
        {
            data[ii++] = c;
            c = fgetc(t);
        }
        len = ii;
        fwrite(&len,4,1,out);
        fwrite(data,len,1,out);
        fclose(t);
    }
    int n = -1;
    fwrite(&n,4,1,out);
    fclose(out);
}

int main(int argc,char* agrv[]) {
    package();
    return 0;
}
