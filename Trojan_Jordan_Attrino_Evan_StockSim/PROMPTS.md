# AI Prompts Log — ESE 224 Final Project
**Student Name:** Jordan Trojan | Evan Attrino
**Student ID:**   116147886 | 116141819

---

## Prompts

### Section: General C++ Concepts
**Prompt:**
What is a getter and a setter

**What you changed / why (optional):**
Used the explanation to understand encapsulation patterns. Wrote my own getters/setters across all classes following the convention.

---

### Section: PriceHistory / printRange
**Prompt:**
void printRange(const string& startDate, const string& endDate) const; what is this supposed to print?

**What you changed / why (optional):**
Got conceptual explanation of what to print. Wrote my own implementation rather than copying. Decided to also use CSVParser::dateInRange for the comparison logic.

---

### Section: PriceHistory / printRange
**Prompt:**
what about using CSVParser::dateinrange?

**What you changed / why (optional):**
AI suggested it would create a circular include dependency, so I kept date comparison inline using direct >= and <= rather than calling CSVParser, since it's a simple string comparison anyway.

---

### Section: PriceHistory / Iterator
**Prompt:**
Iterator::Iterator(PriceNode* node) { } [asking how to fill it in]

**What you changed / why (optional):**
AI showed initializer list syntax. I used the simpler "cur = node;" approach for readability.

---

### Section: PriceHistory / Iterator operator++
**Prompt:**
Iterator& PriceHistory::Iterator::operator++() { } [asking how to fill it in]

**What you changed / why (optional):**
Used the explanation to write my own version. Had to fix variable name mismatch (cur vs current) by checking my header file.

---

### Section: PriceHistory / Iterator scope error
**Prompt:**
[Showed VS Code error] declaration is incompatible with PriceHistory::Iterator

**What you changed / why (optional):**
Learned that the return type Iterator& needs the full scope PriceHistory::Iterator& because the compiler reads left to right. Applied this to all my iterator method definitions.

---

### Section: PriceHistory / operator*
**Prompt:**
PriceHistory::PriceNode& PriceHistory::Iterator::operator*() { return current; } [asking if correct]

**What you changed / why (optional):**
AI pointed out I needed to dereference the pointer with *current, not just return the pointer itself. Also removed unnecessary PriceHistory:: prefix on PriceNode since it's defined globally.

---

### Section: PriceHistory / operator!=
**Prompt:**
bool PriceHistory::Iterator::operator!= (const Iterator& other) const { } what should this do

**What you changed / why (optional):**
AI explained it should compare the underlying node pointers, not the iterator objects themselves. Wrote return current != other.current.

---

### Section: PriceHistory / Iterator concepts
**Prompt:**
if other is an iterator why use .current? / why not just compare the iterator if they are pointers / what would comparing objects do / im confused why do any point to nullptr

**What you changed / why (optional):**
Clarified my understanding of how iterators wrap pointers and how nullptr is used as the stop sentinel for begin/end semantics. No code changes — just conceptual understanding before implementing rbegin/rend correctly.

---

### Section: PriceHistory / begin and end
**Prompt:**
Iterator begin() const; // points to head [asking how to fill it in]

**What you changed / why (optional):**
Wrote both begin() returning Iterator(head) and end() returning Iterator(nullptr). Did the same pattern for rbegin/rend.

---

### Section: PriceHistory / Reverse Iterator review
**Prompt:**
[full PriceHistory.cpp file pasted for review]

**What you changed / why (optional):**
AI noted that I had rbegin() and rend() swapped — rbegin should return ReverseIterator(tail) not nullptr, and rend should return nullptr not head. Fixed both.

---

### Section: CSVParser / extractYear
**Prompt:**
why isnt extract year working [showed undefined identifier error]

**What you changed / why (optional):**
Forgot to use CSVParser:: scope and missing include. Added #include "CSVParser.h" and prefixed the call.

---

### Section: General C++ Concepts
**Prompt:**
what is . vs ->

**What you changed / why (optional):**
Used the rule (object → ., pointer → ->) throughout my code. No code change but clearer understanding.

---

### Section: CSVParser / loadHistory parsing
**Prompt:**
[partial loadHistory implementation showing pattern of find + erase]

**What you changed / why (optional):**
AI showed the rest of the parsing pattern. I continued with the same approach for High, Low, Close, Adj Close, and Volume. Realized later I needed to skip Adj Close column.

