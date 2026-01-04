cmake_minimum_required(VERSION 3.10)
project(MerkelRex)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(merklerex
        main.cpp
        MerkelMain.cpp
        OrderBook.cpp
        OrderBookEntry.cpp
        CSVReader.cpp
        Wallet.cpp
        Candlestick.cpp
        CandlestickAnalyzer.cpp
)

# Copy CSV data file to build directory
add_custom_command(TARGET merklerex POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/20200601.csv
        $<TARGET_FILE_DIR:merklerex>/20200601.csv
    COMMENT "Copying CSV data file to build directory"
)