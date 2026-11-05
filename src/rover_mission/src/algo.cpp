#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

vector<int> reversed(vector<int> &nums)
{        
    int left = nums.size() -1;
    for (int i = 0; i < nums.size() /2; i++)
    {
        swap(nums[i],nums[left -i]);
        
    }
    return nums;
}

bool similar(string word)
{
    int left  =word[word.size() -1 ];
    
    left = word.size() -1;
    
   
    

    for (int i = 0; i < word.size() /2; i++)
    {
        if(word[i]!=word[left -i])
        {
             return false;
        }  
                    
    }
    return true;
}


//arr = {4,2,5,7,3} --->  {7,2,4,5,3} -->  {7,5,2,4,3} ---> {7,5,4,3,2}
void odrder (int arr[],int size)
{
    
    for (int i = 0; i < size-1; i++)
    {
        for (int k =  i; k < size-1 ; k++)
        {
           if(arr[i] > arr[k+1])
           {
            swap(arr[i],arr[k+1]);
           }
        }
        
    }
    for (int i = 0; i < size; i++)
    {
        cout<<arr[i]<<" ";
    }
    
    
    
}



int main()
{
    vector<int> nums = {7,6,5,4,3,2,1};

    string word = "";
    reversed(nums);
    

    // for(auto v : nums)
    // {
    //     cout <<" "<<v;
    // }

   int arr[]={23,34,564,67,87};

    //cout<< boolalpha <<similar(word)<<endl;
    int arr1[]= {4,2,5,7,3,77,44,66,55}; //{2,3,4,5,7,44,55,66,77}

    odrder(arr1,9);
    return 0;

}