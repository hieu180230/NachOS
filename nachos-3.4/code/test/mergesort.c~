#include "syscall.h"

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    // Create temp arrays
    int L[n1], R[n2];
 
    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    // Merge the temp arrays back into arr[l..r
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    // Copy the remaining elements of L[],
    // if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    // Copy the remaining elements of R[],
    // if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
 
        merge(arr, l, m, r);
    }
}

int main()
{
	int arr[100];
	int n;
	int i;
	n = 100;
	
	PrintString("\n\n----MERGE SORT---\n\n");
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
	
	PrintString("\n\n----SORTING----\n\n");
	mergeSort(arr, 0, n - 1);
	
	PrintString("\n\n----FINAL RESULT----\n");
	
	for (i = 0; i < n; i++)
	{
		PrintInt(arr[i]);
		PrintString(" ");
	}
	return 0;
}
