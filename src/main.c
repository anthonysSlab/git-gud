#include "raylib.h"
#include <stdio.h>   // printf()
#include <string.h>  // strtok(), strcpy(), strcat(), strlen()
#include <stdlib.h>  // exit()

#define MAX_INPUT_CHARS 255
#define FONT_SIZE 20.0f
#define CHAR_SPACING 0.0f
#define HELP "help - prints this message\nexit - exits the shell\nprompt - changes the prompt\n";

char prompt[] = "> \0";
char old_buf[MAX_INPUT_CHARS + 1] = "\0";
unsigned int old_offset = 40;

char* git_handler(char* arg) {
   return "git handler\n";
}

char* command_handler(char* command) {
   char* cmd = strtok(command, " ");

   if (strcmp(cmd, "help") == 0) {
      return HELP;
   } 

   if (strcmp(cmd, "exit") == 0) {
      exit(0);
   } 

   if (strcmp(cmd, "echo") == 0) {
      char* arg = command += strlen(cmd) + 1;
      if (*arg == '\0') return "Missing Arg\n";

      strcat(arg, "\n");
      return arg;
   }

   if (strcmp(cmd, "git") == 0) {
      char* arg = strtok(NULL, " ");
      return git_handler(arg);
   }

   if (strcmp(cmd, "prompt") == 0) {
      char* new_prompt = strtok(NULL, " ");
      if (new_prompt == NULL) return "Missing Arg\n";

      strcpy(prompt, new_prompt);
      strcat(prompt, " ");
      return "";
   }

   if (strcmp(cmd, "clear") == 0) {
      memset(old_buf, 0, MAX_INPUT_CHARS);
      old_offset = 40;
      return "";
   }

   return "Unknown command\n";
}

int strlines(const char* str) {
   int lines = 0;
   for (; *str; str++)
      if (*str == '\n') lines++;

   return lines;
}

void clip(char* str, int lines) {
   char* src = str;

   for (int i = 0; i < lines; i++) {
      src = strchr(src, '\n');
      if (src == NULL) return;
      src++;
   }

   memmove(str, src, strlen(src) + 1);
}

int main(void) {
   int h_ = 600;
   int w_ = 800;

   InitWindow(w_, h_, "type");

   char in_buf[MAX_INPUT_CHARS + 1] = "\0";
   strcpy(in_buf, prompt);

   unsigned int letterCount = 0;


   Font font = LoadFont(ASSETS_PATH"/MartianMono-Regular.ttf");
   SetTextLineSpacing(20);

   int framesCounter = 0;
   SetTargetFPS(60);   // this determines the speed n stuff
   // main loop
   while (!WindowShouldClose()) {
      if (IsWindowResized()) {  // update size if window is resized
         h_ = GetScreenHeight();
         w_ = GetScreenWidth();
      }

      int key = GetCharPressed();
      while (key > 0) {  // iter over all pressed keys
         if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS)) {
            in_buf[letterCount + strlen(prompt)] = (char)key;
            letterCount++;
         }
         key = GetCharPressed();
      }

      if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
         letterCount--;
         in_buf[letterCount + strlen(prompt)] = '\0';
      }

      if (IsKeyPressed(KEY_ENTER)) {
         if (strlen(in_buf) == strlen(prompt)) {
            strcat(old_buf, prompt);
            strcat(old_buf, "\n");
            old_offset += 20;
         } 

         else {
            strcat(old_buf, in_buf);
            strcat(old_buf, "\n");
            old_offset += 20;

            char* _stdout = command_handler(in_buf + strlen(prompt));
            strcat(old_buf, _stdout);

            for (int i = 0; i < strlen(_stdout); i++) {
               if (_stdout[i] == '\n')
                  old_offset += 20;
            }

            memset(in_buf, 0, MAX_INPUT_CHARS);   // clear the buffer
            
            letterCount = 0;
            strcat(in_buf, prompt); // add prompt
         }
      }

      // limit the number of lines in the old_buf
      const unsigned int max_lines = (h_ - 40) / 20;
      const int current_lines = strlines(old_buf);
      if (current_lines > max_lines) {
         const int diff = current_lines - max_lines;
         clip(old_buf, diff);
         old_offset -= diff * 20;
      }

      framesCounter++;
      // ------------------------------

      // DRAW 
      // ------------------------------
      BeginDrawing();
         ClearBackground(BLACK);
         DrawTextEx(font, in_buf, (Vector2){5, h_-40}, FONT_SIZE, CHAR_SPACING, WHITE);
         DrawTextEx(font, old_buf, (Vector2){5, h_ - old_offset}, FONT_SIZE, CHAR_SPACING, LIGHTGRAY);

         // blinking cursor
         if (letterCount < MAX_INPUT_CHARS && (framesCounter/20)%2 == 0) {
            Vector2 d = MeasureTextEx(font, in_buf, FONT_SIZE, CHAR_SPACING);
            DrawTextEx(font, "_", (Vector2){8+d.x, h_-40}, FONT_SIZE, CHAR_SPACING, WHITE);
         }
      EndDrawing();
      // ------------------------------
   }

   //
   // De-Initialization
   // ------------------------------
   CloseWindow();
   // ------------------------------
   
   return 0;
}
