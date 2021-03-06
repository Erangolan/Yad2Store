#pragma once

#include "Database.h"
#include <string>
#include <iomanip>

// 1 - 5
int showStorage(string title);
int theOldestCustomer();
int theOldestBookInStore();
int orderList();
int orderList(string title);

//5 - 10
int mostPopularAuthor(string sDate, string fDate);
int topThreeCustomers();
int mostTranslateBook();
int customerSalesHistory(int cust_id);
int customerOrdersHistory(int cust_id);

// 11 - 15
int shippingPrice(int order_id);
int splittingShippOrder(int cust_id);
int specificOrderStatus(int order_id);
int sumAmountInSpecificMonth(string year, string month);
int sumAmountPayedInBit(string year, string month);

// 16 - 20
int transactionsProfitHigherThanAvg();
int sumAmountShipping();
int shippDetailsBook();
int oldCustomers();
int badCustomers();

// 21 - 26
int numOfBooksInStorage(string year);
int storeBought(string sDate, string fDate);
int storeProfit(string year, string month);
int transactionsAmountEveryMonth(string year);
int employSalary(string year, string month, int emp_id);
int bestSellerEmploy(string year, string month);
