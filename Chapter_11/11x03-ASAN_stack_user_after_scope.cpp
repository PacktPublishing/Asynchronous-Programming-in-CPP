
// Check can be disabled in run-time by running:
// ASAN_OPTIONS=detect_stack_use_after_scope=0 <program>

volatile int *p = 0;

int main() {
  {
    int x = 0;
    p = &x;
  }
  *p = 5;
  return 0;
}