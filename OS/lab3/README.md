# Lab3 : Exception and Interrupt

## Introduction

An exception is an event that causes the currently executing program to relinquish the CPU to the corresponding handler. With the exception mechanism, an operating system can

1. do proper handling when an error occurs during execution.

2. A user program can generate an exception to get the corresponding operating system’s service.

3. A peripheral device can force the currently executing program to relinquish the CPU and execute its handler.

## Goals of this lab

- Understand what’s exception levels in Armv8-A.
- Understand what’s exception handling.
- Understand what’s interrupt.
- Understand how rpi3’s peripherals interrupt the CPU by interrupt controllers.
- Understand how to multiplex a timer.
- Understand how to concurrently handle I/O devices.

## Background

### Official Reference

Exceptions are tightly coupled with the CPU’s design. We only briefly introduce the components needed in this lab. If you want to know the details, please refer to ARM’s official introduction and manual’s Chapter D1(page 1496)

### Exception Levels

Principle of least privilege limits the resource that a program can access. The limitation reduces possible errors during execution and the attack surface, so the system’s security and stability are increased. Armv8-A’s CPUs follow the principle and implement exception levels, so an operating system can run diverse user applications without crashing the entire system.



Armv8-A has 4 exception levels(ELs). In general, all user programs are run in EL0, and operating systems are run in EL1. The operating system can decrease the exception level and jump to the user program by setting the system registers and executing an exception return instruction. When an exception is taken during a user program’s execution, the exception level is increased, and the CPU will jump to the exception handler.

### Exception Handling

When a CPU takes an exception, it does the following things.

- Save the current processor’s state(PSTATE) in SPSR_ELx. (x is the target Exception level)
- Save the exception return address in ELR_ELx.
- Disable its interrupt. (PSTATE.{D,A,I,F} are set to 1).
- If the exception is a synchronous exception or an SError interrupt, save the cause of that exception in ESR_ELx.
- Switch to the target Exception level and start at the corresponding vector address.

After the exception handler finishes, it issues eret to return from the exception. Then the CPU,

- Restore program counter from ELR_ELx.
- Restore PSTATE from SPSR_ELx.
- Switch to the corresponding Exception level according to SPSR_ELx.

#### Vector Table






