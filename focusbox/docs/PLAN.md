# FocusBox project plan

## Current status

A working hardware prototype now runs accelerated 10-second focus and 5-second break periods, toggles between green and blue LEDs, pauses/resumes with a debounced button, and sounds an active buzzer briefly on period changes. The current implementation is in `src/main.c`. The longer 25/5/15-minute behavior and long-press reset described below are future improvements, not implemented features.

## Scope and decisions

Version 1 is study mode only. No Wi-Fi, Bluetooth, app, display, adjustable duration or persistent statistics. A screen and duration knob can be considered after version 1 works. GD32 is available now but might need to be returned: isolate hardware dependencies to make a later port practical; do not promise a drop-in replacement.

## State behavior

| State | Short press | Timeout | Output |
|---|---|---|---|
| Idle | Start focus | None | LEDs off |
| Focus running | Pause | Complete focus; increment count; wait for break | Green steady |
| Break running | Pause | Wait for next focus | Blue steady |
| Paused | Resume with preserved remaining time | None | Current phase LED blinks slowly |
| Waiting for next phase | Start pending phase | None | Alternate green/blue |

Hold for 2 seconds in any state: clear the session count and timer, silence sound and return to idle. No skip command in version 1. Paused and cancelled sessions do not count as completed focus sessions. Focus completion chooses 5-minute break, except completion counts 4, 8, 12 etc. choose 15 minutes. Sound once on completion of either phase. Short press acts on release only if no long press has fired; releasing after reset must not start another session.

Use a nonblocking beep sequence and button scanning. Debounce target: 30 ms, to be tuned with the physical switch. Use monotonic timestamps and preserve remaining duration when pausing. Define and test wrap-safe time arithmetic for the chosen counter. Keep ISR work small; specify synchronization for any ISR-shared data. Do not assume an 8 MHz crystal definition is the actual CPU or timer clock.

## Hardware discovery checklist

- [ ] Identify exact GD32 board and carrier board revision; obtain schematic/pinout.
- [ ] Identify usable power rails, ground, occupied pins and accessible headers.
- [ ] Inspect complete course project and any carrier-board libraries.
- [ ] Verify an unchanged baseline build and USB/DFU flash with the existing setup.
- [ ] Verify PA5/PB0 routing before reuse: supplied code reads PA5 low and sets PB0 high, but does not prove which physical peripherals these connect to.
- [ ] Choose a timer or machine-counter API from the actual SDK and confirm its clock rate.
- [ ] Select free GPIO pins, LED resistor values and button pull-up arrangement.
- [ ] Identify buzzer type/rating and whether a transistor driver is needed.
- [ ] Confirm board/header connectors allow solderless external wiring.

Do not drive kit loads directly from unverified GPIO. Keep wiring changes unpowered. USB supplies board power; breadboard logic uses the verified board rails. Avoid combining the kit power module and USB supply without a verified power design.

## Implementation architecture

Planned modules:

- `src/pomodoro.c`, `include/pomodoro.h`: phase rules, counters, transitions and remaining time; no vendor headers.
- `platform/gd32/`: clock/tick, button, LED and buzzer drivers.
- `src/main.c`: read time and button events, update state, update outputs.
- `tests/`: portable state-machine tests runnable with a host compiler.

The prototype currently uses the working course SDK timing interface and Makefile. Keep original uploaded reference files under `reference/`; the active timer firmware is in `src/main.c`. The Makefile requires the course SDK and tools in sibling directories.

## Milestones and completion criteria

### 1. Working baseline

Build and flash the supplied example in its original project layout. Record compiler, SDK, operating system, commands and board revision. Collect all required dependencies or document their legitimate installation sources.

### 2. Portable timer

Host tests: start, pause/resume, reset, focus/break completion, fourth-session long break, manual phase confirmation, no double-counting and time-counter wrap. Long-press handling must not also emit a short press.

### 3. Physical controls

Validate button pull-up and debouncing, both LEDs and responsive pause/reset while counting. Use 10-second focus, 5-second short break and 8-second long break in an explicit test configuration. Production defaults remain 25/5/15 minutes.

### 4. Full timer

Validate buzzer driver and one completion sound per phase. Run four accelerated focus cycles and a long break. Run at least one actual 25-minute session against a reference timer: target absolute error no more than 2 seconds. Report measured error and adjust the timebase if needed. Confirm power cycling starts idle and a held reset button does not immediately restart the timer.

### 5. GitHub release

Add a verified wiring table and schematic, original photos, a short demo and accurate build/flash instructions. Label firmware as hardware-tested only after physical verification. Review licenses before redistributing dependencies or course files; remove reference examples from the public version if reuse permission is unclear. Choose a license for original code before release. Create release v0.1.0 only when the acceptance checks pass.

## Suggested GitHub issues

1. Identify board/carrier pinout and reproduce baseline build.
2. Implement portable Pomodoro state machine and host tests.
3. Integrate hardware timebase and debounced button events.
4. Wire and validate LED/buzzer interface.
5. Perform full-cycle hardware acceptance checks.
6. Finish wiring documentation, demo and reproducible setup.

## Rough effort

Approximately 3–5 short work sessions if the course build environment already works. Hardware identification and missing dependencies are the main schedule uncertainties.
