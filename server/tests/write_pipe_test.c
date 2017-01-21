#include "write_pipe.h"

int main(int argc, char **argv) {
  write_pipe(STDOUT_FILENO, "Hello World !\n");
  write_pipe(STDOUT_FILENO, "%s", "Hello World (bis)\n");
  write_pipe(STDOUT_FILENO, "%s %s\n", "Concat", "two strings");
  write_pipe(STDOUT_FILENO, "Concat with %% and %sHere is a quote : \", and a percent : %s", "a strong string.\n", "%");

  write_pipe(STDOUT_FILENO, "%d\n", 0);
  write_pipe(STDOUT_FILENO, "%d\n", 10);
  write_pipe(STDOUT_FILENO, "%d\n", -100);
  
  int d = 12345;
  write_pipe(STDOUT_FILENO, "%d\n", d);
  d = -678;
  write_pipe(STDOUT_FILENO, "%d\n", d);
  int a = 2, b = 3, c = 4;
  write_pipe(STDOUT_FILENO, "%d\n", a*b*c);
  d = 1002;
  write_pipe(STDOUT_FILENO, "%d\n", d+b+c-a);

  a = 'B';
  write_pipe(STDOUT_FILENO, "%c", 'A');
  write_pipe(STDOUT_FILENO, "%c", (char)a);
  write_pipe(STDOUT_FILENO, "%c", '\n');
  
  write_pipe(STDOUT_FILENO, "%f\n", 255.00);
  write_pipe(STDOUT_FILENO, "%f\n", 0.0);
  write_pipe(STDOUT_FILENO, "%f\n", 0.51234);
  write_pipe(STDOUT_FILENO, "%f\n", -0.5);
  write_pipe(STDOUT_FILENO, "%f\n", 0.75);
  write_pipe(STDOUT_FILENO, "%f\n", 203.775);
  write_pipe(STDOUT_FILENO, "%f\n", -203.775);
  write_pipe(STDOUT_FILENO, "%f\n", -302.703);
  write_pipe(STDOUT_FILENO, "%f\n", 302.703);
	     
  double da = 1.5, db = 2, dc = 7.503;
  double pi = 3.14159265359;
  write_pipe(STDOUT_FILENO, "%f\n%f\n", da*db+dc, pi);

  write_pipe(STDOUT_FILENO, "Here's a %s example with string : %s\nNumber : %d, Float %f and even a char : %c.\n", "complete", "[foobar]%s", d, -213.97054, 'C');
  
  return EXIT_SUCCESS;
}
