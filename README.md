# FreeRTOS-EDF-Scheduler
## Fixed-Priority Scheduling
* FreeRTOS supports priority scheduling.
* There is no EDF support.
## Adding Support for EDF
* Identify where/when scheduling decisions are made.
    * vTaskSwitchContext
* Add proper deadline information to task information (i.e., in TCB).
* Add code to pickup a ready job with the earliest deadline at the re-scheduling points.
    * In this project, it is okay to use linear search
* Add deadline information in the TCB.
    * You can pass them to tasks upon creation via the user-provided parameter.
* Upon re-scheduling, visit the TCB list linearly; find the ready task whose deadline is the earliest.
* Before a task delays for the next period, advance its deadline to the next period.
