#include "sort.h"

uint8_t* quicksort(uint8_t number[6],uint8_t first,uint8_t last);

uint8_t* quicksort(uint8_t number[6],uint8_t first,uint8_t last)
{
	uint8_t i, j, pivot, temp;

	if(first<last){
		pivot=first;
		i=first;
		j=last;

		while(i<j){
			while(number[i]<=number[pivot]&&i<last)
			i++;
			while(number[j]>number[pivot])
			j--;
			if(i<j){
				temp=number[i];
				number[i]=number[j];
				number[j]=temp;
			}
		}

		temp=number[pivot];
		number[pivot]=number[j];
		number[j]=temp;
		quicksort(number,first,j-1);
		quicksort(number,j+1,last);
	}
	return number;
}

uint8_t* sort(uint8_t number[6])
{
	return quicksort(number, 0, 5);
}
