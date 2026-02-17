Advanced Bank Engine
Systems-Level Financial Engine in C

====================================================

OVERVIEW

Advanced Bank Engine is a systems-level C project that implements a modular financial transaction engine with strict ownership-based memory management, deterministic lifecycle control, and layered architecture.

This project demonstrates:

Structured C data modeling

Ownership-driven memory discipline

Dynamic container expansion strategy (80% rule)

Transaction lifecycle tracking

Modular architecture design

UI and business logic separation

Persistence abstraction

Multi-user locking strategy planning

This is NOT a web backend.
This is a systems architecture demonstration project written in pure C.

====================================================

ARCHITECTURE (LAYERED DESIGN)

UI Layer (ncurses) [Phase 2]
↓
Core Engine (Memory) [Phase 1]
↓
Persistence Interface [Phase 3]
↓
SQLite Implementation [Phase 3]

The engine is completely independent of UI and database layers.

====================================================

PHASE 1 — PURE C ENGINE (IN-MEMORY CLOSED LOOP)

Goal
Build a deterministic, fully functional in-memory engine with strict lifecycle control.

No GUI.
No database.
Optional transaction log print only.

Core Engine Components

Bank struct (root owner)

Customer dynamic array (80% expansion rule)

Stock inventory pool

Investment pool

Portfolio ownership model

Savings account

Credit card with limit enforcement

Dynamic transaction log per customer

Transaction Model

Each transaction includes:

Type

Amount

Timestamp

Description

Each customer owns:

A dynamic transaction log

Trading / Validation Rules (inside engine)

Validate liquidity

Validate inventory

Validate lock period

Validate credit limit

Unified error codes

Memory Discipline

Clear ownership tree

No leaks

No double free

Reverse-order cleanup

Deterministic lifecycle control

Lock Abstraction (Interface Only)

Define lock interface for future multi-user support:

lock_init()
lock_acquire()
lock_release()
lock_destroy()

Implementation may be dummy in Phase 1 (single-process mode).

Deliverable

Clean CLI test version

Fully working engine

Stable lifecycle

Deterministic behavior

====================================================

PHASE 2 — UI LAYER (NCURSES)

Goal
Add terminal UI without touching engine logic.

Rules

UI must not access internal structs directly

UI calls only engine APIs

All validation remains inside engine

Error codes mapped to readable messages

Features

Menu navigation

Customer selection

Transaction execution

Transaction history display

Sync button (placeholder)

Deliverable

Interactive terminal system

Clean separation of concerns

====================================================

PHASE 3 — PERSISTENCE + SQLITE INTEGRATION

Goal
Add database-backed persistence using a clean abstraction layer.

Architecture

UI
↓
Engine (Memory)
↓
Persistence Interface
↓
SQLite Implementation

Startup Flow

Connect database

Load customers

Load inventories

Load transactions

Build memory state

Sync Flow

Begin transaction

Serialize memory state

Update database

Commit transaction

Locking Strategy

Level 1: In-Process (Optional)

pthread mutex

Level 2: Multi-Process (Real Lock)

SQLite transactions

Optional file locking if required

SQLite provides safe concurrent access through transactional locking.

====================================================

MULTI-USER STRATEGY (FINAL MODEL)

In-memory

Optional mutex for thread safety

Database

Use SQLite transactional locking

Avoid manual low-level file locking

Engine remains unaware of database details.

====================================================

COMPLEXITY CONTROL STRATEGY

Do NOT combine phases.

Correct development order:

Memory engine stable

UI integration

Database persistence

Locking reinforcement

====================================================

ENGINEERING PRINCIPLES DEMONSTRATED

Structured C modular design

Ownership-based lifecycle control

Dynamic container expansion strategy (80% rule)

Constraint-driven transaction engine

Inventory consistency enforcement

Time-based validation

Separation of UI and business logic

Persistence abstraction

Locking strategy planning

====================================================

REALISTIC TIMELINE

Phase 1: 2 weeks
Phase 2: 1–2 weeks
Phase 3: 1–2 weeks

Total: 4–6 weeks (high-quality pace)

====================================================

WHAT THIS PROJECT IS NOT

Not a web service

Not a distributed system

Not MongoDB / InfluxDB based

Not a beginner C exercise

====================================================

TARGET DOMAIN

Systems Programming

Embedded-level Architecture Thinking

C Memory Management

Deterministic Lifecycle Design

Financial Engine Modeling

====================================================

PLANNED BUILD ENVIRONMENT

Language: C (C17)

UI: ncurses

Database: SQLite

Build System: Makefile

OS: Linux (primary)

====================================================

FINAL DIRECTION (LOCKED)

Target domain: Systems / Embedded-level C

Database: SQLite (future integration)

UI: ncurses

Locking: Abstracted → SQLite transactional safety