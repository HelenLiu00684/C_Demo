#include <transaction.h>
#include <error.h>
#include <time.h>
ErrorCode transaction_create(Transaction *tx,unsigned long id,TransactionType type,double price,double quantity,double amount,const char *symbol){
    if (tx == NULL)
        return ERROR_INVLID_POINTER;
    tx->timestamp = time(NULL);
    tx->id = id;
    tx->quantity = quantity;
    if (tx->type == DEPOSIT ||
        tx->type == WITHDRAW ||
        tx->type == CREDIT_SPEND ||
        tx->type == CREDIT_REPAY){
            if (amount <= 0)
            return ERROR_INVALID_ARGUMENT;
            tx->amount = amount;
            tx->symbol[0] = '\0';
            tx->price = 0;
            tx->quantity = 0;
        }

    else if (tx->type == STOCK_BUY ||
        tx->type == STOCK_SELL ||
        tx->type == FUND_BUY ||
        tx->type == FUND_SELL ){
        if (amount != 0 || price <= 0 || quantity <=0)
          return ERROR_INVALID_ARGUMENT;
        tx->amount = price * quantity;
        if (symbol == NULL)
            return ERROR_INVALID_ARGUMENT;
        if(strlen(symbol)>=sizeof(tx->symbol))
            return ERROR_INVALID_ARGUMENT;
        strncpy(tx->symbol,symbol,sizeof(tx->symbol)-1); //copy string array to an array and set the last char = '\0'
        tx->symbol[sizeof(tx->symbol)-1]='\0';        
    }else
        return ERROR_INVALID_ARGUMENT;
    return ERROR_OK;

};
/*
 * -------------------------------------------------------------
 * Cash & Credit Transaction Constraints
 * -------------------------------------------------------------
 *
 * Applies to:
 *   TX_DEPOSIT
 *   TX_WITHDRAW
 *   TX_CREDIT_SPEND
 *   TX_CREDIT_REPAY
 *
 * Rules:
 *
 * 1. `amount` must be strictly greater than 0.
 *    Transaction direction is determined by `type`,
 *    not by negative values.
 *
 * 2. `price` must be 0.
 *
 * 3. `quantity` must be 0.
 *
 * 4. `symbol` must be an empty string.
 *
 * 5. `amount` is provided by the caller and is treated
 *    as the single source of truth.
 *
 * 6. No asset-related fields are valid for cash/credit transactions.
 *
 * Rationale:
 *   Cash-based transactions represent direct monetary flows.
 *   They are not tied to any tradable asset and therefore
 *   must not contain asset metadata.
 *
 * -------------------------------------------------------------
 */
/*
 * -------------------------------------------------------------
 * Stock & Investment Transaction Constraints
 * -------------------------------------------------------------
 *
 * Applies to:
 *   TX_STOCK_BUY
 *   TX_STOCK_SELL
 *   TX_FUND_BUY
 *   TX_FUND_SELL
 *
 * Rules:
 *
 * 1. `symbol` must not be NULL.
 *
 * 2. `symbol` length must be strictly less than
 *    sizeof(tx->symbol).
 *
 * 3. `price` must be strictly greater than 0.
 *
 * 4. `quantity` must be strictly greater than 0.
 *
 * 5. `amount` must be 0 when passed to the function.
 *    The actual transaction amount is internally computed as:
 *
 *        amount = price * quantity
 *
 * 6. `amount` derived from price and quantity is treated
 *    as the single source of truth.
 *
 * Rationale:
 *   Asset-based transactions are tied to tradable instruments.
 *   The transaction value must be derived from asset price and
 *   quantity to prevent inconsistencies or tampering.
 *
 * -------------------------------------------------------------
 */