---

### Section: CSVParser / how parsing works
**Prompt:**
what is going on though / so you not moving the cursor? / what is stod / why pos+1? / and erase erases from 0 to pos-1?

**What you changed / why (optional):**
Clarified my understanding of how find + erase works on strings (no cursor — the string actually shrinks). Confirmed stod = string-to-double. Confirmed erase(start, count) takes count not end index. Used this knowledge to fix off-by-one errors in my Low field erase that was missing +1.

---

### Section: CSVParser / try-catch
**Prompt:**
what is ... in catch / why cant i do catch()

**What you changed / why (optional):**
Learned that catch(...) catches all exceptions and you can't have empty parens. Wrapped my CSV row parsing in try/catch(...) to skip malformed rows.

---

### Section: CSVParser / static keyword
**Prompt:**
[showed loadHistory with static keyword in cpp]

**What you changed / why (optional):**
AI pointed out that static goes only in the header, not the .cpp implementation. Removed static from all CSVParser method definitions in the .cpp.

---

### Section: CSVParser / volume parsing
**Prompt:**
[showed cpp with volume reading still using find]

**What you changed / why (optional):**
AI noted volume is the last field with no trailing comma, so I just call stol(line) on the remaining string instead of using find.

---

### Section: ETF / printSummary inheritance
**Prompt:**
yea but what if it is called in etf / yea but if a stock is called there now wont be a bottom line

**What you changed / why (optional):**
Discussed how to handle the formatting border lines when one print function calls another. Decided to skip the === lines entirely to keep both Stock and ETF print clean and simple.

---

### Section: ETF / 10-Year CAGR
**Prompt:**
double calculate10YearCAGR() const; [asking for help]

**What you changed / why (optional):**
AI initially gave a version using head/tail prices. I noticed it should be the LAST 10 years specifically, so we discussed using a reverse iterator to walk back from the tail to find the price 10 years before the most recent date. Implemented with rbegin().

---

### Section: ETF / setter const
**Prompt:**
void ETF::setExpenseRatio (double ratio) const { expenseRatio = ratio; } [showed code]

**What you changed / why (optional):**
AI pointed out setters can't be const because they modify state. Removed const from setter signature.

---

### Section: TradeStack / pop empty case
**Prompt:**
return TradeRecord(); // Return default record if stack is empty / really it does

**What you changed / why (optional):**
Discussed risks of silently returning default values. Kept the cout error message + default return for now since the project rubric says preconditions are caller's responsibility.

---

### Section: TradeStack / destructor
**Prompt:**
[showed destructor calling pop() in a loop]

**What you changed / why (optional):**
AI suggested directly deleting nodes instead of calling pop() to avoid unnecessary TradeRecord copies. Rewrote the destructor with manual pointer walking.

---

### Section: TradeStack / stack node next pointer
**Prompt:**
why does stacknode point to top?

**What you changed / why (optional):**
Clarified how new nodes link to the previous top in a stack so the chain isn't lost. No code change.

---

### Section: OrderQueue / printAll bug
**Prompt:**
void OrderQueue::printAll() const { ... while (current != back) { ... } }

**What you changed / why (optional):**
AI caught that current != back skips the last node. Changed to current != nullptr so the last element is also printed.

---

### Section: OrderQueue / isEmpty syntax
**Prompt:**
[showed destructor with !isEmpty without parens]

**What you changed / why (optional):**
AI noted that isEmpty needs parentheses to call the function — without them I was checking if the function pointer is null. Fixed to !isEmpty().

---

### Section: OrderQueue / nullptr initialization
**Prompt:**
why current!= nullptr? / next is auto initialized to nullptr?

**What you changed / why (optional):**
Confirmed that next is NOT auto-initialized — I have to set it explicitly when creating a new node. Made sure my QueueNode constructor passes nullptr.

---

### Section: Portfolio / buyShares
**Prompt:**
do buyshares

**What you changed / why (optional):**
AI gave a version with average cost basis calculation. I used this as a reference but rewrote it to match my exact member variable names. The avgCostBasis weighted-average formula was the key part to keep.

---

### Section: Portfolio / sellShares logic
**Prompt:**
[showed sellShares I wrote that incorrectly subtracted cash and updated avgCostBasis]

**What you changed / why (optional):**
AI pointed out three bugs: (1) selling adds cash not subtracts, (2) avgCostBasis doesn't change on sell, (3) the for loop had to come before the shares check. Rewrote the entire function with these fixes.

