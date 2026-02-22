# HUTrain — Railway Classification Yard Simulator

> **BBM203 Software Laboratory I — Programming Assignment 1**  
> Hacettepe University, Department of Computer Engineering

A C++ simulation of a railway classification yard system. Wagons are sorted by destination and cargo type, assembled into trains, validated for coupler load limits, and queued on departure tracks — all driven by a text-based command interface.

---

## Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Data Structures](#data-structures)
- [Commands](#commands)
- [Running](#running)
- [Project Structure](#project-structure)
- [Example I/O](#example-io)

---

## Overview

The system models a real-world classification yard workflow:

1. **Incoming wagons** are added to the yard and automatically sorted into bins indexed by `[Destination][CargoType]`.
2. When an `ASSEMBLE_TRAIN` command is issued, the yard merges all cargo blocks for that destination into a single named train, placing hazardous wagons last for safety.
3. The assembled train is **validated for coupler load limits** — if any coupler would be overloaded, the train is split into multiple shorter trains automatically.
4. Resulting trains are enqueued on a per-destination **departure track**.
5. Trains can be dispatched manually or automatically when the total weight on a track exceeds 2 000 tons.

---

## Architecture

```
┌─────────────────────────────────────────────────────┐
│                     RailMarshal                     │
│  Parses commands and routes them to subsystems      │
│                                                     │
│   ┌──────────────────────┐  ┌────────────────────┐  │
│   │  ClassificationYard  │  │  TrainTrack × 5    │  │
│   │  WagonList[5][6]     │  │  (per destination) │  │
│   │  (2-D block matrix)  │  │  FIFO Train queue  │  │
│   └──────────────────────┘  └────────────────────┘  │
└─────────────────────────────────────────────────────┘
         │                           │
   Wagon objects               Train objects
  (doubly-linked)            (own WagonLists)
```

| Class | Responsibility |
|---|---|
| `Wagon` | Atomic unit — carries ID, cargo type, destination, weight, and max coupler load |
| `WagonList` | Doubly-linked list of wagons; supports sorted insert, split-by-ID, detach, move semantics |
| `Train` | Named, destination-bound sequence of wagons; owns coupler-overload split logic |
| `ClassificationYard` | `WagonList[NUM_DESTINATIONS][NUM_CARGOTYPES]` matrix; sorts incoming wagons and assembles trains |
| `TrainTrack` | Singly-linked FIFO queue of `Train*`; supports auto-dispatch at configurable weight limit |
| `RailMarshal` | Top-level controller — owns one yard and five tracks, parses every command line |
| `Enums` | `Destination` (5 values) + `CargoType` (6 values), plus string parse/print helpers |

---

## Data Structures

### WagonList — Doubly Linked List

Wagons are stored in **descending weight order** (heaviest first). Key operations:

| Method | Description |
|---|---|
| `insertSorted(w)` | O(n) insertion maintaining descending weight order |
| `detachById(id)` | Unlinks and returns a specific wagon by ID |
| `splitAtById(id)` | Splits the list at the given wagon, returning the tail as a new `WagonList` |
| `appendList(&&)` | Move-appends another list, maintaining order |

### TrainTrack — Singly Linked Queue (FIFO)

Each destination has its own track. Trains are enqueued at the rear and dispatched from the front. If `AUTO_DISPATCH` is enabled, the oldest train is automatically dispatched when total track weight would exceed **2 000 tons**.

### ClassificationYard — 2-D Matrix of WagonLists

```
           COAL  OIL  HAZARDOUS  LIVESTOCK  MAIL  OTHER
ANKARA   [  …  ][  …  ][   …   ][    …    ][  …  ][  …  ]
IZMIR    [  …  ][  …  ][   …   ][    …    ][  …  ][  …  ]
ESKISEHIR[  …  ][  …  ][   …   ][    …    ][  …  ][  …  ]
ADANA    [  …  ][  …  ][   …   ][    …    ][  …  ][  …  ]
OTHERS   [  …  ][  …  ][   …   ][    …    ][  …  ][  …  ]
```

---

## Commands

Commands are read line-by-line from an input file. Lines starting with `#` are ignored.

| Command | Syntax | Description |
|---|---|---|
| `ADD_WAGON` | `ADD_WAGON <id> <cargo> <dest> <weight> <maxCoupler>` | Creates a wagon and inserts it into the yard |
| `REMOVE_WAGON` | `REMOVE_WAGON <id>` | Finds and permanently removes a wagon from the yard |
| `ASSEMBLE_TRAIN` | `ASSEMBLE_TRAIN <destination>` | Assembles all wagons for the given destination into one or more trains; adds them to the departure track |
| `DISPATCH_TRAIN` | `DISPATCH_TRAIN <destination>` | Departs the front-most train from the given destination's track |
| `PRINT_YARD` | `PRINT_YARD` | Prints the current state of the classification yard |
| `PRINT_TRACK` | `PRINT_TRACK <destination>` | Prints all trains queued on the specified track |
| `AUTO_DISPATCH` | `AUTO_DISPATCH ON` / `AUTO_DISPATCH OFF` | Enables/disables automatic dispatch when track weight exceeds 2 000 tons |
| `CLEAR` | `CLEAR` | Resets the entire system |

### Valid Destinations
`ANKARA` · `IZMIR` · `ESKISEHIR` · `ADANA` · `OTHERS`

### Valid Cargo Types
`COAL` · `OIL` · `HAZARDOUS` · `LIVESTOCK` · `MAIL` · `OTHER`

> **Hazardous safety rule:** When assembling a train, `HAZARDOUS` wagons are always placed at the **rear** of the train, regardless of weight order.

---

## Running

```bash
./bin/HUTrain <input_file>
```

**Example:**

```bash
./bin/HUTrain input.txt
./bin/HUTrain sample_io_2/input2.txt
```

---

## Project Structure

```
PA1/
├── main.cpp               # Entry point — reads command file, drives RailMarshal
├── RailMarshal.h/.cpp     # Top-level controller; command parser
├── ClassificationYard.h/.cpp  # 2-D WagonList matrix; train assembly
├── TrainTrack.h/.cpp      # Per-destination FIFO train queue
├── Train.h/.cpp           # Named train; coupler-split logic
├── WagonList.h/.cpp       # Doubly-linked wagon list; sorted insert, split
├── Wagon.h/.cpp           # Single wagon node
├── Enums.h/.cpp           # Destination & CargoType enums + helpers
├── input.txt              # Sample input
├── output.txt             # Expected output
├── sample_io_2/
│   ├── input2.txt         # Second sample input
│   └── output2.txt        # Second expected output
├── bin/                   # Compiled executable (generated)
└── obj/                   # Object files (generated)
```

---

## Example I/O

**Input:**
```
ADD_WAGON 101 COAL ANKARA 100 200
ADD_WAGON 102 MAIL ADANA 80 150
ADD_WAGON 103 LIVESTOCK IZMIR 120 250
PRINT_YARD
ASSEMBLE_TRAIN ANKARA
PRINT_TRACK ANKARA
```

**Output:**
```
Wagon [ID:101 | Cargo:COAL | Dest:ANKARA | Weight:100 | MaxCoupler:200] added to yard.
Wagon [ID:102 | Cargo:MAIL | Dest:ADANA | Weight:80 | MaxCoupler:150] added to yard.
Wagon [ID:103 | Cargo:LIVESTOCK | Dest:IZMIR | Weight:120 | MaxCoupler:250] added to yard.
--- classification Yard ---
Destination ANKARA:
  CargoType COAL: W101(100ton)
Destination IZMIR:
  CargoType LIVESTOCK: W103(120ton)
Destination ADANA:
  CargoType MAIL: W102(80ton)
Train Train_ANKARA_1 assembled with W101(100ton) wagons.
[Track 0] Train_ANKARA_1(100ton)-W101(100ton) ->
```

### Coupler Split Example

When the cumulative weight of wagons behind a coupler exceeds that coupler's `maxCouplerLoad`, the train is automatically split:

```
ADD_WAGON 201 COAL ADANA 150 200
ADD_WAGON 202 COAL ADANA 250 180   ← coupler limit 180, but 150+100 = 250 > 180
ADD_WAGON 203 COAL ADANA 100 300
ASSEMBLE_TRAIN ADANA
```

```
Train Train_ADANA_1 split due to coupler overload before Wagon 201
→ Train_ADANA_1       : W202(250ton)
→ Train_ADANA_1_split_1 : W201(150ton) - W203(100ton)
```

---

## Notes

- `WagonList` copy constructor and copy assignment are **deleted** — only move semantics are supported to avoid accidental deep copies.
- Train names follow the pattern `Train_<DESTINATION>_<N>` and splits append `_split_<K>`.
- The `AUTO_DISPATCH_LIMIT` constant (default **2 000 tons**) is defined in `TrainTrack.h`.

## License

This project is part of the BBM203 Data Structures course.
