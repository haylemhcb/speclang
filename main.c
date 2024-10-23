#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PGN
{
  FILE *f;
  FILE *scode;
} pgn = {NULL, NULL};


void open_pgn(void);
void close_pgn(void);
void interpreter(const char *codefile);
void process_code(FILE *scode);

/* implementa title "titulo de la pagina" */
void implement_title(const char *line);
void implement_header(const char *line);


int main(int argc, char *argv[])
{
  char cmd[500] = {'\0'};

  if(argc != 3)
  {
    printf("Uso: %s <sourceprog> <port>\n", argv[0]);
    return 1;
  }

  if(argv[1][0] == '\0') return 1;
  if(argv[1] == NULL) return 1;
  if(argv[2][0] == '\0') return 1;
  if(argv[2] == NULL) return 1;

  open_pgn();
  interpreter(strdup(argv[1]));
  close_pgn();

  sprintf(cmd, "python3 ./server.py %s", argv[2]);
  system(cmd);

  return 0;
}

void open_pgn(void)
{
  pgn.f = fopen("index.html", "w");
  if (pgn.f == NULL) return;
}


void close_pgn(void)
{
  if (pgn.f == NULL) return;
  fclose(pgn.f);;
}

void interpreter(const char *codefile)
{
  if (pgn.f == NULL) return;

  pgn.scode = fopen(codefile, "r");

  if(pgn.scode == NULL) return;

  printf("[...] Ejecutando %s\n", codefile);
  process_code(pgn.scode);

  fclose(pgn.scode);
}

void process_code(FILE *scode)
{
  char line[500] = {'\0'};

  fgets(line, sizeof(line), scode);
  while(!feof(scode))
  {
    line[strlen(line) - 1] = '\0';
    implement_title(line);
    implement_header(line);
    fgets(line, sizeof(line), scode);
  }
}

void implement_title(const char *line)
{
    char *tok1 = NULL;
    char *tok2 = NULL;

    if(strstr(strdup(line), "title") != NULL)
    {
      tok1 = strtok(strdup(line), ":");
      if(tok1 != NULL)
      {
        tok2 = strtok(NULL, ":");
        if(tok2 != NULL)
        {
          fprintf(pgn.f, "<title>%s</title>\n", strdup(tok2));
        }
      }
    }

}


void implement_header(const char *line)
{
    char *tok1 = NULL;
    char *tok2 = NULL;
    char *tok3 = NULL;
    char *tok4 = NULL;
    char *tok5 = NULL;
    char snum = '\0';
    char cnum[80] = {'\0'};

    unsigned short int num = 0;


    if(strstr(strdup(line), "header") != NULL)
    {
      tok1 = strtok(strdup(line), ":");
      if(tok1 != NULL)
      {
        snum = tok1[strlen(tok1) - 1];
        tok2 = strtok(NULL, ":");
        if(tok2 != NULL)
        fprintf(pgn.f, "<h%c>%s</h%c>\n", snum, tok2, snum);
      }
    }

}