---

### Section: Portfolio / sellShares scope
**Prompt:**
[showed code accessing holdings[i] after erase()]

**What you changed / why (optional):**
AI flagged that accessing holdings[i] after erasing it crashes. Restructured to set currentPrice before potentially erasing, and to use the loop variable i correctly.

---

### Section: Portfolio / undoLastTrade
**Prompt:**
TradeRecord last = tradeHistory.pop(); ... tradeHistory.pop(); doesnt selli gor buying also add to the stack so you have to po[p

**What you changed / why (optional):**
We discussed whether to call buyShares/sellShares (which push to stack and require double-pop) or to manually adjust cashBalance and holdings. Went with the manual approach since the undo shouldn't leave a trace in trade history.

---

### Section: Portfolio / executeNextOrder
**Prompt:**
[showed executeNextOrder I wrote] / dodes it matter if it deqsueus before or after?

**What you changed / why (optional):**
Discussed dequeue ordering. Confirmed my original placement (dequeue at end, outside all branches) is correct since the spec says orders are always discarded after attempted execution. Added "Order Skipped" message for limit orders.

---

### Section: Portfolio / sortHoldingsByTicker — lambda
**Prompt:**
Why ... what is lamda? / is there antoher way to do this / cant you just compare the tickers? / yea but like without the helper

**What you changed / why (optional):**
Long discussion about how std::sort needs a comparator and what lambdas actually do. Went with the lambda approach since I understood it. Wrote my own version: sort with [](const Position& a, const Position& b) { return a.ticker < b.ticker; }.

---

### Section: Portfolio / printHoldings
**Prompt:**
void Portfolio::printHoldings() const { } [asking for help]

**What you changed / why (optional):**
Used as reference for layout. Rewrote in my own format using the variable names from my header.

---

### Section: StockManager / template private data
**Prompt:**
why isnt pricate data handlesa in construcftor?

**What you changed / why (optional):**
Confirmed that vector<T*> auto-initializes to empty so no constructor body needed. Kept empty constructor.

---

### Section: TradingStrategy / what is backtest
**Prompt:**
what is backtest? / so we simulate only one asset?

**What you changed / why (optional):**
Conceptual question to understand what the strategy classes were supposed to do. Clarified single-asset simulation. No code change — just understanding before implementing.

---

### Section: FixedSIPStrategy / lastClose timing
**Prompt:**
the only uestion i have is wqhy is lastClose = node.close after the if statuement / why is it even in the for loop then / why doesnt node stay why cant it bhe accessed?

**What you changed / why (optional):**
Clarified scope — node is declared inside the loop so it goes out of scope after, which is why I save lastClose outside. Wrote my own backtest using this understanding.

---

### Section: FixedSIPStrategy / structure review
**Prompt:**
[showed early backtest implementation with several bugs: lastMonth = y, no lastClose update, calculations inside loop, no return statement]

**What you changed / why (optional):**
AI listed five bugs. Fixed: (1) typo lastMonth = y → lastYear = y, (2) added lastClose = node.close inside loop, (3) added portfolioValues.push_back inside loop, (4) moved result calculations outside loop, (5) added return result.

---

### Section: MomentumStrategy / constructor parameter naming
**Prompt:**
yea but the constructor doesnt use m or d / so this compare by ticker tells the ai the means of what to swap

**What you changed / why (optional):**
Discussed parameter name shadowing and why this-> is needed when names match between parameters and member variables. Used short parameter names (m, d) to avoid the issue.

---

### Section: MomentumStrategy / ReverseIterator from Iterator
**Prompt:**
[showed nested for loop trying to construct ReverseIterator from Iterator]

**What you changed / why (optional):**
AI showed I needed to construct the ReverseIterator from a node pointer using &(*it) since my Iterator class doesn't expose a ptr() accessor. Used this approach to avoid modifying the header.

---

### Section: MomentumStrategy / cash variable confusion
**Prompt:**
where does cash come from? / it isnt it porfoloi or anything / so you invest eerything (all sold + monthly cap ital for that month)?

**What you changed / why (optional):**
Clarified that cash is a local simulation variable, not connected to Portfolio. The strategy simulates internally to compute the SimResult. Confirmed buy logic invests both accumulated cash and current month's contribution.

---

### Section: MomentumStrategy / monthly capital tracking bug
**Prompt:**
For this if trailingReturn > momentumThreshold but it is a new month does monthly capitcal gett added to cash?

**What you changed / why (optional):**
AI caught that monthly capital was only being added inside the buy branch — meaning during hold or sell months the money disappeared. Restructured so cash += monthlyCapital and totalInvested += monthlyCapital happen unconditionally on first day of month, then the buy/sell/hold logic operates on accumulated cash.

---

### Section: GoldenCrossStrategy / multiple bugs
**Prompt:**
[showed full GoldenCrossStrategy.cpp with several issues]

**What you changed / why (optional):**
AI identified: (1) duplicate portfolioValues.push_back and lastClose lines, (2) prevValues never set to true, (3) MA queues only fed within range (need warmup), (4) monthly investment was unconditional instead of cash-based for the cross signals. Made all four fixes — the warmup feed (always enqueue regardless of year) was the most important change.

---

### Section: DynamicSIPStrategy / strategy choice
**Prompt:**
ok rewrite the file using elitestrategy, but rename it to dynamicsipstrategy / can you write a comment in the .h saying how this works / but it sells everythiung thought? / why three layers? / what are the dip thresholds calculated as?

**What you changed / why (optional):**
Started with a basic Dynamic SIP, then chose to upgrade to a hybrid 3-layer strategy combining MA crossover (entry/exit), tiered dip-buying (mild/severe), and monthly SIP base. Wrote header comment explaining the three layers and got AI to clarify that thresholds are calculated as % drop from 12-month rolling high using the ReverseIterator.

---

### Section: DynamicSIPStrategy / sweep showing identical results
**Prompt:**
[showed parameter sweep output where every combination produced exactly $626,781]

**What you changed / why (optional):**
AI flagged that since cash was being deployed every month regardless of dip signal, the dip thresholds were never reached. Removed the "else if (newCashAvailable)" branch so cash only deploys on actual dip signals. After fix, sweep showed real variation — different parameters now produce different results between $668k and $682k.

---

### Section: parameterSweep / variable expansion
**Prompt:**
ok expand sweep to have it vary a lot more

**What you changed / why (optional):**
Expanded sweep from 3 variables (mild/severe/multiplier) to 6 variables (added shortWindow, longWindow, lookbackDays). Total combinations went from 1683 to ~13500. Took ~10 minutes to run but found a better optimum at sw=30 lw=300.

---

### Section: parameterSweep / interpreting results
**Prompt:**
where does it show the best because the last line is m=8_s=35_x=100 / i know but i dont see m=8_s=19_x=100 (there are muiltiple good strategioes?) / is this the best strategy?

**What you changed / why (optional):**
AI explained that BST inorder prints worst→best so the last entry is the best, but tied values just show whichever was inserted last. Many m=8 combos gave identical results because severe threshold rarely fires in this dataset. Used this insight to write a better discussion section in the report.

---

### Section: Main Menu / structure
**Prompt:**
[various menuLoadData, menuRunStrategy, menuCompareStrategies questions]

**What you changed / why (optional):**
Implemented all 16 menu items myself with handler functions per menu choice. Used AI mainly to spot bugs (missing spx variable references after find/replace, wrong vector type for BSTNode pointer needing StockBST::BSTNode prefix).

---

### Section: Main / Unicode characters
**Prompt:**
these characters (the = ones) do not run correctly [Windows terminal can't render Unicode box chars]

**What you changed / why (optional):**
Replaced all Unicode box-drawing characters with ASCII equivalents (+, |, =) so the menu renders correctly on Windows PowerShell.

---

### Section: Build / CMake setup
**Prompt:**
what command do i use to run this? / from where in the code / can i try cmake? / which path? / i had to reset vscode entirely. new issue

**What you changed / why (optional):**
Multiple issues with CMake on Windows — wrong directory, hidden file extensions, MinGW not in PATH, wrong MinGW version. Eventually used the direct g++ command:
g++ -std=c++11 -Iinclude src/*.cpp main.cpp -o stocksim

---

### Section: Compilation / linker errors
**Prompt:**
[showed multiple compilation error screenshots]

**What you changed / why (optional):**
Fixed missing #include statements one by one (CSVParser.h, fstream, vector, iomanip, cmath, algorithm). Also fixed circular include in GoldenCrossStrategy.h that was causing duplicate definition errors — turned out a duplicate .h file was sitting in src/ alongside the .cpp.

---