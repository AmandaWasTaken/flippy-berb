#include "scores.h"
#include <stdio.h>

int main(){

	int scores[10];
	int cnt = fetch_scores(scores);

	for(int i = 0; i < 10; i++){
		if(scores[i] > 0 && scores[i] < 1000)
			printf("%i\n", scores[i]);
	}

	return 0;
}
