#include<iostream>

void insertSort(int *arr, int size) {
    for(int i = 1; i<size; i++) {
        int t = arr[i];
        int j;
        for(j = i -1; j >= 0; j--) {
            if(arr[j] > t) {
                arr[j+1] = arr[j];
            }else {
                break;
            }
        }
        arr[j+1] = t;
    }
}

int main() {

    int arr[] = {1,2,3,5,99,43,5,5,6,66};
    int SZ = 10;

    for(int i = 0; i< SZ; i++) {
        std::cout << arr[i] << ",";
    }
    std::cout << std::endl;

    insertSort(arr,SZ);
    
    for(int i = 0; i< SZ; i++) {
        std::cout << arr[i] << ",";
    }
    std::cout << std::endl;


    return 0;
}
