# AI Prompts Log — ESE 224 Final Project
**Student Name:** Jordan Trojan | Evan Attrino
**Student ID:**   116147886 | 116141819

---

## Instructions
For every AI prompt you use during this project, add an entry below in the format shown.
This file gets submitted with your project and is the source of truth for your AI Documentation grade (10 pts).
You must also fill out the AI Usage Feedback Form after completing the project:
https://docs.google.com/forms/d/1NpkLKFrnvGgSsycC4Ba0IhCPSL_MIHtGaU-X84D5CnY

---

## Format
```
### Section: [e.g., PriceHistory / CSVParser / GoldenCrossStrategy / Report Section 3]
**Prompt:**
[paste your exact prompt here]
**What you changed / why (if any):**
[what was wrong or insufficient, and what you modified]
```

---

## Prompts

### Section: PriceNode.h
**Prompt:**
No AI used.

**What you changed / why (optional):**
Wrote the struct directly from the project specification.

---

### Section: PriceHistory.h / PriceHistory.cpp
**Prompt:**
void printRange(const string& startDate, const string& endDate) const; what is this supposed to print?

**What you changed / why (optional):**
Got conceptual explanation of what to print. Wrote my own implementation.

---

### Section: PriceHistory.h / PriceHistory.cpp
**Prompt:**
what about using CSVParser::dateinrange?

**What you changed / why (optional):**
AI suggested it would create a circular include dependency, so I kept date comparison inline using direct >= and <= rather than calling CSVParser.

---

### Section: PriceHistory.h / PriceHistory.cpp
**Prompt:**
Iterator::Iterator(PriceNode* node) { } [asking how to fill it in]

**What you changed / why (optional):**
AI showed initializer list syntax. I used the simpler "cur = node;" approach for readability.

---

### Section: PriceHistory.h / PriceHistory.cpp
**Prompt:**
Iterator& PriceHistory::Iterator::operator++() { } [asking how to fill it in]

**What you changed / why (optional):**
Used the explanation to write my own version. Had to fix variable name mismatch (cur vs current) by checking my header file.

---

### Section: PriceHistory.h / PriceHistory.cpp
**Prompt:**
[Showed VS Code error] declaration is incompatible with PriceHistory::Iterator

**What you changed / why (optional):**
Learned that the return type Iterator& needs the full scope PriceHistory::Iterator& because the compiler reads left to right. Applied this to all my iterator method definitions.

---

### Section: PriceHistory.h / PriceHistory.cpp
**Prompt:**
PriceHistory::PriceNode& PriceHistory::Iterator::operator*() { return current; } [asking if correct]

**What you changed / why (optional):**
AI pointed out I needed to dereference the pointer with *current, not just return the pointer itself. Also removed unnecessary PriceHistory:: prefix on PriceNode since it's defined globally.

---

### Section: PriceHistory.h / PriceHistory.cpp
**Prompt:**
bool PriceHistory::Iterator::operator!= (const Iterator& other) const { } what should this do

**What you changed / why (optional):**
AI explained it should compare the underlying node pointers, not the iterator objects themselves. Wrote return current != other.current.

---

### Section: PriceHistory.h / PriceHistory.cpp
**Prompt:**
if other is an iterator why use .current? / why not just compare the iterator if they are pointers / what would comparing objects do / im confused why do any point to nullptr

**What you changed / why (optional):**
Clarified my understanding of how iterators wrap pointers and how nullptr is used as the stop sentinel for begin/end semantics.

---

### Section: PriceHistory.h / PriceHistory.cpp
**Prompt:**
Iterator begin() const; // points to head [asking how to fill it in]

**What you changed / why (optional):**
Wrote both begin() returning Iterator(head) and end() returning Iterator(nullptr). Did the same pattern for rbegin/rend.

---

### Section: PriceHistory.h / PriceHistory.cpp
**Prompt:**
[full PriceHistory.cpp file pasted for review]

**What you changed / why (optional):**
AI noted that I had rbegin() and rend() swapped — rbegin should return ReverseIterator(tail) not nullptr, and rend should return nullptr not head. Fixed both.

---

### Section: FinancialAsset.h / FinancialAsset.cpp
**Prompt:**
What is a getter and a setter

**What you changed / why (optional):**
Used the explanation to understand encapsulation patterns. Wrote my own getters/setters across all classes.

---

### Section: FinancialAsset.h / FinancialAsset.cpp
**Prompt:**
No AI used.

**What you changed / why (optional):**
Implemented constructor and getters/setters directly from the header. Empty virtual destructor for the abstract base class.

---

### Section: CSVParser.h / CSVParser.cpp
**Prompt:**
why isnt extract year working [showed undefined identifier error]

**What you changed / why (optional):**
Forgot to use CSVParser:: scope and missing include. Added #include "CSVParser.h" and prefixed the call.

