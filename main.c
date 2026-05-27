#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 700
#define HEIGHT 270

#define MAX_STR_LEN 500

#define BUTTON_HEIGHT 70
#define BUTTON_MARGIN 10
#define WINDOW_MARGIN 20

#define FLAG_FILE "-f"
#define FLAG_STRING "-s"
#define FLAG_HELP "-h"

typedef struct {

  char *label;
  char *command;

} Option;

typedef struct {
  char **content;
  bool success;
} ParsedOptionFile;

int countOptions(char *inp, char *delimiter) {

  if (inp == NULL || delimiter == NULL) {
    return 0;
  }

  int inp_len = strlen(inp);
  int delim_len = strlen(delimiter);
  if (inp_len == 0 || delim_len == 0) {
    return 0;
  }

  int commands = 1;

  if (delim_len == 1) {
    // Single char delimiter — use strchr
    for (int i = 0; i < inp_len; i++) {
      if (inp[i] == delimiter[0]) {
        commands++;
      }
    }
  } else {
    // Multi-char delimiter — use strstr
    char *cursor = inp;
    char *match;
    while ((match = strstr(cursor, delimiter)) != NULL) {
      commands++;
      cursor = match + delim_len; // advance past the match
    }
  }

  return commands;
}

Option *parseOption(char *inp) {

  int inp_len = strlen(inp);
  Option *opt = malloc(sizeof(Option));

  int c = 0;
  while (c < inp_len) {
    if (inp[c] == ';') {
      break;
    }
    c++;
  }

  opt->label = malloc(c + 1);
  opt->command = malloc(inp_len - c + 1);

  bool sw = 0;
  int i = 0;
  int j = 0;
  while (i < inp_len) {
    if (i == c) {
      j = 0;
      i++;
      sw = !sw;
      continue;
    }
    if (!sw) {
      opt->label[j] = inp[i];
    } else {
      opt->command[j] = inp[i];
    }
    j++;
    i++;
  }

  opt->label[strlen(opt->label)] = '\0';
  opt->command[strlen(opt->command)] = '\0';

  return opt;
}

void parseOptionsFromStringInput(char *inp, Option *optBuf[], int count,
                                 char *delimiter) {

  char *inp_cpy = strdup(inp);

  int input_len = strlen(inp);
  if (input_len > MAX_STR_LEN) {
    exit(1);
  }

  char *token = strtok(inp_cpy, delimiter);
  int index = 0;
  while (token != NULL) {
    Option *op = parseOption(token);
    if (op == NULL) {
      token = strtok(NULL, delimiter);
      continue;
    }

    optBuf[index] = op;

    index++;
    token = strtok(NULL, delimiter);
  }
  free(inp_cpy);
}

typedef struct {
  int count;
  Option **options;

} ParsedOptions;

ParsedOptions parseOptionsFromFile(char *path) {

  int count = 0;
  const int line_count = 20;
  ParsedOptionFile result = (ParsedOptionFile){
      .content = malloc(sizeof(char *) * line_count), .success = false};

  FILE *f = fopen(path, "r");

  char line[256];

  int default_options_array_size = 20;
  int current_size = default_options_array_size;
  Option **opts = malloc(sizeof(Option) + default_options_array_size);

  while (fgets(line, sizeof(line), f)) {
    Option *opt = parseOption(line);
    if (opt == NULL) {
      continue;
    }
    if (current_size <= count) {
      opts = realloc(opts, current_size + default_options_array_size);
    }
    opts[count] = opt;
    count++;
  }

  fclose(f);
  return (ParsedOptions){.options = opts, .count = count};
}

void drawOptionButtons(int count, Option *opts[]) {

  for (int i = 0; i < count; ++i) {
    int margin = 10;
    char *label = opts[i]->label;
    float w = WIDTH * 0.8;
    int xOffset = WIDTH * 0.1;
    int yOffset = BUTTON_HEIGHT * i + (BUTTON_MARGIN * i) + WINDOW_MARGIN;

    float textPosY = yOffset + ((float)BUTTON_HEIGHT / 2 - 8);
    float textPosX = w / 2 - (strlen(label));
    float mouseX = GetMouseX();
    float mouseY = GetMouseY();
    Color color = WHITE;
    bool is_in_bunds_x = mouseX > xOffset && mouseX < xOffset + w;
    bool is_in_bunds_y = mouseY > yOffset && mouseY < yOffset + BUTTON_HEIGHT;

    if (is_in_bunds_x && is_in_bunds_y) {
      // mouse event handler
      if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        system(opts[i]->command);
      }
      color = BLACK;
      DrawRectangle(xOffset, yOffset, w, BUTTON_HEIGHT, WHITE);
    } else {
      DrawRectangle(xOffset, yOffset, w, BUTTON_HEIGHT, BLACK);
    }

    DrawRectangleLines(xOffset, yOffset, w, BUTTON_HEIGHT, WHITE);
    DrawText(label, textPosX + (float)xOffset / 2, textPosY, 18, color);
  }
}

void printHelpFlagMessage() {
  printf("This will only accept 3 flags\n");
  printf("    -f file flag, eg. -f file_path \n");
  printf("    -s string flag eg. -s \"Shutdown;shutdown,Reboot;reboot\" (';' "
         "this is the delimiter used to separate commands)\n");
  printf("    The supported formant for options is [label];[command] eg. "
         "shurdown my pc;/sbin/shutdown\n");
  printf("    -h [This menu]\n");
}

int main(int argc, char **argv) {

  const Color BACKGROUND_COLOR = BLACK;

  if (argv[1] == NULL) {
    printf("Missing arguments.\n\n");
    return 1;
  }

  ParsedOptions opts;

  if (!strcmp(argv[1], FLAG_FILE)) {
    if (argv[2] == NULL) {
      printf("NOT YER SUPPORTED");
      return 1;
    }
    opts = parseOptionsFromFile(argv[2]);
  } else if (!strcmp(argv[1], FLAG_STRING)) {
  } else if (!strcmp(argv[1], FLAG_HELP)) {
    printHelpFlagMessage();
    return 0;
  } else {
    printf("Unsuported input\n");
    return 1;
  }

  float win_height = HEIGHT;
  float win_width = WIDTH;

  win_height = (BUTTON_HEIGHT + BUTTON_MARGIN) * opts.count + WINDOW_MARGIN;

  setenv("RESOURCE_NAME", "kmenu", 1);
  setenv("WM_CLASS", "kmenu", 1);

  InitWindow(win_width, win_height, "kmenu");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    ClearBackground(BACKGROUND_COLOR);
    BeginDrawing();
    drawOptionButtons(opts.count, opts.options);
    EndDrawing();
  }

  free(opts.options);

  return 0;
}
