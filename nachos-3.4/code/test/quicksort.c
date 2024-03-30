#include "syscall.h"

int partition(int arr[],int low,int high)
{
  //choose the pivot
  
  int pivot = 0;
  int i;
  int j;
  int temp;
  pivot=arr[high];
  //Index of smaller element and Indicate
  //the right position of pivot found so far
  i=(low-1);
  j = low;
  for(;j<=high;j++)
  {
    //If current element is smaller than the pivot
    if(arr[j]<pivot)
    {
      //Increment index of smaller element
      i++;
      temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
    }
  }
  temp = arr[i+1];
  arr[i+1] = arr[high];
  arr[high] = temp;
  return (i+1);
}

void quickSort(int arr[],int low,int high)
{
  // when low is less than high
  if(low<high)
  {
    // pi is the partition return index of pivot
    
    int pi = 0;
    pi=partition(arr,low,high);
    
    //Recursion Call
    //smaller element than pivot goes left and
    //higher element goes right
    quickSort(arr,low,pi-1);
    quickSort(arr,pi+1,high);
  }
}

int main()
{
	int arr[100];
	int n;
	int i;
	n = 100;
	
	PrintString("\n\n----QUICK SORT---\n\n");
	while (n >= 100)
	{
		PrintString("Array length (n < 100): ");
		n = ReadInt();
	}
	
	PrintString("Elements input: \n");
	
	for (i = 0; i < n; i++)
	{
		PrintString("Element ");
		PrintInt(i+1);
		PrintString(": ");
		arr[i] = ReadInt();
	}
	
	PrintString("\n\----SORTING----\n\n");
	quickSort(arr, 0, n - 1);
	
	PrintString("\n\n----FINAL RESULT----\n");
	
	for (i = 0; i < n; i++)
	{
		PrintInt(arr[i]);
		PrintString(" ");
	}
	return 0;
}
             
 
