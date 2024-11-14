#include <stdio.h>

#define MAX_PROCESSES 10

struct Process {
    int id, arrival_time, burst_time, remaining_time, completion_time, waiting_time, turnaround_time;
};

void round_robin(struct Process p[], int n, int time_quantum) {
    int current_time = 0, completed = 0, total_waiting_time = 0, total_turnaround_time = 0;

    while (completed != n) {
        int idle = 1; //A flag that is set to 1 (true) initially, indicating that the CPU might be idle in this time unit if no processes are ready to execute.

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0) {
                idle = 0;
                int exec_time = (p[i].remaining_time <= time_quantum) ? p[i].remaining_time : time_quantum; //If the remaining time is less than or equal to the time quantum, the process will execute to completion.
                                                                                                             //Otherwise, it will run for the duration of the time quantum.
                current_time += exec_time;
                p[i].remaining_time -= exec_time;

                if (p[i].remaining_time == 0) {
                    p[i].completion_time = current_time;
                    p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
                    p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
                    total_waiting_time += p[i].waiting_time;
                    total_turnaround_time += p[i].turnaround_time;
                    completed++;
                }
            }
        }
        if (idle) current_time++;
    }

    printf("\nProcess\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\n", p[i].id, p[i].arrival_time, p[i].burst_time, p[i].completion_time, p[i].waiting_time, p[i].turnaround_time);

    printf("\nAverage Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}

int main() {
    int n, time_quantum;
    struct Process p[MAX_PROCESSES];

    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);

    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter arrival and burst times for P%d: ", p[i].id);
        scanf("%d %d", &p[i].arrival_time, &p[i].burst_time);
        p[i].remaining_time = p[i].burst_time;
    }

    round_robin(p, n, time_quantum);
    return 0;
}
