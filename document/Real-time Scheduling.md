# Real time scheduling

RTOS uses priority-based scheduling. So how are priorities assigned to each task? 

There are two scheduling policy

1. RMS (Rate Monotonic Scheduling)
2. EDF (Earliest Deadline First scheduling)

# RMS

### assigns higher priority to tasks with shorter periods.

- static priority
- optimal
- **µC/OS-II uses this policy**

# EDF

### assigns higher priority to tasks closer to their deadlines.

- dynamic priority method.
    - Priorities are recalculated after each task execution.
- higher CPU utilization than RMS.