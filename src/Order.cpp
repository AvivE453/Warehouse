#include "../include/Order.h"
#include <iostream>

using namespace std;

Order::Order(int id, int customerId, int distance) : id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER)
{
}
int Order ::getId() const
{
  return id;
}
int Order ::getCustomerId() const
{
  return customerId;
}
int Order ::getDistance() const
{
  return distance;
};
void Order ::setStatus(OrderStatus status)
{
  this->status = status;
};
void Order ::setCollectorId(int collectorId)
{
  this->collectorId = collectorId;
}
void Order ::setDriverId(int driverId)
{
  this->driverId = driverId;
}
int Order ::getCollectorId() const
{
  return collectorId;
};
int Order ::getDriverId() const
{
  return driverId;
};
OrderStatus Order ::getStatus() const
{
  return status;
};
const string Order ::toString() const
{
  string statusType = "Pending";
  if (status == OrderStatus::COLLECTING)
    statusType = "Collecting";
  else if (status == OrderStatus::DELIVERING)
    statusType = "Delivering";
  else if (status == OrderStatus::COMPLETED)
    statusType = "Completed";
  return "OrderID:" + to_string(id) + ",CustomerID:" + to_string(customerId) + ",Status:" + statusType;
}
Order *Order::clone() const
{
  return new Order(*this);
}