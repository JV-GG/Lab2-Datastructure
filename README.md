# Emergency Department Officer — Priority Queue (C++)

This console application simulates an Emergency Department Officer workflow using a priority queue to manage emergency cases by severity.

- Highest priority number is treated as the most critical (e.g., 5 is critical, 1 is low).
- When two cases have the same priority, the one logged earlier is processed first.

## Features
- Log Emergency Case (patient name, type of emergency, priority)
- Process Most Critical Case (attend highest-priority pending case)
- View Pending Emergency Cases (displayed in priority order)

## Build

Using Make (recommended):

```bash
make
```

This produces an executable named `emergency_officer`.

Without Make:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -c functionality.cpp -o functionality.o
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -c main.cpp -o main.o
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -o emergency_officer main.o functionality.o
```

## Run

```bash
./emergency_officer
```

Follow the on-screen menu to log, process, and view cases.

## Notes
- Priority input range is 1 (low) to 5 (critical), but you can change bounds in `main.cpp` if required.
- Viewing pending cases does not change the queue; it uses a temporary copy to display items in order.
