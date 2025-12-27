#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 1024
#define MAX_SCORES 10

// Used for sorting top scores
int _compare(const void* a, const void* b){
	int x = *(const int*) a;
	int y = *(const int*) b;
	return y - x;
}

void save_score(int score){

	cJSON* root   = NULL;
	cJSON* scores = NULL;

	FILE* f = fopen("data/scores.json", "r");
	if(f){
		fseek(f, 0, SEEK_END);
		long len = ftell(f);
		rewind(f);

		char* data = malloc(len + 1);
		fread(data, 1, len, f);
		data[len] = '\0';
		fclose(f);
		
		root = cJSON_Parse(data);
		if(!root){
			printf("JSON parse failed\n");
		}
		free(data);
	}

	if(!root){
		root = cJSON_CreateObject();
	}

	scores = cJSON_GetObjectItem(root, "Scores");
	if(!scores){
		scores = cJSON_CreateArray();
		cJSON_AddItemToObject(root, "Scores", scores);
	} else if(!cJSON_IsArray(scores)){
		cJSON_Delete(root);
		return;
	}

	cJSON* entry = cJSON_CreateObject();
	cJSON_AddNumberToObject(entry, "Score", score);
	cJSON_AddItemToArray(scores, entry);

	char* outp = cJSON_Print(root);
	f = fopen("data/scores.json", "w");
	if(f){
		fputs(outp, f);
		printf("Added new score to db\n");
		fclose(f);
	}

	cJSON_free(outp);
	cJSON_Delete(root);
}

int fetch_scores(int* res){

	FILE* f = fopen("data/scores.json", "r");
	    if (!f) {
	        perror("fopen");
	        return 0;
	    }
	
	    fseek(f, 0, SEEK_END);
	    long len = ftell(f);
	    rewind(f);
	
	    if (len <= 0) {
	        fclose(f);
	        return 0;
	    }
	
	    char* data = malloc(len + 1);
	    fread(data, 1, len, f);
	    data[len] = '\0';
	    fclose(f);
	
	    cJSON *root = cJSON_Parse(data);
	    free(data);
	
	    if (!root) {
	        fprintf(stderr, "JSON parse error\n");
	        return 0;
	    }
	
	    cJSON* scores = cJSON_GetObjectItem(root, "Scores");
	    if (!cJSON_IsArray(scores)) {
	        cJSON_Delete(root);
	        return 0;
	    }
	
	    int count = 0;
	    cJSON* entry;
	    cJSON_ArrayForEach(entry, scores) {
	        if (count >= MAX_SCORES) break;
	
	        cJSON* score = cJSON_GetObjectItem(entry, "Score");
	        if (!cJSON_IsNumber(score)) continue;
	
		if(score->valueint > 0 && score->valueint < 1000){
	        	res[count++] = score->valueint;
	    	} else {
			res[count++] = 0;
		}
	    }
	
	    cJSON_Delete(root);
	    return count;
}
