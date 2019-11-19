#include<stdio.h>

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

int getbit(unsigned char buf[],int i) { // 0 ~ 127 位
    unsigned char mask = 1;
    mask = mask << (i%8);
    return  buf[i/8]  & mask;
}

#include<iostream>
#include<string>
#include<vector>
std::vector<std::string> Code; // 各字符编码
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

// 文件中出现字符的个数
void decode(char *srcf,char *tagf) {
    FILE *src = fopen(srcf,"rb");
    FILE *tag = fopen(tagf,"wb");
    SIZE = 0;
    maxIndex = 257;
    fread(&SIZE,4,1,src);   //4bit

    huffman[0].weight = 0;
    huffman[0].tag = 1;
    for(int i = 1; i <= 256; i++) { // 256 * 4 bit
        huffman[i].tag = 1;
        int tmp;
        fread(&tmp,4,1,src); 
        huffman[i].weight = tmp;
        if(huffman[i].weight) huffman[i].tag = 0;
    }

    CreateHuffmanTree();
    table();

    unsigned char buf[16] = {0}; // 最大深度为 129 最大编码个数为 128位 16字节
    int p = maxIndex-1; //指向huffman树根

    fread(buf,16,1,src);
//当只有一种字符时后面考虑
    while (!feof(src) && SIZE > 0) {
        for(int i = 0; i<128;i++) {
            if(p < 257) {
                int __tmp = p - 1;
                fwrite(&__tmp,1,1,tag);
                SIZE --;
                p = maxIndex - 1;//重新指向根结点
            }
            if(SIZE == 0) break;
            int bit = getbit(buf,i);
            if(bit) {
                p = huffman[p].lchild;
            }else {
                p = huffman[p].rchild;
            }
        }
        fread(buf,16,1,src);
    }
    if(SIZE > 0) {
        for(int i = 0; i<128;i++) {
            if(p < 257) {
                int __tmp = p - 1;
                fwrite(&p,1,1,tag);
                SIZE --;
                p = maxIndex - 1;//重新指向根结点
            }
            if(SIZE == 0) break;
            int bit = getbit(buf,i);
            if(bit) {
                p = huffman[p].lchild;
            }else {
                p = huffman[p].rchild;
            }
        }
    }
    fclose(src);
    fclose(tag);
}

int main(int argc,char *argv[]) {
    if(argc < 3) return 0;
    if(!argv[1]) return 0;
    if(!argv[2]) return 0;
    decode(argv[1],argv[2]);
    return 0;
}