---

### Section: CSVParser.h / CSVParser.cpp
**Prompt:**
what is . vs ->

**What you changed / why (optional):**
Used the rule (object → ., pointer → ->) throughout my code.

---

### Section: CSVParser.h / CSVParser.cpp
**Prompt:**
[partial loadHistory implementation showing pattern of find + erase]

**What you changed / why (optional):**
AI showed the rest of the parsing pattern. I continued with the same approach for High, Low, Close, Adj Close, and Volume.

---

### Section: CSVParser.h / CSVParser.cpp
**Prompt:**
what is going on though / so you not moving the cursor? / what is stod / why pos+1? / and erase erases from 0 to pos-1?

**What you changed / why (optional):**
Clarified my understanding of how find + erase works on strings. Used this knowledge to fix off-by-one errors in my Low field erase that was missing +1.

---

### Section: CSVParser.h / CSVParser.cpp
**Prompt:**
what is ... in catch / why cant i do catch()

**What you changed / why (optional):**
Learned that catch(...) catches all exceptions and you can't have empty parens. Wrapped CSV row parsing in try/catch(...) to skip malformed rows.

---

### Section: CSVParser.h / CSVParser.cpp
**Prompt:**
[showed loadHistory with static keyword in cpp]

**What you changed / why (optional):**
AI pointed out that static goes only in the header, not the .cpp implementation. Removed static from all CSVParser method definitions in the .cpp.

---

### Section: CSVParser.h / CSVParser.cpp
**Prompt:**
[showed cpp with volume reading still using find]

**What you changed / why (optional):**
AI noted volume is the last field with no trailing comma, so I just call stol(line) on the remaining string instead of using find.

---

### Section: Stock.h / Stock.cpp
**Prompt:**
No AI used.

**What you changed / why (optional):**
Implemented constructor, destructor, loadFromCSV (delegates to CSVParser), getPriceOnDate, getYearStartPrice/getYearEndPrice (using forward and reverse iterators respectively), and printSummary on my own.

---

### Section: ETF.h / ETF.cpp
**Prompt:**
yea but what if it is called in etf / yea but if a stock is called there now wont be a bottom line

**What you changed / why (optional):**
Discussed how to handle formatting border lines when one print function calls another. Decided to skip the === lines entirely.

---

### Section: ETF.h / ETF.cpp
**Prompt:**
double calculate10YearCAGR() const; [asking for help]

**What you changed / why (optional):**
AI initially gave a version using head/tail prices. I noticed it should be the LAST 10 years specifically, so used a reverse iterator to walk back from the tail to find the price 10 years before the most recent date.

---

### Section: ETF.h / ETF.cpp
**Prompt:**
void ETF::setExpenseRatio (double ratio) const { expenseRatio = ratio; } [showed code]

**What you changed / why (optional):**
AI pointed out setters can't be const because they modify state. Removed const from setter signature.

---

### Section: CircularQueue.h / CircularQueue.cpp
**Prompt:**
No AI used.

**What you changed / why (optional):**
Implemented the circular buffer entirely on my own using head/tail/count tracking with modular arithmetic. The trickiest part was the enqueue-when-full case which advances head along with tail to overwrite the oldest value.

---

### Section: TradeStack.h / TradeStack.cpp
**Prompt:**
return TradeRecord(); // Return default record if stack is empty / really it does

**What you changed / why (optional):**
Discussed risks of silently returning default values. Kept the cout error message + default return since the project rubric says preconditions are caller's responsibility.

---

### Section: TradeStack.h / TradeStack.cpp
**Prompt:**
[showed destructor calling pop() in a loop]

**What you changed / why (optional):**
AI suggested directly deleting nodes instead of calling pop() to avoid unnecessary TradeRecord copies. Rewrote the destructor with manual pointer walking.

---

### Section: TradeStack.h / TradeStack.cpp
**Prompt:**
why does stacknode point to top?

**What you changed / why (optional):**
Clarified how new nodes link to the previous top in a stack so the chain isn't lost.

---

### Section: OrderQueue.h / OrderQueue.cpp
**Prompt:**
void OrderQueue::printAll() const { ... while (current != back) { ... } }

**What you changed / why (optional):**
AI caught that current != back skips the last node. Changed to current != nullptr so the last element is also printed.

---

### Section: OrderQueue.h / OrderQueue.cpp
**Prompt:**
[showed destructor with !isEmpty without parens]

**What you changed / why (optional):**
AI noted that isEmpty needs parentheses to call the function. Fixed to !isEmpty().

---

### Section: OrderQueue.h / OrderQueue.cpp
**Prompt:**
why current!= nullptr? / next is auto initialized to nullptr?

**What you changed / why (optional):**
Confirmed that next is NOT auto-initialized — I have to set it explicitly when creating a new node. Made sure my QueueNode constructor passes nullptr.

