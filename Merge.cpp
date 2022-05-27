#include <iostream>
#include <vector>

void merge(std::vector<int>& vec, int start, int middle, int end)
{
    int leftSize = middle - start + 1;
    int rightSize = end - middle;
    std::vector<int> leftArr(leftSize);
    std::vector<int> rightArr(rightSize);
    for(int i{}; i < leftSize; ++i) {
        leftArr[i] = vec[start + i];
    }
    for(int j{}; j < leftSize; ++j) {
        rightArr[j] = vec[middle + 1 +j];
    }

    int num1 = 0;
    int num2 = 0;

    for(int k = start; k <= end; ++k) {
        if(num1 < leftSize && num2 < rightSize)
        {
            if(leftArr[num1] < rightArr[num2]){
                vec[k] = leftArr[num1];
                ++num1;
            }
            else {
                vec[k] = rightArr[num2];
                ++num2;
            }
        }
        else if (!num1 < leftSize && num2 < rightSize) {
            vec[k] = rightArr[num2];
            ++num2;
        }
        else {
            vec[k] = leftArr[num1];
            ++num1;
        }
    }
}

void mergeSort(std::vector<int>& vec, int start, int end) {
    int middle = (start + end) / 2;
    if(start < end) {
        mergeSort(vec,start,middle);
        mergeSort(vec, middle + 1, end);
        merge(vec, start, middle, end);
    }
}

void printArray(std::vector<int>& vec, int size) {
  for (int i{}; i < vec.size(); i++)
    std::cout << vec[i] << " " << std::endl;;
}

int main() {
    std::vector<int> vec;
    int size;
    int index;
    std::cout << "Please enter the size of the array" << std::endl;
    std::cin >> size;
    std::cout << "Now please input an array and it will get sorted" << std::endl;
    for( int i = 0; i < size; ++i) {
        std::cin>>index;
        vec.push_back(index);
    }
    std::cout << "Here's the result!" << std::endl;
    mergeSort(vec, 0, vec.size() - 1);
    printArray(vec, vec.size() - 1);
    return 0;
}







