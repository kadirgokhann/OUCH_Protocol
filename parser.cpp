#include <iostream>
#include <cstring>  // For memcpy
#include <netinet/in.h>  // For ntohl (network byte order to host byte order)

// Common header for all OUCH messages
struct OuchMessageHeader {
    char message_type;  // Single byte message type
};

// New Order Message
struct OuchNewOrder {
    char message_type;      // Message Type ('O' for New Order)
    char order_token[14];    // Unique token for the order
    char buy_sell_indicator; // 'B' for Buy, 'S' for Sell
    uint32_t shares;         // Number of shares
    char stock[8];           // Stock symbol
    uint32_t price;          // Price in cents
    char time_in_force[4];   // Time in force
    char firm[4];            // Firm ID
    char display;            // Display instruction
    char capacity;           // Capacity ('A' for agency, etc.)
    char intermarket_sweep_eligibility; // 'Y' for yes, 'N' for no

    void parse(const char* buffer) {
        // Parse the raw message buffer directly into the struct fields
        memcpy(order_token, buffer, 14);
        buy_sell_indicator = buffer[14];
        shares = ntohl(*reinterpret_cast<const uint32_t*>(buffer + 15));  // Network to host byte order
        memcpy(stock, buffer + 19, 8);
        price = ntohl(*reinterpret_cast<const uint32_t*>(buffer + 27));
        memcpy(time_in_force, buffer + 31, 4);
        memcpy(firm, buffer + 35, 4);
        display = buffer[39];
        capacity = buffer[40];
        intermarket_sweep_eligibility = buffer[41];
    }

    void print() const {
        std::cout << "New Order:\n"
                  << "Order Token: " << std::string(order_token, 14) << "\n"
                  << "Buy/Sell Indicator: " << buy_sell_indicator << "\n"
                  << "Shares: " << shares << "\n"
                  << "Stock: " << std::string(stock, 8) << "\n"
                  << "Price: " << price << "\n"
                  << "Firm: " << std::string(firm, 4) << "\n";
    }
};

// Cancel Order Message
struct OuchCancelOrder {
    char message_type;     // Message Type ('X' for Cancel)
    char order_token[14];   // Unique token for the order to be canceled

    void parse(const char* buffer) {
        memcpy(order_token, buffer, 14);
    }

    void print() const {
        std::cout << "Cancel Order:\n"
                  << "Order Token: " << std::string(order_token, 14) << "\n";
    }
};

// Parser class
class OuchParser {
public:
    static void parse_message(const char* buffer, size_t length) {
        if (length < 1) return;  // Ensure at least one byte for the message type

        char message_type = buffer[0];
        switch (message_type) {
            case 'O': {  // New Order
                OuchNewOrder new_order;
                new_order.parse(buffer + 1);  // Skip message type byte
                new_order.print();
                break;
            }
            case 'X': {  // Cancel Order
                OuchCancelOrder cancel_order;
                cancel_order.parse(buffer + 1);  // Skip message type byte
                cancel_order.print();
                break;
            }
            default:
                std::cerr << "Unknown message type: " << message_type << "\n";
                break;
        }
    }
};

int main() {
    // Example buffer containing a new order message ('O')
    char message_buffer[] = {
        'O',  // Message type
        'T', 'O', 'K', 'E', 'N', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',  // Order Token
        'B',  // Buy/Sell indicator
        0x00, 0x00, 0x03, 0xE8,  // Shares (1000 in network byte order)
        'S', 'T', 'O', 'C', 'K', 'A', ' ', ' ',  // Stock
        0x00, 0x00, 0x0F, 0xA0,  // Price (4000 cents, $40 in network byte order)
        'D', 'A', 'Y', ' ',      // Time in Force
        'F', 'I', 'R', 'M',      // Firm
        'Y',  // Display
        'A',  // Capacity
        'N'   // Intermarket Sweep Eligibility
    };

    // Parse and display the message
    OuchParser::parse_message(message_buffer, sizeof(message_buffer));

    return 0;
}
