export interface Transaction {
  id: string;
  amount: number;
  merchantName: string;
  status: 'Success' | 'Failed' | 'Pending';
  timestamp: string;
}