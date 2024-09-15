# OUCH Protocol Message Parser in C++

This project implements a simple parser for the OUCH protocol, which is commonly used in financial exchanges for order and cancel messages. The project includes classes for parsing and printing "New Order" and "Cancel Order" messages.

## Features

- **New Order Message Parsing**: Parses and prints details such as order token, buy/sell indicator, shares, stock symbol, price, firm, and more.
- **Cancel Order Message Parsing**: Parses and prints the order token of the order to be canceled.
- **Custom Message Parser**: Automatically identifies the message type (`New Order` or `Cancel Order`) and parses accordingly.

## Project Structure

- `OuchNewOrder`: Handles parsing and printing of new order messages.
- `OuchCancelOrder`: Handles parsing and printing of cancel order messages.
- `OuchParser`: A utility class to parse incoming messages based on their type.

## How to Run

1. Compile the project using any C++ compiler:
   ```bash```
   g++ -o ouch_parser ouch_parser.cpp

   char message_buffer[] = {
    'O', 'T', 'O', 'K', 'E', 'N', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'B', 0x00, 0x00, 0x03, 0xE8, 'S', 'T', 'O', 'C', 'K', 'A', ' ', ' ',
    0x00, 0x00, 0x0F, 0xA0, 'D', 'A', 'Y', ' ', 'F', 'I', 'R', 'M', 'Y', 'A', 'N'
};