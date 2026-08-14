#ifndef EXPENSE_MANAGER_H
#define EXPENSE_MANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "json.hpp"

struct Expense {
    long long id;
    std::string payer;
    double amountLKR;
    std::string splitTypeRaw; 
    std::vector<std::string> splitBetweenRaw;
    std::unordered_map<std::string, double> exactAmountsMap;
    std::string splitTypeDisplay;
    std::string splitBetweenDisplay;
    std::string date;
};

class ExpenseManager {
private:
    std::vector<std::string> groupMembers;
    std::unordered_map<std::string, long long> balances;
    std::vector<Expense> transactionHistory;

    void recalculateAllBalances();

public:
    void addPerson(const std::string& name);
    void addExpense(const Expense& exp);
    void deleteExpense(long long id);
    void clearAll();
    
    // Generates the full application state to send to the web browser
    nlohmann::json getStateJSON() const;
};

#endif