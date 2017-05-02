//Ronald Baasland
//COSC 320 Project
//Parallel merge sort

#include <iostream>
#include <cstdlib> 
#include <time.h>
#include <omp.h>
using namespace std;

int* a = NULL;
void merge(int,int,int,int);
void merge_sort1(int low,int high, int n)
{
	int mid;
	if(low<high)
	{
		mid=(low+high)/2;
		merge_sort1(low,mid,n);
		merge_sort1(mid+1,high,n);
		merge(low,mid,high, n);
	}
}
void merge_sort(int low,int high, int n)
{
	int this_thread = omp_get_thread_num(); //gets current thread number
	int mid;
	if(low<high)
	{
		mid=(low+high)/2;
		#pragma omp parallel //Declare the parallel directive
		{
			if(this_thread == 0) //If the first thread, sort left side
			{
				merge_sort(low,mid,n);
			}
			else //Else its the second thread and it will sort the right side
			{
				merge_sort(mid+1,high,n);
			}
		}
		merge(low,mid,high, n);
	}
}

void merge(int low,int mid,int high, int total)
{
	int* b = NULL;
	b = new int[total];
	int h,i,j,k;
	h=low;
	i=low;
	j=mid+1;
	
	while((h<=mid)&&(j<=high))
	{
		if(a[h]<=a[j])
		{
			b[i]=a[h];
			h++;
		}
		else
		{
			b[i]=a[j];
			j++;
		}
		i++;
	}
	if(h>mid)
	{
		for(k=j;k<=high;k++)
		{
			b[i]=a[k];
			i++;
		}
	}
	else
	{
		for(k=h;k<=mid;k++)
		{
			b[i]=a[k];
			i++;
		}
	}
	for(k=low;k<=high;k++) a[k]=b[k];
	delete [] b;  // When done, free memory pointed to by a.
	b = NULL; 
}

int main()
{
	int num,i;
	cout << "***Running merge sort***"<<endl; 
	cout<<"How many elements do you wish to sort?: ";
	cin>>num;
	cout<<endl;
	a = new int[num];
	for(i=0;i<num;i++)
	{
		int random_integer = rand(); 
		a[i] = random_integer;
	}
	cout<<"The array is being sorted."<<endl;
	cout<<endl;
	clock_t start = clock(), finish;
	merge_sort(0,num-1, num);
	finish = clock();
	double total = double( finish - start ) / (double)CLOCKS_PER_SEC;
	cout << "Parallel merge sort: " << total << " seconds." << endl;
	clock_t start1 = clock(), finish1;
	merge_sort1(0,num-1, num);
	finish1 = clock();
	double total1 = double( finish1 - start1 ) / (double)CLOCKS_PER_SEC;
	cout << "Sequential Merge Sort: " << total1 << " seconds." << endl;
	if (total < total1)
	{
		cout << "Parallel merge sort is faster" <<endl;
		cout << "Speed up: " << total1 / total << endl;
	}
	else
	{
		cout << "Sequential merge sort is faster" <<endl;
		cout << "Speed up: " << total / total1 << endl;
	}

	delete [] a;  // When done, free memory pointed to by a.
	a = NULL; 
	return 0;
	
}
