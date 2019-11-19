#include<vector>
#include<stack>
#include<iostream>

typedef struct HuffmanTreeNode {
    int weight; 
    int parent;
    int lchild;
    int rchild;
    int tag;
    HuffmanTreeNode() :weight(0),parent(0),lchild(0),rchild(0),tag(1){};
}HuffmanTreeNode;


int maxIndex; //最大有效下标(+1)
HuffmanTreeNode huffman[256*2]; // huffman 树

std::vector<std::string> Code; // 各字符编码

int SIZE;

//获得最小的
int min(int &m) {
    m = 0 ;
    int i = 1;
    for( ; i< maxIndex; i++) {
        if(huffman[i].tag == 0) {
            m = i;
            break;
        }
    }

    if(m == 0) return 0;

    for(int i = 1; i < maxIndex; i++) {
        if(!huffman[i].tag) {
            if(huffman[m].weight > huffman[i].weight) m = i;
        }
    }
    return 1;
}


#include<stdio.h> 

// 文件中出现字符的个数
void count(char *filename) {
    SIZE = 0;
    maxIndex = 257;
    for(int i = 0; i < maxIndex; i++) {
        huffman[i].weight = 0;
        huffman[i].tag = 1;
    }
    FILE *f = fopen(filename,"rb");
    char t = 0;
    t = getc(f);
    while (!feof(f)) {
        int index = 0;
        *((char*)&index) = t; 
        SIZE++;
        huffman[ index + 1 ].weight  ++;
        huffman[ index + 1 ].tag  = 0;
        t = getc(f);
    }
    fclose(f);
}

void CreateHuffmanTree() {
    while (true)
    {
        int a,b;
        if(min(a)) {
            huffman[a].tag = 1;
        }else {
            break;
        }

        if(min(b)) {
            huffman[b].tag = 1;
        }else { 
            break; 
        }
        HuffmanTreeNode h;
        huffman[a].parent = maxIndex;
        huffman[b].parent = maxIndex;

        h.lchild = a;
        h.rchild = b;
        h.parent = 0;
        h.tag = 0;
        h.weight = huffman[a].weight + huffman[b].weight;
        huffman[maxIndex] = h;
        maxIndex ++;
    }
}


//使用huffman编码
void table() {
    for(int i = 0; i< 256;i++) { 
        std::string tmp = "";
        int t = i+1;
        while(huffman[t].parent) {
            int m = huffman[t].parent;
            if(huffman[m].lchild == t) {
                tmp = "1"+tmp;
            } else {
                tmp = "0"+tmp;
            }
            t = huffman[t].parent;
        }
        Code.push_back(tmp);
    }
    for(int i = 0; i < 256; i++) {
        if(huffman[i+1].weight) { // 如果权值>0说明，存在这个字符，才计算 编码
            std::cout /*<< ((char)(i))*/ <<"ASCII::(" << ((int)i)<< ")"<< ":" << huffman[i+1].weight<<"\t\tcode:"<< Code[i]<<std::endl; 
        }  
    }

}

void setbit(unsigned char buf[],int i,int val) { // 0 ~ 127 位
    uint8_t mask = 1 << (i%8);
    if(val) {
        buf[i/8] = buf[i/8]  | mask;
    }else {
        mask = ~mask;
        buf[i/8] = buf[i/8]  & mask;
    }
}

/*
默认 大端
生成文件说明：
[4字节]         文件大小
[256*4字节]       ascii 0 - 255 出现次数
[数据]          huffman编码数据部分

Note
为了方便读取，我们把数据分为 16字节一组，如果最一组数据不够16字节也存16字节
*/

void code(char *srcf,char *tagf) {
    unsigned char buf[16] = {0}; // 最大深度为 129 最大编码个数为 128位 16字节
    int p = 0;
    FILE *src = fopen(srcf,"rb");
    FILE *tag = fopen(tagf,"wb");
    fwrite(&SIZE,4/*sizeof(int)*/,1,tag);//保存文件大小                             4bit
    for(int i = 1; i<= 256; i++) {
        fwrite(&(huffman+i)->weight,4/*sizeof(int)*/,1,tag);            //256 * 4   bit
    }
    char t = 0; // 这里开始我还不知道有 unsigned char这种类型，所以 用char  
    t = getc(src);
    while (!feof(src)) {
        int index = 0;
        *((char*)&index) = t; 
        std::string strcode = Code[index];

        int size = strcode.size();
        for(int i = 0; i < size; i++) {
            if(p>127) {
                fwrite(buf,16,1,tag);
                p = 0;
            }
            switch (strcode[i])
            {
            case '0':
                setbit(buf,p++,0);
                break;
            case '1':
                setbit(buf,p++,1);
                break;
            default:
                break;
            }
        }

        t = getc(src);
    }
    if(p>0) {
        fwrite(buf,16,1,tag);
    }
    fclose(src);
    fclose(tag);
}
#include<string.h>
int main(int argc,char *argv[]) {
        char *def = ".huf";
        char fname[2550]={'\0'};
        if(argc < 2) return 0;
        for(int i = 1; i< argc;i++) {
            count(argv[i]);
    //        std::cout <<SIZE<<std::endl;
            CreateHuffmanTree();
            table();
            fname[0] = '\0';
            strcat(fname,argv[i]);
            strcat(fname,def);
            code(argv[1],fname);  
        }

        return 0;
}
