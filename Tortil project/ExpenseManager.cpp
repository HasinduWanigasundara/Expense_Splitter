#include "ExpenseManager.h"
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>

void ExpenseManager::addPerson(const std::string& name) {
    if (std::find(groupMembers.begin(), groupMembers.end(), name) == groupMembers.end()) {
        groupMembers.push_back(name);
        balances[name] = 0;
    }
}

void ExpenseManager::addExpense(const Expense& exp) {
    // Check if we are editing an existing expense or adding a new one
    bool found = false;
    for (auto& t : transactionHistory) {
        if (t.id == exp.id) {
            t = exp;
            found = true;
            break;
        }
    }
    if (!found) transactionHistory.push_back(exp);
    recalculateAllBalances();
}

void ExpenseManager::deleteExpense(long long id) {
    transactionHistory.erase(std::remove_if(transactionHistory.begin(), transactionHistory.end(),
        [id](const Expense& e) { return e.id == id; }), transactionHistory.end());
    recalculateAllBalances();
}

void ExpenseManager::clearAll() {
    groupMembers.clear();
    balances.clear();
    transactionHistory.clear();
}

void ExpenseManager::recalculateAllBalances() {
    for (const auto& m : groupMembers) balances[m] = 0;

    for (const auto& txn : transactionHistory) {
        long long totalCents = std::round(txn.amountLKR * 100.0);
        balances[txn.payer] += totalCents;

        if (txn.splitTypeRaw == "EQUAL") {
            long long numPeople = txn.splitBetweenRaw.size();
            if (numPeople == 0) continue;
            long long baseShare = totalCents / numPeople;
            long long remainder = totalCents % numPeople;

            for (const auto& person : txn.splitBetweenRaw) {
                long long currentShare = baseShare + (remainder > 0 ? 1 : 0);
                if (remainder > 0) remainder--;
                balances[person] -= currentShare;
            }
        } else if (txn.splitTypeRaw == "EXACT") {
            for (const auto& person : txn.splitBetweenRaw) {
                auto it = txn.exactAmountsMap.find(person);
                double amt = (it != txn.exactAmountsMap.end()) ? it->second : 0.0;
                balances[person] -= std::round(amt * 100.0);
            }
        }
    }
}

nlohmann::json ExpenseManager::getStateJSON() const {
    nlohmann::json state;
    
    state["groupMembers"] = groupMembers;
    
    // Map balances back to floats for the UI
    std::unordered_map<std::string, double> floatBalances;
    for (const auto& pair : balances) {
        floatBalances[pair.first] = pair.second / 100.0;
    }
    state["balances"] = floatBalances;

    // Serialize history
    nlohmann::json historyArr = nlohmann::json::array();
    for (const auto& txn : transactionHistory) {
        nlohmann::json j;
        j["id"] = txn.id;
        j["payer"] = txn.payer;
        j["amountLKR"] = txn.amountLKR;
        j["splitTypeRaw"] = txn.splitTypeRaw;
        j["splitBetweenRaw"] = txn.splitBetweenRaw;
        j["exactAmountsMap"] = txn.exactAmountsMap;
        j["splitTypeDisplay"] = txn.splitTypeDisplay;
        j["splitBetweenDisplay"] = txn.splitBetweenDisplay;
        j["date"] = txn.date;
        historyArr.push_back(j);
    }
    state["history"] = historyArr;

    // Execute Settle Up Graph Algorithm dynamically
    std::vector<std::pair<std::string, long long>> debtors, creditors;
    for (const auto& p : balances) {
        if (p.second < 0) debtors.push_back({p.first, -p.second});
        else if (p.second > 0) creditors.push_back({p.first, p.second});
    }

    std::vector<nlohmann::json> settleTransactions;
    while (!debtors.empty() && !creditors.empty()) {
        auto maxDebtor = std::max_element(debtors.begin(), debtors.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
        auto maxCreditor = std::max_element(creditors.begin(), creditors.end(), [](const auto& a, const auto& b) { return a.second < b.second; });

        long long amount = std::min(maxDebtor->second, maxCreditor->second);
        
        nlohmann::json txn;
        txn["debtor"] = maxDebtor->first;
        txn["creditor"] = maxCreditor->first;
        txn["amountLKR"] = amount / 100.0;
        settleTransactions.push_back(txn);

        maxDebtor->second -= amount;
        maxCreditor->second -= amount;

        if (maxDebtor->second == 0) debtors.erase(maxDebtor);
        if (maxCreditor->second == 0) creditors.erase(maxCreditor);
    }
    state["settleUp"] = settleTransactions;

    return state;
}