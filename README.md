*This project has been created as part of the 42 curriculum by atawbi.*

# Philosophers

## Description

Philosophers is part of the 42 curriculum and is an introduction to **multithreading and synchronisation** in C, based on Dijkstra's dining philosophers problem.

N philosophers sit around a table. Between each pair of neighbours there is one chopstick, so there are N chopsticks in total. A philosopher needs **both** adjacent chopsticks to eat, so two neighbours can never eat at the same time. A philosopher who goes too long without eating dies, and the simulation stops.

Each philosopher is a **thread** (`pthread`), each chopstick is a **mutex**, and the main thread acts as a **monitor** that watches for deaths and for the "everybody has eaten enough" condition.

### How it works

- **Data**: a shared `t_table` holds the settings, the array of chopstick mutexes and the philosophers. Each `t_philo` has its own `state_lock` protecting its `last_meal_time` and `meals_eaten`.
- **Deadlock avoidance**: two measures are combined.
  - Philosophers with an even id wait 1 ms before starting, so neighbours do not all grab a chopstick at once.
  - Each philosopher locks its two chopsticks **in memory-address order**, regardless of which one is on its left, so a circular wait cannot form.
- **Monitor**: the main thread loops every millisecond. It marks the simulation as ended when a philosopher's time since their last meal is `>= time_to_die`, or when every philosopher has eaten the required number of meals.
- **Race-free output and state**: all printing goes through `message_lock`, and the end flag (`is_ended`) is read and written only under `end_lock`. Once the simulation has ended, nothing else is printed.
- **Precise waiting**: `pause_execution()` sleeps in short `usleep(500)` steps against `gettimeofday()` instead of one long `usleep`, so delays stay accurate without burning a CPU core.
- **Single philosopher**: with only one chopstick the philosopher can never eat, so `main` handles this case directly: it prints the thinking line, waits `time_to_die` and prints the death.

### Project layout

```
Philosophers/
└── philo/
    ├── Makefile
    ├── philo.h          structures and prototypes
    ├── philo.c          main
    ├── clean_setup.c    table setup and cleanup
    ├── routin.c         philosopher routine, forks, eating
    ├── simulation.c     thread creation, monitor, join
    └── utiles.c         parsing, time, end-flag helpers
```

## Instructions

### Requirements

- Linux
- `gcc` (or `cc`) and `make`
- POSIX threads (`-pthread`)

### Compilation

```bash
cd philo
make
```

Built with `-Wall -Wextra -Werror -pthread`.

| Target       | Effect                                   |
|--------------|------------------------------------------|
| `make`       | build `philo`                            |
| `make clean` | remove the object files                  |
| `make fclean`| remove the object files and the binary   |
| `make re`    | rebuild from scratch                     |

### Execute

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Argument | Meaning |
|----------|---------|
| `number_of_philosophers` | number of philosophers (and chopsticks) |
| `time_to_die` | milliseconds a philosopher can live without starting a meal |
| `time_to_eat` | milliseconds spent eating (holding both chopsticks) |
| `time_to_sleep` | milliseconds spent resting after eating |
| `number_of_times_each_philosopher_must_eat` | optional; the simulation stops once every philosopher has eaten this many times. If omitted, it stops only when someone dies |

### Examples

```bash
./philo 5 800 200 200        # nobody should die
./philo 5 800 200 200 7      # stops once everyone has eaten 7 times
./philo 4 310 200 100        # tight timing, someone is expected to die
./philo 1 800 200 200        # a lone philosopher dies after 800 ms
```

Each log line has the form:

```
<timestamp_in_ms> <philosopher_id> <message>
```

Messages are `thinking`, `has taken left chopstick`, `has taken right chopstick`, `eating` and `has died`.

### Checking for races

```bash
gcc -Wall -Wextra -Werror -g -fsanitize=thread -pthread *.c -o philo_tsan
./philo_tsan 5 800 200 200 5
```

`valgrind --tool=helgrind ./philo 5 800 200 200 5` is another option.

## Resources

- [Dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- `man pthread_create`, `man pthread_mutex_lock`, `man gettimeofday`, `man usleep`
- [POSIX Threads Programming (LLNL)](https://hpc-tutorials.llnl.gov/posix/)

### AI Usage

AI (Claude) was used to draft this README from the source code. The project itself was written and tested by the author.
