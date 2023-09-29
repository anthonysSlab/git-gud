#include <ncurses.h>
#include <stdio.h>

int main() {
   initscr(); // Initialize ncurses

    // Enable keypad input (for capturing function keys)
   keypad(stdscr, TRUE);

   printw("Press Enter or Backspace (Escape to exit):\n");

   while (1) {
      int ch = getch(); // Get a character from the user

      if (ch == KEY_ENTER) {
         printw("Enter key pressed\n");
      } else if (ch == KEY_BACKSPACE) {
         printw("Backspace key pressed\n");
      } else if (ch == 27) { // 27 is the ASCII code for Escape
         break; // Exit the loop on Escape key press
      }
   }

   endwin(); // Clean up ncurses
   return 0;
}
