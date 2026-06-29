Real-Time 3D Target Tracking Engine (RTEMS)
A high-performance, multitasking target tracking engine architected for the RTEMS Real-Time Operating System (RTOS). This project implements a deterministic, priority-based multitasking architecture to simulate, track, and intercept 3D targets in noisy environments using Kalman filtering and real-time inter-process communication.
System Architecture
The system follows a modular, event-driven architecture where distinct functional blocks operate asynchronously, communicating via RTEMS Message Queues to ensure zero-latency data transfer:
Sensor Task (High Priority): Simulates a 3D target trajectory using non-linear math (sine/cosine waves) with injected Gaussian noise to replicate real-world sensor jitter.
Tracker Task (Computational): Consumes raw telemetry, applies a simplified Kalman Filter to derive the optimal state estimate, and performs anomaly detection for evasive maneuvers.
Actuator Task (I/O Bound): Translates filtered coordinate data into spherical orientation vectors (Azimuth/Elevation) for interceptor hardware control.


ComponentImplementation Detail
OSRTEMS 6 (ARM RealView PBX-A9 BSP)
ConcurrencyRTEMS Tasking (Priority-based scheduling)
Inter-Process Commrtems_message_queue
Filtering LogicLinear State Estimation (Kalman)
SimulationQEMU (ARM System Emulation)

Project Workflow
The system utilizes a closed-loop simulation pattern for data analysis:
Simulate: The Sensor Task emits 3D coordinate data.
Filter: The Tracker Task decouples the signal from noise.
Log: Data is streamed via stdout to CSV for persistent logging.
Visualize: A custom Python/Matplotlib post-processing suite reconstructs the flight path for performance verification.
Getting Started
Prerequisites
Toolchain: arm-rtems6-gcc (RTEMS 6)
Environment: Linux (Ubuntu/Debian)
Dependencies: qemu-system-arm, python3, matplotlib
