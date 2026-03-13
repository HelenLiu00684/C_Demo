# Bank Engine Demo (C)

This project implements a small in-memory banking and asset trading engine written in C.

It is designed as a systems programming exercise focusing on:

- data modeling
- modular architecture
- memory-safe state management
- transaction consistency

The system simulates a simplified financial backend capable of processing different types of transactions and maintaining internal state for accounts and bank inventory.

---

# Features

The engine currently supports the following operations:

- Savings account transactions
  - deposit
  - withdraw

- Credit account transactions
  - credit spend
  - credit repay

- Asset trading
  - stock buy / sell
  - fund buy / sell

- Bank inventory management for assets

- Transaction recording and history tracking

- JSON state export for external systems or monitoring tools

All operations are executed in memory and validated through unit tests.

---

# System Architecture

The project follows a modular design.
Bank
└── Customer
└── Account
├── Savings / Credit
└── Asset Accounts
├── Stock positions
└── Fund positions
│
Transaction Records

Core processing is handled by the **engine module**, which coordinates account updates, bank inventory updates, and transaction creation.

---

# Project Structure
bank-engine-demo
│
├── include
│ ├── account.h
│ ├── transaction.h
│ ├── inventory.h
│ ├── bank.h
│ ├── customer.h
│ ├── symbolmap.h
│ └── error.h
│
├── src
│ ├── account.c
│ ├── transaction.c
│ ├── inventory.c
│ ├── engine.c
│ ├── bank.c
│ ├── customer.c
│ └── symbolmap.c
│
├── tests
│ └── test_transaction.c
│
└── main.c
---

# Core Modules

### transaction
Defines the transaction data model and creates transaction records.

### account
Manages account state including balances and asset positions.

### inventory
Maintains bank inventory for available assets.

### engine
Implements the core business logic for executing transactions.

### symbolmap
Maps asset symbols to asset types (stock or fund).

---

# Build

Compile using GCC:
gcc tests/test_transaction.c src/transaction.c -Iinclude -o tests/test_transaction

Run unit tests:
./test_transaction
---

# Unit Testing

The project includes simple unit tests using `assert()`.

Test coverage includes:

- valid savings and credit transactions
- valid stock and fund trades
- invalid parameters (amount, price, quantity)
- invalid transaction types
- symbol length validation

The tests are organized by logical groups:
test_savings_credit_valid
test_asset_valid
test_savings_credit_invalid
test_asset_invalid

---

# JSON Output

The engine is designed to export internal system state in JSON format.  
This allows integration with higher-level management systems.

Example JSON output:

```json
{
  "account_id": 1023,
  "type": "stock",
  "symbol": "AAPL",
  "quantity": 100,
  "price": 180.25
}

Possible exported data includes:

account balances

asset positions

bank inventory

transaction history
Purpose

This project is part of a larger series of system-level programming demos focusing on:

C data structure design

modular backend architecture

financial transaction modeling

safe memory management

state serialization (JSON)

Future extensions may include:

integration testing

stress testing

additional system modules

telemetry or monitoring interfaces
