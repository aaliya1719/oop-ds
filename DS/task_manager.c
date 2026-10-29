#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_NAME_LENGTH 100

typedef struct {
    char name[MAX_NAME_LENGTH];
    int priority;
    unsigned long order;
} Task;

typedef struct {
    Task tasks[MAX_TASKS];
    int size;
    unsigned long next_order;
} PriorityQueue;

static void discard_remaining_input(void) {
    int character;

    while ((character = getchar()) != '\n' && character != EOF) {
        /* Discard input left after a long line or invalid number. */
    }
}

static int read_integer(const char *prompt, int *value) {
    char line[64];
    char extra;

    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0;
    }
    if (strchr(line, '\n') == NULL) {
        discard_remaining_input();
    }

    if (sscanf(line, " %d %c", value, &extra) != 1) {
        return -1;
    }

    return 1;
}

static int read_task_name(char *name, size_t name_size) {
    printf("Enter task name: ");
    if (fgets(name, name_size, stdin) == NULL) {
        return 0;
    }

    if (strchr(name, '\n') == NULL) {
        discard_remaining_input();
    } else {
        name[strcspn(name, "\n")] = '\0';
    }

    if (name[0] == '\0') {
        printf("Task name cannot be empty.\n");
        return -1;
    }

    return 1;
}

static int task_has_higher_priority(const Task *first, const Task *second) {
    if (first->priority != second->priority) {
        return first->priority > second->priority;
    }

    return first->order < second->order;
}

static void swap_tasks(Task *first, Task *second) {
    Task temporary = *first;
    *first = *second;
    *second = temporary;
}

static void sift_up(PriorityQueue *queue, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;

        if (!task_has_higher_priority(&queue->tasks[index],
                                      &queue->tasks[parent])) {
            break;
        }

        swap_tasks(&queue->tasks[index], &queue->tasks[parent]);
        index = parent;
    }
}

static void sift_down(PriorityQueue *queue, int index) {
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int highest = index;

        if (left < queue->size &&
            task_has_higher_priority(&queue->tasks[left],
                                     &queue->tasks[highest])) {
            highest = left;
        }

        if (right < queue->size &&
            task_has_higher_priority(&queue->tasks[right],
                                     &queue->tasks[highest])) {
            highest = right;
        }

        if (highest == index) {
            break;
        }

        swap_tasks(&queue->tasks[index], &queue->tasks[highest]);
        index = highest;
    }
}

static int find_task(const PriorityQueue *queue, const char *name) {
    int index;

    for (index = 0; index < queue->size; index++) {
        if (strcmp(queue->tasks[index].name, name) == 0) {
            return index;
        }
    }

    return -1;
}

static void insert_task(PriorityQueue *queue) {
    Task task;
    int result;

    if (queue->size == MAX_TASKS) {
        printf("The queue is full. Cannot insert another task.\n");
        return;
    }

    result = read_task_name(task.name, sizeof(task.name));
    if (result <= 0) {
        return;
    }

    if (find_task(queue, task.name) >= 0) {
        printf("A task with that name already exists.\n");
        return;
    }

    result = read_integer("Enter priority (higher number = higher priority): ",
                          &task.priority);
    if (result == 0) {
        return;
    }
    if (result < 0) {
        printf("Invalid priority. Please enter an integer.\n");
        return;
    }

    task.order = queue->next_order++;
    queue->tasks[queue->size] = task;
    sift_up(queue, queue->size);
    queue->size++;
    printf("Task inserted successfully.\n");
}

static void process_task(PriorityQueue *queue) {
    Task completed_task;

    if (queue->size == 0) {
        printf("The queue is empty.\n");
        return;
    }

    completed_task = queue->tasks[0];
    queue->size--;
    if (queue->size > 0) {
        queue->tasks[0] = queue->tasks[queue->size];
        sift_down(queue, 0);
    }

    printf("Processed task: %s (priority %d)\n", completed_task.name,
           completed_task.priority);
}

static void display_tasks(const PriorityQueue *queue) {
    PriorityQueue copy = *queue;
    int position = 1;

    if (copy.size == 0) {
        printf("The queue is empty.\n");
        return;
    }

    printf("\nTasks in processing order:\n");
    while (copy.size > 0) {
        Task task = copy.tasks[0];

        copy.size--;
        if (copy.size > 0) {
            copy.tasks[0] = copy.tasks[copy.size];
            sift_down(&copy, 0);
        }

        printf("%d. %s (priority %d)\n", position++, task.name, task.priority);
    }
}

static void update_priority(PriorityQueue *queue) {
    char name[MAX_NAME_LENGTH];
    int new_priority;
    int old_priority;
    int index;
    int result;

    result = read_task_name(name, sizeof(name));
    if (result <= 0) {
        return;
    }

    index = find_task(queue, name);
    if (index < 0) {
        printf("Task not found.\n");
        return;
    }

    result = read_integer("Enter new priority (higher number = higher priority): ",
                          &new_priority);
    if (result == 0) {
        return;
    }
    if (result < 0) {
        printf("Invalid priority. Please enter an integer.\n");
        return;
    }

    old_priority = queue->tasks[index].priority;
    queue->tasks[index].priority = new_priority;
    if (new_priority > old_priority) {
        sift_up(queue, index);
    } else if (new_priority < old_priority) {
        sift_down(queue, index);
    }
    printf("Priority updated successfully.\n");
}

static void display_menu(void) {
    printf("\n--- Priority Task Manager ---\n");
    printf("1. Insert task\n");
    printf("2. Process highest-priority task\n");
    printf("3. Display all tasks\n");
    printf("4. Update task priority\n");
    printf("5. Exit\n");
}

int main(void) {
    PriorityQueue queue = {0};
    int choice;
    int result;

    printf("Priority Queue Task Manager\n");

    while (1) {
        display_menu();
        result = read_integer("Choose an option: ", &choice);
        if (result == 0) {
            printf("\nInput ended. Exiting.\n");
            break;
        }
        if (result < 0) {
            printf("Invalid choice. Please enter a number from 1 to 5.\n");
            continue;
        }

        switch (choice) {
            case 1:
                insert_task(&queue);
                break;
            case 2:
                process_task(&queue);
                break;
            case 3:
                display_tasks(&queue);
                break;
            case 4:
                update_priority(&queue);
                break;
            case 5:
                printf("Exiting task manager.\n");
                return EXIT_SUCCESS;
            default:
                printf("Invalid choice. Please enter a number from 1 to 5.\n");
        }
    }

    return EXIT_SUCCESS;
}
