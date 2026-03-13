1️⃣ System Architecture

整体系统结构：

Bank
 └── Customer
      └── Account
           ├── Balances
           │     ├── Savings
           │     └── Credit
           │
           ├── Inventory
           │     ├── Stock Inventory
           │     └── Fund Inventory
           │
           └── Transactions

完整图：

                +----------------------+
                |        Bank          |
                |----------------------|
                | customers[]          |
                | customer_count       |
                +----------+-----------+
                           |
                           v
                +----------------------+
                |      Customer        |
                |----------------------|
                | id                   |
                | name                 |
                | accounts[]           |
                +----------+-----------+
                           |
                           v
                +----------------------+
                |       Account        |
                |----------------------|
                | savings_balance      |
                | credit_balance       |
                | stock_inventory      |
                | fund_inventory       |
                | transactions[]       |
                +----------+-----------+
                           |
            +--------------+--------------+
            |                             |
            v                             v
     +-------------+             +----------------+
     | Transaction |             |   Inventory    |
     +-------------+             +----------------+
2️⃣ Engine Workflow

Engine 是系统逻辑核心。

User Input
     |
     v
Create Transaction
     |
     v
Engine Dispatcher
     |
     v
switch(transaction->type)

逻辑路径：

                 Transaction
                      |
                      v
               Engine Dispatcher
                      |
        +-------------+-------------+
        |             |             |
        v             v             v
     Savings       Credit         Asset
        |             |             |
        v             v             v
update balance   update credit   update inventory
                                      |
                                      v
                              inventory_add_position()

核心思想：

Event → Engine → State Update
3️⃣ Inventory Container Design

Inventory 本质是 动态数组容器：

BankInventory
│
├── positions[]
│
├── size
│
└── capacity

Position 结构：

Position
 ├── symbol
 ├── quantity
 └── avg_price

数组示例：

positions[]

+--------------------+
| symbol: AAPL       |
| quantity: 10       |
| avg_price: 180.00  |
+--------------------+

+--------------------+
| symbol: VOO        |
| quantity: 20       |
| avg_price: 420.00  |
+--------------------+

当：

size >= capacity

触发：

expand_inventory()

扩容逻辑：

capacity = capacity * 2
4️⃣ Transaction → Inventory Data Flow

交易处理流程：

Transaction
     |
     v
Engine
     |
     v
Account
     |
     v
Inventory
     |
     v
Position Update

例如：

ASSET_BUY
     |
     v
Engine
     |
     v
Account
     |
     v
inventory_add_position()
     |
     v
update quantity
update avg_price
5️⃣ Demo1 模块结构
bank_engine
│
├── bank.c / bank.h
├── customer.c / customer.h
├── account.c / account.h
├── transaction.c / transaction.h
├── inventory.c / inventory.h
├── engine.c / engine.h
└── main.c

模块职责：

Bank        → system container
Customer    → account owner
Account     → balances + holdings
Transaction → event record
Inventory   → asset positions
Engine      → business logic
6️⃣ Demo1 本质架构

这个系统实际上是：

Event Driven State Engine

模式：

Event
  |
  v
Engine
  |
  v
State Update