---

### Section: StockBST.h / StockBST.cpp
**Prompt:**
No AI used.

**What you changed / why (optional):**
Wrote all BST logic myself: insertHelper (recursive with duplicates going right), searchHelper, rangeSearchHelper (with pruning), all three traversals, heightHelper, clearHelper (post-order delete), and findMax. Only thing I had to look up later was that BSTNode is nested inside StockBST so external code needs StockBST::BSTNode.

---

### Section: Portfolio.h / Portfolio.cpp
**Prompt:**
do buyshares

**What you changed / why (optional):**
AI gave a version with average cost basis calculation. I used this as a reference but rewrote it to match my exact member variable names.

---

### Section: Portfolio.h / Portfolio.cpp
**Prompt:**
[showed sellShares I wrote that incorrectly subtracted cash and updated avgCostBasis]

**What you changed / why (optional):**
AI pointed out three bugs: (1) selling adds cash not subtracts, (2) avgCostBasis doesn't change on sell, (3) the for loop had to come before the shares check. Rewrote with these fixes.

---

### Section: Portfolio.h / Portfolio.cpp
**Prompt:**
[showed code accessing holdings[i] after erase()]

**What you changed / why (optional):**
AI flagged that accessing holdings[i] after erasing it crashes. Restructured to set currentPrice before potentially erasing.

---

