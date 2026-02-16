# Advanced Bank & Portfolio Management System (C)

## Overview

This project extends a basic bank account system into a modular
financial engine implemented entirely in C.

It supports:

-   Dynamic multi-customer management\
-   Savings accounts\
-   Credit card accounts\
-   Stock inventory pool\
-   Investment pool with time-lock constraints\
-   Portfolio ownership tracking\
-   Automatic dynamic memory expansion (80% threshold rule)\
-   ncurses-based terminal interface\
-   File input/output support\
-   JSON state export

The system is designed with strict separation between:

-   Core business logic\
-   User interface\
-   Persistence layer

------------------------------------------------------------------------

## System Architecture

The project is divided into three logical layers:

### 1. Core Engine

Handles:

-   Customer lifecycle management\
-   Asset inventory management\
-   Transaction validation\
-   Trading logic\
-   Credit control\
-   Memory ownership and cleanup

### 2. UI Layer (ncurses)

Provides:

-   Interactive menu navigation\
-   Account selection\
-   Real-time transaction feedback\
-   Error display\
-   Structured terminal rendering

The UI layer does not contain business logic.\
It interacts only with exposed engine APIs.

### 3. Persistence Layer

Supports:

-   Loading system state from file\
-   Saving system state to file\
-   JSON export of full runtime state

------------------------------------------------------------------------

## Core Functional Features

### Customer Management

-   Customers are stored in a dynamic array\
-   Automatic expansion occurs when size reaches 80% of capacity\
-   Capacity doubles during expansion\
-   Safe realloc pattern is used

Each customer contains:

-   Savings account\
-   Credit card account\
-   Portfolio (owned stocks and investments)

------------------------------------------------------------------------

### Savings Account

-   Stores liquid cash balance\
-   Used to purchase stocks and investments\
-   Updated after every transaction

------------------------------------------------------------------------

### Credit Card System

Each customer has:

-   Credit limit\
-   Used credit balance

Constraint:

    used <= limit

-   Spending increases used credit\
-   Repayment decreases used credit\
-   Credit usage cannot exceed the defined limit

------------------------------------------------------------------------

### Stock Inventory Pool (System-Level)

Each stock includes:

-   Unique ID\
-   Name\
-   Total quantity\
-   Remaining quantity\
-   Price

Rules:

-   System prevents overselling\
-   Remaining quantity decreases on buy\
-   Remaining quantity increases on sell

------------------------------------------------------------------------

### Investment Pool (Time-Locked Assets)

Each investment includes:

-   Unique ID\
-   Name\
-   Total quantity\
-   Remaining quantity\
-   Price\
-   Lock period (days)

Investments cannot be sold before unlock time.

------------------------------------------------------------------------

### Portfolio Ownership Model

Customers do not duplicate stock definitions.

Instead, each owned asset stores:

-   Asset ID reference\
-   Quantity\
-   Purchase timestamp\
-   Unlock timestamp (for investments)

This maintains:

-   Single source of truth\
-   Inventory consistency\
-   Clean ownership modeling

------------------------------------------------------------------------

## Trading Logic

### Buying Assets

Transaction is allowed only if:

-   Savings balance is sufficient\
-   Inventory has sufficient remaining quantity

If valid:

-   Savings decreases\
-   Inventory remaining quantity decreases\
-   Portfolio position increases

------------------------------------------------------------------------

### Selling Assets

Transaction is allowed only if:

-   Customer owns sufficient quantity\
-   Investment lock period has expired (if applicable)

If valid:

-   Savings increases\
-   Inventory remaining quantity increases\
-   Portfolio position decreases

------------------------------------------------------------------------

## Dynamic Memory Strategy

Dynamic arrays are used for:

-   Customers\
-   Stocks\
-   Investments\
-   Customer portfolios

Expansion rule:

    If size >= 80% of capacity → capacity doubles

This ensures scalability and minimizes frequent memory reallocations.

------------------------------------------------------------------------

## File Input / Output

The system supports:

-   Loading initial bank state from file\
-   Saving current runtime state\
-   Appending transaction logs\
-   Safe file open and error handling

All file operations validate input data before applying changes.

------------------------------------------------------------------------

## JSON Export

The system can export a complete runtime snapshot in JSON format.

The JSON output includes:

-   Customer information\
-   Savings balance\
-   Credit usage\
-   Portfolio holdings\
-   System inventory state

JSON export enables:

-   Debugging\
-   State inspection\
-   External integration\
-   Future persistence upgrades

------------------------------------------------------------------------

## Memory Ownership Model

Ownership hierarchy:

Bank ├── Customers\[\] │ ├── OwnedStocks\[\] │ └── OwnedInvestments\[\]
├── StockInventory\[\] └── InvestmentInventory\[\]

Responsibilities:

-   Bank frees all global containers\
-   Each customer frees its own portfolio arrays\
-   Memory is released in reverse ownership order

------------------------------------------------------------------------

## Error Handling

The system validates and prevents:

-   Insufficient savings balance\
-   Credit limit overflow\
-   Inventory shortage\
-   Investment lock violations\
-   Invalid transaction input\
-   Memory allocation failure\
-   File I/O failure

------------------------------------------------------------------------

## Engineering Concepts Demonstrated

-   Structured modular C design\
-   Ownership-based memory management\
-   Dynamic container resizing\
-   Constraint-based transaction engine\
-   Inventory consistency control\
-   Time-based state validation\
-   Separation of UI and core logic\
-   Persistence abstraction

------------------------------------------------------------------------

## Future Extensions

-   Interest accrual engine\
-   Dividend distribution\
-   Transaction audit logs\
-   Persistent database backend\
-   Risk scoring model\
-   Multi-user simulation\
-   Thread-safe transaction processing
