#include "sort.h"

void quicksort(struct Pwmlength *pwmlengths,int8_t first,int8_t last);

void quicksort(struct Pwmlength *pwmlengths,int8_t first,int8_t last)
{
	uint8_t i, j, pivot;
	struct Pwmlength temp;
	if(first<last){
		pivot=first;
		i=first;
		j=last;

		while(i<j){
			while(pwmlengths[i].pwmlength<=pwmlengths[pivot].pwmlength&&i<last)
			i++;
			while(pwmlengths[j].pwmlength>pwmlengths[pivot].pwmlength)
			j--;
			if(i<j){
				temp=pwmlengths[i];
				pwmlengths[i]=pwmlengths[j];
				pwmlengths[j]=temp;
			}
		}

		temp=pwmlengths[pivot];
		pwmlengths[pivot]=pwmlengths[j];
		pwmlengths[j]=temp;
		quicksort(pwmlengths,first,j-1);
		quicksort(pwmlengths,j+1,last);
	}
}

void sort(struct Pwmlength *pwmlengths)
{
	quicksort(pwmlengths, 0, 5);
}
