import { BarcodeScanner } from 'nativescript-barcodescanner';
import { PaymentDetails } from '../models/payment.model';
import { parsePaymentUrl } from '../utils/url.utils';

export class QRScannerService {
  private scanner: BarcodeScanner;

  constructor() {
    this.scanner = new BarcodeScanner();
  }

  async scanQRCode(): Promise<PaymentDetails> {
    const hasPermission = await this.scanner.hasCameraPermission();
    if (!hasPermission) {
      const granted = await this.scanner.requestCameraPermission();
      if (!granted) {
        throw new Error('Camera permission is required');
      }
    }

    const result = await this.scanner.scan({
      formats: ['QR_CODE'],
      message: 'Place the QR code inside the scan area',
      showFlipCameraButton: false,
      preferFrontCamera: false,
      showTorchButton: true
    });

    if (!result.text) {
      throw new Error('No QR code detected');
    }

    return parsePaymentUrl(result.text);
  }
}