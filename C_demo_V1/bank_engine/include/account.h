typedef struct{
    char symbol[16];
    double quantity;
}Position;

typedef struct{
    Position *ptrPos;
    size_t capacity; //size_t is unsign type
    size_t count;
}AssetAccount;

typedef enum{
    Savings = 0 ;
    Credit;
    Stock;
    Fund;
}AccountType;

typedef struct{
    AccountType type;

    Transaction *tx;
    size_t tx_capacity;
    size_t tx_count;

    union{
        struct{
            double balance;
        } savings;
        struct{
            double credit_limit;
            double credit_used;
        } credit;
        AssetAccount stock;
        AssetAccount fund;
    } data;


}Account