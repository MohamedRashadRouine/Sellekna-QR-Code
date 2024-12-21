import { Http } from '@nativescript/core';
import { API_CONFIG } from '../config/api.config';
import { PaymentDetails } from '../models/payment.model';

export class PaymentService {
  async processPayment(paymentDetails: PaymentDetails): Promise<boolean> {
    try {
      const response = await Http.request({
        url: `${API_CONFIG.BASE_URL}${API_CONFIG.ENDPOINTS.PAYMENTS}`,
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        content: JSON.stringify(paymentDetails)
      });
      
      return response.statusCode === 200;
    } catch (error) {
      console.error('Payment processing error:', error);
      throw new Error('Payment processing failed. Please try again.');
    }
  }
}