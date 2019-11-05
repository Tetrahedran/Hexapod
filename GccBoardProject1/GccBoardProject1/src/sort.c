#include "sort.h"

struct Pwmlength* quicksort(struct Pwmlength pwmlengths[6],uint8_t first,uint8_t last);

struct Pwmlength* quicksort(struct Pwmlength pwmlengths[6],uint8_t first,uint8_t last)
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
	return pwmlengths;
}

struct Pwmlength* sort(struct Pwmlength pwmlengths[6])
{
	return quicksort(pwmlengths, 0, 5);
}
