cmake_minimum_required(VERSION 3.10)
project(StockSim CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/PriceHistory.cpp
    src/FinancialAsset.cpp
    src/CSVParser.cpp
    src/Stock.cpp
    src/ETF.cpp
    src/CircularQueue.cpp
    src/TradeStack.cpp
    src/OrderQueue.cpp
    src/StockBST.cpp
    src/Portfolio.cpp
    src/TradingStrategy.cpp
    src/FixedSIPStrategy.cpp
    src/DynamicSIPStrategy.cpp
    src/GoldenCrossStrategy.cpp
    src/MomentumStrategy.cpp
    main.cpp
)

add_executable(stocksim ${SOURCES})

target_compile_options(stocksim PRIVATE -Wall -Wextra -O2)