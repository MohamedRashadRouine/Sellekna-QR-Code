#include <iostream>
#include <string>
#include "qrcodegen.hpp"
#include "url.hpp"

using qrcodegen::QrCode;
using qrcodegen::QrSegment;
using sellekna::UrlBuilder;

std::string generateQRCode(const std::string& merchantId, double amount, const std::string& description) {
    // Create payment URL with parameters
    std::string paymentUrl = UrlBuilder::createPaymentUrl(merchantId, amount, description);

    // Generate QR code with error correction
    QrCode qr = QrCode::encodeText(paymentUrl.c_str(), QrCode::Ecc::HIGH);
    
    // Convert to string representation
    std::string result;
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            result += qr.getModule(x, y) ? "██" : "  ";
        }
        result += "\n";
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <merchant_id> <amount> <description>" << std::endl;
        return 1;
    }

    try {
        std::string merchantId = argv[1];
        double amount = std::stod(argv[2]);
        
        if (amount <= 0) {
            throw std::invalid_argument("Amount must be greater than 0");
        }
        
        std::string description = argv[3];
        std::string qrCode = generateQRCode(merchantId, amount, description);
        std::cout << qrCode << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}