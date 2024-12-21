import { PaymentDetails } from '../models/payment.model';

export function parsePaymentUrl(url: string): PaymentDetails {
  try {
    const parsedUrl = new URL(url);
    return {
      merchantId: parsedUrl.searchParams.get('merchant') || '',
      amount: Number(parsedUrl.searchParams.get('amount')) || 0,
      description: parsedUrl.searchParams.get('desc') || ''
    };
  } catch (error) {
    throw new Error('Invalid QR code format');
  }
}