#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN 100 // 한 단어 최대 길이
#define BUF_SIZE 4096 // 버퍼 크기

typedef struct {
    unsigned long word_count;
    unsigned long line_count;
} WordCount;

typedef struct {
    char word[MAX_TOKEN];
    unsigned long freq;
} WordFreq;

//함수 선언
void print_usage(const char *progname);
FILE *open_text_file(const char *path);
WordCount count_words(FILE *fp, WordFreq* dict, size_t* dict_size, size_t* dict_capacity);
void add_word(WordFreq* dict, size_t* dict_size, size_t* dict_capacity, const char* token);
int compare_word(const void* a, const void* b);
void print_top_words(WordFreq* dict, size_t dict_size, size_t top_n);

int main(int argc, char *argv[])
{
    WordFreq* dict = NULL;
    size_t dict_size = 0;
    size_t dict_capacity = 0;

    if (argc != 2)
    {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    FILE *fp = open_text_file(argv[1]);
    if (!fp) return EXIT_FAILURE;

    // 초기 사전 크기 설정
    dict_capacity = 128;
    dict = (WordFreq*)malloc(dict_capacity * sizeof(WordFreq));
    if (!dict) {
        perror("메모리 할당 실패");
        fclose(fp);
        return EXIT_FAILURE;
    }

    WordCount wc = count_words(fp, dict, &dict_size, &dict_capacity);
    printf("단어 개수: %lu\n", wc.word_count);
    printf("줄 개수: %lu\n", wc.line_count);
    print_top_words(dict, dict_size, 10);

    free(dict);
    fclose(fp);

    printf("프로그램 종료\n");

    return EXIT_SUCCESS;
}

// 함수 정의
void print_usage(const char *progname)
{
    printf("사용법: %s <텍스트 파일 경로>\n", progname);
}

FILE *open_text_file(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        perror("파일 열기 실패");
    }
    return fp;
}

/* 핵심 함수: 단어 세기 */
WordCount count_words(FILE *fp, WordFreq* dict, size_t* dict_size, size_t* dict_capacity)
{
    char buffer[BUF_SIZE];
    WordCount wc = {0, 0};
    int in_word = 0;
    int in_line = 0;
    char token[MAX_TOKEN];
    size_t token_len = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        for (char *p = buffer; *p; ++p) {
            // 공백 문자 처리
            if (isspace((unsigned char)*p)) {
                in_word = 0;
                if (*p == '\n') in_line = 0;
            } else {
                // 단어 시작
                if (!in_word) {
                    wc.word_count++;
                    in_word = 1;
                    if (!in_line) {
                        wc.line_count++;
                        in_line = 1;
                    }
                }
            }
            // 알파벳 또는 숫자 처리
            if (isalpha((unsigned char)*p) || isdigit((unsigned char)*p)){
                if (token_len < MAX_TOKEN - 1){
                    token[token_len++] = tolower((unsigned char)*p);
                }
            } else {
                // 단어 끝
                if (token_len > 0){
                    token[token_len] = '\0';
                    add_word(dict, dict_size, dict_capacity, token);
                    token_len = 0;
                }
            }
        }
    }
    // 남은 단어 처리
    if (token_len > 0){
        token[token_len] = '\0';
        add_word(dict, dict_size, dict_capacity, token);
    }
    return wc;
}

void add_word(WordFreq* dict, size_t* dict_size, size_t* dict_capacity, const char* token){
    // 사전에 단어가 있는지 확인
    for (size_t i = 0; i < *dict_size; i++){
        if (strcmp(dict[i].word, token) == 0){
            dict[i].freq++;
            return;
        }
    }

    // 사전에 단어가 없으면 추가
    // 메모리 부족 시 메모리 재할당
    if (*dict_size >= *dict_capacity){
        *dict_capacity *= 2;
        WordFreq* new_dict = (WordFreq*)realloc(dict, *dict_capacity * sizeof(WordFreq));
        if (!new_dict) {
            perror("메모리 재할당 실패");
            return;
        }
        dict = new_dict;
    }
    strcpy(dict[*dict_size].word, token);
    dict[*dict_size].freq = 1;
    (*dict_size)++;
}

int compare_word(const void* a, const void* b){
    const WordFreq* A = a;
    const WordFreq* B = b;
    if (A->freq != B->freq)
        return B->freq - A->freq;
    return strcmp(A->word, B->word);
}

void print_top_words(WordFreq* dict, size_t dict_size, size_t top_n){
    qsort(dict, dict_size, sizeof(WordFreq), compare_word);
    size_t count = (top_n < dict_size) ? top_n : dict_size;
    printf("가장 많이 나온 단어:\n");
    for (size_t i = 0; i < count; i++){
        printf("%2zu. %-15s: %lu\n", i+1, dict[i].word, dict[i].freq);
    }
}
