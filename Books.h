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
/*8 to complete, i didn't understand*/
int customerSalesHistory(int cust_id);
int customerOrdersHistory(int cust_id);

// 11 - 15
int shippingPrice(int order_id);
/*-----to complete 12!!-------*/
int specificOrderStatus(int order_id);
int sumAmountInSpecificMonth(string year, string month);
int sumAmountPayedInBit(string year, string month);