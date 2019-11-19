#include<iostream>

void bobleSort(int arr[], int size) {
    for(int i = 0; i < size - 1; i++) {
        for(int j = 1; j < size - i;j++) {
            if(arr[j] < arr[j-1]) {
                int t = arr[j];
                arr[j] = arr[j-1];
                arr[j-1] = t;
            }
        }
    }
}


int main() {

    int arr[] = {1,2,3,5,99,43,5,5,6,66};
    int SZ = 10;

    for(int i = 0; i< SZ; i++) {
        std::cout << arr[i] << ",";
    }std::cout << std::endl;

    bobleSort(arr,SZ);
    
    for(int i = 0; i< SZ; i++) {
        std::cout << arr[i] << ",";
    }std::cout << std::endl;


    return 0;
}
