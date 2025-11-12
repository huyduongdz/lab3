#include "scheduler.h"

sTask SCH_tasks_G[SCH_MAX_TASKS];
sTask* SCH_pHead_Task = NULL;

static void SCH_Insert_Task(sTask* pTaskToInsert);

void SCH_Init(void) {
    uint32_t i;
    for (i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks_G[i].pTask = NULL;
        SCH_tasks_G[i].pNext = NULL;
        SCH_tasks_G[i].Delay = 0;
        SCH_tasks_G[i].Period = 0;
        SCH_tasks_G[i].RunMe = 0;
        SCH_tasks_G[i].TaskID = i;
    }
    SCH_pHead_Task = NULL;
}

uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD) {
    uint32_t task_index = 0;

    while ((task_index < SCH_MAX_TASKS) && (SCH_tasks_G[task_index].pTask != NULL)) {
        task_index++;
    }

    if (task_index == SCH_MAX_TASKS) {
        return SCH_MAX_TASKS;
    }

    sTask* pNewTask = &SCH_tasks_G[task_index];

    pNewTask->pTask = pFunction;
    pNewTask->Delay = DELAY;
    pNewTask->Period = PERIOD;
    pNewTask->RunMe = 0;
    pNewTask->pNext = NULL;

    SCH_Insert_Task(pNewTask);

    return task_index;
}

static void SCH_Insert_Task(sTask* pTaskToInsert) {
    if (SCH_pHead_Task == NULL || pTaskToInsert->Delay < SCH_pHead_Task->Delay) {

        if (SCH_pHead_Task != NULL) {
            SCH_pHead_Task->Delay -= pTaskToInsert->Delay;
        }

        pTaskToInsert->pNext = SCH_pHead_Task;
        SCH_pHead_Task = pTaskToInsert;
    }
    else {
        sTask* pCurrent = SCH_pHead_Task;

        pTaskToInsert->Delay -= pCurrent->Delay;

        while (pCurrent->pNext != NULL && pTaskToInsert->Delay >= pCurrent->pNext->Delay) {
            pTaskToInsert->Delay -= pCurrent->pNext->Delay;
            pCurrent = pCurrent->pNext;
        }

        if (pCurrent->pNext != NULL) {
            pCurrent->pNext->Delay -= pTaskToInsert->Delay;
        }

        pTaskToInsert->pNext = pCurrent->pNext;
        pCurrent->pNext = pTaskToInsert;
    }
}


uint8_t SCH_Delete_Task(const uint32_t TASK_ID) {
    if (TASK_ID >= SCH_MAX_TASKS || SCH_tasks_G[TASK_ID].pTask == NULL) {
        return 0;
    }

    sTask* pTaskToDel = &SCH_tasks_G[TASK_ID];

    sTask* pCurrent = SCH_pHead_Task;
    sTask* pPrev = NULL;

    while (pCurrent != NULL && pCurrent != pTaskToDel) {
        pPrev = pCurrent;
        pCurrent = pCurrent->pNext;
    }

    if (pCurrent == pTaskToDel) {
        if (pPrev == NULL) {
            SCH_pHead_Task = pCurrent->pNext;
        } else {
            pPrev->pNext = pCurrent->pNext;
        }

        if (pCurrent->pNext != NULL) {
            pCurrent->pNext->Delay += pCurrent->Delay;
        }
    }

    pTaskToDel->pTask = NULL;
    pTaskToDel->pNext = NULL;
    pTaskToDel->Delay = 0;
    pTaskToDel->Period = 0;
    pTaskToDel->RunMe = 0;

    return 1;
}

void SCH_Update(void) {
    if (SCH_pHead_Task == NULL) {
        return;
    }

    SCH_pHead_Task->Delay--;

    while (SCH_pHead_Task != NULL && SCH_pHead_Task->Delay == 0) {

        SCH_pHead_Task->RunMe = 1;

        sTask* pTaskToRun = SCH_pHead_Task;
        SCH_pHead_Task = pTaskToRun->pNext;
        pTaskToRun->pNext = NULL;
    }
}

void SCH_Dispatch_Tasks(void) {
    uint32_t i;

    for (i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks_G[i].RunMe == 1) {

            (*SCH_tasks_G[i].pTask)();

            SCH_tasks_G[i].RunMe = 0;

            if (SCH_tasks_G[i].Period > 0) {
                SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period;
                SCH_Insert_Task(&SCH_tasks_G[i]);
            } else {
                SCH_tasks_G[i].pTask = NULL;
            }
        }
    }
}
