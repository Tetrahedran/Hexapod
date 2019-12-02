#ifndef SORT_H
#define SORT_H
#include <asf.h>

struct Pwmlength {
	uint8_t pwmlength;
	uint8_t pin;
};

extern void sort(struct Pwmlength *pwmlengths);

#endif