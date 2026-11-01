#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <error.h>

static void help_exit();
static void process(const char* code);
static char* read_file(const char* file_path);

int
main(int argc, char** argv) 
{	
	if (argc == 1) help_exit();
	
	char* code = read_file(argv[1]);
	if (code == NULL) err(1, argv[1]);

	process(code);
	free(code);
	return 0;
}

static void
interprete_command(unsigned char command, unsigned char* memset, int* current, int* line)
{
	switch(command) {
		case '+': 
			++memset[*current];
			break;
		case '-':
			--memset[*current];
			break;
		case '.': 
			putchar(memset[*current]);
			break;
		case '>':
			++(*current);
			break;
		case '<':
			--(*current);
			break;
		case ',':
			memset[*current] = getchar();
			break;
		case '\n':
			++(*line);
		case '\t':
		case '\r':
		case  ' ':
			break;

		default:
			// ignore 
			break;

	}
}

static void
process(const char* code)
{
	int line = 1;

	unsigned char memset[30000] = {0};
	int current                 =  0;

	for (size_t i = 0; i < strlen(code); ++i) {
		switch(code[i]) {
			case '[': {
				if (memset[current] != 0) break;
				int depth = 1;
				while (depth > 0 && ++i < strlen(code)) {
					if (code[i] == '\n') line++;
					else if (code[i] == '[') depth++;
					else if (code[i] == ']') depth--;
				}
				break;
			}
			case ']': {
				if (memset[current] == 0) break;
				int depth = 1;
				while (depth > 0 && i > 0) {
					--i;
					if (code[i] == '\n') line++;
					else if (code[i] == ']') depth++;
					else if (code[i] == '[') depth--;
				}	
				break;
			}
			default:
				interprete_command(code[i], memset, &current, &line);
				break;
		}
	}
}

static char* read_file(const char* file_path) {
	size_t size;
	FILE *f = fopen(file_path, "rb");
	if (!f) return NULL;
	
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek (f, 0, SEEK_SET);
	
	char *buffer = malloc(size);
	fread (buffer, 1, size, f);
	fclose (f);
	buffer[size] = '\0';
	return buffer;
}

static void help_exit() {
	printf("BrainFuck interpreter\n");
	printf("Usage: bfi <file>\n");
	exit(1);
}
