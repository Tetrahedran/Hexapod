#ifndef SORT_H
#define SORT_H
#include <asf.h>

struct Pwmlength {
	uint8_t pwmlength;
	uint8_t pin;
};

extern struct Pwmlength* sort(struct Pwmlength pwmlengths[6]);

#endif