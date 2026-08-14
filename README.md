# Expense Splitter App

A modern, responsive web application built to track group trip expenses in Sri Lankan Rupees (LKR) and calculate the minimum number of transactions required to settle up.

## Features
* **Group Management**: Add members dynamically to the trip pool.
* **Flexible Splitting**: Support for both equal splits and custom exact-amount distributions with robust floating-point/cent-precision handling.
* **Audit Trail**: View past transaction history with inline edit and delete capabilities that automatically and accurately recalculate running balances.
* **Minimal Debt Simplification**: Utilizes a greedy graph algorithm to minimize the number of cash transfers required for settling up.
* **Local Persistence**: Uses browser `localStorage` to ensure data survives page refreshes.

## Tech Stack
* HTML5 / Tailwind CSS (via CDN)
* Vanilla JavaScript (ES6+)
