import { Observable } from '@nativescript/core';
import { QRScannerService } from '../services/qr-scanner.service';
import { PaymentService } from '../services/payment.service';

export class MainViewModel extends Observable {
  private qrScanner: QRScannerService;
  private paymentService: PaymentService;
  
  constructor() {
    super();
    this.qrScanner = new QRScannerService();
    this.paymentService = new PaymentService();
  }

  async onScanQRCode() {
    try {
      const paymentDetails = await this.qrScanner.scanQRCode();
      const success = await this.paymentService.processPayment(paymentDetails);
      
      if (success) {
        this.notify({ eventName: 'paymentSuccess', data: paymentDetails });
      }
    } catch (error) {
      this.notify({ eventName: 'paymentError', data: error.message });
    }
  }
}