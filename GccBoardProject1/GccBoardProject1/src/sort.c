#include "sort.h"

struct Pwmlength* quicksort(struct Pwmlength pwmlengths[6],uint8_t first,uint8_t last);

struct Pwmlength* quicksort(struct Pwmlength pwmlengths[6],uint8_t first,uint8_t last)
{
	uint8_t i, j, pivot, temp;

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
				temp=pwmlengths[i].pwmlength;
				pwmlengths[i].pwmlength=pwmlengths[j].pwmlength;
				pwmlengths[j].pwmlength=temp;
			}
		}

		temp=pwmlengths[pivot].pwmlength;
		pwmlengths[pivot].pwmlength=pwmlengths[j].pwmlength;
		pwmlengths[j].pwmlength=temp;
		quicksort(pwmlengths,first,j-1);
		quicksort(pwmlengths,j+1,last);
	}
	return pwmlengths;
}

struct Pwmlength* sort(struct Pwmlength pwmlengths[6])
{
	return quicksort(pwmlengths, 0, 5);
}
