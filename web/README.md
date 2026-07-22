# Smart Customer Purchase Pattern Analyzer — Prem Supermart

A C++ console application (with an optional web dashboard) that records
customer purchases and analyzes buying patterns: revenue, top products,
top customers, category-wise sales, and product recommendations.

## What changed in this upgrade

### C++ application — restructured into proper modules
Previously all logic lived in one `main.cpp`, and `Purchase.h/.cpp`,
`Filehandler.h/.cpp`, and `Analyzer.h/.cpp` existed but were empty. The
project is now properly split:

- **`Purchase.h / Purchase.cpp`** — the `Purchase` record class (getters,
  setters, CSV serialization).
- **`Filehandler.h / Filehandler.cpp`** — all file I/O: loading/saving
  records and exporting reports.
- **`Analyzer.h / Analyzer.cpp`** — all analytics logic (revenue, top
  product, top customer, category sales, recommendations, search,
  averages) using STL `map`/`set` for cleaner, faster logic.
- **`main.cpp`** — thin menu/driver layer that wires the above together.

### Bugs fixed
- The old menu's exit logic was broken: the loop condition was
  `while (choice != 8)`, but option 8 was labeled "Save Records" and
  option 9 ("Exit") never actually exited. The menu now has a single,
  correct Exit option (13).
- No input validation previously existed — entering text where a number
  was expected (or a negative quantity/price) could crash the program or
  loop forever. All numeric input is now validated with retry prompts.
- Malformed lines in `data.txt` no longer crash the loader; they're
  skipped with a warning.

### New features (C++)
- **Search records by Customer ID**
- **Edit an existing record**
- **Delete a record**
- **Average Order Value** and **Unique Customer Count** stats
- **Business Summary Report** — view a full formatted report on-screen
  and optionally export it to `summary-report.txt` for your project
  submission/screenshots.

### Web dashboard upgrade
- **Charts** (Chart.js): a category-sales doughnut chart and a top-5
  products bar chart, alongside the existing metric cards and category
  table.
- **Edit / Delete** buttons directly in the records table.
- **Search/filter** box to quickly find records by customer, product,
  or category.
- New metrics: Average Order Value, Unique Customers, Total Orders.
- CSV import/export, browser-local-storage save/load, and the
  `data.txt` bridge to the C++ app all still work as before.

## Running the C++ app

```bash
g++ -std=c++17 -o analyzer main.cpp Purchase.cpp Filehandler.cpp Analyzer.cpp
./analyzer
```

(On Windows with MinGW: `g++ -std=c++17 -o analyzer.exe main.cpp Purchase.cpp Filehandler.cpp Analyzer.cpp`)

Records are read from and saved to `data.txt` in the same folder.

## Running the web dashboard

```bash
node server.js
```

Then open `http://localhost:3000` in your browser. Use **Load C++ Data**
to pull in whatever `data.txt` currently holds, or **Save to C++ Data**
to push the dashboard's records back into `data.txt` for the C++ app to
pick up.

## File structure

```
main.cpp            - menu-driven entry point
Purchase.h/.cpp      - Purchase record class
Filehandler.h/.cpp   - file load/save/export helpers
Analyzer.h/.cpp      - all analytics functions
data.txt             - persisted purchase records (CSV, no header row)
purchase-records.csv - sample dataset (CSV, with header row)
server.js            - tiny static file server + data.txt bridge
web/                 - dashboard (index.html, app.js, styles.css)
```
