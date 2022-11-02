#define mu_assert(test) do { if (!(test)) return "FAILED: "#test; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                            if (message) return message; } while (0)