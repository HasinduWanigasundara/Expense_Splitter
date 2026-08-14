# Expense Splitter Pro

A modern, responsive web application built to track group trip expenses in Sri Lankan Rupees (LKR) and calculate the minimum number of transactions required to settle up.

---

## 🚀 How to Run It

This is a lightweight, zero-configuration client-side application. No terminal commands, backend servers, or package managers (like Node.js or npm) are required to run it.

1. **Clone or Download** this repository.
2. Open the folder and double-click the **`index.html`** file, or open it directly in any modern web browser (Chrome, Firefox, Edge, Safari).
3. *(Optional)* If you are using VS Code, you can right-click `index.html` and select **"Open with Live Server"**.

---

## 💡 Assumptions Made and Why

* **Local-First, Single-Session Architecture:** It was assumed that a multi-user database and remote server infrastructure were unnecessary for a quick trip-sharing tool. Utilizing browser `localStorage` allows data to persist across refreshes on a single device without adding login or user account overhead[cite: 1].
* **Integer-Based Currency Precision:** All calculations are converted and processed internally in "cents" (multiplying LKR amounts by 100). This avoids floating-point rounding errors (like `0.1 + 0.2 != 0.3`) commonly encountered in JavaScript financial apps.
* **Greedy Settlement Model:** It is assumed that minimizing the absolute number of cash transfers between debtors and creditors using a greedy debt-simplification algorithm provides the best user experience for a group settling up at the end of a trip.

---

## ⏳ Anything Left Incomplete, and Why (Prioritization)

* **Cloud Sync and Multi-User Access:** True multi-user features—such as remote databases, user roles (Admin vs. Guest), and live link-sharing across different devices—were left out. 
* **Why:** Per the project guidelines, the primary focus was prioritized heavily on the **correctness of the core math logic, cent-precision rounding, and minimal-transaction graph calculations**, rather than getting bogged down in backend infrastructure and authentication edge cases[cite: 1].

---

## 🔮 What You’d Do Differently, or Build Next, with More Time

* **High-Performance C++ Backend:** Migrate the core transaction ledger and Settle Up graph algorithms into a compiled C++ REST API (using lightweight headers like `cpp-httplib`), turning the frontend into a pure view layer communicating via HTTP.
* **Graphical Visualizations:** Integrate charting libraries (such as Chart.js or Vis.js) to render dynamic bar graphs of running balances and interactive node-network diagrams mapping out the exact debt transfer paths.
* **Advanced Split Types:** Fully implement percentage-based splitting alongside equal and exact amount splits with automated remainder balancing.
