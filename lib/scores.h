// Internal function for comparing 2 numbers 
// Used by qsort() to sort top scores
int _compare(const void* a, const void* b);
void reset_scores(void);
void save_score(int score); // Write score to json
int fetch_scores(int* res); // Fetch scores from json


