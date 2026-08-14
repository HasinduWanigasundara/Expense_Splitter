#include "httplib.h"
#include "json.hpp"
#include "ExpenseManager.h"
#include <iostream>

using json = nlohmann::json;

// Helper to handle CORS so the browser accepts the response
void set_cors(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS, DELETE");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

int main() {
    httplib::Server svr;
    ExpenseManager manager;

    // Pre-flight CORS handler
    svr.Options(R"(.*)", [](const httplib::Request&, httplib::Response& res) {
        set_cors(res);
    });

    svr.Get("/api/state", [&](const httplib::Request& req, httplib::Response& res) {
        set_cors(res);
        res.set_content(manager.getStateJSON().dump(), "application/json");
    });

    svr.Post("/api/person", [&](const httplib::Request& req, httplib::Response& res) {
        set_cors(res);
        auto payload = json::parse(req.body);
        manager.addPerson(payload["name"]);
        res.set_content(manager.getStateJSON().dump(), "application/json");
    });

    svr.Post("/api/expense", [&](const httplib::Request& req, httplib::Response& res) {
        set_cors(res);
        auto payload = json::parse(req.body);
        
        Expense exp;
        exp.id = payload["id"];
        exp.payer = payload["payer"];
        exp.amountLKR = payload["amountLKR"];
        exp.splitTypeRaw = payload["splitTypeRaw"];
        
        for (const auto& p : payload["splitBetweenRaw"]) exp.splitBetweenRaw.push_back(p);
        
        if (payload.contains("exactAmountsMap")) {
            for (auto& el : payload["exactAmountsMap"].items()) {
                exp.exactAmountsMap[el.key()] = el.value();
            }
        }
        
        exp.splitTypeDisplay = payload["splitTypeDisplay"];
        exp.splitBetweenDisplay = payload["splitBetweenDisplay"];
        exp.date = payload["date"];

        manager.addExpense(exp);
        res.set_content(manager.getStateJSON().dump(), "application/json");
    });

    svr.Post("/api/expense/delete", [&](const httplib::Request& req, httplib::Response& res) {
        set_cors(res);
        auto payload = json::parse(req.body);
        manager.deleteExpense(payload["id"]);
        res.set_content(manager.getStateJSON().dump(), "application/json");
    });

    svr.Post("/api/clear", [&](const httplib::Request& req, httplib::Response& res) {
        set_cors(res);
        manager.clearAll();
        res.set_content(manager.getStateJSON().dump(), "application/json");
    });

    std::cout << "Engine running. Server started at http://localhost:8080\n";
    std::cout << "Keep this terminal open, and open your index.html in the browser.\n";
    svr.listen("localhost", 8080);
}