### Section: Portfolio.h / Portfolio.cpp
**Prompt:**
TradeRecord last = tradeHistory.pop(); ... tradeHistory.pop(); doesnt selli gor buying also add to the stack so you have to po[p

**What you changed / why (optional):**
We discussed whether to call buyShares/sellShares (which push to stack and require double-pop) or to manually adjust cashBalance and holdings. Went with the manual approach since the undo shouldn't leave a trace in trade history.

---

### Section: Portfolio.h / Portfolio.cpp
**Prompt:**
[showed executeNextOrder I wrote] / dodes it matter if it deqsueus before or after?

**What you changed / why (optional):**
Discussed dequeue ordering. Confirmed my placement (dequeue at end, outside all branches) is correct since the spec says orders are always discarded after attempted execution.

---

### Section: Portfolio.h / Portfolio.cpp
**Prompt:**
Why ... what is lamda? / is there antoher way to do this / cant you just compare the tickers? / yea but like without the helper

**What you changed / why (optional):**
Long discussion about how std::sort needs a comparator and what lambdas do. Wrote my own sort with [](const Position& a, const Position& b) { return a.ticker < b.ticker; }.

---

### Section: Portfolio.h / Portfolio.cpp
**Prompt:**
void Portfolio::printHoldings() const { } [asking for help]

**What you changed / why (optional):**
Used as reference for layout. Rewrote in my own format using the variable names from my header.

---

### Section: StockManager.h
**Prompt:**
why isnt pricate data handlesa in construcftor?

**What you changed / why (optional):**
Confirmed that vector<T*> auto-initializes to empty so no constructor body needed.

---

### Section: StockManager.h
**Prompt:**
No AI used.

**What you changed / why (optional):**
Implemented the entire template class in the header. All methods follow patterns I'd already used elsewhere — vector iteration, lambda comparators, ticker-based linear search.

---

### Section: TradingStrategy.h / TradingStrategy.cpp
**Prompt:**
what is backtest? / so we simulate only one asset?

**What you changed / why (optional):**
Conceptual question to understand what the strategy classes were supposed to do. Clarified single-asset simulation.

---

### Section: TradingStrategy.h / TradingStrategy.cpp
**Prompt:**
No AI used.

**What you changed / why (optional):**
Implemented printResult, calculateCAGR using the formula (endVal/startVal)^(1/years) - 1, and calculateMaxDrawdown by tracking running peak in a single pass.

---

### Section: FixedSIPStrategy.h / FixedSIPStrategy.cpp
**Prompt:**
the only uestion i have is wqhy is lastClose = node.close after the if statuement / why is it even in the for loop then / why doesnt node stay why cant it bhe accessed?

**What you changed / why (optional):**
Clarified scope — node is declared inside the loop so it goes out of scope after, which is why I save lastClose outside.

---

### Section: FixedSIPStrategy.h / FixedSIPStrategy.cpp
**Prompt:**
[showed early backtest implementation with several bugs: lastMonth = y, no lastClose update, calculations inside loop, no return statement]

**What you changed / why (optional):**
AI listed five bugs. Fixed: (1) typo lastMonth = y → lastYear = y, (2) added lastClose = node.close inside loop, (3) added portfolioValues.push_back inside loop, (4) moved result calculations outside loop, (5) added return result.

---

### Section: MomentumStrategy.h / MomentumStrategy.cpp
**Prompt:**
yea but the constructor doesnt use m or d / so this compare by ticker tells the ai the means of what to swap

**What you changed / why (optional):**
Discussed parameter name shadowing and why this-> is needed when names match. Used short parameter names (m, d) to avoid the issue.

---

### Section: MomentumStrategy.h / MomentumStrategy.cpp
**Prompt:**
[showed nested for loop trying to construct ReverseIterator from Iterator]

**What you changed / why (optional):**
AI showed I needed to construct the ReverseIterator from a node pointer using &(*it) since my Iterator class doesn't expose a ptr() accessor.

---

### Section: MomentumStrategy.h / MomentumStrategy.cpp
**Prompt:**
where does cash come from? / it isnt it porfoloi or anything / so you invest eerything (all sold + monthly cap ital for that month)?

**What you changed / why (optional):**
Clarified that cash is a local simulation variable, not connected to Portfolio. Confirmed buy logic invests both accumulated cash and current month's contribution.

---

### Section: MomentumStrategy.h / MomentumStrategy.cpp
**Prompt:**
For this if trailingReturn > momentumThreshold but it is a new month does monthly capitcal gett added to cash?

**What you changed / why (optional):**
AI caught that monthly capital was only being added inside the buy branch. Restructured so cash += monthlyCapital and totalInvested += monthlyCapital happen unconditionally on first day of month.

---

### Section: GoldenCrossStrategy.h / GoldenCrossStrategy.cpp
**Prompt:**
[showed full GoldenCrossStrategy.cpp with several issues]

**What you changed / why (optional):**
AI identified four bugs: (1) duplicate portfolioValues.push_back and lastClose lines, (2) prevValues never set to true, (3) MA queues only fed within range (need warmup), (4) monthly investment was unconditional instead of cash-based for the cross signals. Made all four fixes.

---

### Section: DynamicSIPStrategy.h / DynamicSIPStrategy.cpp
**Prompt:**
ok rewrite the file using elitestrategy, but rename it to dynamicsipstrategy / can you write a comment in the .h saying how this works / but it sells everythiung thought? / why three layers? / what are the dip thresholds calculated as?

**What you changed / why (optional):**
Started with a basic Dynamic SIP, then chose to upgrade to a hybrid 3-layer strategy combining MA crossover (entry/exit), tiered dip-buying (mild/severe), and monthly SIP base. Wrote header comment explaining the three layers. Confirmed thresholds are calculated as % drop from 12-month rolling high using the ReverseIterator.

---

### Section: DynamicSIPStrategy.h / DynamicSIPStrategy.cpp
**Prompt:**
[showed parameter sweep output where every combination produced exactly $626,781]

**What you changed / why (optional):**
AI flagged that since cash was being deployed every month regardless of dip signal, the dip thresholds were never reached. Removed the "else if (newCashAvailable)" branch so cash only deploys on actual dip signals. After fix, sweep showed real variation.

---

### Section: main.cpp
**Prompt:**
ok expand sweep to have it vary a lot more

**What you changed / why (optional):**
Expanded the parameterSweep function from 3 variables to 6 variables. Total combinations went from 1683 to ~13500.

---

### Section: main.cpp
**Prompt:**
where does it show the best because the last line is m=8_s=35_x=100 / i know but i dont see m=8_s=19_x=100 (there are muiltiple good strategioes?) / is this the best strategy?

**What you changed / why (optional):**
AI explained that BST inorder prints worst→best so the last entry is the best, but tied values just show whichever was inserted last. Used this insight to write the discussion section in the report.

---

### Section: main.cpp
**Prompt:**
[various menu function questions]

**What you changed / why (optional):**
Implemented all 16 menu items myself with handler functions. Used AI to spot bugs (missing spx variable references after find/replace, wrong vector type for BSTNode pointer needing StockBST::BSTNode prefix).

---

### Section: main.cpp
**Prompt:**
these characters (the = ones) do not run correctly [Windows terminal can't render Unicode box chars]

**What you changed / why (optional):**
Replaced all Unicode box-drawing characters with ASCII equivalents (+, |, =) so the menu renders correctly on Windows PowerShell.

---

### Section: CMakeLists.txt
**Prompt:**
what command do i use to run this? / from where in the code / can i try cmake? / which path? / i had to reset vscode entirely. new issue

**What you changed / why (optional):**
Multiple issues with CMake on Windows. Eventually used the direct g++ command:
g++ -std=c++11 -Iinclude src/*.cpp main.cpp -o stocksim

---

### Section: Build / Compilation
**Prompt:**
[showed multiple compilation error screenshots]

**What you changed / why (optional):**
Fixed missing #include statements one by one (CSVParser.h, fstream, vector, iomanip, cmath, algorithm). Also fixed circular include in GoldenCrossStrategy.h that was causing duplicate definition errors — turned out a duplicate .h file was sitting in src/ alongside the .cpp.

---


