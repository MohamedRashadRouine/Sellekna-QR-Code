import { Observable } from '@nativescript/core';
import { QRScannerService } from './services/qr-scanner.service';
import { PaymentService } from './services/payment.service';
import { PaymentDetails } from './models/payment.model';

export class MainViewModel extends Observable {
  private qrScanner: QRScannerService;
  private paymentService: PaymentService;
  
  private _isProcessing: boolean = false;
  private _errorMessage: string = '';
  private _successMessage: string = '';

  constructor() {
    super();
    this.qrScanner = new QRScannerService();
    this.paymentService = new PaymentService();
  }

  get isProcessing(): boolean {
    return this._isProcessing;
  }

  set isProcessing(value: boolean) {
    if (this._isProcessing !== value) {
      this._isProcessing = value;
      this.notifyPropertyChange('isProcessing', value);
    }
  }

  get errorMessage(): string {
    return this._errorMessage;
  }

  set errorMessage(value: string) {
    if (this._errorMessage !== value) {
      this._errorMessage = value;
      this.notifyPropertyChange('errorMessage', value);
    }
  }

  get successMessage(): string {
    return this._successMessage;
  }

  set successMessage(value: string) {
    if (this._successMessage !== value) {
      this._successMessage = value;
      this.notifyPropertyChange('successMessage', value);
    }
  }

  async onScanQRCode() {
    try {
      this.isProcessing = true;
      this.errorMessage = '';
      this.successMessage = '';

      const qrData = await this.qrScanner.scanQRCode();
      
      const paymentDetails: PaymentDetails = {
        merchantId: qrData.merchantId,
        amount: qrData.amount,
        description: qrData.description
      };

      await this.paymentService.processPayment(paymentDetails);
      this.successMessage = `Payment of ${paymentDetails.amount} successfully processed`;
    } catch (error) {
      this.errorMessage = error.message || 'An error occurred while processing the payment';
    } finally {
      this.isProcessing = false;
    }
  }
}