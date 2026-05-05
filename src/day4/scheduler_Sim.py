# FreeRTOS Scheduler Simulation
# Shows which task runs at each tick and why

print("FreeRTOS Preemptive Scheduler Simulation")
print("Safety(p=4)  Processor(p=3)  Sensor(p=2)")
print("=" * 55)

# 3 tasks — same as your AutoRTOS-Sim project
tasks = {
    "Safety":    {"priority": 4, "blocked_until": 0},
    "Processor": {"priority": 3, "blocked_until": 0},
    "Sensor":    {"priority": 2, "blocked_until": 0},
}

# How long each task blocks after running once
delays = {
    "Safety":    3,   # blocks for 3 ticks after running
    "Processor": 4,   # blocks for 4 ticks after running
    "Sensor":    5,   # blocks for 5 ticks after running
}

for tick in range(1, 25):

    # Step 1 — find all Ready tasks (not blocked)
    ready = {
        name: info
        for name, info in tasks.items()
        if tick >= info["blocked_until"]
    }

    # Step 2 — pick highest priority Ready task
    if not ready:
        print(f"Tick {tick:2d}: CPU IDLE — all tasks blocked")
        continue

    running = max(ready, key=lambda n: ready[n]["priority"])

    # Step 3 — show the state of all tasks
    waiting = [n for n in ready if n != running]
    blocked = [
        f"{n}(wakes@{tasks[n]['blocked_until']})"
        for n in tasks
        if tick < tasks[n]["blocked_until"]
    ]

    print(f"Tick {tick:2d}:  "
          f"RUNNING={running:10s}  "
          f"READY={waiting}  "
          f"BLOCKED={blocked}")

    # Step 4 — block this task after it runs
    tasks[running]["blocked_until"] = tick + delays[running]

print("=" * 55)
print()
print("What to observe:")
print("1. Safety ALWAYS runs when it is Ready")
print("2. Processor only runs when Safety is Blocked")
print("3. Sensor only runs when BOTH Safety and Processor are Blocked")
print("4. This is exactly what your AutoRTOS-Sim will do in C")