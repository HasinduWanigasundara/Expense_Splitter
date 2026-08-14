# Expense Splitter Pro

A modern, responsive web application built to track group trip expenses in Sri Lankan Rupees (LKR) and calculate the minimum number of transactions required to settle up.

---

## 📂 Project Structure & Process

The project was developed using an iterative, multi-phase engineering process:

1. **Client-Side SPA Phase (`index.html`)**: Built as a fully functional, self-contained single-page web application using HTML5, Tailwind CSS, and Vanilla JavaScript. This version handles all UI rendering, local data persistence via `localStorage`, and real-time balance calculations.
2. **Full-Stack C++ Architecture Phase (`tortil project/`)**: Attempted an advanced architectural pivot by moving the core business logic and state management into a high-performance C++ backend engine using a lightweight HTTP header library (`cpp-httplib`) and JSON parsing (`json.hpp`), communicating with the frontend via REST API endpoints.

---

## 🚀 How to Run It

### Option 1: The Working Frontend (Recommended)
Because the application is built entirely into a standalone file:
1. Open the repository root folder.
2. Double-click **`index.html`** to open it directly in any modern web browser.
3. *(Optional)* Alternatively, open it via a local development tool like VS Code's **Live Server** extension.

### Option 2: The Experimental C++ Backend Approach (`tortil project/`)
All files related to the C++ API engine are located inside the `tortil project/` folder. 
* *Note:* This approach is included to demonstrate system architecture exploration, but **it is currently incomplete and not working properly** due to local network loopback socket binding / server connection handshake errors encountered during execution.

To attempt running it:
1. Navigate into the `tortil project/` directory via your terminal.
2. Compile the server using `g++`:
   ```powershell
   g++ server.cpp ExpenseManager.cpp -o server.exe -lws2_32
   ## Running the Executable

```powershell
.\server.exe
