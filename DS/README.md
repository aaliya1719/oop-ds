# Priority Queue Task Manager

This mini project implements a task manager in C using an array-based **max-heap priority queue**.

## Features

- Insert a task with an integer priority.
- Process the task with the highest priority.
- Display all tasks in processing order.
- Update the priority of an existing task.
- Preserve insertion order when two tasks have the same priority.
- Reject duplicate task names and handle full or empty queues.

Higher priority numbers are processed first.

## Build and run

Using GCC:

```text
gcc -std=c11 -Wall -Wextra -pedantic task_manager.c -o task_manager
./task_manager
```

On Windows with MinGW:

```text
gcc -std=c11 -Wall -Wextra -pedantic task_manager.c -o task_manager.exe
task_manager.exe
```

## VS Code setup on Windows

The project is configured with `.vscode/tasks.json` to use
`C:\MinGW\bin\gcc.exe`:

1. Open this folder in VS Code.
2. Select **Terminal > Run Task > Run task_manager**.
3. Or press `Ctrl+Shift+B` to build only.

To use `gcc` directly from any terminal, add `C:\MinGW\bin` to your Windows
user `Path`. In PowerShell, verify the compiler with:

```text
C:\MinGW\bin\gcc.exe --version
```

If you add the directory to `Path`, close and reopen VS Code and verify:

```powershell
gcc --version
```

## Data structure and complexity

The queue stores up to 100 tasks in an array. The heap maintains the task with the
highest priority at index 0.

| Operation | Complexity |
|---|---:|
| Insert | `O(log n)` |
| Process highest priority | `O(log n)` |
| Update priority | `O(log n)` |
| Display all tasks | `O(n log n)` |
| Search by task name | `O(n)` |
